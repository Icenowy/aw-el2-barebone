#ifndef _SPINLOCK_H_
#define _SPINLOCK_H_

#include <stdint.h>
#include <stdbool.h>

typedef bool spinlock;

static inline void spinlock_init(spinlock *l) {
	__atomic_clear(l, __ATOMIC_RELAXED);
}

static inline void spinlock_lock(spinlock *l) {
	while (__atomic_test_and_set(l, __ATOMIC_ACQUIRE)) ;
}

static inline void spinlock_unlock(spinlock *l) {
	__atomic_clear(l, __ATOMIC_RELEASE);
}

#endif
