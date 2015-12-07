#include "catch.hpp"
#include "chip8/VirtualMachine.hpp"
#include "chip8/Opcodes.hpp"
#include <utility>

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

  SECTION( "ops::skipIfVxEqualsVy increments the program counter by 1 if VX == VY" ) {
    vm.registers[0x3] = 0x42;
    vm.registers[0xD] = 0x42;
    auto pc = vm.programCounter;

    chip8::ops::skipIfVxEqualsVy(vm, 0x43D0);

    REQUIRE( vm.programCounter == (pc + 1) );
  }

  SECTION( "ops::skipIfVxEqualsVy does not increment the program counter by 1 if VX != VY" ) {
    vm.registers[0x3] = 0x42;
    vm.registers[0xD] = 0x31;
    auto pc = vm.programCounter;

    chip8::ops::skipIfVxEqualsVy(vm, 0x43D0);

    REQUIRE( vm.programCounter == pc );
  }
}