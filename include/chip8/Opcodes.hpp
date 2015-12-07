#pragma once
#include "chip8/Types.hpp"

namespace chip8 {
  struct VirtualMachine;

  namespace ops {
    void jump(VirtualMachine & vm, Instruction instruction);
    void clearScreen(VirtualMachine & vm, Instruction instruction);
    void returnFromSubroutine(VirtualMachine & vm, Instruction instruction);
    void callProgramAtAddress(VirtualMachine & vm, Instruction instruction);
    void callSubroutine(VirtualMachine & vm, Instruction instruction);
    void skipIfEquals(VirtualMachine & vm, Instruction instruction);
    void skipIfNotEquals(VirtualMachine & vm, Instruction instruction);
    void skipIfVxEqualsVy(VirtualMachine & vm, Instruction instruction);
  }
}