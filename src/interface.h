#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>

#ifndef DEF_CONSTANTES

#define DEF_CONSTANTES


    #define TAILLE_BLOC         48 // Taille d'un bloc (carré) en pixels

    #define NB_BLOCS_LARGEUR    10

    #define NB_BLOCS_HAUTEUR    10

    #define LARGEUR_FENETRE     TAILLE_BLOC * NB_BLOCS_LARGEUR

    #define HAUTEUR_FENETRE     TAILLE_BLOC * NB_BLOCS_HAUTEUR


    enum {HAUT, BAS, GAUCHE, DROITE};

    enum {VIDE, MUR, CAISSE, OBJECTIF, MARIO, CAISSE_OK};


#endif



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
