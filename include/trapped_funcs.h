#ifndef _TRAPPED_FUNCS_H_
#define _TRAPPED_FUNCS_H_

#include <stdint.h>
#include <stdbool.h>

#include <asm/ptrace.h>

#define ESR_EC_MASK (0x3fU << 26)
#define ESR_EC_SMC_64 (0x17U << 26)
#define ESR_SMC_64_IMM_MASK 0xffffU

bool process_low_sync(struct pt_regs *pt_regs, unsigned int esr);

#endif
