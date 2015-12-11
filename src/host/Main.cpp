#include "chip8/Types.hpp"
#include "chip8/Constants.hpp"
#include "chip8/Timers.hpp"
#include "chip8/VirtualMachine.hpp"
#include "chip8/Functions.hpp"
#include "host/FileUtilities.hpp"
#include "host/Application.hpp"
#include <iostream>
#include <string>
#include <random>
#include <vector>

int main(int argc, char** argv) {
  using namespace chip8;

  const std::vector<std::string> allArgs(argv, argv + argc);

  std::string filePath{"brix.chip8"};

  if(allArgs.size() > 1) {
    filePath = allArgs[1];
  }

  VirtualMachine vm;
  host::Application app{vm};

  std::random_device rd;
  std::mt19937 mt{rd()};
  std::uniform_int_distribution<std::uint16_t> dist{0x00, 0xFF};

  vm.rng = [&dist, &mt](Byte seed) -> Byte {
    return static_cast<Byte>(dist(mt));
  };

  loadFontData(vm, chip8::FONT_DATA);
  loadRomData(vm, host::readFileAsChar(filePath));

  return app.run();
}