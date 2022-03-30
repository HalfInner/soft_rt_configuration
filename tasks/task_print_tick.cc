#include <algorithm>
#include <array>
#include <cstdio>
#include <iostream>
#include <numeric>
#include <iterator>
#include <random>

std::array<int, 2*1024*1024> arr;
int main() {
    std::iota(begin(arr), end(arr), 0);
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(begin(arr), end(arr), g);

    std::copy(begin(arr), begin(arr) + 30, std::ostream_iterator<int>(std::cout, " "));
}