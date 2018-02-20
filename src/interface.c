#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <SDL/SDL.h>
#include "interface.h"

int main()
{
  SDL_Surface *screen = NULL;
  SDL_Surface *square = NULL;
  SDL_Rect pos;
 
  if (SDL_Init(SDL_INIT_VIDEO) == -1)
  {
    fprintf(stderr, "Error : %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  
  screen = SDL_SetVideoMode(480, 480, 32, SDL_HWSURFACE);
  square = SDL_CreateRGBSurface(SDL_HWSURFACE,48,48,32,0,0,0,0);
  if(screen == NULL)
  {
    fprintf(stderr, "Impossible to load the mode video : %s\n", SDL_GetError());
    exit(EXIT_FAILURE);	
  }

  SDL_WM_SetCaption("Game", NULL);

  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));

  pos.x = 0;
  pos.y = 0;
 
  SDL_FillRect(square,NULL, SDL_MapRGB(screen->format, 0, 0, 0));
 
  for(int i = 0; i < 10; i++)
  {
    for(int j = 0; j < 10; j++)
    {
      if(i % 2 == 0 && j % 2 == 1)
      {
        SDL_BlitSurface(square, NULL, screen, &pos);
        pos.y = pos.y + 96;
      }
      if(i % 2 == 1 && j % 2 == 0)
      {
        if(pos.y == 0)
          pos.y = 48;
        SDL_BlitSurface(square, NULL, screen, &pos);
        pos.y = pos.y + 96;
      }
    }
    pos.x = pos.x + 48;
    pos.y = 0;  
  }




  SDL_Flip(screen);
  sleep(3);
  //have_a_break();
  SDL_FreeSurface(square);
  SDL_Quit();
  return EXIT_SUCCESS;
}
