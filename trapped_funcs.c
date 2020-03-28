#include "trapped_funcs.h"

bool wrap_psci(struct pt_regs *pt_regs)
{
	pt_regs->regs[0] = (uint64_t) -1;
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
