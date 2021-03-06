#include <iostream>
#include <thread>
#include <chrono>
#include "../spinlock.h"

bool simple_test() {
  Spinlock lock;

  lock.lock();
  if (lock.try_lock()) {
    std::cout << "Error: try_lock true after lock" << std::endl;
    return false;
  }
  lock.unlock();
  if (!lock.try_lock()) {
    std::cout << "Error: try_lock false after unlock" << std::endl;
    return false;
  }
  return true;
}


bool two_threads() {
  Spinlock lock;

  auto start = std::chrono::high_resolution_clock::now();
  std::thread capturer([&lock]{
    // проверка на то, что мы первыми взяли лок
    if (!lock.try_lock()) {
      std::cout << "Test result is undefined" << std::endl;
      return;
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));
    lock.unlock();
  });

  // мы хотим, чтобы главный поток взял лок вторым, если же он берет лок первым, то результат теста неопределен (считается как провал)
  std::this_thread::sleep_for(std::chrono::seconds(1));
  lock.lock();

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed = end-start;

  capturer.join();

  if (elapsed.count() < 2000) {
    std::cout << "Error: main thread entered critical section before 2 seconds elapsed" << std::endl;
    return false;
  }
  return true;
}

void guard(Spinlock& lock, size_t guard_num, std::atomic<int> &tower, std::atomic<bool> &verdict) {
  for (size_t i = 0; i < 5; ++i) {
    lock.lock();
    tower.store(guard_num);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    if (tower.load() != guard_num) {
      std::cout << "Error: another guard is in the tower!" << std::endl;
      verdict.store(false);
    }
    lock.unlock();
  }
}

bool guard_test(size_t guard_count) {
  Spinlock lock;
  std::atomic<int> tower;
  std::atomic<bool> verdict;
  verdict.store(true);

  std::thread arr[guard_count];
  for (size_t i = 0; i < guard_count; ++i) {
    arr[i] = std::thread(
      [&lock, i, &tower, &verdict] {
        guard(lock, i, tower, verdict);
      });
  }
  for (size_t i = 0; i < guard_count; ++i) {
    arr[i].join();
  }
  return verdict.load();
}

int main() {
  std::string answers[2] = {"FAILED", "OK"};
  bool verdict;

  verdict = simple_test();
  std::cout << "Simple test: " << answers[verdict] << std::endl;
  verdict = two_threads();
  std::cout << "Two-threads test: " << answers[verdict] << std::endl;
  verdict = guard_test(6);
  std::cout << "Guard test: " << answers[verdict] << std::endl;

  return 0;
}