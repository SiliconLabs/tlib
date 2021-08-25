#include "def-helper.h"

/* Exceptions */
DEF_HELPER_2(raise_exception, void, env, i32)
DEF_HELPER_1(raise_exception_debug, void, env)
DEF_HELPER_3(raise_exception_mbadaddr, void, env, i32, tl)

/* Floating Point - fused */
DEF_HELPER_5(fmadd_s, i64, env, i64, i64, i64, i64)
DEF_HELPER_5(fmadd_d, i64, env, i64, i64, i64, i64)
DEF_HELPER_5(fmsub_s, i64, env, i64, i64, i64, i64)
DEF_HELPER_5(fmsub_d, i64, env, i64, i64, i64, i64)
DEF_HELPER_5(fnmsub_s, i64, env, i64, i64, i64, i64)
DEF_HELPER_5(fnmsub_d, i64, env, i64, i64, i64, i64)
DEF_HELPER_5(fnmadd_s, i64, env, i64, i64, i64, i64)
DEF_HELPER_5(fnmadd_d, i64, env, i64, i64, i64, i64)

/* Floating Point - Single Precision */
DEF_HELPER_4(fadd_s, i64, env, i64, i64, i64)
DEF_HELPER_4(fsub_s, i64, env, i64, i64, i64)
DEF_HELPER_4(fmul_s, i64, env, i64, i64, i64)
DEF_HELPER_4(fdiv_s, i64, env, i64, i64, i64)
DEF_HELPER_3(fmin_s, i64, env, i64, i64)
DEF_HELPER_3(fmax_s, i64, env, i64, i64)
DEF_HELPER_3(fsqrt_s, i64, env, i64, i64)
DEF_HELPER_3(fle_s, tl, env, i64, i64)
DEF_HELPER_3(flt_s, tl, env, i64, i64)
DEF_HELPER_3(feq_s, tl, env, i64, i64)
DEF_HELPER_3(fcvt_w_s, tl, env, i64, i64)
DEF_HELPER_3(fcvt_wu_s, tl, env, i64, i64)
#if defined(TARGET_RISCV64)
DEF_HELPER_3(fcvt_l_s, i64, env, i64, i64)
DEF_HELPER_3(fcvt_lu_s, i64, env, i64, i64)
#endif
DEF_HELPER_3(fcvt_s_w, i64, env, tl, i64)
DEF_HELPER_3(fcvt_s_wu, i64, env, tl, i64)
#if defined(TARGET_RISCV64)
DEF_HELPER_3(fcvt_s_l, i64, env, i64, i64)
DEF_HELPER_3(fcvt_s_lu, i64, env, i64, i64)
#endif
DEF_HELPER_2(fclass_s, tl, env, i64)

/* Floating Point - Double Precision */
DEF_HELPER_4(fadd_d, i64, env, i64, i64, i64)
DEF_HELPER_4(fsub_d, i64, env, i64, i64, i64)
DEF_HELPER_4(fmul_d, i64, env, i64, i64, i64)
DEF_HELPER_4(fdiv_d, i64, env, i64, i64, i64)
DEF_HELPER_3(fmin_d, i64, env, i64, i64)
DEF_HELPER_3(fmax_d, i64, env, i64, i64)
DEF_HELPER_3(fcvt_s_d, i64, env, i64, i64)
DEF_HELPER_3(fcvt_d_s, i64, env, i64, i64)
DEF_HELPER_3(fsqrt_d, i64, env, i64, i64)
DEF_HELPER_3(fle_d, tl, env, i64, i64)
DEF_HELPER_3(flt_d, tl, env, i64, i64)
DEF_HELPER_3(feq_d, tl, env, i64, i64)
DEF_HELPER_3(fcvt_w_d, tl, env, i64, i64)
DEF_HELPER_3(fcvt_wu_d, tl, env, i64, i64)
#if defined(TARGET_RISCV64)
DEF_HELPER_3(fcvt_l_d, i64, env, i64, i64)
DEF_HELPER_3(fcvt_lu_d, i64, env, i64, i64)
#endif
DEF_HELPER_3(fcvt_d_w, i64, env, tl, i64)
DEF_HELPER_3(fcvt_d_wu, i64, env, tl, i64)
#if defined(TARGET_RISCV64)
DEF_HELPER_3(fcvt_d_l, i64, env, i64, i64)
DEF_HELPER_3(fcvt_d_lu, i64, env, i64, i64)
#endif
DEF_HELPER_2(fclass_d, tl, env, i64)

/* Special functions */
DEF_HELPER_3(csrrw, tl, env, tl, tl)
DEF_HELPER_4(csrrs, tl, env, tl, tl, tl)
DEF_HELPER_4(csrrc, tl, env, tl, tl, tl)
DEF_HELPER_2(sret, tl, env, tl)
DEF_HELPER_2(mret, tl, env, tl)
DEF_HELPER_1(wfi, void, env)
DEF_HELPER_1(tlb_flush, void, env)
DEF_HELPER_1(fence_i, void, env)

DEF_HELPER_1(acquire_global_memory_lock, void, env)
DEF_HELPER_1(release_global_memory_lock, void, env)
DEF_HELPER_2(reserve_address, void, env, tl)
DEF_HELPER_2(check_address_reservation, tl, env, tl)

/* Vector Extension */
DEF_HELPER_6(vsetvl, tl, env, tl, tl, tl, tl, i32)

DEF_HELPER_4(vle8, void, env, i32, i32, i32)
DEF_HELPER_4(vle8ff, void, env, i32, i32, i32)
DEF_HELPER_4(vle16, void, env, i32, i32, i32)
DEF_HELPER_4(vle16ff, void, env, i32, i32, i32)
DEF_HELPER_4(vle32, void, env, i32, i32, i32)
DEF_HELPER_4(vle32ff, void, env, i32, i32, i32)
DEF_HELPER_4(vle64, void, env, i32, i32, i32)
DEF_HELPER_4(vle64ff, void, env, i32, i32, i32)
DEF_HELPER_5(vlse8, void, env, i32, i32, i32, i32)
DEF_HELPER_5(vlse16, void, env, i32, i32, i32, i32)
DEF_HELPER_5(vlse32, void, env, i32, i32, i32, i32)
DEF_HELPER_5(vlse64, void, env, i32, i32, i32, i32)
DEF_HELPER_5(vlxei8, void, env, i32, i32, i32, i32)
DEF_HELPER_5(vlxei16, void, env, i32, i32, i32, i32)
DEF_HELPER_5(vlxei32, void, env, i32, i32, i32, i32)
DEF_HELPER_5(vlxei64, void, env, i32, i32, i32, i32)

