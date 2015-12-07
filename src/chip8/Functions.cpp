#include "chip8/Functions.hpp"
#include "chip8/VirtualMachine.hpp"
#include "chip8/Opcodes.hpp"

namespace chip8 {

  Instruction fetch(VirtualMachine & vm) {
    auto instruction = vm.memory[vm.programCounter++];
    return instruction;
  }

  void execute(VirtualMachine & vm, Instruction instruction) {
    // no-op
    const Instruction OP_MASK = 0xF000;

    switch(instruction & OP_MASK) {
      case 0x0:
      break;

      case 0x1000:
        ops::jump(vm, instruction);
      break;

      default:
      break;
    }
  }
}