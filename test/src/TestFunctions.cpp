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

  SECTION( "getXYN returns the three nibbles from an opcode" ) {
    const chip8::Instruction opcode = 0x1234;
    auto result = chip8::getXYN(opcode);
    chip8::Nibble x, y, z;

    std::tie(x, y, z) = result;

    REQUIRE( x == 0x2 );
    REQUIRE( y == 0x3 );
    REQUIRE( z == 0x4 );
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

TEST_CASE( "Integer rotation functions", "rotating unsigned integers left and right" ) {

  SECTION( "rotateLeft rotates to the left by a specificed amount" ) {
    chip8::Byte input = 0b01000001;

    input = chip8::rotateLeft(input, 1);
    REQUIRE( input == 0b10000010 );
    input = chip8::rotateLeft(input, 1);
    REQUIRE( input == 0b00000101 );
    input = chip8::rotateLeft(input, 2);
    REQUIRE( input == 0b00010100 );
    input = chip8::rotateLeft(input, 3);
    REQUIRE( input == 0b10100000 );
  }

  SECTION( "rotateRight rotates to the right by a specificed amount" ) {
    chip8::Byte input = 0b01000001;

    input = chip8::rotateRight(input, 1);
    REQUIRE( input == 0b10100000 );
    input = chip8::rotateRight(input, 1);
    REQUIRE( input == 0b01010000 );
    input = chip8::rotateRight(input, 2);
    REQUIRE( input == 0b00010100 );
    input = chip8::rotateRight(input, 3);
    REQUIRE( input == 0b10000010 );
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
  REQUIRE( vm.awaitingKeypress == false );
  REQUIRE( vm.nextKeypressRegister == 0x0 );

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

  SECTION( "cycle should not update the VM state if waiting for a keypress" ) {
    const auto pc = vm.programCounter;
    vm.awaitingKeypress = true;
    vm.timers.delay = 1;
    vm.timers.sound = 1;

    chip8::cycle(vm);

    REQUIRE( vm.timers.delay == 1 );
    REQUIRE( vm.timers.sound == 1 );
    REQUIRE( vm.awaitingKeypress == true );
    REQUIRE( vm.programCounter == pc );
  }

  SECTION( "handleKeypress should set the keyboard bit for the corresponding key" ) {
    chip8::handleKeypress(vm, 0x0);
    REQUIRE( vm.keyboard[0x0] == 1 );
    chip8::handleKeypress(vm, 0x1);
    REQUIRE( vm.keyboard[0x1] == 1 );
    chip8::handleKeypress(vm, 0x2);
    REQUIRE( vm.keyboard[0x2] == 1 );
    chip8::handleKeypress(vm, 0x3);
    REQUIRE( vm.keyboard[0x3] == 1 );
  }

  SECTION( "handleKeypress should set awaitingKeypress to false and store the corresponding key" ) {
    vm.awaitingKeypress = true;
    vm.nextKeypressRegister = 0x3;

    chip8::handleKeypress(vm, 0xF);
    REQUIRE( vm.awaitingKeypress == false );
    REQUIRE( vm.registers[0x3] == 0xF );
  }

  SECTION( "handleKeyRelease should unset the keyboard bit for the corresponding key" ) {
    vm.keyboard[0x0] = 1;
    vm.keyboard[0x1] = 1;
    vm.keyboard[0x2] = 1;
    vm.keyboard[0x3] = 1;

    chip8::handleKeyRelease(vm, 0x0);
    REQUIRE( vm.keyboard[0x0] == 0 );
    chip8::handleKeyRelease(vm, 0x1);
    REQUIRE( vm.keyboard[0x1] == 0 );
    chip8::handleKeyRelease(vm, 0x2);
    REQUIRE( vm.keyboard[0x2] == 0 );
    chip8::handleKeyRelease(vm, 0x3);
    REQUIRE( vm.keyboard[0x3] == 0 );
  }
}
