#include "chip8/Opcodes.hpp"
#include "chip8/Functions.hpp"
#include "chip8/VirtualMachine.hpp"
#include <exception>
#include <iostream>

namespace chip8 {
  namespace ops {
    void disambiguate0x0(VirtualMachine & vm, Instruction instruction) {
      if(0x00E0 == instruction) {
        clearScreen(vm, instruction);
      } else if(0x00EE == instruction) {
        returnFromSubroutine(vm, instruction);
      } else {
        callProgramAtAddress(vm, instruction);
      }
    }

    void jump(VirtualMachine & vm, Instruction instruction) {
      vm.programCounter = getAddress(instruction);
    }

    void clearScreen(VirtualMachine & vm, Instruction instruction) {
      throw std::runtime_error("Function not implemented");
    }

    void returnFromSubroutine(VirtualMachine & vm, Instruction instruction) {
      if(vm.stack.empty()) {
        throw std::runtime_error("Cannot return from subroutine since stack is empty");
      }

      auto address = vm.stack.top();
      vm.stack.pop();
      vm.programCounter = address;
    }

    void callProgramAtAddress(VirtualMachine & vm, Instruction instruction) {
      throw std::runtime_error("Function not implemented");
    }

    void callSubroutine(VirtualMachine & vm, Instruction instruction) {
      vm.stack.push(vm.programCounter);
      vm.programCounter = getAddress(instruction);
    }

    void skipIfEquals(VirtualMachine & vm, Instruction instruction) {
      Nibble x;
      Byte nn;

      std::tie(x, nn) = getXNN(instruction);

      auto value = vm.registers[x];

      if(value == nn) {
        vm.programCounter += 1;
      }
    }

    void skipIfNotEquals(VirtualMachine & vm, Instruction instruction) {
      Nibble x;
      Byte nn;

      std::tie(x, nn) = getXNN(instruction);

      auto value = vm.registers[x];

      if(value != nn) {
        vm.programCounter += 1;
      }
    }

    void skipIfVxEqualsVy(VirtualMachine & vm, Instruction instruction) {
      Byte x, y;

      std::tie(x, y) = getXY(instruction);

      auto registerX = vm.registers[x];
      auto registerY = vm.registers[y];

      if(registerX == registerY) {
        vm.programCounter += 1;
      }
    }

  }
}