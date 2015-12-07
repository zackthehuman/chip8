#include "catch.hpp"
#include "chip8/VirtualMachine.hpp"
#include "chip8/Functions.hpp"

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