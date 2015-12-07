#include "chip8/Opcodes.hpp"
#include "chip8/Functions.hpp"
#include "chip8/VirtualMachine.hpp"
#include <exception>

namespace chip8 {
  namespace ops {
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

  }
}