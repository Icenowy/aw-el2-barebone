#include SOC_HEADER
#include "uart.h"
#include "mmio.h"
#include "pgtables.h"
#include "trapped_funcs.h"

#include <stddef.h>

#include <asm/system.h>

void *secondary_el1_ep[4];

int init()
{
	uint64_t mpidr;
	asm volatile("mrs %0, mpidr_el1" : "=r" (mpidr) : : "cc");
	uart_puts(SOC_UART0, "Starting hypervisor on processor ");
	uart_hexval(SOC_UART0, mpidr);
	uart_puts(SOC_UART0, "\n");

	if (current_el() != 2) {
		uart_puts(SOC_UART0, "Error: current EL is not EL2\n");
		return 1;
	}

	current_hole_page = 0;

	init_pgtables();
	install_pgtables();

	uart_puts(SOC_UART0, "Switching to EL1\n");

	return 0;
}

void *secondary_init()
{
	uint64_t mpidr;
	asm volatile("mrs %0, mpidr_el1" : "=r" (mpidr) : : "cc");
	uart_puts_debug(SOC_UART0, "Starting hypervisor on processor ");
	uart_hexval_debug(SOC_UART0, mpidr);
	uart_puts_debug(SOC_UART0, "\n");

	if (current_el() != 2) {
		uart_puts_debug(SOC_UART0, "Error: current EL is not EL2\n");
		return NULL;
	}

	if ((mpidr & 0x00ffffff) >= 4) {
		uart_puts_debug(SOC_UART0, "Unknown MPIDR\n");
		return NULL;
	}

	install_pgtables();

	uart_puts_debug(SOC_UART0, "Switching to EL1\n");

	return secondary_el1_ep[mpidr & 0x00ffffff];
}
