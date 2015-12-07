#pragma once
#include "chip8/Types.hpp"

namespace chip8 {
  struct VirtualMachine;

  namespace ops {
    void disambiguate0x0(VirtualMachine & vm, Instruction instruction);
    void jump(VirtualMachine & vm, Instruction instruction);
    void clearScreen(VirtualMachine & vm, Instruction instruction);
    void returnFromSubroutine(VirtualMachine & vm, Instruction instruction);
    void callProgramAtAddress(VirtualMachine & vm, Instruction instruction);
    void callSubroutine(VirtualMachine & vm, Instruction instruction);
    void skipIfEquals(VirtualMachine & vm, Instruction instruction);
    void skipIfNotEquals(VirtualMachine & vm, Instruction instruction);
    void skipIfVxEqualsVy(VirtualMachine & vm, Instruction instruction);
    void setVx(VirtualMachine & vm, Instruction instruction);
    void addToVx(VirtualMachine & vm, Instruction instruction);
    void disambiguate0x8(VirtualMachine & vm, Instruction instruction);
    void setVxToVy(VirtualMachine & vm, Instruction instruction);
    void orVxVy(VirtualMachine & vm, Instruction instruction);
    void andVxVy(VirtualMachine & vm, Instruction instruction);
    void xorVxVy(VirtualMachine & vm, Instruction instruction);
    void skipIfVxNotEqualsVy(VirtualMachine & vm, Instruction instruction);
  }
}