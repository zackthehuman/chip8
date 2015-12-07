#pragma once
#include <array>
#include <cstdint>
#include <stack>

namespace chip8 {
  using Byte = std::int8_t;

  template <std::size_t N>
  using ByteArray = std::array<Byte, N>;

  using Stack = std::stack<Byte>;
}