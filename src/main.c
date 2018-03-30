# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "board.h"
# include "piece.h"
# include "list.h"
# include "find_move.h"
# include "history.h"
# include "exec_move.h"
# include "IA.h"
# include <SDL/SDL.h>
# include <SDL/SDL_image.h>
# include "constants.h"

void fflush_stdin()
{
  int c = 0;
  while (c != '\n' && c != EOF)
    c = getchar();
}

int parse_input(int *curLine, int *curCol,
                int *destLine, int *destCol,
                int seq, int *i_seq, char *filename,
                int *shell_mode)
{
  puts("Shell mode");
  char input[20] = {0};

  if (fgets(input, 20, stdin) != NULL) // reads 18 char + \n, store to input
  {
    if (strchr(input, '\n') == NULL) // input too long, didn't find \n
      fflush_stdin(); // prevent overflow

    if (strncmp(input, "sdl", 3) == 0) // switch to mouse
    {
      *shell_mode = 0;
      puts("Use mouse now");
      return SWITCH_TO_MOUSE;
    }

    if (strncmp(input, "quit", 4) == 0)
      return QUIT;

    if (strncmp(input, "help", 4) == 0)
      return HELP;

    if (strncmp(input, "undo", 4) == 0)
      return UNDO;

    if (strncmp(input, "redo", 4) == 0)
      return REDO;

    if (strncmp(input, "save", 4) == 0)
    {
      char *tmp; // type "save filename"
      tmp = strtok(input, " "); // parse save
      tmp = strtok(NULL, " "); // parse filename
      if (tmp) // if user has typed a filename
        strncpy(filename, tmp, strlen(tmp) - 1);
      return SAVE;
    }

      if (input[0] >= 48 && input[0] <= 57) // digit
      {
        if (seq) // > 0
        {
          sscanf(input, "%d", i_seq);
          if (*i_seq > seq || *i_seq <= 0)
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

  int a;
  int cpu = PLAYER_BLACK;
  int human = PLAYER_WHITE;
  puts("Type a digit:\n"
       "0: 2 players\n"
       "1: human plays white, cpu plays black\n"
       "2: human plays black, cpu plays white\n");
  scanf("%d", &a);
  if (a == 0)
  {
    puts("Mode 2 joueurs");
    cpu = 0;
    human = 0;
  }
  else if (a == 1)
  {
    puts("human plays white, cpu plays black");
  }
  else
  {
    puts("human plays black, cpu plays white");
    cpu = PLAYER_WHITE;
    human = PLAYER_BLACK;
  }

// Print it
  printf("This is the start of the game\n");
  printBoard(board);

// Init
  undo_init(board);
  redo_init(board);

  int *curLine, *curCol, *destLine, *destCol, *i_seq;
  curLine  = malloc(sizeof (int));
  curCol   = malloc(sizeof (int));
  destLine = malloc(sizeof (int));
  destCol  = malloc(sizeof (int));
  i_seq    = calloc(1, sizeof (int));
  char *filename = calloc(1024, 1);
  int res, nb_seq;
  struct moves *moves_list = NULL;

//------------------------------SDL init-------------------------------------//
  SDL_Surface *screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT,
                       32, SDL_HWSURFACE | SDL_DOUBLEBUF);
  SDL_Surface *W    = IMG_Load("image_case/caseblanche.png");
  SDL_Surface *B    = IMG_Load("image_case/casemaron.png" );
  SDL_Surface *B_WP = IMG_Load("image_case/casemaronpieceblanche.png");
  SDL_Surface *B_BP = IMG_Load("image_case/casemaronpiecenoire.png");
  SDL_Surface *B_WPS = IMG_Load("image_case/casemaronpieceblancheselec.png");
  SDL_Surface *B_BPS = IMG_Load("image_case/casemaronpiecenoireselec.png");
  SDL_Surface *B_WK = IMG_Load("image_case/casemaronpieceblanchedames.png");
  SDL_Surface *B_BK = IMG_Load("image_case/casemaronpiecenoiredames.png");
  SDL_Surface *B_WKS = IMG_Load("image_case/casemaronpieceblanchedamesselec.png");
  SDL_Surface *B_BKS = IMG_Load("image_case/casemaronpiecenoiredamesselec.png");
  SDL_Surface *B_DES = IMG_Load("image_case/casemarondest.png");
  SDL_Surface *B_WKSE = IMG_Load("image_case/casemaronpieceblanchedamesselected.png");
  SDL_Surface *B_BKSE = IMG_Load("image_case/casemaronpiecenoiredamesselected.png");
  SDL_Surface *B_BPSE = IMG_Load("image_case/casemaronpiecenoireselected.png");
  SDL_Surface *B_WPSE = IMG_Load("image_case/casemaronpieceblancheselected.png");


  SDL_Init(SDL_INIT_VIDEO);
  SDL_Rect position;
  SDL_Event event;

  if (argc >= 2)
    SDL_WM_SetCaption(argv[1], NULL);
  else
    SDL_WM_SetCaption("Game", NULL);

//---------------------------SDL fin init------------------------------------//


// Main loop
  int can_play = 1;
  int shell_mode = 0;
  int selected_x = -1;
  int selected_y = -1;
  int dest_x = -1;
  int dest_y = -1;

  while (can_play)
  {
    boardInitColor(board);
// END OF GAME
    if ((board->player == PLAYER_WHITE && board->nb_white == 0)
     || (board->player == PLAYER_BLACK && board->nb_black == 0))
    {
      can_play = 0;
      puts("No more pieces!");
      continue;
    }

// HUMAN PLAYER
    if (cpu == 0 || board->player == human)
    {
// FIND MANDATORY JUMPS
      if (moves_list)
        free_moves(moves_list);
      moves_list = build_moves(board);
      nb_seq = list_len(moves_list);
      set_orig_cases(board, moves_list, nb_seq);
      if (nb_seq > 0)
      {
        printf("You have %d mandatory moves\n", nb_seq);
        list_print(moves_list);
        puts("Which sequence do you want to play?");
      }
//--------------------------SDL handle mouse---------------------------------//
  SDL_Rect pos;

      if (!shell_mode)
      {
          if (nb_seq > 0)
          {
          //--------------------------SDL print board------------------------//
             SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0,0,0));
             SDL_Surface *s;

             for (int i = 0 ; i < 10; i++)
             {
               for (int j = 0 ; j < 10; j++)
               {
                 position.y = i * BLOCK_SIZE;
                 position.x = j * BLOCK_SIZE;

                 struct cell c = board->cells[i][j];

                 if (c.background == LIGHT)
                   SDL_BlitSurface(W, NULL, screen, &position);

                 else
                 {
                   switch (c.data)
                   {
                     case 0:
                         if (c.background == DEST)
                             s = B_DES;
                         else
                             s = B;
                         SDL_BlitSurface(s, NULL, screen, &position);
                         break;
                     case BP:
                         if (c.background == SELECTED)
                           s = B_BPS;
                         else if (c.background == ORIG)
                           s = B_BPSE;
                         else
                           s = B_BP;
                         SDL_BlitSurface(s, NULL, screen, &position);
                         // SDL_BlitSurface(B_BP, NULL, screen, &position);
                         break;
                     case BK:
                         if (c.background == SELECTED)
                           s = B_BKS;
                         else if (c.background == ORIG)
                           s= B_BKSE;
                         else
                           s = B_BK;
                         SDL_BlitSurface(s, NULL, screen, &position);
                         //SDL_BlitSurface(B_BK , NULL, screen, &position);
                         break;
                     case WP:
                         if (c.background == SELECTED)
                           s = B_WPS;
                         else if (c.background == ORIG)
                           s = B_WPSE;
                         else
                           s = B_WP;
                         SDL_BlitSurface(s, NULL, screen, &position);
                         break;
                     case WK:
                         if (c.background == SELECTED)
                           s = B_WKS;
                         else if (c.background == ORIG)
                           s = B_WKSE;
                         else
                           s = B_WK;
                         SDL_BlitSurface(s, NULL, screen, &position);
                         //SDL_BlitSurface(B_WK, NULL, screen, &position);
                         break;
                     }
                  }
                }
              }

              SDL_Flip(screen);
          //--------------------------SDL end print--------------------------//
          }
  ev:  SDL_WaitEvent(&event);
       switch(event.type)
       {
         case SDL_QUIT:
           break;
         case SDL_MOUSEBUTTONDOWN:
           if (event.button.button == SDL_BUTTON_LEFT)
           {
             puts("Left Click! Click right to use terminal");
             pos.x = event.button.x / BLOCK_SIZE;
             pos.y = event.button.y / BLOCK_SIZE;
             if (nb_seq <= 0)
             {
               if (board->cells[pos.y][pos.x].data == board->player)
               {
                 board->cells[pos.y][pos.x].background = SELECTED;
                 selected_x = pos.y;
                 selected_y = pos.x;
               }

               if (board->cells[pos.y][pos.x].data == 0)
               {
                 dest_x = pos.y;
                 dest_y = pos.x;
               }
             }
             else
             {
               if (board->cells[pos.y][pos.x].background == ORIG)
               {
                 board->cells[pos.y][pos.x].background = SELECTED;
                 selected_x = pos.y;
                 selected_y = pos.x;
               }
             }
             printf("%d, %d\n",pos.y,pos.x);
           }
         else if (event.button.button == SDL_BUTTON_RIGHT) // Switch to shell
         {
           puts("Use shell now, you may press ENTER");
           shell_mode = 1;
           fflush_stdin();
           fputc('\n', stdin);
         }
         break;
       case SDL_KEYDOWN:
         switch (event.key.keysym.sym)
         {
           case SDLK_ESCAPE:
             res = QUIT;
             break;
           case SDLK_h:
             res = HELP;
             break;
           case SDLK_s:
             res = SAVE;
             break;
           case SDLK_u:
             res = UNDO;
             break;
           case SDLK_r:
             res = REDO;
             break;
           case SDLK_k:
             puts("Use shell now, you may press ENTER");
             shell_mode = 1;
             fflush_stdin();
             fputc('\n', stdin);
             break;
           default:
             break;
         }
         break;
       default:
         goto ev;
       }
     }
  } // END of human part

//--------------------------SDL print board----------------------------------//
PRINT:
   SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0,0,0));
   SDL_Surface *s;

   for (int i = 0 ; i < 10; i++)
   {
     for (int j = 0 ; j < 10; j++)
     {
       position.y = i * BLOCK_SIZE;
       position.x = j * BLOCK_SIZE;

       struct cell c = board->cells[i][j];

       if (c.background == LIGHT)
         SDL_BlitSurface(W, NULL, screen, &position);

       else
       {
         switch (c.data)
         {
           case 0:
               if (c.background == DEST)
                   s = B_DES;
               else
                   s = B;
               SDL_BlitSurface(s, NULL, screen, &position);
               break;
           case BP:
               if (c.background == SELECTED)
                 s = B_BPS;
               else if (c.background == ORIG)
                 s = B_BPSE;
               else
                 s = B_BP;
               SDL_BlitSurface(s, NULL, screen, &position);
               // SDL_BlitSurface(B_BP, NULL, screen, &position);
               break;
           case BK:
               if (c.background == SELECTED)
                 s = B_BKS;
               else if (c.background == ORIG)
                 s= B_BKSE;
               else
                 s = B_BK;
               SDL_BlitSurface(s, NULL, screen, &position);
               //SDL_BlitSurface(B_BK , NULL, screen, &position);
               break;
           case WP:
               if (c.background == SELECTED)
                 s = B_WPS;
 	             else if (c.background == ORIG)
                 s = B_WPSE;
               else
                 s = B_WP;
               SDL_BlitSurface(s, NULL, screen, &position);
               break;
           case WK:
               if (c.background == SELECTED)
                 s = B_WKS;
               else if (c.background == ORIG)
                 s = B_WKSE;
               else
                 s = B_WK;
               SDL_BlitSurface(s, NULL, screen, &position);
               //SDL_BlitSurface(B_WK, NULL, screen, &position);
               break;
           }
        }
      }
    }

    SDL_Flip(screen);
