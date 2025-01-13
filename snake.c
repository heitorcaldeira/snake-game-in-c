#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define WIDTH 800
#define HEIGHT 600
#define CELL_SIZE 20
#define GRID_WIDTH WIDTH / CELL_SIZE
#define GRID_HEIGHT HEIGHT / CELL_SIZE

char dir = 'd';
bool quit = false;
bool render = true;
int count = 3;
int food[2] = { -1, -1 };
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

int* generate_food() {
  int c = (rand() % GRID_WIDTH);
  int r = (rand() % GRID_HEIGHT);
  food[0] = c;
  food[1] = r;
  return food;
}

void unshift_pair(int snake[][2], int *size, int capacity, int newX, int newY) {
  if (*size >= capacity) {
    printf("Error: Not enough capacity to unshift.\n");
    return;
  }

  for (int i = *size; i > 0; i--) {
    snake[i][0] = snake[i - 1][0];
    snake[i][1] = snake[i - 1][1];
  }

  snake[0][0] = newX;
  snake[0][1] = newY;

  (*size)++;
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

  for (int i = 0; i < count; i++) {
    SDL_Rect rect = { snake[i][0] * CELL_SIZE + 1, snake[i][1] * CELL_SIZE + 1, CELL_SIZE - 1, CELL_SIZE - 1 };
    sdlc(SDL_RenderFillRect(renderer, &rect));
  }

  if (food[0] == -1 && food[1] == -1) {
    generate_food();
  } else {
    sdlc(SDL_SetRenderDrawColor(renderer, 200, 18, 18, 255));
    SDL_Rect rect = { food[0] * CELL_SIZE + 1, food[1] * CELL_SIZE + 1, CELL_SIZE - 1, CELL_SIZE - 1 };
    sdlc(SDL_RenderFillRect(renderer, &rect));
  }

  int old[2] = { snake[count-1][0], snake[count-1][1] };
  if (dir == 'd') {
    snake[count-1][0] = snake[count-1][0] + 1;
  } else if (dir == 'a') {
    snake[count-1][0] = snake[count-1][0] - 1;
  } else if (dir == 'w') {
    snake[count-1][1] = snake[count-1][1] - 1;
  } else if (dir == 's') {
    snake[count-1][1] = snake[count-1][1] + 1;
  }

  for (int i = count - 1; i >= 0; i--) {
    if (i > 0) {
      int o[2] = { snake[i-1][0], snake[i-1][1] };
      snake[i-1][0] = old[0];
      snake[i-1][1] = old[1];
      old[0] = o[0];
      old[1] = o[1];
    }
  }

  if (snake[count-1][0] == food[0] && snake[count-1][1] == food[1]) {
    unshift_pair(snake, &count, 1024, old[0], old[1]);
    generate_food();
  }

  if (snake[count-1][0] > GRID_WIDTH - 1) snake[count-1][0] = 0;
  else if (snake[count-1][0] < 0) snake[count-1][0] = GRID_WIDTH - 1;
  else if (snake[count-1][1] < 0) snake[count-1][1] = GRID_HEIGHT - 1;
  else if (snake[count-1][1] > GRID_HEIGHT - 1) snake[count-1][1] = 0;

  for (int s = 0; s < count - 1; s++) {
    if (snake[count-1][0] == snake[s][0] && snake[count-1][1] == snake[s][1]) {
      render = false;
    }
  }
}

int main(void) {
  srand((unsigned)time(NULL));
  SDL_Window* window = sdlp(SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0));
  SDL_Renderer* renderer = sdlp(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));

  sdlc(SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255));
  sdlc(SDL_RenderClear(renderer));

  Uint32 lastTime = SDL_GetTicks();

  while(!quit) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          quit = true;
          break;
        // TODO: there is a bug when hit two keys too fast
        // If the snake is moving to the right and we press `s-a`, the key `s` is computed on the dir variable but not computed to the snake movement
        // because the game_loop is called only after 60ms
        case SDL_KEYDOWN:
          {
            SDL_Keycode code = event.key.keysym.sym;
            if (code == 'd') {
              if (dir != 'a') dir = 'd';
            } 
            if (code == 'a') {
              if (dir != 'd') dir = 'a';
            }
            if (code == 'w') {
              if (dir != 's') dir = 'w';
            }
            if (code == 's') {
              if (dir != 'w') dir = 's';
            }
          }
          break;
      }
    }

    if (render && SDL_GetTicks() > lastTime + 60) {
      game_loop(renderer);
      lastTime = SDL_GetTicks();
    }

    SDL_RenderPresent(renderer);
  }

  SDL_Quit();
}
