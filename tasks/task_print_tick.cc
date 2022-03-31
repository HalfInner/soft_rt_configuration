#include <algorithm>
#include <array>
#include <cstdio>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>

#include "naive_ipc.hh"

std::array<unsigned char, 2 * 1024 * 1024> arr;
int main() {
  naive_ipc::MQ server, client;
  server.initialize(naive_ipc::MQ::WorkPolicy::e_producer);
  client.initialize(naive_ipc::MQ::WorkPolicy::e_consumer);

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

    auto received_data = client.receive();
    for (auto el : received_data) {
      std::cout << static_cast<int>(el) << ' ';
    }
    std::cout << "\n\n";
  }
}