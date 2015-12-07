#include "chip8/Functions.hpp"
#include "chip8/VirtualMachine.hpp"

namespace chip8 {
  Instruction fetch(VirtualMachine & vm) {
    auto instruction = vm.memory[vm.programCounter++];
    return instruction;
  }

  void execute(VirtualMachine & vm, Instruction instruction) {
    // no-op
  }
}