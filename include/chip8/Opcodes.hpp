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
    void addVxVyUpdateCarry(VirtualMachine & vm, Instruction instruction);
    void subtractVxVyUpdateCarry(VirtualMachine & vm, Instruction instruction);
    void rightshiftVx(VirtualMachine & vm, Instruction instruction);
    void subtractVxFromVyUpdateCarry(VirtualMachine & vm, Instruction instruction);
    void leftshiftVx(VirtualMachine & vm, Instruction instruction);
    void skipIfVxNotEqualsVy(VirtualMachine & vm, Instruction instruction);
    void setIToAddress(VirtualMachine & vm, Instruction instruction);
    void jumpPlusV0(VirtualMachine & vm, Instruction instruction);
    void randomVxModNn(VirtualMachine & vm, Instruction instruction);
    void blit(VirtualMachine & vm, Instruction instruction);
    void disambiguate0xE(VirtualMachine & vm, Instruction instruction);
    void skipIfKeyIsPressed(VirtualMachine & vm, Instruction instruction);
    void skipIfKeyIsNotPressed(VirtualMachine & vm, Instruction instruction);
    void disambiguate0xF(VirtualMachine & vm, Instruction instruction);
    void setVxToDelayTimer(VirtualMachine & vm, Instruction instruction);
    void waitForKeyPress(VirtualMachine & vm, Instruction instruction);
    void setDelayTimer(VirtualMachine & vm, Instruction instruction);
    void setSoundTimer(VirtualMachine & vm, Instruction instruction);
    void addVxToI(VirtualMachine & vm, Instruction instruction);
    void setIToCharacter(VirtualMachine & vm, Instruction instruction);
    void storeBcdOfVx(VirtualMachine & vm, Instruction instruction);
    void storeV0ToVx(VirtualMachine & vm, Instruction instruction);
    void loadV0ToVx(VirtualMachine & vm, Instruction instruction);
  }
}