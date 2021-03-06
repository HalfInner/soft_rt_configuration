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
int main(int argc, char *argv[]) {
  std::cout << "Starting server...";
  size_t magic_number_to_get_1ms_load = 1000000;
  if (argc > 1) {
    std::cout << "Load arg=" << argv[1] << "...";
    magic_number_to_get_1ms_load = std::stoll(argv[1]);
  }
  std::cout << "Use n=" << magic_number_to_get_1ms_load << " to load shuffling... ";
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
  std::vector<std::byte> v;
  v.reserve(elements_to_send);
  auto t = HolidayBag::SportTimer("\tServer Work", "us", finish_line);
  auto t_2 = HolidayBag::SportTimer("\tServer Sleep", "us", finish_line);
  auto t_3 = HolidayBag::SportTimer("Server Total", "us", finish_line);
  while (true) {
    {
      std::shuffle(begin(arr), begin(arr) + magic_number_to_get_1ms_load, g);
      std::transform(begin(arr), begin(arr) + elements_to_send, begin(v),
                     [](auto el) -> std::byte { return std::byte{el}; });

      server_a.send_data(v);
      std::transform(begin(arr) + elements_to_send,
                     begin(arr) + elements_to_send + elements_to_send, begin(v),
                     [](auto el) -> std::byte { return std::byte{el}; });

      server_b.send_data(v);
    }
    t.mini_lap();
    t_2.mini_lap(true);
    std::this_thread::yield();
    // std::this_thread::sleep_for(10us);
    t_2.mini_lap();
    t.mini_lap(true);
    if (++loops >= finish_line) {
      loops = 0;
      std::cout << t.getInterSummaryBag().unknit();
    }
    t_3.mini_lap();
  }
}