#ifndef _UART_H_
#define _UART_H_

#include "mmio.h"

#define UART_RBR(uart) (uart + 0x0)
#define UART_THR(uart) (uart + 0x0)
#define UART_DLL(uart) (uart + 0x0)
#define UART_DLH(uart) (uart + 0x4)
#define UART_IER(uart) (uart + 0x4)
#define UART_IIR(uart) (uart + 0x8)
#define UART_FCR(uart) (uart + 0x8)
#define UART_LCR(uart) (uart + 0xc)
#define UART_LSR(uart) (uart + 0x14)

#define uart_putc(uart, c) do { \
	while (!(readl(UART_LSR(uart)) & (1 << 6))) {} \
	writel((c), UART_THR(uart)); \
} while (0);

void uart_puts(uint64_t uart, const char *str);
void uart_hexval(uint64_t uart, uint64_t val);

#ifdef NDEBUG
#define uart_puts_debug(a, b)
#define uart_hexval_debug(a, b)
#else
#define uart_puts_debug(a, b) uart_puts(a, b)
#define uart_hexval_debug(a, b) uart_hexval(a, b)
#endif

#endif
