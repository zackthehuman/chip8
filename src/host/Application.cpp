#include "host/Application.hpp"
#include "chip8/Functions.hpp"

namespace host {

  Application::Application(chip8::VirtualMachine & vm) : vm{vm} {

  }

  int Application::run() {
    chip8::reset(vm);
    return 0;
  }
}