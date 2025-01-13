#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define WIDTH 800
#define HEIGHT 600

bool quit = false;

int sdlc(int c) {
  if (c != 0) {
    printf("Error executing SDL: %s", SDL_GetError());
    exit(1);
  }

  return c;
}

void* sdlp(void* p) {
  if (p == NULL) {
    printf("Error executing SDL: %s", SDL_GetError());
    exit(1);
  }

  return p;
}

int main(void) {
  SDL_Window* window = sdlp(SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0));
  SDL_Renderer* renderer = sdlp(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));

  sdlc(SDL_SetRenderDrawColor(renderer, 18, 18, 18, 255));
  sdlc(SDL_RenderClear(renderer));

  SDL_RenderPresent(renderer);

  while(!quit) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          quit = true;
          break;
      }
    }
  }

  SDL_Quit();
}
