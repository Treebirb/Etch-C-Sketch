#include "SDL2/SDL.h"
#include "args.h"
#include "program.h"
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
  {
    SDL_Log("ERROR: %s", SDL_GetError());
    return -1;
  }

  atexit(SDL_Quit);

  program_t p = open_program(800, 600, "Etch-C-Sketch - Read controls.txt for Help", argv[0]);

  if (!p)
  {
    SDL_Log("ERROR: Program failed to initialize properly! Exiting now...");
    return -1;
  }

  while (p->running)
  {
    Uint64 start = SDL_GetPerformanceCounter();
    input(p);
    update(p);
    draw(p);
    Uint64 end = SDL_GetPerformanceCounter();
    float elapsed = (end - start) / (float)SDL_GetPerformanceCounter() * 1000.f;
    SDL_Delay(SDL_floorf((1000 / 60.f) - elapsed));
  }

  close_program(p);

  return 0;
}
