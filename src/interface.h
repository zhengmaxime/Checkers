#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>

void have_a_break()
{
    int continuer = 1;
    SDL_Event event;

    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                continuer = 0;
        }
    }
}