DEF_HELPER_4(vle8_m, void, env, i32, i32, i32)
DEF_HELPER_4(vle8ff_m, void, env, i32, i32, i32)
DEF_HELPER_4(vle16_m, void, env, i32, i32, i32)
DEF_HELPER_4(vle16ff_m, void, env, i32, i32, i32)
DEF_HELPER_4(vle32_m, void, env, i32, i32, i32)
DEF_HELPER_4(vle32ff_m, void, env, i32, i32, i32)
DEF_HELPER_4(vle64_m, void, env, i32, i32, i32)
DEF_HELPER_4(vle64ff_m, void, env, i32, i32, i32)
DEF_HELPER_5(vlse8_m, void, env, i32, i32, i32, i32)
DEF_HELPER_5(vlse16_m, void, env, i32, i32, i32, i32)
DEF_HELPER_5(vlse32_m, void, env, i32, i32, i32, i32)
DEF_HELPER_5(vlse64_m, void, env, i32, i32, i32, i32)
DEF_HELPER_5(vlxei8_m, void, env, i32, i32, i32, i32)
DEF_HELPER_5(vlxei16_m, void, env, i32, i32, i32, i32)
DEF_HELPER_5(vlxei32_m, void, env, i32, i32, i32, i32)
DEF_HELPER_5(vlxei64_m, void, env, i32, i32, i32, i32)

DEF_HELPER_4(vse8, void, env, i32, i32, i32)
DEF_HELPER_4(vse16, void, env, i32, i32, i32)
DEF_HELPER_4(vse32, void, env, i32, i32, i32)
DEF_HELPER_4(vse64, void, env, i32, i32, i32)
DEF_HELPER_5(vsse8, void, env, i32, i32, i32, i32)
DEF_HELPER_5(vsse16, void, env, i32, i32, i32, i32)
DEF_HELPER_5(vsse32, void, env, i32, i32, i32, i32)
DEF_HELPER_5(vsse64, void, env, i32, i32, i32, i32)
DEF_HELPER_5(vsxei8, void, env, i32, i32, i32, i32)
DEF_HELPER_5(vsxei16, void, env, i32, i32, i32, i32)
DEF_HELPER_5(vsxei32, void, env, i32, i32, i32, i32)
DEF_HELPER_5(vsxei64, void, env, i32, i32, i32, i32)

DEF_HELPER_4(vse8_m, void, env, i32, i32, i32)
DEF_HELPER_4(vse16_m, void, env, i32, i32, i32)
DEF_HELPER_4(vse32_m, void, env, i32, i32, i32)
DEF_HELPER_4(vse64_m, void, env, i32, i32, i32)
DEF_HELPER_5(vsse8_m, void, env, i32, i32, i32, i32)
DEF_HELPER_5(vsse16_m, void, env, i32, i32, i32, i32)
DEF_HELPER_5(vsse32_m, void, env, i32, i32, i32, i32)
DEF_HELPER_5(vsse64_m, void, env, i32, i32, i32, i32)
DEF_HELPER_5(vsxei8_m, void, env, i32, i32, i32, i32)
DEF_HELPER_5(vsxei16_m, void, env, i32, i32, i32, i32)
DEF_HELPER_5(vsxei32_m, void, env, i32, i32, i32, i32)
DEF_HELPER_5(vsxei64_m, void, env, i32, i32, i32, i32)

DEF_HELPER_4(vl_wr, void, env, i32, i32, i32)
DEF_HELPER_4(vs_wr, void, env, i32, i32, i32)
DEF_HELPER_3(vlm, void, env, i32, i32)
DEF_HELPER_3(vsm, void, env, i32, i32)

DEF_HELPER_3(vmv_ivi, void, env, i32, i64)
DEF_HELPER_3(vmv_ivv, void, env, i32, i32)
DEF_HELPER_4(vadd_ivi, void, env, i32, i32, s64)
DEF_HELPER_4(vadd_ivi_m, void, env, i32, i32, s64)
DEF_HELPER_4(vadd_ivv, void, env, i32, i32, i32)
DEF_HELPER_4(vadd_ivv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vsub_ivv, void, env, i32, i32, i32)
DEF_HELPER_4(vsub_ivv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vrsub_ivi, void, env, i32, i32, s64)
DEF_HELPER_4(vrsub_ivi_m, void, env, i32, i32, s64)

DEF_HELPER_4(vwaddu_mvv, void, env, i32, i32, i32)
DEF_HELPER_4(vwaddu_mvv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vwadd_mvv, void, env, i32, i32, i32)
DEF_HELPER_4(vwadd_mvv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vwsubu_mvv, void, env, i32, i32, i32)
DEF_HELPER_4(vwsubu_mvv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vwsub_mvv, void, env, i32, i32, i32)
DEF_HELPER_4(vwsub_mvv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vwaddu_mvx, void, env, i32, i32, tl)
DEF_HELPER_4(vwaddu_mvx_m, void, env, i32, i32, tl)
DEF_HELPER_4(vwadd_mvx, void, env, i32, i32, tl)
DEF_HELPER_4(vwadd_mvx_m, void, env, i32, i32, tl)
DEF_HELPER_4(vwsubu_mvx, void, env, i32, i32, tl)
DEF_HELPER_4(vwsubu_mvx_m, void, env, i32, i32, tl)
DEF_HELPER_4(vwsub_mvx, void, env, i32, i32, tl)
DEF_HELPER_4(vwsub_mvx_m, void, env, i32, i32, tl)
DEF_HELPER_4(vwaddu_mwv, void, env, i32, i32, i32)
DEF_HELPER_4(vwaddu_mwv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vwadd_mwv, void, env, i32, i32, i32)
DEF_HELPER_4(vwadd_mwv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vwsubu_mwv, void, env, i32, i32, i32)
DEF_HELPER_4(vwsubu_mwv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vwsub_mwv, void, env, i32, i32, i32)
DEF_HELPER_4(vwsub_mwv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vwaddu_mwx, void, env, i32, i32, tl)
DEF_HELPER_4(vwaddu_mwx_m, void, env, i32, i32, tl)
DEF_HELPER_4(vwadd_mwx, void, env, i32, i32, tl)
DEF_HELPER_4(vwadd_mwx_m, void, env, i32, i32, tl)
DEF_HELPER_4(vwsubu_mwx, void, env, i32, i32, tl)
DEF_HELPER_4(vwsubu_mwx_m, void, env, i32, i32, tl)
DEF_HELPER_4(vwsub_mwx, void, env, i32, i32, tl)
DEF_HELPER_4(vwsub_mwx_m, void, env, i32, i32, tl)

