#ifndef _MMIO_H_
#define _MMIO_H_

#include <stdint.h>

static inline void writel(uint32_t data, uint64_t reg)
{
	*(volatile uint32_t *)reg = data;
}

static inline uint32_t readl(uint64_t reg)
{
	return *(volatile uint32_t *)reg;
}

#endif
