#include SOC_HEADER
#include "uart.h"
#include "mmio.h"

#include <asm/system.h>

void panic(const char *reason)
{
	uart_puts(SOC_UART0, "Panic: ");
	uart_puts(SOC_UART0, reason);
	uart_puts(SOC_UART0, "\n");

	while (1) {
		wfi();
	}
}
