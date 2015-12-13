#pragma once
#include <SDL.h>
#include <cstdint>

namespace host {

  class ToneGenerator {
  private:
    static void generateTone(void * userData, std::uint8_t * stream, int length);
    static const int PAUSE_AUDIO = 1;
    static const int UNPAUSE_AUDIO = 0;

    SDL_AudioSpec desired;
    SDL_AudioSpec obtained;
    SDL_AudioDeviceID device;

    bool active;

  public:
    ToneGenerator();
    ~ToneGenerator();

    void activate();
    void deactivate();
  };

}