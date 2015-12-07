#include "catch.hpp"
#include "chip8/VirtualMachine.hpp"
#include "chip8/Functions.hpp"
#include "chip8/Opcodes.hpp"
#include <utility>

TEST_CASE( "Byte extraction function", "extracting bytes from opcodes" ) {

  SECTION( "getHighByte returns the high byte of an opcode" ) {
    const chip8::Instruction opcode = 0x1234;
    const chip8::Byte result = chip8::getHighByte(opcode);

    REQUIRE( result == 0x12 );
  }

  SECTION( "getLowByte returns the low byte of an opcode" ) {
    const chip8::Instruction opcode = 0x1234;
    const chip8::Byte result = chip8::getLowByte(opcode);

    REQUIRE( result == 0x34 );
  }

  SECTION( "getAddress returns the address from an opcode" ) {
    const chip8::Instruction opcode = 0x1234;
    const chip8::Address result = chip8::getAddress(opcode);

    REQUIRE( result == 0x234 );
  }

  SECTION( "getXY returns the two nibbles from an opcode" ) {
    const chip8::Instruction opcode = 0x1234;
    auto result = chip8::getXY(opcode);
    chip8::Nibble x, y;

    std::tie(x, y) = result;

    REQUIRE( x == 0x2 );
    REQUIRE( y == 0x3 );
  }

  SECTION( "getXNN returns the a nibble and a byte from an opcode" ) {
    const chip8::Instruction opcode = 0x1234;
    auto result = chip8::getXNN(opcode);
    chip8::Nibble x;
    chip8::Byte nn;

    std::tie(x, nn) = result;

    REQUIRE( x == 0x2 );
    REQUIRE( nn == 0x34 );
  }
}

TEST_CASE( "Basic VM functions", "fetch return value and side effects" ) {
  chip8::VirtualMachine vm;

  REQUIRE( vm.memory.size() == 4096 );
  REQUIRE( vm.registers.size() == 16 );
  REQUIRE( vm.programCounter == 0 );
  REQUIRE( vm.I == 0 );
  REQUIRE( vm.timers.sound == 0 );
  REQUIRE( vm.timers.delay == 0 );
  REQUIRE( vm.stack.size() == 0 );

  SECTION( "fetch should return the value at the current program counter, increasing the program counter by 1" ) {
    vm.memory[0] = 0xAB;

    auto pc = vm.programCounter;
    auto result = chip8::fetch(vm);

    REQUIRE( result == 0xAB );
    REQUIRE( vm.programCounter == (pc + 1) );
  }
}

TEST_CASE( "VM opcode functions", "execution of opcodes" ) {
  chip8::VirtualMachine vm;

  REQUIRE( vm.memory.size() == 4096 );
  REQUIRE( vm.registers.size() == 16 );
  REQUIRE( vm.programCounter == 0 );
  REQUIRE( vm.I == 0 );
  REQUIRE( vm.timers.sound == 0 );
  REQUIRE( vm.timers.delay == 0 );
  REQUIRE( vm.stack.size() == 0 );

  SECTION( "ops::jump changes the program counter" ) {
    chip8::ops::jump(vm, 0x1234);

    REQUIRE( vm.programCounter == 0x234 );
  }

  SECTION( "ops::callSubroutine changes the program counter, pushes old program counter to stack" ) {
    vm.programCounter = 0x42;

    chip8::ops::callSubroutine(vm, 0x2123);

    REQUIRE( vm.programCounter == 0x123 );
    REQUIRE( vm.stack.size() == 1 );
    REQUIRE( vm.stack.top() == 0x42 );
  }

  SECTION( "ops::returnFromSubroutine changes the program counter, pops stack" ) {
    vm.stack.push(0x0867);

    chip8::ops::returnFromSubroutine(vm, 0x00EE);

    REQUIRE( vm.programCounter == 0x0867 );
    REQUIRE( vm.stack.size() == 0 );
  }

  SECTION( "ops::skipIfEquals increments the program counter by 1 if VX == NN" ) {
    vm.registers[0xD] = 0x42;
    auto pc = vm.programCounter;

    chip8::ops::skipIfEquals(vm, 0x3D42);

    REQUIRE( vm.programCounter == (pc + 1) );
  }

  SECTION( "ops::skipIfEquals does not increment the program counter by 1 if VX != NN" ) {
    vm.registers[0xD] = 0x99;
    auto pc = vm.programCounter;

    chip8::ops::skipIfEquals(vm, 0x3D42);

    REQUIRE( vm.programCounter == pc );
  }

  SECTION( "ops::skipIfNotEquals increments the program counter by 1 if VX != NN" ) {
    vm.registers[0xD] = 0x42;
    auto pc = vm.programCounter;

    chip8::ops::skipIfNotEquals(vm, 0x4D99);

    REQUIRE( vm.programCounter == (pc + 1) );
  }

  SECTION( "ops::skipIfNotEquals does not increment the program counter by 1 if VX == NN" ) {
    vm.registers[0xD] = 0x42;
    auto pc = vm.programCounter;

    chip8::ops::skipIfNotEquals(vm, 0x4D42);

    REQUIRE( vm.programCounter == pc );
  }
}