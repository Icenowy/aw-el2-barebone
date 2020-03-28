#include SOC_HEADER

#include <asm/system.h>

.globl _start

_start:
	mov x1, #1
	msr spsel, x1
	ldr x1, =stack_generic
	mov sp, x1

	ldr x1, =vectors
	msr vbar_el2, x1

	bl init

	ldr x1, =SOC_NEXT_STAGE
	cbz x0, jump_to_el1
	/* When main() returns non-zero, just go */
fail:	br x1

jump_to_el1:
	/* Initialize Generic Timers */
	mrs	x6, cnthctl_el2
	/* Enable EL1 access to timers */
	orr	x6, x6, #(CNTHCTL_EL2_EL1PCEN_EN |\
		CNTHCTL_EL2_EL1PCTEN_EN)
	msr	cnthctl_el2, x6
	msr	cntvoff_el2, xzr

	/* Initilize MPID/MPIDR registers */
	mrs	x6, midr_el1
	msr	vpidr_el2, x6
	mrs	x6, mpidr_el1
	msr	vmpidr_el2, x6

	/* Disable coprocessor traps */
	mov	x6, #CPTR_EL2_RES1
	msr	cptr_el2, x6		/* Disable coprocessor traps to EL2 */
	msr	hstr_el2, xzr		/* Disable coprocessor traps to EL2 */
	mov	x6, #CPACR_EL1_FPEN_EN
	msr	cpacr_el1, x6		/* Enable FP/SIMD at EL1 */

	/* SCTLR_EL1 initialization
	 *
	 * setting RES1 bits (29,28,23,22,20,11) to 1
	 * and RES0 bits (31,30,27,21,17,13,10,6) +
	 * UCI,EE,EOE,WXN,nTWE,nTWI,UCT,DZE,I,UMA,SED,ITD,
	 * CP15BEN,SA0,SA,C,A,M to 0
	 */
	ldr	x6, =(SCTLR_EL1_RES1 | SCTLR_EL1_UCI_DIS |\
			SCTLR_EL1_EE_LE | SCTLR_EL1_WXN_DIS |\
			SCTLR_EL1_NTWE_DIS | SCTLR_EL1_NTWI_DIS |\
			SCTLR_EL1_UCT_DIS | SCTLR_EL1_DZE_DIS |\
			SCTLR_EL1_ICACHE_DIS | SCTLR_EL1_UMA_DIS |\
			SCTLR_EL1_SED_EN | SCTLR_EL1_ITD_EN |\
			SCTLR_EL1_CP15BEN_DIS | SCTLR_EL1_SA0_DIS |\
			SCTLR_EL1_SA_DIS | SCTLR_EL1_DCACHE_DIS |\
			SCTLR_EL1_ALIGN_DIS | SCTLR_EL1_MMU_DIS)
	msr	sctlr_el1, x6

	mrs	x6, vbar_el2
	msr	vbar_el1, x6		/* Migrate VBAR */

	/* Initialize HCR_EL2 */
	ldr	x6, =(HCR_EL2_RW_AARCH64 | HCR_EL2_HCD_DIS | HCR_EL2_TSC_ENABLE | HCR_EL2_VM_ENABLE)
	msr	hcr_el2, x6

	/* Return to the EL1_SP1 mode from EL2 */
	ldr	x6, =(SPSR_EL_DEBUG_MASK | SPSR_EL_SERR_MASK |\
			SPSR_EL_IRQ_MASK | SPSR_EL_FIQ_MASK |\
			SPSR_EL_M_AARCH64 | SPSR_EL_M_EL1H)
	msr	spsr_el2, x6
	msr     elr_el2, x1
	eret

.global _secondary_start
_secondary_start:
	mov x1, #1
	msr spsel, x1

	mrs x1, mpidr_el1
	and x1, x1, #0x00ffffff
	cmp x1, #0
	beq 1f
	cmp x1, #1
	beq 2f
	cmp x1, #2
	beq 3f
	cmp x1, #3
	beq 4f
spin:	b spin
1:	ldr x1, =stack_generic
	b 6f
2:	ldr x1, =stack_generic_core1
	b 6f
3:	ldr x1, =stack_generic_core2
	b 6f
4:	ldr x1, =stack_generic_core3
	b 6f
6:	mov sp, x1

	ldr x1, =vectors
	msr vbar_el2, x1

	stp x0, x2, [sp, #-16]!
	bl secondary_init
	cbz x0, spin
	mov x1, x0
	ldp x0, x2, [sp], #16
	b jump_to_el1
