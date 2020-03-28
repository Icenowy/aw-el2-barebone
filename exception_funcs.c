#include SOC_HEADER
#include "panic.h"
#include "uart.h"
#include "trapped_funcs.h"

#include <asm/ptrace.h>

static void dump_registers(struct pt_regs *pt_regs, unsigned int esr)
{
	uint64_t reg;
#define PUT_PT_REG(reg) do {  \
	uart_puts(SOC_UART0, "\nx" #reg ": "); \
	uart_hexval(SOC_UART0, pt_regs->regs[reg]); \
} while (0)
	uart_puts(SOC_UART0, "\nelr: ");
	uart_hexval(SOC_UART0, pt_regs->elr);
	uart_puts(SOC_UART0, "\nesr: ");
	uart_hexval(SOC_UART0, esr);
	uart_puts(SOC_UART0, "\nfar: ");
	asm volatile("mrs %0, far_el2" : "=r" (reg) : : "cc");
	uart_hexval(SOC_UART0, reg);
	PUT_PT_REG(0);
	PUT_PT_REG(1);
	PUT_PT_REG(2);
	PUT_PT_REG(3);
	PUT_PT_REG(4);
	PUT_PT_REG(5);
	PUT_PT_REG(6);
	PUT_PT_REG(7);
	PUT_PT_REG(8);
	PUT_PT_REG(9);
	PUT_PT_REG(10);
	PUT_PT_REG(11);
	PUT_PT_REG(12);
	PUT_PT_REG(13);
	PUT_PT_REG(14);
	PUT_PT_REG(15);
	PUT_PT_REG(16);
	PUT_PT_REG(17);
	PUT_PT_REG(18);
	PUT_PT_REG(19);
	PUT_PT_REG(20);
	PUT_PT_REG(21);
	PUT_PT_REG(22);
	PUT_PT_REG(23);
	PUT_PT_REG(24);
	PUT_PT_REG(25);
	PUT_PT_REG(26);
	PUT_PT_REG(27);
	PUT_PT_REG(28);
	PUT_PT_REG(29);
	PUT_PT_REG(30);
	uart_puts(SOC_UART0, "\n");
#undef PUT_PT_REG
}

void do_sync()
{
	panic(__func__);
}

void do_irq()
{
	panic(__func__);
}

void do_fiq()
{
	panic(__func__);
}

void do_error()
{
	panic(__func__);
}

void do_bad_sync()
{
	panic(__func__);
}

void do_bad_irq()
{
	panic(__func__);
}

void do_bad_fiq()
{
	panic(__func__);
}

void do_bad_error()
{
	panic(__func__);
}

void do_low_sync(struct pt_regs *pt_regs, unsigned int esr)
{
	dump_registers(pt_regs, esr);
	if (!process_low_sync(pt_regs, esr)) {
		panic(__func__);
	}
}

void do_low_irq()
{
	panic(__func__);
}

void do_low_fiq()
{
	panic(__func__);
}

void do_low_error()
{
	panic(__func__);
}
