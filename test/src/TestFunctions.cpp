#include "catch.hpp"
#include "chip8/VirtualMachine.hpp"
#include "chip8/Functions.hpp"
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