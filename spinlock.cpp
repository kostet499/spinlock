#include "spinlock.h"

void Spinlock::lock() {
  while (flag.load() || flag.exchange(1)) {
  }
}

bool Spinlock::try_lock() {
  return !flag.exchange(1);
}

void Spinlock::unlock() {
  flag.store(0);
}

Spinlock::~Spinlock() {
  unlock();
}

Spinlock::Spinlock() {
  flag.store(0);
}
