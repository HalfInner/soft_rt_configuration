#include <algorithm>
#include <array>
#include <cstdio>
#include <iostream>
#include <numeric>
#include <iterator>
#include <random>

#include "naive_ipc.hh"

std::array<int, 2*1024*1024> arr;
int main() {
    std::iota(begin(arr), end(arr), 0);
    std::random_device rd;
    std::mt19937 g(rd());

    
    std::shuffle(begin(arr), end(arr), g);

    naive_ipc::MQ server, client;
    server.initialize(naive_ipc::MQ::WorkPolicy::e_producer);
    client.initialize(naive_ipc::MQ::WorkPolicy::e_consumer);

    std::vector<std::byte> v;
    constexpr size_t elements_to_send = 30;
    v.reserve(elements_to_send);
    std::transform(begin(arr), begin(arr) + elements_to_send, std::back_inserter(v), [](auto el) -> std::byte { return std::byte{el}; });

    server.send_data(v);
    
    auto received_data = client.receive();
    // std::copy(begin(received_data), std::end(received_data), std::ostream_iterator<std::byte>(std::cout, " "));
}