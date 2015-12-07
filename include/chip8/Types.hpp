#pragma once
#include <array>
#include <cstdint>
#include <stack>

namespace chip8 {
  using Byte = std::uint8_t;
  using Address = std::uint16_t;
  using Instruction = std::uint16_t;
  using Stack = std::stack<Address>;

  template <std::size_t N>
  using ByteArray = std::array<Byte, N>;
}