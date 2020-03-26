#include SOC_HEADER
#include "uart.h"
#include "mmio.h"

#include <asm/system.h>

int init()
{
	if (current_el() != 2) {
		uart_puts(SOC_UART0, "Error: current EL is not EL2\n");
		return 1;
	}
	uart_puts(SOC_UART0, "Switching to EL1\n");

	return 0;
}
