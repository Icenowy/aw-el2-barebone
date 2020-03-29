#ifndef _TRAPPED_FUNCS_H_
#define _TRAPPED_FUNCS_H_

#include "spinlock.h"

#include <stdint.h>
#include <stdbool.h>

#include <asm/ptrace.h>

#define ESR_EC_MASK (0x3fU << 26)
#define ESR_EC_SMC_64 (0x17U << 26)
#define ESR_EC_DATA_ABORT (0x24U << 26)
#define ESR_DATA_ABORT_ISV (1 << 24)
#define ESR_DATA_ABORT_SAS_MASK (0x3 << 22)
#define ESR_DATA_ABORT_SAS_B (0x0 << 22)
#define ESR_DATA_ABORT_SAS_HW (0x1 << 22)
#define ESR_DATA_ABORT_SAS_W (0x2 << 22)
#define ESR_DATA_ABORT_SAS_DW (0x3 << 22)
#define ESR_DATA_ABORT_SSE (1 << 21)
#define ESR_DATA_ABORT_SRT_MASK (0x1f << 16)
#define ESR_DATA_ABORT_SRT_SHIFT 16
#define ESR_DATA_ABORT_SF (1 << 15)
#define ESR_DATA_ABORT_AR (1 << 14)
#define ESR_DATA_ABORT_WnR (1 << 6)
#define ESR_DATA_ABORT_TYPE_MASK 0x3fU
#define ESR_DATA_ABORT_TRANSLATION_LEVEL_3 0x07U
#define ESR_SMC_64_IMM_MASK 0xffffU

bool process_low_sync(struct pt_regs *pt_regs, unsigned int esr);

extern spinlock cutpage_lock;

#endif
