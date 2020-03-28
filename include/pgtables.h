#ifndef _PGTABLES_H_
#define _PGTABLES_H

/*
 * This project uses 64KiB granule, 2 level page tables.
 *
 * Level 0 doesn't exist when 64KiB granule, Level 1 is omitted, and level 2
 * has 8 items (for a 32-bit address space).
 *
 * Only the starting level 2 block has its internal structure (because it's
 * where the Allwinner I/O sits).
 */
extern uint64_t pgtable_lv2[];

#define VTCR_T0SZ_VALUE 32
#define VTCR_T0SZ_SHIFT 0
#define VTCR_SL0_VALUE 1
#define VTCR_SL0_SHIFT 6
#define VTCR_IRGN0_VALUE 1
#define VTCR_IRGN0_SHIFT 8
#define VTCR_ORGN0_VALUE 1
#define VTCR_ORGN0_SHIFT 10
#define VTCR_SH0_VALUE 3
#define VTCR_SH0_SHIFT 12
#define VTCR_TG0_VALUE 1
#define VTCR_TG0_SHIFT 14
#define VTCR_PS_VALUE 0
#define VTCR_PS_SHIFT 16
#define VTCR_RES1 0x80000000

#define PTE_MT_S2_NORMAL 0xf
#define PTE_MT_S2_DEVICE_nGnRE 0x1
#define PTE_AP_S2_RW 0x3

#define PTE_VALID (1 << 0)
#define PTE_BLOCK (0 << 1)
#define PTE_TABLE (1 << 1)
#define PTE_PAGE (1 << 1)
#define PTE_MT_S2_SHIFT 2
#define PTE_AP_S2_SHIFT 6
#define PTE_SH_INNER (3 << 8)
#define PTE_ACCESS_FLAG (1 << 10)

#define VTCR_VALUE (VTCR_RES1 | \
		    (VTCR_T0SZ_VALUE << VTCR_T0SZ_SHIFT) | \
		    (VTCR_SL0_VALUE << VTCR_SL0_SHIFT) | \
		    (VTCR_IRGN0_VALUE << VTCR_IRGN0_SHIFT) | \
		    (VTCR_ORGN0_VALUE << VTCR_ORGN0_SHIFT) | \
		    (VTCR_SH0_VALUE << VTCR_SH0_SHIFT) | \
		    (VTCR_TG0_VALUE << VTCR_TG0_SHIFT) | \
		    (VTCR_PS_VALUE << VTCR_PS_SHIFT))

void setup_pgtables();

#endif
