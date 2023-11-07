/*
 * Copyright (c) Antmicro
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdbool.h>
#include <stddef.h>

#include "cpu.h"
#include "pmu.h"

// *INDENT-OFF*
static pmu_event pmu_events[] = {
    {
        .identifier = PMU_EVENT_SOFTWARE_INCREMENT,
        .name = "Software increment"
    },
    {
        .identifier = PMU_EVENT_INSTRUCTIONS_EXECUTED,
        .name = "Instruction architecturally executed"
    },
    {
        .identifier = PMU_EVENT_CYCLES,
        .name = "CPU Cycle"
    }
};
// *INDENT-ON*

inline static void pmu_set_snapshot(pmu_counter *const counter, uint64_t snapshot)
{
    counter->snapshot = snapshot;
    counter->has_snapshot = true;
    if (unlikely(env->pmu.extra_logs_enabled)) {
        tlib_printf(LOG_LEVEL_DEBUG, "Took snapshot for PMU counter %d: %" PRIu64, counter->id, counter->snapshot);
    }
}

inline static void pmu_reset_snapshot(pmu_counter *const counter)
{
    counter->snapshot = 0;
    counter->has_snapshot = false;
    if (unlikely(env->pmu.extra_logs_enabled)) {
        tlib_printf(LOG_LEVEL_DEBUG, "Reset snapshot for PMU counter %d:", counter->id);
    }
}

inline static pmu_counter *pmu_get_counter(int counter_id)
{
    return counter_id == PMU_CYCLE_COUNTER_ID ? &env->pmu.cycle_counter : &env->pmu.counters[env->pmu.selected_counter_id];
}

inline void pmu_init_reset(CPUState *env)
{
    env->pmu.cycles_divisor = 1;

    // TODO: the number of counters should be configurable per-SoC/per-core
    // ideally from PMU external interface
    env->pmu.counters_number = PMU_MAX_PROGRAMMABLE_COUNTERS;
    assert(env->pmu.counters_number <= PMU_MAX_PROGRAMMABLE_COUNTERS);
    assert(PMU_MAX_PROGRAMMABLE_COUNTERS >= PMU_CYCLE_COUNTER_ID);

    // Implementer fields in PMCR and CPUID/MIDR have the same position and width
    env->cp15.c9_pmcr |= (env->cp15.c0_cpuid & PMCR_IMPL);                                                // Extract Implementer field and copy to PMCR
    env->cp15.c9_pmcr |= ((PMCR_NO_COUNTERS_MASK & env->pmu.counters_number) << PMCR_NO_COUNTERS_OFFSET); // Get the number of event counters

    for (int i = 0; i < PMU_MAX_PROGRAMMABLE_COUNTERS; ++i) {
        env->pmu.counters[i].id = i;
        // Snapshot and value are automatically reset by memset in `cpu_reset`
    }

    // Copy events to statically reserved space
    assert(sizeof(pmu_events) == sizeof(env->pmu.implemented_events));
    memcpy(env->pmu.implemented_events, pmu_events, sizeof(pmu_events));

    // CPU cycles event to special counter
    env->pmu.cycle_counter.measured_event_id = PMU_EVENT_CYCLES;
    // A special value - id for this makes no sense, but it will be easier to see in debug logs
    // since there already exists a "normal" counter with id `0`
    env->pmu.cycle_counter.id = PMU_CYCLE_COUNTER_ID;

    // Precalculate value of c9_pmceid0 (supported events id) CP15 register
    for (int i = 0; i < PMU_EVENTS_NUMBER; ++i) {
        uint32_t iden = env->pmu.implemented_events[i].identifier;
        if (iden > 0x1F) {
            // Cannot report this event - this is still valid (B4.1.114)
            // since this register only reports "common architectural [...] feature events"
            continue;
        }
        // Event Id represents n-th enabled bit
        env->cp15.c9_pmceid0 |= 1 << iden;
    }

    env->pmu.insns_overflow_nearest_limit = UINT32_MAX;
    env->pmu.cycles_overflow_nearest_limit = UINT32_MAX;
}

// Enable counters
void set_c9_pmcntenset(struct CPUState *env, uint64_t val)
{
    env->cp15.c9_pmcnten |= val;

    for (int off = 0; off < env->pmu.counters_number; ++off) {
        if (val & (1 << off) && !env->pmu.counters[off].enabled) {
            pmu_counter *const counter = &env->pmu.counters[off];

            if (pmu_is_insns_or_cycles_counter(counter)) {
                pmu_set_snapshot(counter, env->instructions_count_total_value);
            }
            counter->enabled = true;
            if (unlikely(env->pmu.extra_logs_enabled)) {
                tlib_printf(LOG_LEVEL_DEBUG, "Enabled PMU counter %d", off);
            }
        }
    }

    if (val & (1 << 31) && !env->pmu.cycle_counter.enabled) {
        env->pmu.cycle_counter.enabled = true;
        pmu_set_snapshot(&env->pmu.cycle_counter, env->instructions_count_total_value);
        if (unlikely(env->pmu.extra_logs_enabled)) {
            tlib_printf(LOG_LEVEL_DEBUG, "Enabled PMU cycles counter");
        }
    }

    pmu_recalculate_cycles_instruction_limit();
}

// Disable counters
void set_c9_pmcntenclr(struct CPUState *env, uint64_t val)
{
    env->cp15.c9_pmcnten &= ~val;

    for (int off = 0; off < env->pmu.counters_number; ++off) {
        if (val & (1 << off) && env->pmu.counters[off].enabled) {
            pmu_counter *const counter = &env->pmu.counters[off];

            if (pmu_is_insns_or_cycles_counter(counter)) {
                /* Copy value to `val` and clear snapshot
                 * So effectively - calculate the lazy value and store it inside counter,
                 * that is being disabled, so we can't rely on lazy calculation anymore
                 */
                counter->val = pmu_get_insn_cycle_value(counter);
                pmu_reset_snapshot(counter);
            }
            counter->enabled = false;
            if (unlikely(env->pmu.extra_logs_enabled)) {
                tlib_printf(LOG_LEVEL_DEBUG, "Disabled PMU counter %d", off);
            }
        }
    }

    if (val & (1 << 31) && env->pmu.cycle_counter.enabled) {
        // See comment above
        env->pmu.cycle_counter.val = pmu_get_insn_cycle_value(&env->pmu.cycle_counter);
        pmu_reset_snapshot(&env->pmu.cycle_counter);
        env->pmu.cycle_counter.enabled = false;
        if (unlikely(env->pmu.extra_logs_enabled)) {
            tlib_printf(LOG_LEVEL_DEBUG, "Disabled PMU cycles counter");
        }
    }
    pmu_recalculate_cycles_instruction_limit();
}

