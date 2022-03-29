#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_events.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#define PI 3.14159265

void white_noise(Sint16 *stream, size_t stream_len) {
  for (size_t i = 0; i < stream_len; i++) {
    Sint16 value = rand() % (1 << 10);
    Sint16 sign = (rand() % 2) * 2 - 1;
    stream[i] = value * sign;
  }
}

void sine_wave(Sint16 *stream, size_t stream_len) {
  for (size_t i = 0; i < stream_len; i++) {
    double period = PI / 180;
    Sint16 value = (Sint16)100 * sin(5 * period * (double)i);
    stream[i] = value;
  }
}

void my_beep_callback(void *userdata, Uint8 *stream, int len) {
  (void)userdata;
  assert(len % 2 == 0);
  sine_wave((Sint16 *)stream, len / 2);
}

int main(int argc, char *argv[]) {
  assert(argc > 1);
  int cl_freq_param = atoi(argv[1]);

  if (SDL_Init(SDL_INIT_AUDIO) < 0) {
    fprintf(stderr, "ERROR: could not initialize SDL: %s\n", SDL_GetError());
  }

  SDL_AudioSpec desired = {
      .freq = cl_freq_param,
      .format = AUDIO_S16LSB,
      .channels = 1,
      .callback = my_beep_callback,
  };

  if (SDL_OpenAudio(&desired, NULL) < 0) {
    fprintf(stderr, "ERROR: could not open audio device: %s\n", SDL_GetError());
    exit(1);
  }

  SDL_PauseAudio(0);

  bool quit = false;
  while (!quit) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT: {
        quit = true;
        break;
      }
      }
    }
  }

  printf("\nLeaving, bye!\n");

  SDL_Quit();

  return 0;
}
