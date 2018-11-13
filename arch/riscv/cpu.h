#if !defined (__RISCV_CPU_H__)
#define __RISCV_CPU_H__

#include <stdbool.h>

#include "cpu-defs.h"
#include "softfloat.h"
#include "host-utils.h"

#include "cpu-common.h"

// This could possibly be generalized. 0 and 1 values are used as "is_write". This conflicts in a way with READ_ACCESS_TYPE et al.
#define MMU_DATA_LOAD 0
#define MMU_DATA_STORE 1
#define MMU_INST_FETCH 2

#define TARGET_PAGE_BITS 12 /* 4 KiB Pages */
#if TARGET_LONG_BITS == 64
#define TARGET_RISCV64
#define TARGET_PHYS_ADDR_SPACE_BITS 50
#define TARGET_VIRT_ADDR_SPACE_BITS 39
#elif TARGET_LONG_BITS == 32
#define TARGET_RISCV32
#define TARGET_PHYS_ADDR_SPACE_BITS 34
#define TARGET_VIRT_ADDR_SPACE_BITS 32
#else
#error "Target arch can be only 32-bit or 64-bit."
#endif

#include "cpu_bits.h"

#define RV(x) ((target_ulong)1 << (x - 'A'))

#define TRANSLATE_FAIL 1
#define TRANSLATE_SUCCESS 0
#define NB_MMU_MODES 4

#define MAX_RISCV_PMPS (16)

#define get_field(reg, mask) (((reg) & (target_ulong)(mask)) / ((mask) & ~((mask) << 1)))
#define set_field(reg, mask, val) (((reg) & ~(target_ulong)(mask)) | (((target_ulong)(val) * ((mask) & ~((mask) << 1))) & (target_ulong)(mask)))

#define assert(x) {if (!(x)) tlib_abortf("Assert not met in %s:%d: %s", __FILE__, __LINE__, #x);}while(0)

typedef struct CPUState CPUState;

#include "pmp.h"

// +---------------------------------------+
// | ALL FIELDS WHICH STATE MUST BE STORED |
// | DURING SERIALIZATION SHOULD BE PLACED |
// | BEFORE >CPU_COMMON< SECTION.          |
// +---------------------------------------+
struct CPUState {
    target_ulong gpr[32];
    uint64_t fpr[32]; /* assume both F and D extensions */
    target_ulong pc;
    target_ulong load_res;

    target_ulong frm;
    target_ulong fflags;

    target_ulong badaddr;

    uint32_t mucounteren;

    target_ulong priv;

    target_ulong misa;
    target_ulong misa_mask;
    target_ulong mstatus;

    target_ulong mhartid;

    pthread_mutex_t mip_lock;
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

    uint64_t mcycle_snapshot_offset;
    uint64_t mcycle_snapshot;

    uint64_t minstret_snapshot_offset;
    uint64_t minstret_snapshot;

    /* if privilege architecture v1.10 is not set, we assume 1.09 */
    bool privilege_architecture_1_10;

    CPU_COMMON
};

#define CPU_PC(x) x->pc

int cpu_exec(CPUState *s);
int cpu_init(const char *cpu_model);

void riscv_set_mode(CPUState *env, target_ulong newpriv);

void helper_raise_exception(CPUState *env, uint32_t exception);

int cpu_riscv_handle_mmu_fault(CPUState *cpu, target_ulong address, int rw,
                              int mmu_idx);
int riscv_cpu_mmu_index(CPUState *env);
int riscv_cpu_hw_interrupts_pending(CPUState *env);

#define cpu_handle_mmu_fault cpu_riscv_handle_mmu_fault
#define cpu_mmu_index riscv_cpu_mmu_index

#include "cpu-all.h"
#include "exec-all.h"

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

static inline int riscv_mstatus_fs(CPUState *env)
{
    return env->mstatus & MSTATUS_FS;
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

static inline int riscv_has_ext(CPUState *env, target_ulong ext)
{
    return (env->misa & ext) != 0;
}

#endif /* !defined (__RISCV_CPU_H__) */
