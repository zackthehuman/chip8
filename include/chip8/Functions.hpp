#pragma once
#include "chip8/Types.hpp"
#include "chip8/Constants.hpp"
#include <tuple>

namespace chip8 {
  struct VirtualMachine;

  Instruction fetch(VirtualMachine & vm);
  void execute(VirtualMachine & vm, Instruction instruction);

  inline Byte getHighByte(Instruction ins) noexcept {
    return static_cast<Byte>((ins & HIGH_BYTE_MASK) >> HIGH_BYTE_SHIFT);
  }

  inline Byte getLowByte(Instruction ins) noexcept {
    return static_cast<Byte>(ins & LOW_BYTE_MASK);
  }

  inline Address getAddress(Instruction ins) noexcept {
    return static_cast<Address>(ins & ADDRESS_BYTE_MASK);
  }

  inline std::tuple<Nibble, Nibble> getXY(Instruction ins) {
    Nibble x = (ins & 0x0F00) >> 8;
    Nibble y = (ins & 0x00F0) >> 4;
    return { x, y };
  }

  inline std::tuple<Nibble, Byte> getXNN(Instruction ins) {
    Nibble x = (ins & 0x0F00) >> 8;
    Byte y = (ins & 0x00FF);
    return { x, y };
  }
}