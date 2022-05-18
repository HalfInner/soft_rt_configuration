#include <algorithm>
#include <array>
#include <chrono>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <thread>

#include "naive_ipc.hh"
#include "scoped_timer.hh"

using namespace std::chrono_literals;
int main() {
  std::cout << "Starting client A...";
  naive_ipc::MQ client{"/A"};
  client.initialize(naive_ipc::MQ::WorkPolicy::e_consumer);
  std::cout << "OK\n";
  while (true) {
    {
      auto t = HolidayBag::SportTimer("Client A", "us");
      auto received_data_opt = client.receive();
      if (received_data_opt) {
        auto &received_data = received_data_opt.value();
        std::sort(begin(received_data), end(received_data));
        auto res = std::accumulate(
            begin(received_data), end(received_data), 0,
            [](auto sum, auto v) { return sum + static_cast<int>(v); });
        (void)res;
      }
      t.stop();
      if (received_data_opt) {
        std::cout << t.getInterSummaryBag().unknit();
      }
    }
    // std::this_thread::sleep_for(1ms);
    std::this_thread::yield();
  }
}