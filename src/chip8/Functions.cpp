#include "chip8/Functions.hpp"
#include "chip8/VirtualMachine.hpp"
#include "chip8/Opcodes.hpp"
#include <array>
#include <bitset>
#include <functional>
#include <iomanip>
#include <iostream>

namespace chip8 {

  std::array<std::function<void(VirtualMachine &, Instruction)>, 16> FUNCTION_TABLE { {
    ops::disambiguate0x0,     // 0x0000
    ops::jump,                // 0x1000
    ops::callSubroutine,      // 0x2000
    ops::skipIfEquals,        // 0x3000
    ops::skipIfNotEquals,     // 0x4000
    ops::skipIfVxEqualsVy,    // 0x5000
    ops::setVx,               // 0x6000
    ops::addToVx,             // 0x7000
    ops::disambiguate0x8,     // 0x8000
    ops::skipIfVxNotEqualsVy, // 0x9000
    ops::setIToAddress,       // 0xA000
    ops::jumpPlusV0,          // 0xB000
    ops::randomVxModNn,       // 0xC000
    ops::blit                 // 0xD000
                              // 0xE000
                              // 0xF000
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
      case 0x0000:
        ops::disambiguate0x0(vm, instruction);
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

      case 0xB000:
        ops::jumpPlusV0(vm, instruction);
        break;

      case 0xC000:
        ops::randomVxModNn(vm, instruction);
        break;

      case 0xD000:
        ops::blit(vm, instruction);
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

  void printGraphicsBufferToConsole(VirtualMachine & vm) {
    const auto & graphics = vm.graphics;
    const auto rows = graphics.size();

    for(std::size_t y = 0; y < rows; y++) {
      const auto pixels = graphics[y];
      std::cout << std::setfill('0') << std::bitset<64>{pixels} << "\n";
    }

    std::cout << "\n" << std::endl;
  }
}