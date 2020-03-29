#ifndef _SPINLOCK_H_
#define _SPINLOCK_H_

#include <stdint.h>
#include <stdbool.h>

typedef int spinlock;

static inline void spinlock_init(spinlock *l) {
	*l = 0;
}

static inline void spinlock_lock(spinlock *l) {
	while(!__sync_bool_compare_and_swap(l, 0, 1)) asm volatile ("wfe");
}

static inline void spinlock_unlock(spinlock *l) {
	*l = 0;
}

#endif
