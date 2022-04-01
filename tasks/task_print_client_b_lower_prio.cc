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
int main() {
  std::cout << "Starting client B...";
  naive_ipc::MQ client{"/B"};
  client.initialize(naive_ipc::MQ::WorkPolicy::e_consumer);
  std::cout << "OK\n";
  while (true) {
    auto received_data = client.receive();

    if (!received_data.empty()) {
      std::cout << "B:";
      for (auto el : received_data) {
        std::cout << static_cast<int>(el) << ' ';
      }
      std::cout << "\n";
    } else {
    }
    std::this_thread::sleep_for(1s);
  }
}