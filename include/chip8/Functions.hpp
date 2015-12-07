#pragma once
#include "chip8/Types.hpp"

namespace chip8 {
  struct VirtualMachine;

  Instruction fetch(VirtualMachine & vm);
  void execute(VirtualMachine & vm, Instruction instruction);
}