static inline void pmu_recalculate_all_lazy()
{
    // See comment in `c9_pmcntclr` why we need this
    if (env->pmu.cycle_counter.enabled) {
        env->pmu.cycle_counter.val = pmu_get_insn_cycle_value(&env->pmu.cycle_counter);
        pmu_reset_snapshot(&env->pmu.cycle_counter);
    }

    for (int i = 0; i < env->pmu.counters_number; ++i) {
        if (pmu_is_insns_or_cycles_counter(&env->pmu.counters[i]) && env->pmu.counters[i].enabled) {
            env->pmu.counters[i].val = pmu_get_insn_cycle_value(&env->pmu.counters[i]);
            pmu_reset_snapshot(&env->pmu.counters[i]);
        }
    }
}

static inline void pmu_take_all_snapshots()
{
    if (env->pmu.cycle_counter.enabled) {
        pmu_set_snapshot(&env->pmu.cycle_counter, env->instructions_count_total_value);
    }

    for (int i = 0; i < env->pmu.counters_number; ++i) {
        if (pmu_is_insns_or_cycles_counter(&env->pmu.counters[i]) && env->pmu.counters[i].enabled) {
            pmu_set_snapshot(&env->pmu.counters[i], env->instructions_count_total_value);
        }
    }
}

void set_c9_pmcr(struct CPUState *env, uint64_t val)
{
    // C (Cycle counter reset) bit (write-only)
    if (val & PMCR_C) {
        if (unlikely(env->pmu.extra_logs_enabled)) {
            tlib_printf(LOG_LEVEL_DEBUG, "Resetting PMU cycle counter");
        }
        env->pmu.cycle_counter.val = 0;
        if (env->pmu.cycle_counter.enabled) {
            pmu_set_snapshot(&env->pmu.cycle_counter, env->instructions_count_total_value);
        }
        pmu_update_cycles_instruction_limit(&env->pmu.cycle_counter);
    }

    // E (Enable) bit
    if ((val & PMCR_E) && !env->pmu.counters_enabled) {
        if (unlikely(env->pmu.extra_logs_enabled)) {
            tlib_printf(LOG_LEVEL_DEBUG, "Enabling PMU");
        }
        env->pmu.counters_enabled = true;

        // We snapshot all counters but not enable them individually
        // Only if the counter is enabled, otherwise we will do it on counter enable
        pmu_take_all_snapshots();

        // After enable we need to obtain nearest limit again
        pmu_recalculate_cycles_instruction_limit();
    } else if (((val & (1 << 0)) == 0) && env->pmu.counters_enabled) {
        if (unlikely(env->pmu.extra_logs_enabled)) {
            tlib_printf(LOG_LEVEL_DEBUG, "Disabling PMU");
        }
        pmu_recalculate_all_lazy();
        env->pmu.counters_enabled = false;
    }

    // P (Reset Counters - except cycle counter) bit
    if (val & PMCR_P) {
        if (unlikely(env->pmu.extra_logs_enabled)) {
            tlib_printf(LOG_LEVEL_DEBUG, "Resetting all PMU counters except Cycle Counter");
        }
        for (int i = 0; i < env->pmu.counters_number; ++i) {
            env->pmu.counters[i].val = 0;
            if (pmu_is_insns_or_cycles_counter(&env->pmu.counters[i]) && env->pmu.counters[i].enabled) {
                pmu_set_snapshot(&env->pmu.counters[i], env->instructions_count_total_value);
            }
        }
        // Cycle counter might still be active - so it will dictate the new limit
        if (env->pmu.cycle_counter.enabled) {
            env->pmu.cycles_overflow_nearest_limit = UINT32_MAX - pmu_get_insn_cycle_value(&env->pmu.cycle_counter);
        } else {
            env->pmu.cycles_overflow_nearest_limit = UINT32_MAX;
        }
        // Instruction counting has been definitely reset
        env->pmu.insns_overflow_nearest_limit = UINT32_MAX;
    }

    // D (Cycle count divisor) bit
    // We need to evaluate the counter's value if we change divisor
    if ((val & PMCR_D) && ((env->cp15.c9_pmcr & PMCR_D) == 0)) {
        pmu_recalculate_all_lazy();
        env->pmu.cycles_divisor = 64;
        pmu_take_all_snapshots();
        pmu_recalculate_cycles_instruction_limit();
    } else if (((val & PMCR_D) == 0) && (env->cp15.c9_pmcr & PMCR_D)) {
        pmu_recalculate_all_lazy();
        env->pmu.cycles_divisor = 1;
        pmu_take_all_snapshots();
        pmu_recalculate_cycles_instruction_limit();
    }

    /* Only the DP, X, D, C, P and E bits are writable
     * C and P are write-only
     * X and DP have no effect for us at the moment
     */
    uint32_t rw_mask = PMCR_E | PMCR_D | PMCR_DP | PMCR_X;
    env->cp15.c9_pmcr &= ~rw_mask;
    env->cp15.c9_pmcr |= (val & rw_mask);
}

