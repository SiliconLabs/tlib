#if !defined (__RISCV_CPU_H__)
#define __RISCV_CPU_H__

#include <stdbool.h>

#define RISCV_START_PC 0x200

#include "cpu-defs.h"
#include "softfloat.h"
#include "host-utils.h"

#include "cpu-common.h"

#define TARGET_PAGE_BITS 12 /* 4 KiB Pages */
#if TARGET_LONG_BITS == 64
#define TARGET_RISCV64
#define TARGET_PHYS_ADDR_SPACE_BITS 50
#define TARGET_VIRT_ADDR_SPACE_BITS 39
#elif TARGET_LONG_BITS == 32
#define TARGET_RISCV32
#define TARGET_LONG_BITS 32
#define TARGET_PHYS_ADDR_SPACE_BITS 34
#define TARGET_VIRT_ADDR_SPACE_BITS 32
#else
#error "Target arch can be only 32-bit or 64-bit."
#endif

#include "cpu_bits.h"

#define RV(x) (1L << (x - 'A'))

/* RISCV Exception Codes */
#define EXCP_NONE                       -1 /* not a real RISCV exception code */
#define RISCV_EXCP_INST_ADDR_MIS           0x0
#define RISCV_EXCP_INST_ACCESS_FAULT       0x1
#define RISCV_EXCP_ILLEGAL_INST            0x2
#define RISCV_EXCP_BREAKPOINT              0x3
#define RISCV_EXCP_LOAD_ADDR_MIS           0x4
#define RISCV_EXCP_LOAD_ACCESS_FAULT       0x5
#define RISCV_EXCP_STORE_AMO_ADDR_MIS      0x6
#define RISCV_EXCP_STORE_AMO_ACCESS_FAULT  0x7
#define RISCV_EXCP_U_ECALL                 0x8 /* for convenience, report all
                                                  ECALLs as this, handler
                                                  fixes */
#define RISCV_EXCP_S_ECALL                 0x9
#define RISCV_EXCP_H_ECALL                 0xa
#define RISCV_EXCP_M_ECALL                 0xb
#define RISCV_EXCP_INST_PAGE_FAULT         0xc /* since: priv-1.10.0 */
#define RISCV_EXCP_LOAD_PAGE_FAULT         0xd /* since: priv-1.10.0 */
#define RISCV_EXCP_STORE_PAGE_FAULT        0xf /* since: priv-1.10.0 */


#define TRANSLATE_FAIL 1
#define TRANSLATE_SUCCESS 0
#define NB_MMU_MODES 4

#define MAX_RISCV_PMPS (16)

#define get_field(reg, mask) (((reg) & (target_ulong)(mask)) / ((mask) & ~((mask) << 1)))
#define set_field(reg, mask, val) (((reg) & ~(target_ulong)(mask)) | (((target_ulong)(val) * ((mask) & ~((mask) << 1))) & (target_ulong)(mask)))

#define assert(x) {if (!(x)) tlib_abortf("Assert not met in %s:%d: %s", __FILE__, __LINE__, #x);}while(0)

typedef struct CPUState CPUState;

#include "pmp.h"

struct CPUState {
    target_ulong gpr[32];
    uint64_t fpr[32]; /* assume both F and D extensions */
    target_ulong pc;
    target_ulong load_res;

    target_ulong frm;
    target_ulong fstatus;
    target_ulong fflags;

    target_ulong badaddr;

    uint32_t mucounteren;

    target_ulong priv;

    target_ulong misa;
    target_ulong misa_mask;
    target_ulong mstatus;

    target_ulong mhartid;

    target_ulong mip;
    target_ulong mie;
    target_ulong mideleg;

    target_ulong sptbr;  /* until: priv-1.9.1 */
    target_ulong satp;   /* since: priv-1.10.0 */
    target_ulong sbadaddr;
    target_ulong mbadaddr;
    target_ulong medeleg;

    target_ulong stvec;
    target_ulong sepc;
    target_ulong scause;

    target_ulong mtvec;
    target_ulong mepc;
    target_ulong mcause;
    target_ulong mtval;  /* since: priv-1.10.0 */

    uint32_t mscounteren;
    target_ulong scounteren; /* since: priv-1.10.0 */
    target_ulong mcounteren; /* since: priv-1.10.0 */

    target_ulong sscratch;
    target_ulong mscratch;

    /* temporary htif regs */
    uint64_t mfromhost;
    uint64_t mtohost;
    uint64_t timecmp;

    /* physical memory protection */
    pmp_table_t pmp_state;

