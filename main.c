#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_events.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

void white_noise(Sint16 *stream, size_t stream_len) {
  for (size_t i = 0; i < stream_len; i++) {
    Sint16 value = rand() % (1 << 15);
    Sint16 sign = (rand() % 2) * 2 - 1;
    stream[i] = value * sign;
  }
}

void my_beep_callback(void *userdata, Uint8 *stream, int len) {
  (void)userdata;
  assert(len % 2 == 0);
  white_noise((Sint16 *)stream, len / 2);
}

int main(void) {
  if (SDL_Init(SDL_INIT_AUDIO) < 0) {
    fprintf(stderr, "ERROR: could not initialize SDL: %s\n", SDL_GetError());
  }

  SDL_AudioSpec desired = {
      .freq = 40000,
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

  printf("Ok\n");

  SDL_Quit();

  return 0;
}
