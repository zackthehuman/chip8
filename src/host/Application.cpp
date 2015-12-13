#include "host/Application.hpp"
#include "host/ToneGenerator.hpp"
#include "chip8/Functions.hpp"
#include "chip8/VirtualMachine.hpp"
#include <iostream>
#include <chrono>

namespace host {

  Application::Application(chip8::VirtualMachine & vm)
    : vm{vm}
    , window{nullptr, &SDL_DestroyWindow}
    , renderer{nullptr, &SDL_DestroyRenderer}
    , event{}
    , pixelRect{}
    , quit{false}
    , paused{false}
  {
    if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0){
      std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
    }

    window.reset(SDL_CreateWindow(
      "CHIP-8",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      SCREEN_WIDTH,
      SCREEN_HEIGHT,
      SDL_WINDOW_SHOWN
    ));

    pixelRect.x = -10;
    pixelRect.y = -10;
    pixelRect.w = 10;
    pixelRect.h = 10;
  }

  Application::~Application() {
    SDL_Quit();
  }

  int Application::run() {
    ToneGenerator toneGenerator;

    chip8::reset(vm);

    if(window == nullptr) {
      std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
    } else {
      renderer.reset(SDL_CreateRenderer(
        window.get(),
        -1,
        SDL_RENDERER_ACCELERATED
      ));

      if(renderer) {
        SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer.get());
      }

      // 60hz
      using FramePeriod = std::chrono::duration<long long, std::ratio<1, 60>>;
      auto prev = std::chrono::high_resolution_clock::now();
      auto current = prev;

      while(!quit) {
        handleEvents();

        current = std::chrono::high_resolution_clock::now();
        auto difference = current - prev;

        if(!paused) {
          if(difference > FramePeriod{1}) {
            prev = current;

            if(vm.timers.delay > 0) {
              vm.timers.delay -= 1;
            }

            if(vm.timers.sound > 0) {
              vm.timers.sound -= 1;
            }
          }

          if(vm.timers.sound > 0) {
            toneGenerator.activate();
          } else {
            toneGenerator.deactivate();
          }

          updateEmulator();
          updateScreen();
        }
      }
    }

    return 0;
  }

  void Application::handleEvents() {
    while(SDL_PollEvent(&event)) {
      if(event.type == SDL_QUIT) {
        quit = true;
      } else if(event.type == SDL_WINDOWEVENT) {
        switch(event.window.event) {
          case SDL_WINDOWEVENT_FOCUS_GAINED:
            paused = false;
            break;
          case SDL_WINDOWEVENT_FOCUS_LOST:
            paused = true;
            break;
          default:
            break;
        }
      } else if(event.type == SDL_KEYDOWN) {
        switch(event.key.keysym.sym) {
          case SDLK_1:
            chip8::handleKeypress(vm, 0x0);
            break;
          case SDLK_2:
            chip8::handleKeypress(vm, 0x1);
            break;
          case SDLK_3:
            chip8::handleKeypress(vm, 0x2);
            break;
          case SDLK_4:
            chip8::handleKeypress(vm, 0x3);
            break;
          case SDLK_q:
            chip8::handleKeypress(vm, 0x4);
            break;
          case SDLK_w:
            chip8::handleKeypress(vm, 0x5);
            break;
          case SDLK_e:
            chip8::handleKeypress(vm, 0x6);
            break;
          case SDLK_r:
            chip8::handleKeypress(vm, 0x7);
            break;
          case SDLK_a:
            chip8::handleKeypress(vm, 0x8);
            break;
          case SDLK_s:
            chip8::handleKeypress(vm, 0x9);
            break;
          case SDLK_d:
            chip8::handleKeypress(vm, 0xA);
            break;
          case SDLK_f:
            chip8::handleKeypress(vm, 0xB);
            break;
          case SDLK_z:
            chip8::handleKeypress(vm, 0xC);
            break;
          case SDLK_x:
            chip8::handleKeypress(vm, 0xD);
            break;
          case SDLK_c:
            chip8::handleKeypress(vm, 0xE);
            break;
          case SDLK_v:
            chip8::handleKeypress(vm, 0xF);
            break;
          default:
            break;
        }
      } else if(event.type == SDL_KEYUP) {
        switch(event.key.keysym.sym) {
          case SDLK_1:
            chip8::handleKeyRelease(vm, 0x0);
            break;
          case SDLK_2:
            chip8::handleKeyRelease(vm, 0x1);
            break;
          case SDLK_3:
            chip8::handleKeyRelease(vm, 0x2);
            break;
          case SDLK_4:
            chip8::handleKeyRelease(vm, 0x3);
            break;
          case SDLK_q:
            chip8::handleKeyRelease(vm, 0x4);
            break;
          case SDLK_w:
            chip8::handleKeyRelease(vm, 0x5);
            break;
          case SDLK_e:
            chip8::handleKeyRelease(vm, 0x6);
            break;
          case SDLK_r:
            chip8::handleKeyRelease(vm, 0x7);
            break;
          case SDLK_a:
            chip8::handleKeyRelease(vm, 0x8);
            break;
          case SDLK_s:
            chip8::handleKeyRelease(vm, 0x9);
            break;
          case SDLK_d:
            chip8::handleKeyRelease(vm, 0xA);
            break;
          case SDLK_f:
            chip8::handleKeyRelease(vm, 0xB);
            break;
          case SDLK_z:
            chip8::handleKeyRelease(vm, 0xC);
            break;
          case SDLK_x:
            chip8::handleKeyRelease(vm, 0xD);
            break;
          case SDLK_c:
            chip8::handleKeyRelease(vm, 0xE);
            break;
          case SDLK_v:
            chip8::handleKeyRelease(vm, 0xF);
            break;
          default:
            break;
        }
      }
    }
  }

  void Application::updateEmulator() {
    chip8::cycle(vm);
  }

  void Application::updateScreen() {
    if(renderer && vm.graphicsAreDirty) {
      // Clear screen with black.
      SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, SDL_ALPHA_OPAQUE);
      SDL_RenderClear(renderer.get());
      SDL_SetRenderDrawColor(renderer.get(), 255, 255, 255, SDL_ALPHA_OPAQUE);

      // Draw VM's graphics memory to screen.
      const auto & graphics = vm.graphics;
      const auto rows = graphics.size();

      for(std::size_t y = 0; y < rows; y++) {
        const auto pixels = graphics[y];
        std::uint64_t pixelMask = 0b1000000000000000000000000000000000000000000000000000000000000000;

        for(std::size_t x = 0, size = sizeof(pixels) * 8; x < size; x++) {
          bool isToggled = (pixels & pixelMask) != 0;

          if(isToggled) {
            pixelRect.x = x * 10;
            pixelRect.y = y * 10;
            SDL_RenderFillRect(renderer.get(), &pixelRect);
          }

          pixelMask >>= 1;
        }
      }

      // Flip buffers.
      SDL_RenderPresent(renderer.get());

      vm.graphicsAreDirty = false;
    }
  }

}