// Clear overflow bits
void set_c9_pmovsr(struct CPUState *env, uint64_t val)
{
    env->cp15.c9_pmovsr &= ~val;

    for (int off = 0; off < env->pmu.counters_number; ++off) {
        if (val & (1 << off)) {
            env->pmu.counters[off].overflowed = false;
        }
    }

    if (val & (1 << 31)) {
        env->pmu.cycle_counter.overflowed = false;
    }
}

void set_c9_pmuserenr(struct CPUState *env, uint64_t val)
{
    if ((val & PMUSERENR_EN) != (env->cp15.c9_pmuserenr & PMUSERENR_EN)) {
        env->cp15.c9_pmuserenr = val & PMUSERENR_EN;
        /* changes access rights for cp registers, so flush tbs, but only on bit change */
        tb_flush(env);
    }
}

void set_c9_pmintenset(struct CPUState *env, uint64_t val)
{
    env->cp15.c9_pminten |= val;

    for (int off = 0; off < env->pmu.counters_number; ++off) {
        if (val & (1 << off)) {
            env->pmu.counters[off].enabled_overflow_interrupt = true;
            ++env->pmu.is_any_overflow_interrupt_enabled;
            if (unlikely(env->pmu.extra_logs_enabled)) {
                tlib_printf(LOG_LEVEL_DEBUG, "PMU counter %d enabled overflow interrupt", off);
            }
        }
    }

    if (val & (1 << 31)) {
        env->pmu.cycle_counter.enabled_overflow_interrupt = true;
        ++env->pmu.is_any_overflow_interrupt_enabled;
        if (unlikely(env->pmu.extra_logs_enabled)) {
            tlib_printf(LOG_LEVEL_DEBUG, "PMU cycle counter enabled overflow interrupt");
        }
    }
    pmu_recalculate_cycles_instruction_limit();
}