DEF_HELPER_4(vmul_mvv, void, env, i32, i32, i32)
DEF_HELPER_4(vmul_mvv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vmul_mvx, void, env, i32, i32, tl)
DEF_HELPER_4(vmul_mvx_m, void, env, i32, i32, tl)
DEF_HELPER_4(vmulh_mvv, void, env, i32, i32, i32)
DEF_HELPER_4(vmulh_mvv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vmulh_mvx, void, env, i32, i32, tl)
DEF_HELPER_4(vmulh_mvx_m, void, env, i32, i32, tl)
DEF_HELPER_4(vmulhu_mvv, void, env, i32, i32, i32)
DEF_HELPER_4(vmulhu_mvv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vmulhu_mvx, void, env, i32, i32, tl)
DEF_HELPER_4(vmulhu_mvx_m, void, env, i32, i32, tl)
DEF_HELPER_4(vmulhsu_mvv, void, env, i32, i32, i32)
DEF_HELPER_4(vmulhsu_mvv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vmulhsu_mvx, void, env, i32, i32, tl)
DEF_HELPER_4(vmulhsu_mvx_m, void, env, i32, i32, tl)
DEF_HELPER_4(vwmul_mvv, void, env, i32, i32, i32)
DEF_HELPER_4(vwmul_mvv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vwmul_mvx, void, env, i32, i32, tl)
DEF_HELPER_4(vwmul_mvx_m, void, env, i32, i32, tl)
DEF_HELPER_4(vwmulu_mvv, void, env, i32, i32, i32)
DEF_HELPER_4(vwmulu_mvv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vwmulu_mvx, void, env, i32, i32, tl)
DEF_HELPER_4(vwmulu_mvx_m, void, env, i32, i32, tl)
DEF_HELPER_4(vwmulsu_mvv, void, env, i32, i32, i32)
DEF_HELPER_4(vwmulsu_mvv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vwmulsu_mvx, void, env, i32, i32, tl)
DEF_HELPER_4(vwmulsu_mvx_m, void, env, i32, i32, tl)

DEF_HELPER_4(vminu_ivv, void, env, i32, i32, i32)
DEF_HELPER_4(vminu_ivv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vminu_ivi, void, env, i32, i32, tl)
DEF_HELPER_4(vminu_ivi_m, void, env, i32, i32, tl)
DEF_HELPER_4(vmin_ivv, void, env, i32, i32, i32)
DEF_HELPER_4(vmin_ivv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vmin_ivi, void, env, i32, i32, tl)
DEF_HELPER_4(vmin_ivi_m, void, env, i32, i32, tl)
DEF_HELPER_4(vmaxu_ivv, void, env, i32, i32, i32)
DEF_HELPER_4(vmaxu_ivv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vmaxu_ivi, void, env, i32, i32, tl)
DEF_HELPER_4(vmaxu_ivi_m, void, env, i32, i32, tl)
DEF_HELPER_4(vmax_ivv, void, env, i32, i32, i32)
DEF_HELPER_4(vmax_ivv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vmax_ivi, void, env, i32, i32, tl)
DEF_HELPER_4(vmax_ivi_m, void, env, i32, i32, tl)

DEF_HELPER_4(vnsrl_ivi, void, env, i32, i32, tl)
DEF_HELPER_4(vnsrl_ivi_m, void, env, i32, i32, tl)
DEF_HELPER_4(vnsrl_ivv, void, env, i32, i32, i32)
DEF_HELPER_4(vnsrl_ivv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vnsra_ivi, void, env, i32, i32, tl)
DEF_HELPER_4(vnsra_ivi_m, void, env, i32, i32, tl)
DEF_HELPER_4(vnsra_ivv, void, env, i32, i32, i32)
DEF_HELPER_4(vnsra_ivv_m, void, env, i32, i32, i32)

DEF_HELPER_4(vnclipu_ivv, void, env, i32, i32, i32)
DEF_HELPER_4(vnclipu_ivv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vnclipu_ivi, void, env, i32, i32, tl)
DEF_HELPER_4(vnclipu_ivi_m, void, env, i32, i32, tl)
DEF_HELPER_4(vnclip_ivv, void, env, i32, i32, i32)
DEF_HELPER_4(vnclip_ivv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vnclip_ivi, void, env, i32, i32, tl)
DEF_HELPER_4(vnclip_ivi_m, void, env, i32, i32, tl)

DEF_HELPER_4(vcompress_mvv, void, env, i32, i32, i32)

DEF_HELPER_4(vwredsumu_ivv, void, env, i32, i32, i32)
DEF_HELPER_4(vwredsumu_ivv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vwredsum_ivv, void, env, i32, i32, i32)
DEF_HELPER_4(vwredsum_ivv_m, void, env, i32, i32, i32)

