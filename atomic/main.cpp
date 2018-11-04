#include <atomic>
#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

struct data_t {
  std::string s;
};

struct my_t {
  std::shared_ptr<data_t> cur;
  std::atomic<data_t*> next;

  my_t() : cur(), next() {}

  std::string pop() {
    data_t* n = next.exchange(nullptr);
    if (n) {
      cur.reset(n);
    }

    if (cur) {
      return cur->s;
    }
    return "";
  }

  void push(std::string s) { delete next.exchange(new data_t{s}); }
};

void producer(my_t& m, std::chrono::microseconds sleep) {
  while (true) {
    std::this_thread::sleep_for(sleep);
    std::string v = "v: " + std::to_string(std::rand());
    m.push(v);
  }
}

void consumer(my_t& m, std::chrono::microseconds sleep) {
  std::string last;
  while (true) {
    // std::this_thread::sleep_for(sleep);
    std::string n = m.pop();
    if (n != last) {
      last = std::move(n);
      std::cout << last << std::endl;
    }
  }
}

int main(int argc, char* argv[]) {
  my_t m;
  std::cout << std::boolalpha << m.next.is_lock_free() << std::endl;
  m.push("a");
  m.push("b");
  m.push("c");
  std::cout << m.pop() << std::endl;
  std::cout << m.pop() << std::endl;
  m.push("d");
  std::cout << m.pop() << std::endl;
  std::cout << m.pop() << std::endl;
  std::cout << m.pop() << std::endl;
  std::cout << m.pop() << std::endl;
  std::thread p(producer, std::ref(m), std::chrono::milliseconds{100});
  std::thread c(consumer, std::ref(m), std::chrono::milliseconds{1});
  p.join();
  c.join();
  return 0;
}
