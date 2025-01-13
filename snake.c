#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define WIDTH 800
#define HEIGHT 600
#define CELL_SIZE 20
#define GRID_WIDTH WIDTH / CELL_SIZE
#define GRID_HEIGHT HEIGHT / CELL_SIZE

char dir = 'r';
bool quit = false;
int snake[WIDTH * HEIGHT][2] = {
  {10, 10},
  {11, 10},
  {12, 10}
};

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

void game_loop(SDL_Renderer* renderer) {
  sdlc(SDL_SetRenderDrawColor(renderer, 18, 18, 18, 255));

  for (int row = 0; row < GRID_WIDTH; row++) {
    for (int col = 0; col < GRID_HEIGHT; col++) {
      SDL_Rect rect = { row * CELL_SIZE + 1, col * CELL_SIZE + 1, CELL_SIZE - 1, CELL_SIZE - 1 };
      sdlc(SDL_RenderFillRect(renderer, &rect));
    }
  }

  sdlc(SDL_SetRenderDrawColor(renderer, 255, 255, 18, 255));
  int snake_size = sizeof(snake) / sizeof(int) / 2;
  for (int i = 0; i < snake_size; i++) {
    if (snake[i][0] == 0 && snake[i][1] == 0) {
      continue;
    }

    SDL_Rect rect = { snake[i][0] * CELL_SIZE + 1, snake[i][1] * CELL_SIZE + 1, CELL_SIZE - 1, CELL_SIZE - 1 };
    sdlc(SDL_RenderFillRect(renderer, &rect));
  }

  if (dir == 'r') {
    for (int i = 0; i < snake_size; i++) {
      if (snake[i][0] == 0 && snake[i][1] == 0) {
        continue;
      }

      snake[i][0] = snake[i][0] + 1;
    }
  }
}

int main(void) {
  SDL_Window* window = sdlp(SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0));
  SDL_Renderer* renderer = sdlp(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));

  sdlc(SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255));
  sdlc(SDL_RenderClear(renderer));

  Uint32 lastTime = SDL_GetTicks();

  while(!quit) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          quit = true;
          break;
      }
    }

    if (SDL_GetTicks() > lastTime + 60) {
      game_loop(renderer);
      lastTime = SDL_GetTicks();
    }

    SDL_RenderPresent(renderer);
  }

  SDL_Quit();
}
