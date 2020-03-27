#include <stdint.h>
#include "pgtables.h"

uint64_t pgtable_lv2[8] __attribute__((aligned(64)));
uint64_t pgtable_lv3_blk0[8192];

void setup_pgtables()
{
	/* TODO: Set up pgtable_lv3_blk0 */
	for (int i = 0; i < 2; i++) {
		pgtable_lv2[i] = (i << 29) | PTE_VALID | PTE_BLOCK |
				 (PTE_MT_S2_DEVICE_nGnRE << PTE_MT_S2_SHIFT) |
				 (PTE_AP_S2_RW << PTE_AP_S2_SHIFT) |
				 PTE_ACCESS_FLAG;
	}
	for (int i = 2; i < 8; i++) {
		pgtable_lv2[i] = (i << 29) | PTE_VALID | PTE_BLOCK |
				 (PTE_MT_S2_NORMAL << PTE_MT_S2_SHIFT) |
				 (PTE_AP_S2_RW << PTE_AP_S2_SHIFT) |
				 PTE_ACCESS_FLAG;
	}

//	pgtable_lv2[7] = 0;

	asm volatile("msr vtcr_el2, %0" : : "r" (VTCR_VALUE) : "cc");	
	asm volatile("msr vttbr_el2, %0" : : "r" (VTTBR_VALUE) : "cc");
	asm volatile("isb");
}
