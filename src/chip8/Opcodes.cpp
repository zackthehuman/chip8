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
      vm.graphics.fill(0);
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
        vm.programCounter += 2;
      }
    }

    void skipIfNotEquals(VirtualMachine & vm, Instruction instruction) {
      Nibble x;
      Byte nn;

      std::tie(x, nn) = getXNN(instruction);

      auto value = vm.registers[x];

      if(value != nn) {
        vm.programCounter += 2;
      }
    }

    void skipIfVxEqualsVy(VirtualMachine & vm, Instruction instruction) {
      Byte x, y;

      std::tie(x, y) = getXY(instruction);

      auto registerX = vm.registers[x];
      auto registerY = vm.registers[y];

      if(registerX == registerY) {
        vm.programCounter += 2;
      }
    }

    void setVx(VirtualMachine & vm, Instruction instruction) {
      Nibble x;
      Byte nn;

      std::tie(x, nn) = getXNN(instruction);

      vm.registers[x] = nn;
    }

    void addToVx(VirtualMachine & vm, Instruction instruction) {
      Nibble x;
      Byte nn;

      std::tie(x, nn) = getXNN(instruction);

      vm.registers[x] += nn;
    }

    void disambiguate0x8(VirtualMachine & vm, Instruction instruction) {
      const Instruction LAST_NIBBLE_MASK = 0x000F;

      switch(instruction & LAST_NIBBLE_MASK) {
        case 0x0:
          setVxToVy(vm, instruction);
          break;

        case 0x1:
          orVxVy(vm, instruction);
          break;

        case 0x2:
          andVxVy(vm, instruction);
          break;

        case 0x3:
          xorVxVy(vm, instruction);
          break;

        case 0x4:
          addVxVyUpdateCarry(vm, instruction);
          break;

        case 0x5:
          subtractVxVyUpdateCarry(vm, instruction);
          break;

        case 0x6:
        case 0x7:
        case 0xE:

        default:

          break;
      }
    }

    void setVxToVy(VirtualMachine & vm, Instruction instruction) {
      Byte x, y;

      std::tie(x, y) = getXY(instruction);

      auto registerY = vm.registers[y];

      vm.registers[x] = registerY;
    }

    void orVxVy(VirtualMachine & vm, Instruction instruction) {
      Byte x, y;

      std::tie(x, y) = getXY(instruction);

      auto registerX = vm.registers[x];
      auto registerY = vm.registers[y];

      vm.registers[x] = registerX | registerY;
    }

    void andVxVy(VirtualMachine & vm, Instruction instruction) {
      Byte x, y;

      std::tie(x, y) = getXY(instruction);

      auto registerX = vm.registers[x];
      auto registerY = vm.registers[y];

      vm.registers[x] = registerX & registerY;
    }

    void xorVxVy(VirtualMachine & vm, Instruction instruction) {
      Byte x, y;

      std::tie(x, y) = getXY(instruction);

      auto registerX = vm.registers[x];
      auto registerY = vm.registers[y];

      vm.registers[x] = registerX ^ registerY;
    }

    void addVxVyUpdateCarry(VirtualMachine & vm, Instruction instruction) {
      Byte x, y;

      std::tie(x, y) = getXY(instruction);

      auto registerX = vm.registers[x];
      auto registerY = vm.registers[y];

      Byte result = registerY + registerX;

      // Update VF with the carry bit.
      // For unsigned integers we can just check for overflow to determine carry.
      if(result < registerX) {
        vm.registers[0xF] = 1;
      } else {
        vm.registers[0xF] = 0;
      }

      vm.registers[x] = result;
    }

    void subtractVxVyUpdateCarry(VirtualMachine & vm, Instruction instruction) {
      Byte x, y;

      std::tie(x, y) = getXY(instruction);

      auto registerX = vm.registers[x];
      auto registerY = vm.registers[y];

      const bool requiresBorrow = registerY > registerX;

      // VX = VX - VY
      // Inverting and adding plus one allows us to perform addition instead of
      // subtraction, since these are unsigned numbers.
      Byte result = registerX + (~registerY + 1);

      // Update VF with the carry bit.
      if(requiresBorrow) {
        vm.registers[0xF] = 0;
      } else {
        vm.registers[0xF] = 1;
      }

      vm.registers[x] = result;
    }

    void skipIfVxNotEqualsVy(VirtualMachine & vm, Instruction instruction) {
      Byte x, y;

      std::tie(x, y) = getXY(instruction);

      auto registerX = vm.registers[x];
      auto registerY = vm.registers[y];

      if(registerX != registerY) {
        vm.programCounter += 2;
      }
    }

    void setIToAddress(VirtualMachine & vm, Instruction instruction) {
      Address address = getAddress(instruction);

      vm.I = address;
    }

    void jumpPlusV0(VirtualMachine & vm, Instruction instruction) {
      auto offset = vm.registers[0];

      vm.programCounter = getAddress(instruction) + offset;
    }

    void randomVxModNn(VirtualMachine & vm, Instruction instruction) {
      Nibble x;
      Byte nn;

      std::tie(x, nn) = getXNN(instruction);

      const auto random = vm.rng(0);

      vm.registers[x] = random & nn;
    }

    void blit(VirtualMachine & vm, Instruction instruction) {
      // Nibble x, y, n;
      // Address pointer = vm.I;

      // std::tie(x, y, n) = getXYN(instruction);

      // const auto startX = vm.registers[x];
      // const auto startY = vm.registers[y];

      // for(std::size_t i = 0; i < n; i++) {
      //   auto offsetX = x;
      //   auto offsetY = startY + i;


      // }

      throw std::runtime_error("Function not implemented yet.");
    }
  }
}