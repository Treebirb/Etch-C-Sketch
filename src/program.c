#include "program.h"
#include "SDL2/SDL.h"
#include "args.h"
#include <stdlib.h>
#include <string.h>

void clamp(program_t p)
{
  if (p->cursor.x + 1 > p->width)
    p->cursor.x = p->width;
  if (p->cursor.x - 1 < 0)
    p->cursor.x = 0;
  if (p->cursor.y + 1 > p->height)
    p->cursor.y = p->height;
  if (p->cursor.y - 1 < 0)
    p->cursor.y = 0;
}

void insert(program_t p, SDL_Point pos)
{
  if (p->points.count == 0)
  {
    p->points.count = 1;
    if (p->points.data == NULL)
      p->points.data = malloc(sizeof(SDL_Point));
    p->points.data[0] = pos;
  }
  else
  {
    p->points.count++;
    p->points.data =
        realloc(p->points.data, (sizeof(SDL_Point) * p->points.count));
    p->points.data[p->points.count - 1] = pos;
  }
}

void clear(program_t p)
{
  if (p->points.data)
  {
    p->points.count = 0;
    free(p->points.data);
    p->points.data = NULL;
  }
}

program_t open_program(int width, int height, const char* pname,
                       const char* path)
{
  program_t p = calloc(1, sizeof *p);
  if (!p)
    return NULL;

  p->width = width;
  p->height = height;
  SDL_CreateWindowAndRenderer(p->width, p->height, SDL_WINDOW_SHOWN, &p->window,
                              &p->renderer);

  if (!p->window || !p->renderer)
  {
    SDL_Log("ERROR: %s", SDL_GetError());
    return NULL;
  }

  SDL_SetWindowTitle(p->window, pname);

  char* icon = findicon(path, "etch.exe");
  if (!icon)
    return NULL;
  SDL_Surface* ico_surf = SDL_LoadBMP(icon);
  SDL_SetWindowIcon(p->window, ico_surf);
  if (ico_surf)
    SDL_FreeSurface(ico_surf);
  if (icon)
    free(icon);

  p->cursor = (SDL_Point){.x = p->width / 2, .y = p->height / 2};
  p->update = (SDL_Point){.x = 0, .y = 0};

  p->points.count = 0;
  p->points.data = NULL;

  p->running = SDL_TRUE;

  p->speed = 1;

  return p;
}

void close_program(program_t p)
{
  if (p)
  {
    if (p->renderer)
    {
      SDL_DestroyRenderer(p->renderer);
    }

    if (p->window)
    {
      SDL_DestroyWindow(p->window);
    }

    if (p->points.data)
    {
      free(p->points.data);
    }

    free(p);
  }
}

void update(program_t p)
{
  p->cursor.x += p->update.x;
  p->cursor.y += p->update.y;
  clamp(p);
  insert(p, p->cursor);
}

void input(program_t p)
{
  SDL_Event e;
  while (SDL_PollEvent(&e))
  {
    switch (e.type)
    {
      case SDL_QUIT: p->running = SDL_FALSE; break;
      case SDL_KEYDOWN:
        switch (e.key.keysym.sym)
        {
          case SDLK_ESCAPE: p->running = SDL_FALSE; break;
          case SDLK_z: p->update.x = -p->speed; break;
          case SDLK_x: p->update.x = p->speed; break;
          case SDLK_COMMA: p->update.y = p->speed; break;
          case SDLK_PERIOD: p->update.y = -p->speed; break;
          case SDLK_SPACE: clear(p); break;
          
          case SDLK_EQUALS:
          case SDLK_PLUS:
          case SDLK_KP_PLUS:
            p->speed++;
            if (p->speed + 1 > 4)
              p->speed = 4;
            break;
          
          case SDLK_MINUS:
          case SDLK_UNDERSCORE:
          case SDLK_KP_MINUS:
            p->speed--;
            if (p->speed - 1 < 1)
              p->speed = 1;
            break;
          
          case SDLK_LSHIFT:
          case SDLK_RSHIFT:
            clear(p);
            p->cursor.x = p->width / 2;
            p->cursor.y = p->height / 2;
            break;
        }
        break;
      case SDL_KEYUP:
        switch (e.key.keysym.sym)
        {
          case SDLK_z:
            if (p->update.x < 0)
              p->update.x = 0;
            break;
          case SDLK_x:
            if (p->update.x > 0)
              p->update.x = 0;
            break;
          case SDLK_COMMA:
            if (p->update.y > 0)
              p->update.y = 0;
            break;
          case SDLK_PERIOD:
            if (p->update.y < 0)
              p->update.y = 0;
            break;
        }
        break;
    }
  }
}

void draw(program_t p)
{
  SDL_SetRenderDrawColor(p->renderer, 200, 200, 200, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(p->renderer);

  SDL_SetRenderDrawColor(p->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

  SDL_RenderDrawLine(p->renderer, p->cursor.x, p->cursor.y, p->cursor.x + 4,
                     p->cursor.y + 4);
  SDL_RenderDrawLine(p->renderer, p->cursor.x, p->cursor.y, p->cursor.x - 4,
                     p->cursor.y - 4);
  SDL_RenderDrawLine(p->renderer, p->cursor.x, p->cursor.y, p->cursor.x + 4,
                     p->cursor.y - 4);
  SDL_RenderDrawLine(p->renderer, p->cursor.x, p->cursor.y, p->cursor.x - 4,
                     p->cursor.y + 4);

  if (p->points.count > 0)
  {
    SDL_RenderDrawLines(p->renderer, p->points.data, p->points.count);
  }

  SDL_RenderPresent(p->renderer);
}
