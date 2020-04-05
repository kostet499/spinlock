#ifndef SPINLOCK_SPINLOCK_H
#define SPINLOCK_SPINLOCK_H

#include <iostream>

class Spinlock {
public:
  Spinlock();
  ~Spinlock();

  void lock();
  bool try_lock();
  void unlock();

private:
  std::atomic<int> flag;
};


#endif //SPINLOCK_SPINLOCK_H
