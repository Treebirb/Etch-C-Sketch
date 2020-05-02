#ifndef __program_h__
#define __program_h__

#include "SDL2/SDL.h"

typedef struct pointarr_s
{
  unsigned count;
  SDL_Point* data;
} pointarr_t;

typedef struct program_s
{
  int width, height;
  SDL_Window* window;
  SDL_Renderer* renderer;

  SDL_Point cursor, update;

  pointarr_t points;
  SDL_bool running;
  int speed;
} * program_t;

program_t open_program(int width, int height, const char* pname,
                       const char* path);

void close_program(program_t p);

void update(program_t p);
void input(program_t p);
void draw(program_t p);

#endif /* program.h */