void set_c9_pmintenclr(struct CPUState *env, uint64_t val)
{
    env->cp15.c9_pminten &= ~val;

    for (int off = 0; off < env->pmu.counters_number; ++off) {
        if (val & (1 << off)) {
            env->pmu.counters[off].enabled_overflow_interrupt = false;
            --env->pmu.is_any_overflow_interrupt_enabled;
        }
    }

    if (val & (1 << 31)) {
        env->pmu.cycle_counter.enabled_overflow_interrupt = false;
        --env->pmu.is_any_overflow_interrupt_enabled;
    }
    pmu_recalculate_cycles_instruction_limit();
}

uint64_t get_c9_pmccntr(struct CPUState *env)
{
    /* This should return number of "clock cycles" or "clock cycles / 64"
     * But for us we don't differentiate between instructions taking a different amount of clock cycles
     * Neither we simulate delays or peripheral accesses in terms of clock cycles
     * So we return executed instruction number instead
     * Assuming that 1 instruction equals `env->cycles_per_instruction` CPU cycles
     */

    /* The value stored inside cycle or instruction counter is in reality a snapshot
     * taken at the moment the counter is activated.
     * the value is an offset that has been placed inside a register.
     * The real value is stored only when the PMU or the counter is disabled or changes state.
     * This is done to optimize actions in TB header hook.
     * Count_total minus the snapshot is elapsed cycles/executed instructions
     */

    // The value should already be divided by the divisor
    return pmu_get_insn_cycle_value(&env->pmu.cycle_counter);
}

void set_c9_pmselr(struct CPUState *env, uint64_t val)
{
    // *INDENT-OFF*
    if (val >= env->pmu.counters_number && val != PMU_CYCLE_COUNTER_ID) {
        // Access to unsupported counter is UNPREDICTABLE. Let's be more predictable than and prevent the write with error log
        tlib_printf(LOG_LEVEL_ERROR,
                    "Invalid PMU counter selected: %" PRIu64 ". Supported programmable counter IDs are: 0 - %" PRIu16 " and a cycle counter with ID: %d", 
                    val, env->pmu.counters_number - 1, PMU_CYCLE_COUNTER_ID);
        return;
    }
    // *INDENT-ON*
    env->pmu.selected_counter_id = val;

    // Cache event id, if we want to read it
    env->cp15.c9_pmxevtyper = pmu_get_counter(env->pmu.selected_counter_id)->measured_event_id;
}

