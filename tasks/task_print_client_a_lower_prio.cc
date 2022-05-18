#include <algorithm>
#include <array>
#include <chrono>
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
    auto t = HolidayBag::SportTimer("Server", "us");
    auto received_data = client.receive();

    if (!received_data.empty()) {
      std::cout << "A:";
      for (auto el : received_data) {
        std::cout << static_cast<int>(el) << ' ';
      }
      std::cout << "\n";
    } else {
    }
    t.stop()
    std::this_thread::sleep_for(1ms);
    // std::this_thread::yield();
    std::cout << t.getInterSummaryBag().unknit();
  }
}