//--------------------------SDL end print------------------------------------//

// IA PLAYER
  if ((board->player) == cpu)
  {
    puts("IA is thinking...");
    sleep(3);
    // puts("IA is supposed to play there");
    struct move_seq *ia_move = get_IA_move();
    if (ia_move != NULL)
      exec_seq_IA(board, ia_move);
    else
    {
      undo_push(board, NULL);
      print_error("No move has been found by the IA");
    }
    board->player *= -1;
    printBoard(board);
    goto PRINT;
  }

// HUMAN PLAYER
  if (cpu == 0 || board->player == human)
  {
 // PARSE KEYBOARD INPUT
    if (shell_mode)
    {
      res = parse_input(curLine, curCol, destLine, destCol, nb_seq,
                        i_seq, filename, &shell_mode);
      if (res == -2)
        continue;
      while (res == -1) //error
      {
        print_error("Problem when reading your input");
        res = parse_input(curLine, curCol, destLine, destCol, nb_seq,
                          i_seq, filename, &shell_mode);
      }
    }

    else
    {
      if (selected_x != -1 && selected_y != -1 && dest_x != -1 && dest_y != -1)
        if (0 == move(board, selected_x, selected_y, dest_x, dest_y))
        {
          selected_x = -1;
          selected_y = -1;
          dest_x = -1;
          dest_y = -1;
          board->player *= -1;
          *i_seq = 0;
          free_moves(board->redo);
          redo_init(board);
          goto PRINT;
        }
    }

// ACTIONS
// MOVE
    if (res == 0)
    {
      free_moves(board->redo);
      redo_init(board);
      int res2 = -1;
      if (*i_seq > 0) // play the sequence moves_list[i]
        res2 = exec_seq_in_list(board, moves_list, *i_seq);
      else // no capture possible
        res2 = move(board, *curLine, *curCol, *destLine, *destCol);

      if (res2 == 0) // success
        board->player *= -1; // change player

      *i_seq = 0;
      printBoard(board);
    }

// QUIT
    if (res == QUIT)
    {
      can_play = 0;
      continue;
    }

// HELP
    if (res == HELP)
    {
      puts("Type 4 digits separated by space character:"
        " current line and column, destination line and column\n"
        "If you have a mandatory move, just type the corresponding number");
      continue;
    }

// SAVE
    if (res == SAVE)
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

// UNDO
    if (res == UNDO)
    {
      if (list_len(board->undo) > 0)
      {
        undo_move(board);
        if (cpu && (list_len(board->undo) > 0))
          undo_move(board);
        res = -1;
        goto PRINT;
      }
      else
        print_error("No previous move");
      printBoard(board);
    }

// REDO
    if (res == REDO)
    {
      if (list_len(board->redo) > 0)
      {
        redo_move(board);
        if (cpu && (list_len(board->redo) > 0))
          redo_move(board);
        res = -1;
        goto PRINT;
      }
      else
        print_error("No previous move");
      printBoard(board);
    }
} // END of human part
  } // END of main loop

  if (board->player == PLAYER_WHITE)
    printf("Black won!\n");
  else
    printf("White won!\n");

//-------free SDL------------------------------------------------------------//
  SDL_FreeSurface(screen);
  SDL_FreeSurface(W);
  SDL_FreeSurface(B);
  SDL_FreeSurface(B_WP);
  SDL_FreeSurface(B_BP);
  SDL_FreeSurface(B_WK);
  SDL_FreeSurface(B_BK);
  SDL_Quit();
//-------free SDL------------------------------------------------------------//

  free_moves(moves_list);
  free_moves(board->undo);
  free_moves(board->redo);
  free(board);
  free(curLine);
  free(curCol);
  free(destLine);
  free(destCol);
  free(filename);
  free(i_seq);

  return 0;
}