void set_c9_pmxevtyper(struct CPUState *env, uint64_t val)
{
    // TODO: now we don't support EL0/1 etc. event filtering
    env->cp15.c9_pmxevtyper = val & PMXEVTYPER_EVT;
    int selected_event = val & PMXEVTYPER_EVT;

    if (env->pmu.selected_counter_id == PMU_CYCLE_COUNTER_ID) {
        tlib_printf(LOG_LEVEL_ERROR, "Cannot change event type for Cycle Counter");
        return;
    }

    pmu_counter *const selected_counter = &env->pmu.counters[env->pmu.selected_counter_id];

    // We need to calculate value here, because we can decide to change the counter's event type
    if (pmu_is_insns_or_cycles_counter(selected_counter)) {
        selected_counter->val = pmu_get_insn_cycle_value(selected_counter);
        pmu_reset_snapshot(selected_counter);
    }

    // P bit - privileged execution
    selected_counter->ignore_count_at_pl1 = (PMXEVTYPER_P & val) ? 1 : 0;
    // U bit - unprivileged execution
    selected_counter->ignore_count_at_pl0 = (PMXEVTYPER_U & val) ? 1 : 0;

    if (PMXEVTYPER_P & val || PMXEVTYPER_U & val) {
        tlib_printf(LOG_LEVEL_WARNING, "PMU: Filtering events based on EL is not supported");
    }

    // NSK, NSU, NSH are unimplemented, as we don't support Security or Virtualization extensions

    selected_counter->measured_event_id = selected_event;
    if (pmu_is_insns_or_cycles_counter(selected_counter)) {
        if (selected_counter->enabled) {
            pmu_set_snapshot(selected_counter, env->instructions_count_total_value);
        }
    } else {
        // If we don't count cycles, the snapshot should be already reset
        tlib_assert(!selected_counter->has_snapshot);
    }

    if (unlikely(env->pmu.extra_logs_enabled)) {
        tlib_printf(LOG_LEVEL_DEBUG, "PMU counter %d set measured event %d", env->pmu.selected_counter_id, selected_event);
    }
    pmu_recalculate_cycles_instruction_limit();

    // Verify if the event exists, warn if not
    for (int i = 0; i < PMU_EVENTS_NUMBER; ++i) {
        const pmu_event *current_event = &env->pmu.implemented_events[i];

        if (current_event->identifier == selected_event) {
            return;
        }
    }
    tlib_printf(LOG_LEVEL_WARNING, "Invalid/Unimplemented event 0x%x selected for PMU counter %d. It won't count anything",
                selected_event, env->pmu.selected_counter_id);
}

// We don't have to check correctness of selected_counter, it is done when writing to `c9_pmselr`
uint32_t get_c9_pmxevcntr(struct CPUState *env)
{
    pmu_counter *const counter = pmu_get_counter(env->pmu.selected_counter_id);
    // *INDENT-OFF*
    if (pmu_is_insns_or_cycles_counter(counter)) {
        if (unlikely(env->pmu.extra_logs_enabled)) {
            tlib_printf(LOG_LEVEL_DEBUG, "PMU counter %d reading value 0x%" PRIx32, env->pmu.selected_counter_id, pmu_get_insn_cycle_value(counter));
        }
        return pmu_get_insn_cycle_value(counter);
    } else {
        if (counter->has_snapshot) {
            tlib_abortf("Snapshot detected for PMU counter %d. Lazy evaluation for event %d is not supported", counter->id, counter->measured_event_id);
        }
        if (unlikely(env->pmu.extra_logs_enabled)) {
            tlib_printf(LOG_LEVEL_DEBUG, "PMU counter %d reading value 0x%" PRIx32, env->pmu.selected_counter_id, counter->val);
        }
        return counter->val;
    }
    // *INDENT-ON*
}

void set_c9_pmxevcntr(struct CPUState *env, uint64_t val)
{
    if (unlikely(env->pmu.extra_logs_enabled)) {
        tlib_printf(LOG_LEVEL_DEBUG, "PMU counter %d set to 0x%" PRIx32, env->pmu.selected_counter_id, val);
    }
    pmu_counter *const counter = pmu_get_counter(env->pmu.selected_counter_id);

    counter->val = val;
    if (pmu_is_insns_or_cycles_counter(counter) && counter->enabled) {
        pmu_set_snapshot(counter, env->instructions_count_total_value);
    }
    pmu_update_cycles_instruction_limit(counter);
}

