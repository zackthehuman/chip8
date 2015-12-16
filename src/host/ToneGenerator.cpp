#include "host/ToneGenerator.hpp"
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

namespace host {
  void ToneGenerator::generateTone(void * userData, std::uint8_t * stream, int length) {
    static double angle = 0.0;

    for(int i = 0; i < length; i++) {
      int8_t sample = std::sin(angle) >= 0 ? 1 : -1;
      *stream++ = 5 * sample;
      angle += 2 * M_PI / 800;
    }
  }

  ToneGenerator::ToneGenerator()
    : desired{}
    , obtained{}
    , device{}
    , active{false}
  {
    SDL_zero(desired);
    desired.freq = 48000;
    desired.format = AUDIO_S8; // samples are int8_t
    desired.channels = 2;
    desired.samples = 4096;
    desired.callback = ToneGenerator::generateTone;

    device = SDL_OpenAudioDevice(nullptr, 0, &desired, &obtained, SDL_AUDIO_ALLOW_FORMAT_CHANGE);

    if(device == 0) {
      std::cout << "Unable to open audio device. SDL_Error: " << SDL_GetError() << std::endl;
    } else {
    }
  }

  ToneGenerator::~ToneGenerator() {
    SDL_CloseAudioDevice(device);
  }

  void ToneGenerator::activate() {
    if(!active) {
      active = true;
      SDL_PauseAudioDevice(device, UNPAUSE_AUDIO);
    }
  }

  void ToneGenerator::deactivate() {
    if(active) {
      active = false;
      SDL_PauseAudioDevice(device, PAUSE_AUDIO);
    }
  }
}