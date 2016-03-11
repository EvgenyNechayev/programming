#ifndef SPINLOCK_HPP
#define SPINLOCK_HPP

#include <atomic>

class spinlock
{
   std::atomic_flag flag;

public:
   spinlock() : flag(ATOMIC_FLAG_INIT){}
   void lock() { while(flag.test_and_set()){} }
   bool try_lock() { return !flag.test_and_set(); }
   void unlock() { flag.clear(); }
};

#endif // SPINLOCK_HPP
