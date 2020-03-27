#ifndef _PGTABLES_H_
#define _PGTABLES_H

extern uint64_t pgtable_lv2[];

#define VTCR_T0SZ_VALUE 32
#define VTCR_T0SZ_SHIFT 0
#define VTCR_SL0_VALUE 1
#define VTCR_SL0_SHIFT 6
#define VTCR_TG0_VALUE 2
#define VTCR_TG0_SHIFT 14
#define VTCR_PS_VALUE 0
#define VTCR_PS_SHIFT 16
#define VTCR_RES1 0x80000000

#define PTE_MT_S2_NORMAL 0xf
#define PTE_MT_S2_DEVICE_nGnRE 0x1
#define PTE_AP_S2_RW 0x3

#define PTE_VALID (1 << 0)
#define PTE_BLOCK (1 << 1)
#define PTE_MT_S2_SHIFT 2
#define PTE_AP_S2_SHIFT 6
#define PTE_ACCESS_FLAG (1 << 10)

#define VTCR_VALUE (VTCR_RES1 | \
		    (VTCR_T0SZ_VALUE << VTCR_T0SZ_SHIFT) | \
		    (VTCR_SL0_VALUE << VTCR_SL0_SHIFT) | \
		    (VTCR_TG0_VALUE << VTCR_TG0_SHIFT) | \
		    (VTCR_PS_VALUE << VTCR_PS_SHIFT))

#define VTTBR_VALUE (((uint64_t) pgtable_lv2) & 0x00007fffffffffffUL)

void setup_pgtables();

#endif
