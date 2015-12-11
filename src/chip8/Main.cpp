#include "chip8/Types.hpp"
#include "chip8/Constants.hpp"
#include "chip8/Timers.hpp"
#include "chip8/VirtualMachine.hpp"
#include "chip8/Functions.hpp"
#include "host/FileUtilities.hpp"
#include <iostream>
#include <random>

int main(int argc, char** argv) {
  using namespace chip8;

  VirtualMachine vm;

  std::random_device rd;
  std::mt19937 mt{rd()};
  std::uniform_int_distribution<std::uint16_t> dist{0x00, 0xFF};

  vm.rng = [&dist, &mt](Byte seed) -> Byte {
    return static_cast<Byte>(dist(mt));
  };

  loadRomData(vm, host::readFileAsChar("breakout.chip8"));
  reset(vm);

  for(auto i = 0; i < 1000; i++) {
    cycle(vm);
  }

  printGraphicsBufferToConsole(vm);

  return 0;
}