DEF_HELPER_4(vslideup_ivi, void, env, i32, i32, tl)
DEF_HELPER_4(vslideup_ivi_m, void, env, i32, i32, tl)
DEF_HELPER_4(vslidedown_ivi, void, env, i32, i32, tl)
DEF_HELPER_4(vslidedown_ivi_m, void, env, i32, i32, tl)
DEF_HELPER_4(vslide1up, void, env, i32, i32, i64)
DEF_HELPER_4(vslide1up_m, void, env, i32, i32, i64)
DEF_HELPER_4(vslide1down, void, env, i32, i32, i64)
DEF_HELPER_4(vslide1down_m, void, env, i32, i32, i64)

DEF_HELPER_4(vrgather_ivv, void, env, i32, i32, i32)
DEF_HELPER_4(vrgather_ivv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vrgatherei16_ivv, void, env, i32, i32, i32)
DEF_HELPER_4(vrgatherei16_ivv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vrgather_ivi, void, env, i32, i32, tl)
DEF_HELPER_4(vrgather_ivi_m, void, env, i32, i32, tl)

DEF_HELPER_3(vzext_vf8, void, env, i32, i32)
DEF_HELPER_3(vzext_vf8_m, void, env, i32, i32)
DEF_HELPER_3(vsext_vf8, void, env, i32, i32)
DEF_HELPER_3(vsext_vf8_m, void, env, i32, i32)
DEF_HELPER_3(vzext_vf4, void, env, i32, i32)
DEF_HELPER_3(vzext_vf4_m, void, env, i32, i32)
DEF_HELPER_3(vsext_vf4, void, env, i32, i32)
DEF_HELPER_3(vsext_vf4_m, void, env, i32, i32)
DEF_HELPER_3(vzext_vf2, void, env, i32, i32)
DEF_HELPER_3(vzext_vf2_m, void, env, i32, i32)
DEF_HELPER_3(vsext_vf2, void, env, i32, i32)
DEF_HELPER_3(vsext_vf2_m, void, env, i32, i32)

DEF_HELPER_4(vadc_vvm, void, env, i32, i32, i32)
DEF_HELPER_4(vmadc_vv, void, env, i32, i32, i32)
DEF_HELPER_4(vmadc_vvm, void, env, i32, i32, i32)
DEF_HELPER_4(vsbc_vvm, void, env, i32, i32, i32)
DEF_HELPER_4(vmsbc_vv, void, env, i32, i32, i32)
DEF_HELPER_4(vmsbc_vvm, void, env, i32, i32, i32)
DEF_HELPER_4(vadc_vi, void, env, i32, i32, tl)
DEF_HELPER_4(vmadc_vi, void, env, i32, i32, tl)
DEF_HELPER_4(vmadc_vim, void, env, i32, i32, tl)
DEF_HELPER_4(vsbc_vi, void, env, i32, i32, tl)
DEF_HELPER_4(vmsbc_vi, void, env, i32, i32, tl)
DEF_HELPER_4(vmsbc_vim, void, env, i32, i32, tl)

DEF_HELPER_4(vand_ivv, void, env, i32, i32, i32)
DEF_HELPER_4(vand_ivv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vor_ivv, void, env, i32, i32, i32)
DEF_HELPER_4(vor_ivv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vxor_ivv, void, env, i32, i32, i32)
DEF_HELPER_4(vxor_ivv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vand_ivi, void, env, i32, i32, tl)
DEF_HELPER_4(vand_ivi_m, void, env, i32, i32, tl)
DEF_HELPER_4(vor_ivi, void, env, i32, i32, tl)
DEF_HELPER_4(vor_ivi_m, void, env, i32, i32, tl)
DEF_HELPER_4(vxor_ivi, void, env, i32, i32, tl)
DEF_HELPER_4(vxor_ivi_m, void, env, i32, i32, tl)

DEF_HELPER_4(vsll_ivv, void, env, i32, i32, i32)
DEF_HELPER_4(vsll_ivv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vsrl_ivv, void, env, i32, i32, i32)
DEF_HELPER_4(vsrl_ivv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vsra_ivv, void, env, i32, i32, i32)
DEF_HELPER_4(vsra_ivv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vsll_ivi, void, env, i32, i32, tl)
DEF_HELPER_4(vsll_ivi_m, void, env, i32, i32, tl)
DEF_HELPER_4(vsrl_ivi, void, env, i32, i32, tl)
DEF_HELPER_4(vsrl_ivi_m, void, env, i32, i32, tl)
DEF_HELPER_4(vsra_ivi, void, env, i32, i32, tl)
DEF_HELPER_4(vsra_ivi_m, void, env, i32, i32, tl)

DEF_HELPER_4(vmseq_ivv, void, env, i32, i32, i32)
DEF_HELPER_4(vmseq_ivv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vmsne_ivv, void, env, i32, i32, i32)
DEF_HELPER_4(vmsne_ivv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vmsltu_ivv, void, env, i32, i32, i32)
DEF_HELPER_4(vmsltu_ivv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vmslt_ivv, void, env, i32, i32, i32)
DEF_HELPER_4(vmslt_ivv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vmsleu_ivv, void, env, i32, i32, i32)
DEF_HELPER_4(vmsleu_ivv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vmsle_ivv, void, env, i32, i32, i32)
DEF_HELPER_4(vmsle_ivv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vmseq_ivi, void, env, i32, i32, tl)
DEF_HELPER_4(vmseq_ivi_m, void, env, i32, i32, tl)
DEF_HELPER_4(vmsne_ivi, void, env, i32, i32, tl)
DEF_HELPER_4(vmsne_ivi_m, void, env, i32, i32, tl)
DEF_HELPER_4(vmsltu_ivi, void, env, i32, i32, tl)
DEF_HELPER_4(vmsltu_ivi_m, void, env, i32, i32, tl)
DEF_HELPER_4(vmslt_ivi, void, env, i32, i32, tl)
DEF_HELPER_4(vmslt_ivi_m, void, env, i32, i32, tl)
DEF_HELPER_4(vmsleu_ivi, void, env, i32, i32, tl)
DEF_HELPER_4(vmsleu_ivi_m, void, env, i32, i32, tl)
DEF_HELPER_4(vmsle_ivi, void, env, i32, i32, tl)
DEF_HELPER_4(vmsle_ivi_m, void, env, i32, i32, tl)
DEF_HELPER_4(vmsgtu_ivi, void, env, i32, i32, tl)
DEF_HELPER_4(vmsgtu_ivi_m, void, env, i32, i32, tl)
DEF_HELPER_4(vmsgt_ivi, void, env, i32, i32, tl)
DEF_HELPER_4(vmsgt_ivi_m, void, env, i32, i32, tl)