void set_c9_pmswinc(struct CPUState *env, uint64_t val)
{
    for (int off = 0; off < env->pmu.counters_number; ++off) {
        if (val & (1 << off)) {
            pmu_counter *counter = &env->pmu.counters[off];

            // Only if the event is Software Increment
            if (counter->measured_event_id == PMU_EVENT_SOFTWARE_INCREMENT) {
                pmu_update_counter(env, counter, 1);
            } else {
                tlib_printf(LOG_LEVEL_WARNING, "Tried to kick PMU counter %d, but it's not a Software Increment", off);
            }
        }
    }
}

// Compute the instruction/cycle value from a "lazy" counter. Will also work for any counter, that doesn't take snapshots
uint32_t pmu_get_insn_cycle_value(const pmu_counter *const counter)
{
    uint64_t remainder = counter->measured_event_id == PMU_EVENT_CYCLES ? env->pmu.cycles_remainder : 0;
    uint64_t divisor = counter->measured_event_id == PMU_EVENT_CYCLES ? env->pmu.cycles_divisor : 1;
    return counter->val +
           (((counter->has_snapshot) ? (env->instructions_count_total_value - counter->snapshot) : 0) + remainder) / divisor;
}

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
void pmu_update_cycles_instruction_limit(const pmu_counter *const cnt)
{
    if (!cnt->enabled_overflow_interrupt || !cnt->enabled || !env->pmu.counters_enabled) {
        return;
    }

    // *INDENT-OFF*
    if (cnt->measured_event_id == PMU_EVENT_CYCLES) {
        // CPU cycles
        env->pmu.cycles_overflow_nearest_limit = MIN(env->pmu.cycles_overflow_nearest_limit, UINT32_MAX - pmu_get_insn_cycle_value(cnt));
    } else if (cnt->measured_event_id == PMU_EVENT_INSTRUCTIONS_EXECUTED) {
        // Instructions executed
        env->pmu.insns_overflow_nearest_limit = MIN(env->pmu.insns_overflow_nearest_limit, UINT32_MAX - pmu_get_insn_cycle_value(cnt));
    }
    // *INDENT-ON*

}
#undef MIN

// Recalculate value after which a nearest overflow on cycles/instructions will occur
void pmu_recalculate_cycles_instruction_limit()
{
    // We reset the current limit, as we don't know the reason the recalculation is needed
    // It's possible that a counter has been turned off, and we can't rely on comparing with the presaved limit
    env->pmu.cycles_overflow_nearest_limit = UINT32_MAX;
    env->pmu.insns_overflow_nearest_limit = UINT32_MAX;
    for (int i = 0; i < env->pmu.counters_number; ++i) {
        pmu_update_cycles_instruction_limit(&env->pmu.counters[i]);
    }

    pmu_update_cycles_instruction_limit(&env->pmu.cycle_counter);
}

