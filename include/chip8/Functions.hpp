#pragma once
#include "chip8/Types.hpp"
#include "chip8/Constants.hpp"
#include <climits>
#include <tuple>
#include <type_traits>
#include <vector>

namespace chip8 {
  struct VirtualMachine;

  Instruction fetch(VirtualMachine & vm);
  void execute(VirtualMachine & vm, Instruction instruction);
  void cycle(VirtualMachine & vm);
  void reset(VirtualMachine & vm);

  inline bool matchesMask(const Instruction ins, const Instruction mask) {
    return (ins & mask) == mask;
  }

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

  inline std::tuple<Nibble, Nibble, Nibble> getXYN(Instruction ins) {
    Nibble x = (ins & 0x0F00) >> 8;
    Nibble y = (ins & 0x00F0) >> 4;
    Nibble z = (ins & 0x000F);
    return { x, y, z };
  }

  inline std::tuple<Nibble, Byte> getXNN(Instruction ins) {
    Nibble x = (ins & 0x0F00) >> 8;
    Byte y = (ins & 0x00FF);
    return { x, y };
  }

  template <typename T>
  constexpr T rotateLeft(T val, unsigned int moves) {
    static_assert(std::is_unsigned<T>::value, "rotateLeft only makes sense for unsigned types");
    return (val << moves) | (val >> (sizeof(T) * CHAR_BIT - moves));
  }

  template<class T>
  constexpr T rotateRight(T val, unsigned int moves) {
    static_assert(std::is_unsigned<T>::value, "rotateRight only makes sense for unsigned types");
    return (val >> moves) | (val << (sizeof(T) * CHAR_BIT - moves));
  }

  void handleKeypress(VirtualMachine & vm, Byte key);

  void handleKeyRelease(VirtualMachine & vm, Byte key);

  void printGraphicsBufferToConsole(VirtualMachine & vm);

  void loadRomData(VirtualMachine & vm, const std::vector<char> & file);
}