DEF_HELPER_4(vdivu_mvv, void, env, i32, i32, i32)
DEF_HELPER_4(vdivu_mvv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vdiv_mvv, void, env, i32, i32, i32)
DEF_HELPER_4(vdiv_mvv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vremu_mvv, void, env, i32, i32, i32)
DEF_HELPER_4(vremu_mvv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vrem_mvv, void, env, i32, i32, i32)
DEF_HELPER_4(vrem_mvv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vdivu_mvx, void, env, i32, i32, tl)
DEF_HELPER_4(vdivu_mvx_m, void, env, i32, i32, tl)
DEF_HELPER_4(vdiv_mvx, void, env, i32, i32, tl)
DEF_HELPER_4(vdiv_mvx_m, void, env, i32, i32, tl)
DEF_HELPER_4(vremu_mvx, void, env, i32, i32, tl)
DEF_HELPER_4(vremu_mvx_m, void, env, i32, i32, tl)
DEF_HELPER_4(vrem_mvx, void, env, i32, i32, tl)
DEF_HELPER_4(vrem_mvx_m, void, env, i32, i32, tl)

DEF_HELPER_4(vmacc_mvv, void, env, i32, i32, i32)
DEF_HELPER_4(vmacc_mvv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vmacc_mvx, void, env, i32, i32, tl)
DEF_HELPER_4(vmacc_mvx_m, void, env, i32, i32, tl)
DEF_HELPER_4(vnmsac_mvv, void, env, i32, i32, i32)
DEF_HELPER_4(vnmsac_mvv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vnmsac_mvx, void, env, i32, i32, tl)
DEF_HELPER_4(vnmsac_mvx_m, void, env, i32, i32, tl)
DEF_HELPER_4(vmadd_mvv, void, env, i32, i32, i32)
DEF_HELPER_4(vmadd_mvv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vmadd_mvx, void, env, i32, i32, tl)
DEF_HELPER_4(vmadd_mvx_m, void, env, i32, i32, tl)
DEF_HELPER_4(vnmsub_mvv, void, env, i32, i32, i32)
DEF_HELPER_4(vnmsub_mvv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vnmsub_mvx, void, env, i32, i32, tl)
DEF_HELPER_4(vnmsub_mvx_m, void, env, i32, i32, tl)

DEF_HELPER_4(vwmaccu_mvv, void, env, i32, i32, i32)
DEF_HELPER_4(vwmaccu_mvv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vwmaccu_mvx, void, env, i32, i32, tl)
DEF_HELPER_4(vwmaccu_mvx_m, void, env, i32, i32, tl)
DEF_HELPER_4(vwmacc_mvv, void, env, i32, i32, i32)
DEF_HELPER_4(vwmacc_mvv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vwmacc_mvx, void, env, i32, i32, tl)
DEF_HELPER_4(vwmacc_mvx_m, void, env, i32, i32, tl)
DEF_HELPER_4(vwmaccsu_mvv, void, env, i32, i32, i32)
DEF_HELPER_4(vwmaccsu_mvv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vwmaccsu_mvx, void, env, i32, i32, tl)
DEF_HELPER_4(vwmaccsu_mvx_m, void, env, i32, i32, tl)
DEF_HELPER_4(vwmaccus_mvx, void, env, i32, i32, tl)
DEF_HELPER_4(vwmaccus_mvx_m, void, env, i32, i32, tl)

DEF_HELPER_4(vmerge_ivv, void, env, i32, i32, i32)
DEF_HELPER_4(vmerge_ivi, void, env, i32, i32, tl)
DEF_HELPER_4(vfmerge_vfm, void, env, i32, i32, i64)

DEF_HELPER_4(vsaddu_ivv, void, env, i32, i32, i32)
DEF_HELPER_4(vsaddu_ivv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vsaddu_ivi, void, env, i32, i32, tl)
DEF_HELPER_4(vsaddu_ivi_m, void, env, i32, i32, tl)
DEF_HELPER_4(vsadd_ivv, void, env, i32, i32, i32)
DEF_HELPER_4(vsadd_ivv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vsadd_ivi, void, env, i32, i32, tl)
DEF_HELPER_4(vsadd_ivi_m, void, env, i32, i32, tl)
DEF_HELPER_4(vssubu_ivv, void, env, i32, i32, i32)
DEF_HELPER_4(vssubu_ivv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vssubu_ivi, void, env, i32, i32, tl)
DEF_HELPER_4(vssubu_ivi_m, void, env, i32, i32, tl)
DEF_HELPER_4(vssub_ivv, void, env, i32, i32, i32)
DEF_HELPER_4(vssub_ivv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vssub_ivi, void, env, i32, i32, tl)
DEF_HELPER_4(vssub_ivi_m, void, env, i32, i32, tl)

DEF_HELPER_4(vaadd_mvv, void, env, i32, i32, i32)
DEF_HELPER_4(vaadd_mvv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vaadd_mvx, void, env, i32, i32, tl)
DEF_HELPER_4(vaadd_mvx_m, void, env, i32, i32, tl)
DEF_HELPER_4(vaaddu_mvv, void, env, i32, i32, i32)
DEF_HELPER_4(vaaddu_mvv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vaaddu_mvx, void, env, i32, i32, tl)
DEF_HELPER_4(vaaddu_mvx_m, void, env, i32, i32, tl)
DEF_HELPER_4(vasub_mvv, void, env, i32, i32, i32)
DEF_HELPER_4(vasub_mvv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vasub_mvx, void, env, i32, i32, tl)
DEF_HELPER_4(vasub_mvx_m, void, env, i32, i32, tl)
DEF_HELPER_4(vasubu_mvv, void, env, i32, i32, i32)
DEF_HELPER_4(vasubu_mvv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vasubu_mvx, void, env, i32, i32, tl)
DEF_HELPER_4(vasubu_mvx_m, void, env, i32, i32, tl)

