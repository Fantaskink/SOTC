#ifndef REGNAMES_H
#define REGNAMES_H

// MIPS Reg names
#define MIPS_REG_ZERO 0
#define MIPS_REG_AT 1
#define MIPS_REG_V0 2
#define MIPS_REG_V1 3
#define MIPS_REG_A0 4
#define MIPS_REG_A1 5
#define MIPS_REG_A2 6
#define MIPS_REG_A3 7
#define MIPS_REG_T0 8
#define MIPS_REG_T1 9
#define MIPS_REG_T2 10
#define MIPS_REG_T3 11
#define MIPS_REG_T4 12
#define MIPS_REG_T5 13
#define MIPS_REG_T6 14
#define MIPS_REG_T7 15
#define MIPS_REG_S0 16
#define MIPS_REG_S1 17
#define MIPS_REG_S2 18
#define MIPS_REG_S3 19
#define MIPS_REG_S4 20
#define MIPS_REG_S5 21
#define MIPS_REG_S6 22
#define MIPS_REG_S7 23
#define MIPS_REG_T8 24
#define MIPS_REG_T9 25
#define MIPS_REG_K0 26
#define MIPS_REG_K1 27
#define MIPS_REG_GP 28
#define MIPS_REG_SP 29
#define MIPS_REG_S8 30
#define MIPS_REG_FP 30
#define MIPS_REG_RA 31

// COP 0 Reg names
#define COP0_REG_INDEX      0
#define COP0_REG_RANDOM     1
#define COP0_REG_ENTRYLO0   2
#define COP0_REG_ENTRYLO1   3
#define COP0_REG_CONTEXT    4
#define COP0_REG_PAGEMASK   5
#define COP0_REG_WIRED      6
#define COP0_REG_I07        7
#define COP0_REG_BADVADDR   8
#define COP0_REG_COUNT      9
#define COP0_REG_ENTRYHI    10
#define COP0_REG_COMPARE    11
#define COP0_REG_STATUS     12
#define COP0_REG_CAUSE      13
#define COP0_REG_EPC        14
#define COP0_REG_PRID       15
#define COP0_REG_CONFIG     16
#define COP0_REG_I17        17
#define COP0_REG_I18        18
#define COP0_REG_I19        19
#define COP0_REG_I20        20
#define COP0_REG_I21        21
#define COP0_REG_I22        22
#define COP0_REG_I23        23
#define COP0_REG_DEBUG      24
#define COP0_REG_PERF       25
#define COP0_REG_I26        26
#define COP0_REG_I27        27
#define COP0_REG_TAGLO      28
#define COP0_REG_RAGHI      29
#define COP0_REG_ERROREPC   30
#define COP0_REG_I31        31

#define COP0_CAUSE_GET_EXCEPT_CODE(r) ((r >> 2) & 0xF)

#endif /* REGNAMES_H */
