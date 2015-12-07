#include "catch.hpp"
#include "chip8/VirtualMachine.hpp"
#include "chip8/Functions.hpp"

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

  SECTION( "fetch should return the value at the current program counter, increasing the program counter by 2" ) {
    vm.memory[0] = 0xA0;
    vm.memory[1] = 0xB0;

    auto pc = vm.programCounter;
    auto result = chip8::fetch(vm);

    REQUIRE( result == 0xA0B0 );
    REQUIRE( vm.programCounter == (pc + 2) );
  }

  SECTION( "cycle should decrement the delay and sound counters by 1 if they're non-zero" ) {
    vm.memory[0] = 0x80; // set register 0 to itself
    vm.memory[1] = 0x00;
    vm.memory[2] = 0x10; // jump back to the beginning
    vm.memory[3] = 0x00;

    vm.timers.delay = 10;
    vm.timers.sound = 15;

    auto pc = vm.programCounter;

    chip8::cycle(vm);

    REQUIRE( vm.programCounter == (pc + 2) );
    REQUIRE( vm.timers.delay == 9 );
    REQUIRE( vm.timers.sound == 14 );
  }

  SECTION( "cycle should not decrement the delay and sound counters by 1 if they're equal to zero" ) {
    vm.memory[0] = 0x80; // set register 0 to itself
    vm.memory[1] = 0x00;
    vm.memory[2] = 0x10; // jump back to the beginning
    vm.memory[3] = 0x00;

    vm.timers.delay = 0;
    vm.timers.sound = 0;

    chip8::cycle(vm);

    REQUIRE( vm.timers.delay == 0 );
    REQUIRE( vm.timers.sound == 0 );
  }
}
