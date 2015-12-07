#pragma once
#include "chip8/Types.hpp"
#include "chip8/Constants.hpp"
#include "chip8/Timers.hpp"

namespace chip8 {
  struct VirtualMachine {
    ByteArray<RAM_SIZE> memory;
    ByteArray<REGISTER_COUNT> registers;
    Address programCounter;
    Byte I; // address register
    Timers timers;
    Stack stack;

    VirtualMachine()
      : memory{}
      , registers{}
      , programCounter{0}
      , I{0}
      , timers{}
      , stack{}
    {
      memory.fill(0);
      registers.fill(0);
    }
  };
}