void pmu_update_counter(CPUState *env, pmu_counter *const counter, uint64_t amount)
{
    assert(counter != NULL);

    if (!counter->enabled || !env->pmu.counters_enabled) {
        return;
    }

    // We need to look at divisor if cycles are counted
    if (unlikely(counter->measured_event_id == PMU_EVENT_CYCLES)) {
        // Amount is "uint64" to ensure that this and subsequent operations won't overflow it
        amount = (amount + env->pmu.cycles_remainder) / env->pmu.cycles_divisor;
    }

    bool overflowed = false;

    if (unlikely(pmu_is_insns_or_cycles_counter(counter))) {
        /* We need to calculate the real counter value here to properly determine overflow
         * Note that updating cycles counter with this function is highly unoptimal
         * and the counter normally updates itself when it needs to
         * But it still can be used if we intend to inject bogus executed instructions
         *
         * We need to reverse `amount` of ticks to see if the interrupt occurred,
         * As we have the executed value already in the counter, by the time we enter this function.
         * So it is enough to check if the current value is less than the amount by which we incremented
         * If so, the overflow already happened
         */
        if ((uint64_t)pmu_get_insn_cycle_value(counter) < amount) {
            overflowed = true;
        }
        counter->val = pmu_get_insn_cycle_value(counter);
        pmu_set_snapshot(counter, env->instructions_count_total_value);
    } else {
        // Overflow happened
        if (amount + (uint64_t)counter->val > UINT32_MAX) {
            overflowed = true;
        }
        // Unsigned arithmetic, will wrap around
        counter->val += (uint32_t)amount;
    }

    if (unlikely(env->pmu.extra_logs_enabled)) {
        // *INDENT-OFF*
        tlib_printf(LOG_LEVEL_DEBUG, "PMU counter %d updated by 0x%" PRIx64 ", it's now 0x%" PRIx32 ".%s", 
                    counter->id, amount, pmu_get_insn_cycle_value(counter), overflowed ? " Overflow occurred." : "");
        // *INDENT-ON*
    }

    if (overflowed) {
        counter->overflowed = overflowed;
        // Generate overflow interrupt
        if (counter->enabled_overflow_interrupt) {
            // Set interrupt bit in CP register
            env->cp15.c9_pmovsr |= 1 << counter->id;
            // Report interrupt to external PMU interface
            tlib_report_pmu_overflow(counter->id);
        }
    }
}

// Update each enabled counter, subscribing to `event_id` by `amount` (delta) of event ticks
void HELPER(pmu_update_event_counters)(CPUState * env, int event_id, uint32_t amount)
{
    if (!env->pmu.counters_enabled) {
        tlib_printf(LOG_LEVEL_WARNING, "PMU: Trying to update counters for event %d but PMU is not enabled", event_id);
        return;
    }

    for (int i = 0; i < env->pmu.counters_number; ++i) {
        pmu_counter *counter = &env->pmu.counters[i];
        if (counter->measured_event_id == event_id) {
            pmu_update_counter(env, counter, amount);
        }
    }

    // Cycles counter
    if (event_id == PMU_EVENT_CYCLES) {
        pmu_update_counter(env, &env->pmu.cycle_counter, amount);

        // After we updated all counters, update cycles remainder
        uint64_t old_remainder = env->pmu.cycles_remainder;
        env->pmu.cycles_remainder = (amount + old_remainder) % env->pmu.cycles_divisor;
    }

    if (event_id == PMU_EVENT_CYCLES || event_id == PMU_EVENT_INSTRUCTIONS_EXECUTED) {
        pmu_recalculate_cycles_instruction_limit();
    }
}

void pmu_count_instructions_cycles(uint32_t icount)
{
    if (!env->pmu.counters_enabled || !env->pmu.is_any_overflow_interrupt_enabled) {
        return;
    }

    bool will_overflow = false;
    if ((((icount + env->pmu.cycles_remainder) / env->pmu.cycles_divisor) > env->pmu.cycles_overflow_nearest_limit) ||
        (icount > env->pmu.insns_overflow_nearest_limit)) {
        will_overflow = true;
    }

    if (unlikely(will_overflow)) {
        /* When we know we will overflow, we update the counters and force calculation of the "lazy" value
         * This will automatically trigger the overflow interrupt and find next limit for cycles/instructions
         * This is a costly operation, so we do it only when necessary
         */
        helper_pmu_update_event_counters(env, PMU_EVENT_INSTRUCTIONS_EXECUTED, icount);
        helper_pmu_update_event_counters(env, PMU_EVENT_CYCLES, icount);
    } else {
        env->pmu.insns_overflow_nearest_limit -= icount;
        env->pmu.cycles_overflow_nearest_limit -= (icount + env->pmu.cycles_remainder) / env->pmu.cycles_divisor;
        env->pmu.cycles_remainder = (icount + env->pmu.cycles_remainder) % env->pmu.cycles_divisor;
    }
}
