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

using namespace std::chrono_literals;
int main(int argc, char *argv[]) {
  std::string client_name = "DEAFULT";
  if (argc > 1) {
    client_name = argv[1];
  }
  std::cout << "Starting client " << client_name << "...";
  naive_ipc::MQ client{"/" + client_name};
  client.initialize(naive_ipc::MQ::WorkPolicy::e_consumer);
  std::cout << "OK\n";
  while (true) {
    auto received_data_opt = client.receive();
    if (received_data_opt) {
      auto &received_data = received_data_opt.value();
      std::sort(begin(received_data), end(received_data));
      auto res = std::accumulate(
          begin(received_data), end(received_data), 0,
          [](auto sum, auto v) { return sum + static_cast<int>(v); });
      (void)res;
    }
    std::this_thread::yield();
    // std::this_thread::sleep_for(10us);
  }
}