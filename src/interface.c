#include <stdlib.h>
#include <stdio.h>

#include <SDL/SDL.h>

int main()
{
  SDL_Init(SDL_INIT_VIDEO);
  SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);
  SDL_Quit();
  return EXIT_SUCCESS;
}
