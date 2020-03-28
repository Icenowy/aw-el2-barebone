/*
 *  linux/include/asm-arm/proc-armv/ptrace.h
 *
 *  Copyright (C) 1996-1999 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __ASM_PROC_PTRACE_H
#define __ASM_PROC_PTRACE_H

#define PCMASK		0

#ifndef __ASSEMBLY__

/*
 * This struct defines the way the registers are stored
 * on the stack during an exception.
 */
struct pt_regs {
	unsigned long elr;
	unsigned long regs[31];
};

#endif	/* __ASSEMBLY__ */

#endif
