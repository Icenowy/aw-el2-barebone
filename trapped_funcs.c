#include SOC_HEADER
#include "trapped_funcs.h"
#include "uart.h"
#include "panic.h"
#include "init.h"
#include "pgtables.h"
#include "hole.h"

extern void _secondary_start();

spinlock cutpage_lock;

static bool wrap_psci(struct pt_regs *pt_regs)
{
	if (pt_regs->regs[0] == 0xc4000003) {
		uart_puts_debug(SOC_UART0, "PSCI CPU_ON caught.\n");

		/* Check target MPIDR */
		if (pt_regs->regs[1] > 4)
			panic("Unknown target CPU MPIDR when CPU_ON\n");

		uart_puts_debug(SOC_UART0, "CPU ");
		uart_hexval_debug(SOC_UART0, pt_regs->regs[1]);
		uart_puts_debug(SOC_UART0, " EL1 entrypoint is ");
		uart_hexval_debug(SOC_UART0, pt_regs->regs[2]);
		uart_puts_debug(SOC_UART0, "\n");

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

static bool cutpage_io(struct pt_regs *pt_regs, unsigned int esr, uint64_t ipa)
{
	spinlock_lock(&cutpage_lock);

	uint32_t page = (uint32_t) (ipa >> 16);
	uint64_t local_addr = ipa & 0xffff;
	uint64_t real_addr = HOLE_REAL_ADDR | local_addr;

	if (!(esr & ESR_DATA_ABORT_ISV)) {
		spinlock_unlock(&cutpage_lock);
		return false;
	}

	writel(page, HOLE_PAGE_REG);

	uint8_t *pb = (uint8_t *) real_addr;
	uint16_t *phw = (uint16_t *) real_addr;
	uint32_t *pw = (uint32_t *) real_addr;
	uint64_t *pdw = (uint64_t *) real_addr;

	uint64_t *reg = &pt_regs->regs[(esr & ESR_DATA_ABORT_SRT_MASK) >> ESR_DATA_ABORT_SRT_SHIFT];
	if (esr & ESR_DATA_ABORT_WnR) {
		/* Write to memory */
		uint64_t data = *reg;
		if ((esr & ESR_DATA_ABORT_SAS_MASK) == ESR_DATA_ABORT_SAS_B)
			*pb = (uint8_t) data;
		else if ((esr & ESR_DATA_ABORT_SAS_MASK) == ESR_DATA_ABORT_SAS_HW)
			*phw = (uint16_t) data;
		else if ((esr & ESR_DATA_ABORT_SAS_MASK) == ESR_DATA_ABORT_SAS_W)
			*pw = (uint32_t) data;
		else
			*pdw = (uint64_t) data;
	} else {
		/* Read from memory */
		if ((esr & ESR_DATA_ABORT_SAS_MASK) == ESR_DATA_ABORT_SAS_B) {
			uint8_t data = *pb;
			if (esr & ESR_DATA_ABORT_SSE) {
				if (esr & ESR_DATA_ABORT_SF)
					*((int64_t *) reg) = (int8_t) data;
				else
					*((int32_t *) reg) = (int8_t) data;
			} else {
				if (esr & ESR_DATA_ABORT_SF)
					*((uint64_t *) reg) = (uint8_t) data;
				else
					*((uint32_t *) reg) = (uint8_t) data;
			}
		} else if ((esr & ESR_DATA_ABORT_SAS_MASK) == ESR_DATA_ABORT_SAS_HW) {
			uint16_t data = *phw;
			if (esr & ESR_DATA_ABORT_SSE) {
				if (esr & ESR_DATA_ABORT_SF)
					*((int64_t *) reg) = (int16_t) data;
				else
					*((int32_t *) reg) = (int16_t) data;
			} else {
				if (esr & ESR_DATA_ABORT_SF)
					*((uint64_t *) reg) = (uint16_t) data;
				else
					*((uint32_t *) reg) = (uint16_t) data;
			}
		} else if ((esr & ESR_DATA_ABORT_SAS_MASK) == ESR_DATA_ABORT_SAS_W) {
			uint32_t data = *pw;
			if (esr & ESR_DATA_ABORT_SSE) {
				if (esr & ESR_DATA_ABORT_SF)
					*((int64_t *) reg) = (int32_t) data;
				else
					*((int32_t *) reg) = (int32_t) data;
			} else {
				if (esr & ESR_DATA_ABORT_SF)
					*((uint64_t *) reg) = (uint32_t) data;
				else
					*((uint32_t *) reg) = (uint32_t) data;
			}
		} else {
			uint64_t data = *pdw;
			*reg = data;
		}

		if (!(esr & ESR_DATA_ABORT_SF))
			*(((uint32_t *) reg) + 1) = 0;
	}
	spinlock_unlock(&cutpage_lock);

	pt_regs->elr += 4;
	return true;
}

static bool wrap_hole_io(struct pt_regs *pt_regs, unsigned int esr)
{
	uint64_t far;
	asm volatile("mrs %0, far_el2" : "=r" (far) : : "cc");
	uint64_t hpfar;
	asm volatile("mrs %0, hpfar_el2" : "=r" (hpfar) : : "cc");

	uint64_t ipa = ((hpfar & 0xfffff0) << 8) | (far & 0xfff);

	if (ipa < HOLE_START || ipa >= HOLE_END)
		return false;

	uart_puts_debug(SOC_UART0, "Wrapping IO to ");
	uart_hexval_debug(SOC_UART0, ipa);
	uart_puts_debug(SOC_UART0, "\n");

	return cutpage_io(pt_regs, esr, ipa);
}

bool process_low_sync(struct pt_regs *pt_regs, unsigned int esr)
{
	if ((esr & ESR_EC_MASK) == ESR_EC_SMC_64 && (esr & ESR_SMC_64_IMM_MASK) == 0) {
		/* wrapping PSCI */
		return wrap_psci(pt_regs);
	}
	if ((esr & ESR_EC_MASK) == ESR_EC_DATA_ABORT &&
	    (esr & ESR_DATA_ABORT_TYPE_MASK) == ESR_DATA_ABORT_TRANSLATION_LEVEL_3) {
		/* Trying to wrap hole IO */
		return wrap_hole_io(pt_regs, esr);
	}
	return false;
}
