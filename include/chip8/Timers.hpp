#pragma once
#include "chip8/Types.hpp"

namespace chip8 {
  struct Timers {
    Byte delay;
    Byte sound;

    Timers()
      : delay(0)
      , sound(0)
    {

    }
  };
}