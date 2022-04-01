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

  std::vector<std::byte> v;
  v.resize(elements_to_send);
  while (true) {
    std::shuffle(begin(arr), end(arr), g);
    std::transform(begin(arr), begin(arr) + elements_to_send, begin(v),
                   [](auto el) -> std::byte { return std::byte{el}; });

    server_a.send_data(v);
    std::transform(begin(arr) + elements_to_send, begin(arr) + elements_to_send + elements_to_send,
                   begin(v),
                   [](auto el) -> std::byte { return std::byte{el}; });

    server_b.send_data(v);
    std::this_thread::sleep_for(0s);
  }
}