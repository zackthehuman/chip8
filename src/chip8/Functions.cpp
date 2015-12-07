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
        if(0x00E0 == instruction) {
          ops::clearScreen(vm, instruction);
          break;
        } else if(0x00EE == instruction) {
          ops::returnFromSubroutine(vm, instruction);
          break;
        } else {
          ops::callProgramAtAddress(vm, instruction);
          break;
        }
        break;

      case 0x1000:
        ops::jump(vm, instruction);
        break;

      case 0x2000:
        ops::callSubroutine(vm, instruction);
        break;

      case 0x3000:
        ops::skipIfEquals(vm, instruction);
        break;

      case 0x4000:
        ops::skipIfNotEquals(vm, instruction);
        break;

      case 0x5000:
        ops::skipIfVxEqualsVy(vm, instruction);
        break;

      default:
      break;
    }
  }
}