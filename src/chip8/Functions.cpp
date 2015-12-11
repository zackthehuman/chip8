#include "chip8/Functions.hpp"
#include "chip8/VirtualMachine.hpp"
#include "chip8/Opcodes.hpp"
#include <algorithm>
#include <array>
#include <bitset>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>

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
    ops::blit,                // 0xD000
    ops::disambiguate0xE,     // 0xE000
    ops::disambiguate0xF      // 0xF000
  } };

  Instruction fetch(VirtualMachine & vm) {
    Instruction highByte = static_cast<Instruction>(vm.memory[vm.programCounter]) << 8;
    Instruction lowByte = static_cast<Instruction>(vm.memory[vm.programCounter + 1]);

    vm.programCounter += 2;

    return highByte | lowByte;
  }

  void execute(VirtualMachine & vm, Instruction instruction) {
    // no-op
    const Instruction OP_MASK = 0xF000;
    const auto handlerOffset = (instruction & OP_MASK) >> 12;

    FUNCTION_TABLE[handlerOffset](vm, instruction);
  }

  void cycle(VirtualMachine & vm) {
    if(!vm.awaitingKeypress) {
      if(vm.timers.delay > 0) {
        vm.timers.delay -= 1;
      }

      if(vm.timers.sound > 0) {
        vm.timers.sound -= 1;
      }

      execute(vm, fetch(vm));
    }
  }

  void reset(VirtualMachine & vm) {
    // Programs begin at memory location 512.
    vm.programCounter = PROGRAM_START_ADDRESS;
  }

  void handleKeypress(VirtualMachine & vm, Byte key) {
    if(vm.awaitingKeypress) {
      vm.awaitingKeypress = false;
      vm.registers[vm.nextKeypressRegister] = key;
    }

    vm.keyboard[key] = 1;
  }

  void handleKeyRelease(VirtualMachine & vm, Byte key) {
    vm.keyboard[key] = 0;
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

  void loadRomData(VirtualMachine & vm, const std::vector<char> & data) {
    std::transform(
      std::begin(data),
      std::end(data),
      std::begin(vm.memory) + PROGRAM_START_ADDRESS,
      [](const char c) {
        return static_cast<Byte>(c);
      }
    );
  }

  void loadFontData(VirtualMachine & vm, const std::vector<Byte> & data) {
    std::transform(
      std::begin(data),
      std::end(data),
      std::begin(vm.memory), // Load the font dats at the beginning of memory.
      [](const char c) {
        return static_cast<Byte>(c);
      }
    );
  }
}