DEF_HELPER_4(vsmul_ivv, void, env, i32, i32, i32)
DEF_HELPER_4(vsmul_ivv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vsmul_ivx, void, env, i32, i32, tl)
DEF_HELPER_4(vsmul_ivx_m, void, env, i32, i32, tl)

DEF_HELPER_4(vssrl_ivv, void, env, i32, i32, i32)
DEF_HELPER_4(vssrl_ivv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vssrl_ivi, void, env, i32, i32, tl)
DEF_HELPER_4(vssrl_ivi_m, void, env, i32, i32, tl)
DEF_HELPER_4(vssra_ivv, void, env, i32, i32, i32)
DEF_HELPER_4(vssra_ivv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vssra_ivi, void, env, i32, i32, tl)
DEF_HELPER_4(vssra_ivi_m, void, env, i32, i32, tl)

DEF_HELPER_4(vredsum_vs, void, env, i32, i32, i32)
DEF_HELPER_4(vredsum_vs_m, void, env, i32, i32, i32)
DEF_HELPER_4(vredmaxu_vs, void, env, i32, i32, i32)
DEF_HELPER_4(vredmaxu_vs_m, void, env, i32, i32, i32)
DEF_HELPER_4(vredmax_vs, void, env, i32, i32, i32)
DEF_HELPER_4(vredmax_vs_m, void, env, i32, i32, i32)
DEF_HELPER_4(vredminu_vs, void, env, i32, i32, i32)
DEF_HELPER_4(vredminu_vs_m, void, env, i32, i32, i32)
DEF_HELPER_4(vredmin_vs, void, env, i32, i32, i32)
DEF_HELPER_4(vredmin_vs_m, void, env, i32, i32, i32)
DEF_HELPER_4(vredand_vs, void, env, i32, i32, i32)
DEF_HELPER_4(vredand_vs_m, void, env, i32, i32, i32)
DEF_HELPER_4(vredor_vs, void, env, i32, i32, i32)
DEF_HELPER_4(vredor_vs_m, void, env, i32, i32, i32)
DEF_HELPER_4(vredxor_vs, void, env, i32, i32, i32)
DEF_HELPER_4(vredxor_vs_m, void, env, i32, i32, i32)

DEF_HELPER_2(vmv_xs, tl, env, i32)
DEF_HELPER_3(vmv_sx, void, env, i32, tl)
DEF_HELPER_3(vfmv_vf, void, env, i32, i64)

DEF_HELPER_3(vmv1r_v, void, env, i32, i32)
DEF_HELPER_3(vmv2r_v, void, env, i32, i32)
DEF_HELPER_3(vmv4r_v, void, env, i32, i32)
DEF_HELPER_3(vmv8r_v, void, env, i32, i32)

DEF_HELPER_4(vmand_mm, void, env, i32, i32, i32)
DEF_HELPER_4(vmnand_mm, void, env, i32, i32, i32)
DEF_HELPER_4(vmandnot_mm, void, env, i32, i32, i32)
DEF_HELPER_4(vmxor_mm, void, env, i32, i32, i32)
DEF_HELPER_4(vmor_mm, void, env, i32, i32, i32)
DEF_HELPER_4(vmnor_mm, void, env, i32, i32, i32)
DEF_HELPER_4(vmornot_mm, void, env, i32, i32, i32)
DEF_HELPER_4(vmxnor_mm, void, env, i32, i32, i32)

DEF_HELPER_2(vpopc, tl, env, i32)
DEF_HELPER_2(vpopc_m, tl, env, i32)
DEF_HELPER_2(vfirst, tl, env, i32)
DEF_HELPER_2(vfirst_m, tl, env, i32)
DEF_HELPER_3(vmsbf, void, env, i32, i32)
DEF_HELPER_3(vmsbf_m, void, env, i32, i32)
DEF_HELPER_3(vmsif, void, env, i32, i32)
DEF_HELPER_3(vmsif_m, void, env, i32, i32)
DEF_HELPER_3(vmsof, void, env, i32, i32)
DEF_HELPER_3(vmsof_m, void, env, i32, i32)

DEF_HELPER_3(viota, void, env, i32, i32)
DEF_HELPER_3(viota_m, void, env, i32, i32)

DEF_HELPER_2(vid, void, env, i32)
DEF_HELPER_2(vid_m, void, env, i32)

DEF_HELPER_4(vfadd_vv, void, env, i32, i32, i32)
DEF_HELPER_4(vfadd_vv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vfadd_vf, void, env, i32, i32, i64)
DEF_HELPER_4(vfadd_vf_m, void, env, i32, i32, i64)
DEF_HELPER_4(vfsub_vv, void, env, i32, i32, i32)
DEF_HELPER_4(vfsub_vv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vfsub_vf, void, env, i32, i32, i64)
DEF_HELPER_4(vfsub_vf_m, void, env, i32, i32, i64)
DEF_HELPER_4(vfrsub_vf, void, env, i32, i32, i64)
DEF_HELPER_4(vfrsub_vf_m, void, env, i32, i32, i64)

DEF_HELPER_4(vfwadd_vv, void, env, i32, i32, i32)
DEF_HELPER_4(vfwadd_vv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vfwadd_vf, void, env, i32, i32, i64)
DEF_HELPER_4(vfwadd_vf_m, void, env, i32, i32, i64)
DEF_HELPER_4(vfwadd_wv, void, env, i32, i32, i32)
DEF_HELPER_4(vfwadd_wv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vfwadd_wf, void, env, i32, i32, i64)
DEF_HELPER_4(vfwadd_wf_m, void, env, i32, i32, i64)
DEF_HELPER_4(vfwsub_vv, void, env, i32, i32, i32)
DEF_HELPER_4(vfwsub_vv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vfwsub_vf, void, env, i32, i32, i64)
DEF_HELPER_4(vfwsub_vf_m, void, env, i32, i32, i64)
DEF_HELPER_4(vfwsub_wv, void, env, i32, i32, i32)
DEF_HELPER_4(vfwsub_wv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vfwsub_wf, void, env, i32, i32, i64)
DEF_HELPER_4(vfwsub_wf_m, void, env, i32, i32, i64)

