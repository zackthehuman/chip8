#include "host/Application.hpp"
#include "chip8/Functions.hpp"
#include "chip8/VirtualMachine.hpp"
#include <iostream>

namespace host {

  Application::Application(chip8::VirtualMachine & vm)
    : vm{vm}
    , window{nullptr, &SDL_DestroyWindow}
  {
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
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
  }

  Application::~Application() {
    SDL_Quit();
  }

  int Application::run() {

    chip8::reset(vm);

    if(window == nullptr) {
      std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
    } else {
      SDL2RendererPtr renderer { SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED), &SDL_DestroyRenderer };
      SDL_Event e;
      SDL_Rect pixelRect;

      pixelRect.x = -10;
      pixelRect.y = -10;
      pixelRect.w = 10;
      pixelRect.h = 10;

      bool quit = false;

      while(!quit) {
        while(SDL_PollEvent(&e)) {
          if(e.type == SDL_QUIT) {
            quit = true;
          } else if(e.type == SDL_KEYDOWN) {
            switch(e.key.keysym.sym) {
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
          } else if(e.type == SDL_KEYUP) {
            switch(e.key.keysym.sym) {
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

        // Clear screen with black.
        SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer.get());
        SDL_SetRenderDrawColor(renderer.get(), 255, 255, 255, SDL_ALPHA_OPAQUE);

        chip8::cycle(vm);
        // std::cout << "cycle" << std::endl;

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
        SDL_Delay( 3);
      }
    }

    return 0;
  }
}