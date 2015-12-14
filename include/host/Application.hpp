#pragma once
#include <SDL.h>
#include <memory>

namespace chip8 {
  struct VirtualMachine;
}

namespace host {

  using SDL2WindowPtr = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>;
  using SDL2RendererPtr = std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>;
  const int SCREEN_WIDTH {640};
  const int SCREEN_HEIGHT {480};

  class Application {
  private:
    chip8::VirtualMachine & vm;
    SDL2WindowPtr window;
    SDL2RendererPtr renderer;
    SDL_Event event;
    SDL_Rect pixelRect;
    bool quit;
    bool paused;
    bool enableSound;

  public:
    Application(chip8::VirtualMachine & vm);
    ~Application();

    int run();

    void handleEvents();
    void updateEmulator();
    void updateScreen();
  };

}