DEF_HELPER_4(vfmul_vv, void, env, i32, i32, i32)
DEF_HELPER_4(vfmul_vv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vfmul_vf, void, env, i32, i32, i64)
DEF_HELPER_4(vfmul_vf_m, void, env, i32, i32, i64)
DEF_HELPER_4(vfdiv_vv, void, env, i32, i32, i32)
DEF_HELPER_4(vfdiv_vv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vfdiv_vf, void, env, i32, i32, i64)
DEF_HELPER_4(vfdiv_vf_m, void, env, i32, i32, i64)
DEF_HELPER_4(vfrdiv_vf, void, env, i32, i32, i64)
DEF_HELPER_4(vfrdiv_vf_m, void, env, i32, i32, i64)

DEF_HELPER_4(vfwmul_vv, void, env, i32, i32, i32)
DEF_HELPER_4(vfwmul_vv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vfwmul_vf, void, env, i32, i32, i64)
DEF_HELPER_4(vfwmul_vf_m, void, env, i32, i32, i64)

DEF_HELPER_4(vfmacc_vv, void, env, i32, i32, i32)
DEF_HELPER_4(vfmacc_vv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vfmacc_vf, void, env, i32, i32, i64)
DEF_HELPER_4(vfmacc_vf_m, void, env, i32, i32, i64)
DEF_HELPER_4(vfnmacc_vv, void, env, i32, i32, i32)
DEF_HELPER_4(vfnmacc_vv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vfnmacc_vf, void, env, i32, i32, i64)
DEF_HELPER_4(vfnmacc_vf_m, void, env, i32, i32, i64)
DEF_HELPER_4(vfmsac_vv, void, env, i32, i32, i32)
DEF_HELPER_4(vfmsac_vv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vfmsac_vf, void, env, i32, i32, i64)
DEF_HELPER_4(vfmsac_vf_m, void, env, i32, i32, i64)
DEF_HELPER_4(vfnmsac_vv, void, env, i32, i32, i32)
DEF_HELPER_4(vfnmsac_vv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vfnmsac_vf, void, env, i32, i32, i64)
DEF_HELPER_4(vfnmsac_vf_m, void, env, i32, i32, i64)
DEF_HELPER_4(vfmadd_vv, void, env, i32, i32, i32)
DEF_HELPER_4(vfmadd_vv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vfmadd_vf, void, env, i32, i32, i64)
DEF_HELPER_4(vfmadd_vf_m, void, env, i32, i32, i64)
DEF_HELPER_4(vfnmadd_vv, void, env, i32, i32, i32)
DEF_HELPER_4(vfnmadd_vv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vfnmadd_vf, void, env, i32, i32, i64)
DEF_HELPER_4(vfnmadd_vf_m, void, env, i32, i32, i64)
DEF_HELPER_4(vfmsub_vv, void, env, i32, i32, i32)
DEF_HELPER_4(vfmsub_vv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vfmsub_vf, void, env, i32, i32, i64)
DEF_HELPER_4(vfmsub_vf_m, void, env, i32, i32, i64)
DEF_HELPER_4(vfnmsub_vv, void, env, i32, i32, i32)
DEF_HELPER_4(vfnmsub_vv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vfnmsub_vf, void, env, i32, i32, i64)
DEF_HELPER_4(vfnmsub_vf_m, void, env, i32, i32, i64)

DEF_HELPER_4(vfwmacc_vv, void, env, i32, i32, i32)
DEF_HELPER_4(vfwmacc_vv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vfwmacc_vf, void, env, i32, i32, i64)
DEF_HELPER_4(vfwmacc_vf_m, void, env, i32, i32, i64)
DEF_HELPER_4(vfwnmacc_vv, void, env, i32, i32, i32)
DEF_HELPER_4(vfwnmacc_vv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vfwnmacc_vf, void, env, i32, i32, i64)
DEF_HELPER_4(vfwnmacc_vf_m, void, env, i32, i32, i64)
DEF_HELPER_4(vfwmsac_vv, void, env, i32, i32, i32)
DEF_HELPER_4(vfwmsac_vv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vfwmsac_vf, void, env, i32, i32, i64)
DEF_HELPER_4(vfwmsac_vf_m, void, env, i32, i32, i64)
DEF_HELPER_4(vfwnmsac_vv, void, env, i32, i32, i32)
DEF_HELPER_4(vfwnmsac_vv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vfwnmsac_vf, void, env, i32, i32, i64)
DEF_HELPER_4(vfwnmsac_vf_m, void, env, i32, i32, i64)

DEF_HELPER_3(vfsqrt_v, void, env, i32, i32)
DEF_HELPER_3(vfsqrt_v_m, void, env, i32, i32)

DEF_HELPER_4(vfmin_vv, void, env, i32, i32, i32)
DEF_HELPER_4(vfmin_vv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vfmin_vf, void, env, i32, i32, i64)
DEF_HELPER_4(vfmin_vf_m, void, env, i32, i32, i64)
DEF_HELPER_4(vfmax_vv, void, env, i32, i32, i32)
DEF_HELPER_4(vfmax_vv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vfmax_vf, void, env, i32, i32, i64)
DEF_HELPER_4(vfmax_vf_m, void, env, i32, i32, i64)

DEF_HELPER_4(vfsgnj_vv, void, env, i32, i32, i32)
DEF_HELPER_4(vfsgnj_vv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vfsgnj_vf, void, env, i32, i32, i64)
DEF_HELPER_4(vfsgnj_vf_m, void, env, i32, i32, i64)
DEF_HELPER_4(vfsgnjn_vv, void, env, i32, i32, i32)
DEF_HELPER_4(vfsgnjn_vv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vfsgnjn_vf, void, env, i32, i32, i64)
DEF_HELPER_4(vfsgnjn_vf_m, void, env, i32, i32, i64)
DEF_HELPER_4(vfsgnjx_vv, void, env, i32, i32, i32)
DEF_HELPER_4(vfsgnjx_vv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vfsgnjx_vf, void, env, i32, i32, i64)
DEF_HELPER_4(vfsgnjx_vf_m, void, env, i32, i32, i64)

