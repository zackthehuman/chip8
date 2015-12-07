#include <array>
#include <cstdint>
#include <stack>

const std::size_t RAM_SIZE = 4096;
const std::size_t REGISTER_COUNT = 16;

using Byte = std::int8_t;

template <std::size_t N>
using ByteArray = std::array<Byte, N>;

using Stack = std::stack<Byte>;

struct Timers {
  Byte delay;
  Byte sound;

  Timers()
    : delay(0)
    , sound(0)
  {

  }
};

struct CPU {
  ByteArray<REGISTER_COUNT> registers;
  Byte I; // address register
  Timers timers;
  Stack stack;

  CPU()
    : registers{}
    , I{0}
    , timers{}
    , stack{}
  {
    registers.fill(0);
  }
};

int main(int argc, char** argv) {
  ByteArray<RAM_SIZE> memory;
  CPU cpu;

  memory.fill(0);

  return 0;
}