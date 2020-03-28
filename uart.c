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

void uart_hexval(uint64_t uart, uint64_t val)
{
	char result[17];
	int i;

	result[16] = '\0';
	for (i = 0; i < 16; i++)
		result[i] = '0';

	i = 15;
	while (val) {
		uint8_t thisbit = (uint8_t) (val & 0xf);
		if (thisbit < 0xa)
			result[i] = '0' + thisbit;
		else
			result[i] = 'a' - 0xa + thisbit;
		val >>= 4;
		i--;
	}

	uart_puts(uart, result);
}
