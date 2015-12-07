#pragma once
#include <cstdint>
#include "chip8/Types.hpp"

namespace chip8 {
  const std::size_t RAM_SIZE = 4096;
  const std::size_t REGISTER_COUNT = 16;
  const Instruction HIGH_BYTE_MASK = 0xFF00;
  const std::size_t HIGH_BYTE_SHIFT = 8;
  const Instruction LOW_BYTE_MASK = 0x00FF;
  const Address ADDRESS_BYTE_MASK = 0x0FFF;
}