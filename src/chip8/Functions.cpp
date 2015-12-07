#include "chip8/Functions.hpp"
#include "chip8/VirtualMachine.hpp"
#include "chip8/Opcodes.hpp"
#include <array>
#include <functional>

namespace chip8 {

  std::array<std::function<void(VirtualMachine &, Instruction)>, 16> FUNCTION_TABLE { {
    ops::disambiguate0x0,
    ops::jump,
    ops::callSubroutine,
    ops::skipIfEquals,
    ops::skipIfNotEquals,
    ops::skipIfVxEqualsVy,
    ops::setVx,
    ops::addToVx,
    ops::disambiguate0x8,
    ops::skipIfVxNotEqualsVy
  } };

  Instruction fetch(VirtualMachine & vm) {
    Instruction highByte = static_cast<Instruction>(vm.memory[vm.programCounter++]) << 8;
    Instruction lowByte = static_cast<Instruction>(vm.memory[vm.programCounter++]);
    return highByte + lowByte;
  }

  void execute(VirtualMachine & vm, Instruction instruction) {
    // no-op
    const Instruction OP_MASK = 0xF000;

    // FUNCTION_TABLE[instruction & OP_MASK](vm, instruction);

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

      case 0x6000:
        ops::setVx(vm, instruction);
        break;

      case 0x7000:
        ops::addToVx(vm, instruction);
        break;

      case 0x8000:
        ops::disambiguate0x8(vm, instruction);
        break;

      case 0x9000:
        ops::skipIfVxNotEqualsVy(vm, instruction);
        break;

      case 0xA000:
        ops::setIToAddress(vm, instruction);
        break;

      default:
      break;
    }
  }

  void cycle(VirtualMachine & vm) {
    if(vm.timers.delay > 0) {
      vm.timers.delay -= 1;
    }

    if(vm.timers.sound > 0) {
      vm.timers.sound -= 1;
    }

    execute(vm, fetch(vm));
  }
}