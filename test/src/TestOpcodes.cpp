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

  SECTION( "ops::skipIfEquals increments the program counter by 2 if VX == NN" ) {
    vm.registers[0xD] = 0x42;
    auto pc = vm.programCounter;

    chip8::ops::skipIfEquals(vm, 0x3D42);

    REQUIRE( vm.programCounter == (pc + 2) );
  }

  SECTION( "ops::skipIfEquals does not increment the program counter by 2 if VX != NN" ) {
    vm.registers[0xD] = 0x99;
    auto pc = vm.programCounter;

    chip8::ops::skipIfEquals(vm, 0x3D42);

    REQUIRE( vm.programCounter == pc );
  }

  SECTION( "ops::skipIfNotEquals increments the program counter by 2 if VX != NN" ) {
    vm.registers[0xD] = 0x42;
    auto pc = vm.programCounter;

    chip8::ops::skipIfNotEquals(vm, 0x4D99);

    REQUIRE( vm.programCounter == (pc + 2) );
  }

  SECTION( "ops::skipIfNotEquals does not increment the program counter by 2 if VX == NN" ) {
    vm.registers[0xD] = 0x42;
    auto pc = vm.programCounter;

    chip8::ops::skipIfNotEquals(vm, 0x4D42);

    REQUIRE( vm.programCounter == pc );
  }

  SECTION( "ops::skipIfVxEqualsVy increments the program counter by 2 if VX == VY" ) {
    vm.registers[0x3] = 0x42;
    vm.registers[0xD] = 0x42;
    auto pc = vm.programCounter;

    chip8::ops::skipIfVxEqualsVy(vm, 0x53D0);

    REQUIRE( vm.programCounter == (pc + 2) );
  }

  SECTION( "ops::skipIfVxEqualsVy does not increment the program counter by 2 if VX != VY" ) {
    vm.registers[0x3] = 0x42;
    vm.registers[0xD] = 0x31;
    auto pc = vm.programCounter;

    chip8::ops::skipIfVxEqualsVy(vm, 0x53D0);

    REQUIRE( vm.programCounter == pc );
  }

  SECTION( "ops::setVx sets the register at VX to NN" ) {
    chip8::ops::setVx(vm, 0x63D0);

    REQUIRE( vm.registers[0x3] == 0xD0 );
  }

  SECTION( "ops::addToVx adds NN to the register at VX" ) {
    vm.registers[0xA] = 0x11;

    chip8::ops::addToVx(vm, 0x7A22);

    REQUIRE( vm.registers[0xA] == (0x11 + 0x22) );
  }

  SECTION( "ops::setVxToVy sets VX to the value of register VY" ) {
    vm.registers[0x0] = 0x11;
    vm.registers[0x1] = 0x22;

    chip8::ops::setVxToVy(vm, 0x8010);

    REQUIRE( vm.registers[0x0] == 0x22 );
  }

  SECTION( "ops::disambiguate0x8 calls ops::setVxToVy when given 0x8000" ) {
    vm.registers[0x0] = 0x11;
    vm.registers[0x1] = 0x22;

    chip8::ops::disambiguate0x8(vm, 0x8010);

    REQUIRE( vm.registers[0x0] == 0x22 );
  }

  SECTION( "ops::orVxVy sets VX to the value VX | VY" ) {
    vm.registers[0x0] = 0x10;
    vm.registers[0x1] = 0x02;

    chip8::ops::orVxVy(vm, 0x8011);

    REQUIRE( vm.registers[0x0] == (0x10 | 0x02) );
  }

  SECTION( "ops::disambiguate0x8 calls ops::orVxVy when given 0x8001" ) {
    vm.registers[0x0] = 0x10;
    vm.registers[0x1] = 0x02;

    chip8::ops::disambiguate0x8(vm, 0x8011);

    REQUIRE( vm.registers[0x0] == (0x10 | 0x02) );
  }

  SECTION( "ops::andVxVy sets VX to the value VX & VY" ) {
    vm.registers[0x0] = 0x11;
    vm.registers[0x1] = 0x02;

    chip8::ops::andVxVy(vm, 0x8012);

    REQUIRE( vm.registers[0x0] == (0x11 & 0x02) );
  }

  SECTION( "ops::disambiguate0x8 calls ops::andVxVy when given 0x8002" ) {
    vm.registers[0x0] = 0x11;
    vm.registers[0x1] = 0x02;

    chip8::ops::disambiguate0x8(vm, 0x8012);

    REQUIRE( vm.registers[0x0] == (0x11 & 0x02) );
  }

  SECTION( "ops::xorVxVy sets VX to the value VX ^ VY" ) {
    vm.registers[0x0] = 0x11;
    vm.registers[0x1] = 0x02;

    chip8::ops::xorVxVy(vm, 0x8012);

    REQUIRE( vm.registers[0x0] == (0x11 ^ 0x02) );
  }

  SECTION( "ops::disambiguate0x8 calls ops::xorVxVy when given 0x8003" ) {
    vm.registers[0x0] = 0x11;
    vm.registers[0x1] = 0x02;

    chip8::ops::disambiguate0x8(vm, 0x8013);

    REQUIRE( vm.registers[0x0] == (0x11 ^ 0x02) );
  }

  SECTION( "ops::skipIfVxNotEqualsVy increments the program counter by 2 if VX != VY" ) {
    vm.registers[0x3] = 0x42;
    vm.registers[0xD] = 0x24;
    auto pc = vm.programCounter;

    chip8::ops::skipIfVxNotEqualsVy(vm, 0x93D0);

    REQUIRE( vm.programCounter == (pc + 2) );
  }

  SECTION( "ops::skipIfVxNotEqualsVy does not increment the program counter by 2 if VX == VY" ) {
    vm.registers[0x3] = 0x42;
    vm.registers[0xD] = 0x42;
    auto pc = vm.programCounter;

    chip8::ops::skipIfVxNotEqualsVy(vm, 0x93D0);

    REQUIRE( vm.programCounter == pc );
  }

  SECTION( "ops::setIToAddress sets I to the specified address" ) {
    chip8::ops::setIToAddress(vm, 0xA123);

    REQUIRE( vm.I == 0x123 );
  }
}