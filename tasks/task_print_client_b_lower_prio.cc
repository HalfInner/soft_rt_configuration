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
  std::cout << "Starting client B...";
  naive_ipc::MQ client{"/B"};
  client.initialize(naive_ipc::MQ::WorkPolicy::e_consumer);
  std::cout << "OK\n";
  while (true) {
    auto t = HolidayBag::SportTimer("Client B", "us");
    auto received_data = client.receive();

    std::sort(begin(received_data), end(received_data));
    auto res = std::accumulate(begin(received_data), end(received_data), 0,
                               [](auto v) { return std::to_integer<int>(v); });
    (void)res;
    // if (!received_data.empty()) {
    //   std::cout << "B:";
    //   for (auto el : received_data) {
    //     std::cout << static_cast<int>(el) << ' ';
    //   }
    //   std::cout << "\n";
    // } else {
    // }
    t.stop();
    std::this_thread::sleep_for(1s);
    // std::this_thread::yield();
    std::cout << t.getInterSummaryBag().unknit();
  }
}