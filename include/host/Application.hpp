#pragma once

namespace chip8 {
  struct VirtualMachine;
}

namespace host {

  class Application {
  private:
    chip8::VirtualMachine & vm;

  public:
    Application(chip8::VirtualMachine & vm);

    int run();
  };

}