DEF_HELPER_4(vfeq_vv, void, env, i32, i32, i32)
DEF_HELPER_4(vfeq_vv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vfeq_vf, void, env, i32, i32, i64)
DEF_HELPER_4(vfeq_vf_m, void, env, i32, i32, i64)
DEF_HELPER_4(vfne_vv, void, env, i32, i32, i32)
DEF_HELPER_4(vfne_vv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vfne_vf, void, env, i32, i32, i64)
DEF_HELPER_4(vfne_vf_m, void, env, i32, i32, i64)
DEF_HELPER_4(vflt_vv, void, env, i32, i32, i32)
DEF_HELPER_4(vflt_vv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vflt_vf, void, env, i32, i32, i64)
DEF_HELPER_4(vflt_vf_m, void, env, i32, i32, i64)
DEF_HELPER_4(vfle_vv, void, env, i32, i32, i32)
DEF_HELPER_4(vfle_vv_m, void, env, i32, i32, i32)
DEF_HELPER_4(vfle_vf, void, env, i32, i32, i64)
DEF_HELPER_4(vfle_vf_m, void, env, i32, i32, i64)
DEF_HELPER_4(vfgt_vf, void, env, i32, i32, i64)
DEF_HELPER_4(vfgt_vf_m, void, env, i32, i32, i64)
DEF_HELPER_4(vfge_vf, void, env, i32, i32, i64)
DEF_HELPER_4(vfge_vf_m, void, env, i32, i32, i64)

DEF_HELPER_3(vfclass_v, void, env, i32, i32)
DEF_HELPER_3(vfclass_v_m, void, env, i32, i32)

DEF_HELPER_3(vfcvt_xuf_v, void, env, i32, i32)
DEF_HELPER_3(vfcvt_xuf_v_m, void, env, i32, i32)
DEF_HELPER_3(vfcvt_xf_v, void, env, i32, i32)
DEF_HELPER_3(vfcvt_xf_v_m, void, env, i32, i32)
DEF_HELPER_3(vfcvt_rtz_xuf_v, void, env, i32, i32)
DEF_HELPER_3(vfcvt_rtz_xuf_v_m, void, env, i32, i32)
DEF_HELPER_3(vfcvt_rtz_xf_v, void, env, i32, i32)
DEF_HELPER_3(vfcvt_rtz_xf_v_m, void, env, i32, i32)
DEF_HELPER_3(vfcvt_fxu_v, void, env, i32, i32)
DEF_HELPER_3(vfcvt_fxu_v_m, void, env, i32, i32)
DEF_HELPER_3(vfcvt_fx_v, void, env, i32, i32)
DEF_HELPER_3(vfcvt_fx_v_m, void, env, i32, i32)

DEF_HELPER_3(vfwcvt_xuf_v, void, env, i32, i32)
DEF_HELPER_3(vfwcvt_xuf_v_m, void, env, i32, i32)
DEF_HELPER_3(vfwcvt_xf_v, void, env, i32, i32)
DEF_HELPER_3(vfwcvt_xf_v_m, void, env, i32, i32)
DEF_HELPER_3(vfwcvt_rtz_xuf_v, void, env, i32, i32)
DEF_HELPER_3(vfwcvt_rtz_xuf_v_m, void, env, i32, i32)
DEF_HELPER_3(vfwcvt_rtz_xf_v, void, env, i32, i32)
DEF_HELPER_3(vfwcvt_rtz_xf_v_m, void, env, i32, i32)
DEF_HELPER_3(vfwcvt_fxu_v, void, env, i32, i32)
DEF_HELPER_3(vfwcvt_fxu_v_m, void, env, i32, i32)
DEF_HELPER_3(vfwcvt_fx_v, void, env, i32, i32)
DEF_HELPER_3(vfwcvt_fx_v_m, void, env, i32, i32)
DEF_HELPER_3(vfwcvt_ff_v, void, env, i32, i32)
DEF_HELPER_3(vfwcvt_ff_v_m, void, env, i32, i32)

DEF_HELPER_3(vfncvt_xuf_w, void, env, i32, i32)
DEF_HELPER_3(vfncvt_xuf_w_m, void, env, i32, i32)
DEF_HELPER_3(vfncvt_xf_w, void, env, i32, i32)
DEF_HELPER_3(vfncvt_xf_w_m, void, env, i32, i32)
DEF_HELPER_3(vfncvt_rtz_xuf_w, void, env, i32, i32)
DEF_HELPER_3(vfncvt_rtz_xuf_w_m, void, env, i32, i32)
DEF_HELPER_3(vfncvt_rtz_xf_w, void, env, i32, i32)
DEF_HELPER_3(vfncvt_rtz_xf_w_m, void, env, i32, i32)
DEF_HELPER_3(vfncvt_fxu_w, void, env, i32, i32)
DEF_HELPER_3(vfncvt_fxu_w_m, void, env, i32, i32)
DEF_HELPER_3(vfncvt_fx_w, void, env, i32, i32)
DEF_HELPER_3(vfncvt_fx_w_m, void, env, i32, i32)
DEF_HELPER_3(vfncvt_ff_w, void, env, i32, i32)
DEF_HELPER_3(vfncvt_ff_w_m, void, env, i32, i32)
DEF_HELPER_3(vfncvt_rod_ff_w, void, env, i32, i32)
DEF_HELPER_3(vfncvt_rod_ff_w_m, void, env, i32, i32)

DEF_HELPER_4(vfwredosum_vs, void, env, i32, i32, i32)
DEF_HELPER_4(vfwredosum_vs_m, void, env, i32, i32, i32)

DEF_HELPER_2(handle_custom_instruction, i32, i64, i64)
DEF_HELPER_3(vfmv_fs, void, env, i32, i32)
DEF_HELPER_3(vfmv_sf, void, env, i32, i64)

void do_nmi(CPUState *env);

#include "def-helper.h"
