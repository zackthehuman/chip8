# CHIP-8 Emulator

An emulator for [CHIP-8][1] written in C++.

This project is split into two parts: the emulator and the host application. The emulator consists of a virtual machine and a set of functions used to execute instructions. The host application is a simple [SDL2][2]-based shell that draws the emulator's graphics to a window and relays keyboard state to the emulator. 

## Building instructions
Ensure that you have installed the [SDL2][2] development libraries before building the emulator and host application. You'll also need a C++ compiler that supports C++11 and C++14 features like `auto`, `decltype`, `std::tuple`, lambdas, and binary literals.

To build:

    git clone https://github.com/zackthehuman/chip8.git
    mkdir chip8-build
    cd chip8-build
    cmake -G "Unix Makefiles" ../chip8
    make

To load a rom:

    ./chip8 brix.chip8
    
## Notes
There is test coverage for each of the CHIP-8 opcodes and several of the associated helper functions, however, there are probably still bugs that haven't been uncovered.

[1]: https://en.wikipedia.org/wiki/CHIP-8
[2]: https://www.libsdl.org/