    float_status fp_status;

    /* if privilege mode v1.10 is not set, we assume 1.09 */
    bool privilege_mode_1_10;

    CPU_COMMON
};

#define CPU_PC(x) x->pc

/*
 * Compute mmu index
 * Adapted from Spike's mmu_t::translate
 */
static inline int cpu_mmu_index(CPUState *env)
{
    target_ulong mode = env->priv;
    if (get_field(env->mstatus, MSTATUS_MPRV)) {
        mode = get_field(env->mstatus, MSTATUS_MPP);
    }
    if (env->privilege_mode_1_10) {
        if (get_field(env->satp, SATP_MODE) == VM_1_10_MBARE) {
            mode = PRV_M;
        }
    } else {
        if (get_field(env->mstatus, MSTATUS_VM) == VM_1_09_MBARE) {
            mode = PRV_M;
        }
    }
    return mode;
}

#include "cpu-all.h"
#include "exec-all.h"

/*
 * Return RISC-V IRQ number if an interrupt should be taken, else -1.
 * Used in cpu-exec.c
 *
 * Adapted from Spike's processor_t::take_interrupt()
 */
static inline int cpu_riscv_hw_interrupts_pending(CPUState *env)
{
    target_ulong pending_interrupts = env->mip & env->mie;

    target_ulong mie = get_field(env->mstatus, MSTATUS_MIE);
    target_ulong m_enabled = env->priv < PRV_M || (env->priv == PRV_M && mie);
    target_ulong enabled_interrupts = pending_interrupts &
                                      ~env->mideleg & -m_enabled;

    target_ulong sie = get_field(env->mstatus, MSTATUS_SIE);
    target_ulong s_enabled = env->priv < PRV_S || (env->priv == PRV_S && sie);
    enabled_interrupts |= pending_interrupts & env->mideleg &
                          -s_enabled;

    if (enabled_interrupts) {
        return ctz64(enabled_interrupts); /* since non-zero */
    } else {
        return EXCP_NONE; /* indicates no pending interrupt */
    }
}

int cpu_exec(CPUState *s);
int cpu_init(const char *cpu_model);

void cpu_state_reset(CPUState *s);

void set_privilege(CPUState *env, target_ulong newpriv);

/* RISC-V timer unimplemented functions */
uint64_t cpu_riscv_get_cycle (CPUState *env);
uint32_t cpu_riscv_get_random (CPUState *env);
void cpu_riscv_store_compare (CPUState *env, uint64_t value);
void cpu_riscv_start_count(CPUState *env);

void cpu_riscv_store_timew(CPUState *env, uint64_t val_to_write);
uint64_t cpu_riscv_read_mtime(CPUState *env);
uint64_t cpu_riscv_read_stime(CPUState *env);
uint64_t cpu_riscv_read_time(CPUState *env);

void cpu_riscv_store_instret(CPUState *env, uint64_t val_to_write);

void helper_raise_exception(CPUState *env, uint32_t exception);

int cpu_riscv_handle_mmu_fault(CPUState *cpu, target_ulong address, int rw,
                              int mmu_idx);
#define cpu_handle_mmu_fault cpu_riscv_handle_mmu_fault

static inline void cpu_get_tb_cpu_state(CPUState *env, target_ulong *pc,
                                        target_ulong *cs_base, int *flags)
{
    *pc = env->pc;
    *cs_base = 0;
    *flags = 0; // necessary to avoid compiler warning
}

static inline bool cpu_has_work(CPUState *env)
{
    return env->interrupt_request & CPU_INTERRUPT_HARD;
}

void csr_write_helper(CPUState *env, target_ulong val_to_write,
        target_ulong csrno);

void do_interrupt(CPUState *env);

static inline void cpu_pc_from_tb(CPUState *cs, TranslationBlock *tb)
{
    cs->pc = tb->pc;
}

enum riscv_features {
    RISCV_FEATURE_RVI = RV('I'),
    RISCV_FEATURE_RVM = RV('M'),
    RISCV_FEATURE_RVA = RV('A'),
    RISCV_FEATURE_RVF = RV('F'),
    RISCV_FEATURE_RVD = RV('D'),
    RISCV_FEATURE_RVC = RV('C'),
    RISCV_FEATURE_RVS = RV('S'),
    RISCV_FEATURE_RVU = RV('U'),
};

static inline int riscv_feature(CPUState *env, int feature)
{
    return (env->misa & feature) != 0;
}

#endif /* !defined (__RISCV_CPU_H__) */
