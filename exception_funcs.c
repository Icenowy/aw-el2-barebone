#include SOC_HEADER
#include "panic.h"
#include "uart.h"

static void dump_exception_registers()
{
	uint64_t reg;
	uart_puts(SOC_UART0, "\nelr_el2: ");
	asm volatile("mrs %0, elr_el2" : "=r" (reg) : : "cc");
	uart_hexval(SOC_UART0, reg);
	uart_puts(SOC_UART0, "\nesr_el2: ");
	asm volatile("mrs %0, esr_el2" : "=r" (reg) : : "cc");
	uart_hexval(SOC_UART0, reg);
	uart_puts(SOC_UART0, "\nfar_el2: ");
	asm volatile("mrs %0, far_el2" : "=r" (reg) : : "cc");
	uart_hexval(SOC_UART0, reg);
	uart_puts(SOC_UART0, "\n");
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

void do_low_sync()
{
	dump_exception_registers();
	panic(__func__);
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
