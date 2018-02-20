# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "board.h"
# include "piece.h"
# include "list.h"
# include "find_move.h"
# include <SDL/SDL.h>
# include <SDL/SDL_image.h>
# include "constantes.h"

int exec_seq(struct board *b, struct move_seq *list)
{
  if (list == NULL)
    return -1;

  // remove the captured
  for (int i = 0; i < 20 && list->captures[i].x != -1; i++)
    b->cells[list->captures[i].x][list->captures[i].y].data = 0;

  list = list->next; // skip sentinel

  // save the orig cell and clear it
  int cur_piece = b->cells[list->orig.x][list->orig.y].data;
  b->cells[list->orig.x][list->orig.y].data = 0;

  for (; list->next; list = list->next); // go to last node
  b->cells[list->dest.x][list->dest.y].data = cur_piece;

  return 0;
}

int exec_seq_in_list(struct board *b, struct moves *list, int i)
{
  for (; list; list = list->next)
  {
    if (i == 0)
      return exec_seq(b, list->seq);
    i--;
  }
  return -1;
}

void fflush_stdin()
{
  int c = 0;
  while (c != '\n' && c != EOF)
    c = getchar();
}

int parse_input(int *curLine, int *curCol,
                int *destLine, int *destCol,
                int rafle, int *iRafle, char *filename)
{
  char input[20] = {0};

  if (fgets(input, 20, stdin) != NULL) // reads 18 char + \n, store to input
  {
    if (strchr(input, '\n') == NULL) // input too long, didn't find \n
      fflush_stdin(); // prevent overflow

    if (strncmp(input, "quit", 4) == 0)
      return 1;

    if (strncmp(input, "help", 4) == 0)
      return 2;

    if (strncmp(input, "save", 4) == 0)
    {
      char *tmp; // type "save filename"
      tmp = strtok(input, " "); // parse save
      tmp = strtok(NULL, " "); // parse filename
      if (tmp) // if user has typed a filename
        strncpy(filename, tmp, strlen(tmp) - 1);
      return 3;
    }

    if (input[0] >= 48 && input[0] <= 57) // digit
    {
      if (rafle) // > 0
      {
        sscanf(input, "%d", iRafle);
        if (*iRafle > rafle || *iRafle <= 0)
          return -1;
        else
          return 0;
      }

      sscanf(input, "%d %d %d %d", curLine, curCol, destLine, destCol);
      return 0;
    }

    return -1; // error
  }
  else
    return -1; // error
}

int main(int argc, char **argv)
{
  struct board *board = malloc(sizeof(struct board));

  // Init the board.
  // 1st argument is the name of the file that contains the board.
  // If no argument, the default config is used.
  if (argc >= 2)
  {
    if (-1 == open_board_from_file(board, argv[1]))
    {
      print_error("Can not read file");
      free(board);
      return -1;
    }
  }
  else
    boardInit(board);
  boardInitColor(board);

  //print it
  printf("This is the start of the game\n");
  printBoard(board);

  int *curLine, *curCol, *destLine, *destCol, *iRafle;
  curLine  = malloc(sizeof (int));
  curCol   = malloc(sizeof (int));
  destLine = malloc(sizeof (int));
  destCol  = malloc(sizeof (int));
  iRafle   = calloc(1, sizeof (int));
  char *filename = calloc(1024, 1);
  int res;

//------------------------------SDL init-------------------------------------//
  SDL_Surface *ecran = SDL_SetVideoMode(LARGEUR_FENETRE, HAUTEUR_FENETRE,
                       32, SDL_HWSURFACE | SDL_DOUBLEBUF);
  SDL_Surface *W    = IMG_Load("gifs/caisse.jpg");
  SDL_Surface *B    = IMG_Load("nothing" );
  SDL_Surface *B_WP = IMG_Load("gifs/mario_gauche.gif");
  SDL_Surface *B_BP = IMG_Load("gifs/mario_droite.gif");
  SDL_Surface *B_WK = IMG_Load("gifs/mario_haut.gif");
  SDL_Surface *B_BK = IMG_Load("gifs/mario_bas.gif");

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Rect position;
//---------------------------SDL fin init------------------------------------//


  //main loop
  for (;;)
  {
//--------------------------SDL print board----------------------------------//
   SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0,0,0));

   for (int i = 0 ; i < 10; i++)
   {
     for (int j = 0 ; j < 10; j++)
     {
       position.y = i * TAILLE_BLOC;
       position.x = j * TAILLE_BLOC;

       struct cell c = board->cells[i][j];

       if (c.background == LIGHT)
         SDL_BlitSurface(W, NULL, ecran, &position);
       else
       {
         switch (c.data)
         {
           case 0:
               SDL_BlitSurface(B, NULL, ecran, &position);
               break;
           case BP:
               SDL_BlitSurface(B_WP, NULL, ecran, &position);
               break;
           case BK:
               SDL_BlitSurface(B_WK , NULL, ecran, &position);
               break;
           case WP:
               SDL_BlitSurface(B_BP, NULL, ecran, &position);
               break;
           case WK:
               SDL_BlitSurface(B_BK, NULL, ecran, &position);
               break;
           }
        }
      }
    }

    SDL_Flip(ecran);
//--------------------------SDL fin print------------------------------------//


    if ((board->player == PLAYER_WHITE && board->nb_white == 0)
     || (board->player == PLAYER_BLACK && board->nb_black == 0))
      goto LOSE;

    struct moves *moves_list = build_moves(board);
    int nb_rafles = list_len(moves_list);
    if (nb_rafles > 0)
    {
      printf("You have %d mandatory moves\n", nb_rafles);
      list_print(moves_list);
      puts("Which sequence do you want to play?");
    }

    res = parse_input(curLine, curCol, destLine, destCol, nb_rafles, iRafle, filename);
    while (res == -1) //error
    {
      print_error("Problem when reading your input");
      res = parse_input(curLine, curCol, destLine, destCol, nb_rafles, iRafle, filename);
    }

    if (res == 0) //normal
    {
      int res2 = -1;
      if (*iRafle > 0) // play the sequence moves_list[i]
        res2 = exec_seq_in_list(board, moves_list, *iRafle);
      else // no capture possible
        res2 = deplacement(board, *curLine, *curCol, *destLine, *destCol);

      if (res2 == 0) // success
      {
        if (1 == pawn_to_king(board))
          printf("King!\n");
        board->player *= -1;
      }
      *iRafle = 0;
      printBoard(board);
    }

    if (res == 1) //quit
    {
LOSE:
      if (board->player == PLAYER_WHITE)
        printf("Black won!\n");
      else
        printf("White won!\n");
      break;
    }

    if (res == 2) //help
    {
      puts("Type 4 digits separated by space character:"
        " current line and column, destination line and column\n"
        "If you have a mandatory move, just type the corresponding number");
      continue;
    }

    if (res == 3) //save
    {
      if (strlen(filename) == 0)
      {
        // name by default, free crash if static
        filename[0] = 's';
        filename[1] = 'a';
        filename[2] = 'v';
        filename[3] = 'e';
      }
      if (0 != write_board_to_file(board, filename))
        print_error("Can not write board to file");
    }
  }

  free(board);
  free(curLine);
  free(curCol);
  free(destLine);
  free(destCol);
  free(filename);
  return 0;
}
