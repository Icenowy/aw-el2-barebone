#include SOC_HEADER
#include "trapped_funcs.h"
#include "uart.h"
#include "panic.h"
#include "init.h"

extern void _secondary_start();

bool wrap_psci(struct pt_regs *pt_regs)
{
	if (pt_regs->regs[0] == 0xc4000003) {
		uart_puts(SOC_UART0, "PSCI CPU_ON caught.\n");

		/* Check target MPIDR */
		if (pt_regs->regs[1] > 4)
			panic("Unknown target CPU MPIDR when CPU_ON\n");

		uart_puts(SOC_UART0, "CPU ");
		uart_hexval(SOC_UART0, pt_regs->regs[1]);
		uart_puts(SOC_UART0, " EL1 entrypoint is ");
		uart_hexval(SOC_UART0, pt_regs->regs[2]);
		uart_puts(SOC_UART0, "\n");

		secondary_el1_ep[pt_regs->regs[1]] = (void *) pt_regs->regs[2];
		pt_regs->regs[2] = (uint64_t) _secondary_start;
	}

	asm volatile ("mov x0, %0\n"
		      "mov x1, %1\n"
		      "mov x2, %2\n"
		      "mov x3, %3\n"
		      "mov x4, %4\n"
		      "mov x5, %5\n"
		      "mov x6, %6\n"
		      "mov x7, %7\n"
		      "smc #0\n"
		      "mov %0, x0\n"
		      "mov %1, x1\n"
		      "mov %2, x2\n"
		      "mov %3, x3\n"
		      : "+r" (pt_regs->regs[0]),
		        "+r" (pt_regs->regs[1]),
		        "+r" (pt_regs->regs[2]),
		        "+r" (pt_regs->regs[3])
		      : "r" (pt_regs->regs[4]),
		        "r" (pt_regs->regs[5]),
		        "r" (pt_regs->regs[6]),
		        "r" (pt_regs->regs[7])
		      : "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "cc");

	pt_regs->elr += 4;
	return true;
}

bool process_low_sync(struct pt_regs *pt_regs, unsigned int esr)
{
	if ((esr & ESR_EC_MASK) == ESR_EC_SMC_64 && (esr & ESR_SMC_64_IMM_MASK) == 0) {
		/* wrapping PSCI */
		return wrap_psci(pt_regs);
	}
	return false;
}
