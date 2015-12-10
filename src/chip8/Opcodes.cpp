#include "chip8/Opcodes.hpp"
#include "chip8/Functions.hpp"
#include "chip8/VirtualMachine.hpp"
#include <exception>
#include <bitset>
#include <iomanip>
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
          rightshiftVx(vm, instruction);
          break;

        case 0x7:
          subtractVxFromVyUpdateCarry(vm, instruction);
          break;

        case 0xE:
          leftshiftVx(vm, instruction);
          break;

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

    void rightshiftVx(VirtualMachine & vm, Instruction instruction) {
      Byte x;

      std::tie(x, std::ignore) = getXY(instruction);

      const auto registerX = vm.registers[x];

      // Grab the least-significant bit.
      const Byte lsb = registerX & 0b00000001;

      vm.registers[x] = registerX >> 1;
      vm.registers[0xF] = lsb;
    }

    void subtractVxFromVyUpdateCarry(VirtualMachine & vm, Instruction instruction) {
      Byte x, y;

      std::tie(x, y) = getXY(instruction);

      auto registerX = vm.registers[x];
      auto registerY = vm.registers[y];

      const bool requiresBorrow = registerX > registerY;

      // VX = VY - VX
      // Inverting and adding plus one allows us to perform addition instead of
      // subtraction, since these are unsigned numbers.
      Byte result = registerY + (~registerX + 1);

      // Update VF with the carry bit.
      if(requiresBorrow) {
        vm.registers[0xF] = 0;
      } else {
        vm.registers[0xF] = 1;
      }

      vm.registers[x] = result;
    }

    void leftshiftVx(VirtualMachine & vm, Instruction instruction) {
      Byte x;

      std::tie(x, std::ignore) = getXY(instruction);

      const auto registerX = vm.registers[x];

      // Grab the most-significant bit. Need to shift it over to the right
      // so that the value of VF is either 0 or 1.
      const Byte msb = (registerX & 0b10000000) >> 7;

      vm.registers[x] = registerX << 1;
      vm.registers[0xF] = msb;
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
      Nibble x, y, n;
      Address pointer = vm.I;

      std::tie(x, y, n) = getXYN(instruction);

      const auto startX = vm.registers[x];
      const auto startY = vm.registers[y];
      const std::uint64_t rowProjection = 0b0000000000000000000000000000000000000000000000000000000000000000;

      // Loop over sprite rows that need to be rendered.
      for(std::size_t i = 0; i < n; i++) {
        const auto spriteRow = vm.memory[pointer + i];
        auto offsetX = startX;
        auto offsetY = (startY + i) % 32;
        auto shift = ((sizeof(rowProjection) - sizeof(spriteRow)) * 8) % 64;

        // Calculate a shift to the left enough to move the sprite all the way
        // to the left-most position, like a typewriter moving to a new line.
        // Then we rotate right to allow for horizontal positioning and wrapping.
        const auto spriteProjection = rotateRight((rowProjection | spriteRow) << shift, offsetX);

        vm.graphics[offsetY] ^= spriteProjection;

        const auto diff = ~vm.graphics[offsetY] & spriteProjection;

        if(diff != 0) {
          vm.registers[0xF] = 1;
        } else {
          vm.registers[0xF] = 0;
        }
      }
    }

    void disambiguate0xE(VirtualMachine & vm, Instruction instruction) {
      const auto lowByte = getLowByte(instruction);

      switch(lowByte) {
        case 0x9E:
          skipIfKeyIsPressed(vm, instruction);
          break;

        case 0xA1:
          skipIfKeyIsNotPressed(vm, instruction);
          break;

        default:
        break;
      }
    }

    void skipIfKeyIsPressed(VirtualMachine & vm, Instruction instruction) {
      Byte x;

      std::tie(x, std::ignore) = getXY(instruction);

      auto registerX = vm.registers[x];
      auto shouldSkip = vm.keyboard[registerX] == 1;

      if(shouldSkip) {
        vm.programCounter += 2;
      }
    }

    void skipIfKeyIsNotPressed(VirtualMachine & vm, Instruction instruction) {
      Byte x;

      std::tie(x, std::ignore) = getXY(instruction);

      auto registerX = vm.registers[x];
      auto shouldSkip = vm.keyboard[registerX] == 0;

      if(shouldSkip) {
        vm.programCounter += 2;
      }
    }

    void disambiguate0xF(VirtualMachine & vm, Instruction instruction) {
      const auto lowByte = getLowByte(instruction);

      switch(lowByte) {
        case 0x07:
          setVxToDelayTimer(vm, instruction);
          break;

        default:
        break;
      }
    }

    void setVxToDelayTimer(VirtualMachine & vm, Instruction instruction) {
      Byte x;

      std::tie(x, std::ignore) = getXY(instruction);

      vm.registers[x] = vm.timers.delay;
    }

    void waitForKeyPress(VirtualMachine & vm, Instruction instruction) {
      Byte x;

      std::tie(x, std::ignore) = getXY(instruction);

      vm.awaitingKeypress = true;
      vm.nextKeypressRegister = x;
    }
  }
}