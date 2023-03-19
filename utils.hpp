#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <tuple>

using countValue = std::tuple<unsigned int, unsigned int>;

const std::vector<countValue> createSortedHistogram(const std::vector<unsigned int> imageValues);

#endif