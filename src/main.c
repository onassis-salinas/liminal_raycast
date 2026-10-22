#include "game.h"
#include <SDL3/SDL_events.h>

int windowWidth, windowHeight;
float dt;

int main(void) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("SDL_Init failed: %s", SDL_GetError());
    return 1;
  }

  SDL_Window *window =
      SDL_CreateWindow("Liminal Raycast", 800, 600, SDL_WINDOW_RESIZABLE);

  if (!window) {
    SDL_Log("Window creation failed: %s", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

  if (!renderer) {
    SDL_Log("Renderer creation failed: %s", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  bool running = true;
  SDL_Event event;
  SDL_GetCurrentRenderOutputSize(renderer, &windowWidth, &windowHeight);
  Uint64 last = SDL_GetTicksNS();
  initGame();

  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        running = false;
      } else if (event.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED) {
        windowWidth = event.window.data1;
        windowHeight = event.window.data2;
      }
    }

    Uint64 now = SDL_GetTicksNS();
    dt = (float)(now - last) / 1000000000.0f;
    last = now;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    renderGame(renderer);

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}