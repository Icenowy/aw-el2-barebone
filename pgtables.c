#include SOC_HEADER
#include <stdint.h>
#include "pgtables.h"

uint64_t pgtable_lv2[8] __attribute__((aligned(64)));
uint64_t pgtable_lv3_blk0[8192] __attribute__((aligned(65536)));
uint64_t pgtable_lv3_blk2[8192] __attribute__((aligned(65536)));

void init_pgtables()
{
	/* TODO: Set up pgtable_lv3_blk0 */
	for (uint64_t i = 0; i < 2; i++) {
		pgtable_lv2[i] = (i << 29) | PTE_VALID | PTE_BLOCK |
				 (PTE_MT_S2_DEVICE_nGnRE << PTE_MT_S2_SHIFT) |
				 (PTE_AP_S2_RW << PTE_AP_S2_SHIFT) |
				 PTE_ACCESS_FLAG | PTE_SH_INNER;
	}
	for (uint64_t i = 3; i < 8; i++) {
		pgtable_lv2[i] = (i << 29) | PTE_VALID | PTE_BLOCK |
				 (PTE_MT_S2_NORMAL << PTE_MT_S2_SHIFT) |
				 (PTE_AP_S2_RW << PTE_AP_S2_SHIFT) |
				 PTE_ACCESS_FLAG | PTE_SH_INNER;
	}

	for (uint64_t i = 0; i < 8192; i++) {
		uint64_t pgaddr = (2 << 29) | (i << 16);
		pgtable_lv3_blk2[i] = pgaddr | PTE_VALID | PTE_PAGE |
				 (PTE_MT_S2_NORMAL << PTE_MT_S2_SHIFT) |
				 PTE_ACCESS_FLAG | PTE_SH_INNER;

		if (pgaddr >= SOC_EL2_RESV_START &&
		    pgaddr < (SOC_EL2_RESV_START + SOC_EL2_RESV_SIZE))
			pgtable_lv3_blk2[i] |= (PTE_AP_S2_RO << PTE_AP_S2_SHIFT);
		else
			pgtable_lv3_blk2[i] |= (PTE_AP_S2_RW << PTE_AP_S2_SHIFT);
	}

	pgtable_lv2[2] = (uint64_t) pgtable_lv3_blk2 | PTE_VALID | PTE_TABLE;
}

void install_pgtables()
{
	asm volatile("msr vtcr_el2, %0" : : "r" (VTCR_VALUE) : "cc");	
	asm volatile("msr vttbr_el2, %0" : : "r" (pgtable_lv2) : "cc");
	asm volatile("isb");
}
