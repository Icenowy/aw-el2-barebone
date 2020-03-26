#include "uart.h"

void uart_puts(uint64_t uart, const char *str)
{
	while (*str) {
		if (*str == '\n')
			uart_putc(uart, '\r');
		uart_putc(uart, *str);
		str++;
	}
}
