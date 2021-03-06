#pragma once
#include <array>
#include <bitset>
#include <cstdint>
#include <functional>
#include <stack>

namespace chip8 {
  using Byte = std::uint8_t;
  using Nibble = std::uint8_t;
  using Address = std::uint16_t;
  using Instruction = std::uint16_t;
  using Opcode = std::uint16_t;
  using Stack = std::stack<Address>;
  using GraphicsBuffer = std::array<std::uint64_t, 32>;
  using RandomNumberGenerator = std::function<Byte(Byte seed)>;
  using KeyboardInputs = std::bitset<16>;

  template <std::size_t N>
  using ByteArray = std::array<Byte, N>;
}