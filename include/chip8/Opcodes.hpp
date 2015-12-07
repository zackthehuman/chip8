#pragma once
#include "chip8/Types.hpp"

namespace chip8 {
  struct VirtualMachine;

  namespace ops {
    void jump(VirtualMachine & vm, Instruction instruction);
  }
}