#include <algorithm>
#include <array>
#include <chrono>
#include <cstdio>
#include <future>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <thread>

#include "naive_ipc.hh"
#include "scoped_timer.hh"

using namespace std::chrono_literals;
std::array<unsigned char, 2 * 1024 * 1024> arr;
int main() {
  std::cout << "Starting server...";
  naive_ipc::MQ server_a{"/A"}, server_b{"/B"};
  server_a.initialize(naive_ipc::MQ::WorkPolicy::e_producer);
  server_b.initialize(naive_ipc::MQ::WorkPolicy::e_producer);
  std::cout << "OK\n";

  std::iota(begin(arr), end(arr), 0);
  std::random_device rd;
  std::mt19937 g(rd());

  constexpr size_t elements_to_send = naive_ipc::MQ::MAX_MSG_SIZE;

  constexpr int finish_line = 100;
  auto loops = 0;
  auto t = HolidayBag::SportTimer("Server", "us", finish_line);
  while (true) {
    {
      constexpr size_t magic_number_to_get_1ms_load = 1'024;
      std::shuffle(begin(arr), begin(arr) + magic_number_to_get_1ms_load, g);
      std::vector<std::byte> v;
      v.reserve(elements_to_send);
      std::transform(begin(arr), begin(arr) + elements_to_send, begin(v),
                     [](auto el) -> std::byte { return std::byte{el}; });

      server_a.send_data(v);
      v.reserve(elements_to_send);
      std::transform(begin(arr) + elements_to_send,
                     begin(arr) + elements_to_send + elements_to_send, begin(v),
                     [](auto el) -> std::byte { return std::byte{el}; });

      server_b.send_data(v);
      if (++loops >= finish_line) {
        t.lap();
        loops = 0;
        std::cout << t.getInterSummaryBag().unknit();
      }
    }
    std::this_thread::yield();
  }
}