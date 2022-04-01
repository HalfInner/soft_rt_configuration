#include <algorithm>
#include <array>
#include <chrono>
#include <cstdio>
#include <iostream>
#include <iterator>
#include <numeric>
#include <thread>
#include <random>

#include "naive_ipc.hh"

using namespace std::chrono_literals;
std::array<unsigned char, 2 * 1024 * 1024> arr;
int main() {
  std::cout << "Starting server...";
  naive_ipc::MQ server, client;
  server.initialize(naive_ipc::MQ::WorkPolicy::e_producer);
  std::cout << "OK\n";

  std::iota(begin(arr), end(arr), 0);
  std::random_device rd;
  std::mt19937 g(rd());

  while (true) {
    std::shuffle(begin(arr), end(arr), g);

    std::vector<std::byte> v;
    constexpr size_t elements_to_send = 30;
    v.reserve(elements_to_send);
    std::transform(begin(arr), begin(arr) + elements_to_send,
                   std::back_inserter(v),
                   [](auto el) -> std::byte { return std::byte{el}; });

    server.send_data(v);
    std::this_thread::sleep_for(100ms);
  }
}