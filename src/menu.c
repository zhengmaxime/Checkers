#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <unistd.h>


void pause_1()
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

int menu()
{
    SDL_Surface *ecran = NULL, *imageDeFond = NULL; // Surface
    // SDL_Surface *rectangle = NULL;
    SDL_Surface *PVP = NULL;
    SDL_Surface *White = NULL;
    SDL_Surface *Black = NULL;
    SDL_Surface *Quit = NULL;    

    
    SDL_Rect positionFond; // variable pour recuperer position dans la fenetre
    positionFond.x = 0;
    positionFond.y = 0;   

    SDL_Rect pos1;
    pos1.x = 85;
    pos1.y = 200;

    SDL_Rect pos2;
    pos2.x = 85;
    pos2.y = 280;

    SDL_Rect pos3;
    pos3.x = 85;
    pos3.y = 360;

    SDL_Rect pos4;
    pos4.x = 85;
    pos4.y = 440;


    if(SDL_Init(SDL_INIT_VIDEO) == -1) // allocation
    {
	fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    ecran = SDL_SetVideoMode(402, 554, 32, SDL_HWSURFACE); // creation de la fenetre
    
    SDL_WM_SetCaption("Chargement d'images en SDL", NULL); // chargement img bmp dans une surface

    if (ecran == NULL) // test si le format est trop grand
    {
	fprintf(stderr, "Impossible de charger le mode vidéo : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_WM_SetCaption("Checkers", NULL);
    // titre de la fenetre

    // change la couleur de la fenetre dans la memoire
    // 3eme parametre recupere la valeure de la couleur

    imageDeFond = SDL_LoadBMP("image_menu/Checkers.bmp");
    PVP = SDL_LoadBMP("image_menu/Player_vs_Player.bmp");
    White = SDL_LoadBMP("image_menu/White.bmp");
    Black = SDL_LoadBMP("image_menu/Black.bmp");
    Quit = SDL_LoadBMP("image_menu/Quit.bmp");




    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 17, 206, 112));

    SDL_BlitSurface(imageDeFond, NULL, ecran, &positionFond);
    SDL_BlitSurface(PVP, NULL, ecran, &pos1);
    SDL_BlitSurface(White, NULL, ecran, &pos2);
    SDL_BlitSurface(Black, NULL, ecran, &pos3);
    SDL_BlitSurface(Quit, NULL, ecran, &pos4);
    /* On blitte (colle) par dessus la fenetre */
     
    int mode = -1;
    
    while (mode == -1){   
    
    SDL_Flip(ecran);
    SDL_Event evt;
    SDL_WaitEvent(&evt);
        

    switch (evt.type)
    {
       case SDL_QUIT:
        mode = 42;
        break;
      
      case SDL_MOUSEBUTTONDOWN:
        
        if (evt.button.button == SDL_BUTTON_LEFT)
        {
          if (evt.button.x > 85 && evt.button.x < 329 && evt.button.y > 200 && evt.button.y < 237)
          {
//            printf("PVP \n");
            mode = 0;
          }
          if (evt.button.x > 85 && evt.button.x < 329 && evt.button.y > 280 && evt.button.y < 317)
          {
  //          printf("W \n");
            mode = 1;
          }
          if (evt.button.x > 85 && evt.button.x < 329 && evt.button.y > 360 && evt.button.y < 397)
          {
    //        printf("B \n");
            mode = 2;
          }
          if (evt.button.x > 85 && evt.button.x < 329 && evt.button.y > 440 && evt.button.y < 477)
          {
      //      printf("Q\n");
            mode = 42;
          }

        }
       break;
      default:
        break;
    
    }


   // SDL_Flip(ecran);
    //pause_1();
}     
   

    SDL_FreeSurface(imageDeFond);
    SDL_FreeSurface(PVP);
    SDL_FreeSurface(White);    
    SDL_FreeSurface(Black);
    SDL_FreeSurface(Quit);

    




    //SDL_FreeSurface(rectangle); // liberation de la memoire de la surface en couleur
    SDL_Quit(); // liberation de la memoire pour la fenetre

    return mode;
}
