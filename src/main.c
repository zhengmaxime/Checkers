#define IA_WORKS 1

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "board.h"
# include "piece.h"
# include "list.h"
# include "find_move.h"
# include "history.h"
# include "exec_move.h"
# include "simple_move.h"
# include "shell.h"
# include "IA.h"
# include "file.h"
# include <SDL/SDL.h>
# include <SDL/SDL_image.h>
# include "constants.h"
# include "time.h"
# include "menu.h"
# include "menu_ia.h"

# define IS_VALID(x, y) x >= 0 && y >= 0

struct move_seq *get_random_move(struct board *b)
{
  struct moves *moves_list = NULL;
  unsigned int mandatory_jumps;
  int move_choice, moves_list_len;

  moves_list = build_moves(b);
  mandatory_jumps = list_len(moves_list);

  if (mandatory_jumps == 0)
  {
    free_moves(moves_list);
    moves_list = build_moves_not_mandatory(b);
	}
  moves_list_len = list_len(moves_list);

	if (moves_list_len == 0)
    return NULL;

  srand(time(NULL));
  move_choice = rand() % moves_list_len;

	moves_list = moves_list->next; //sentinel jumped

  for(int i = 0; i < move_choice; i++)
	{
		moves_list = moves_list->next;
	}

  if (moves_list == NULL)
    return NULL;

  return moves_list->seq;
}

int main(int argc, char **argv)
{
  int menu_ret;
start:
  menu_ret = menu();
  if (menu_ret == 42)
    return 0;

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
  reset_last_move_trace(board);

// Mode choice
  int cpu = PLAYER_BLACK;
  int human = PLAYER_WHITE;
  int level = 0;
/*
  puts("Type a digit:\n"
       "0: 2 players\n"
       "1: human plays white, cpu plays black\n"
       "2: human plays black, cpu plays white\n");
  scanf("%d", &a);
*/
 if (menu_ret == 0)
  {
    puts("2 players mode");
    cpu = 0;
    human = 0;
  }
  else if (menu_ret == 1)
  {
    level = menu_ia();
    puts("human plays white, cpu plays black");
  }
  else if (menu_ret == 2)
  {
    level = menu_ia();
    puts("human plays black, cpu plays white");
    cpu = PLAYER_WHITE;
    human = PLAYER_BLACK;
  }
  else
    return 0;

  if (level == 42)
  {
    free(board);
    goto start;
  }

// for minimax
  int deep;
  if (level == 1)
    deep = 3;
  else if (level == 2)
    deep = 7;
  else
    deep = 3;

// Init
  undo_init(board);
  redo_init(board);

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
  SDL_Surface *B_CRPT = IMG_Load("image_case/casemaroncrosspt.png");
  SDL_Surface *B_WKSE = IMG_Load("image_case/casemaronpieceblanchedamesselected.png");
  SDL_Surface *B_BKSE = IMG_Load("image_case/casemaronpiecenoiredamesselected.png");
  SDL_Surface *B_BPSE = IMG_Load("image_case/casemaronpiecenoireselected.png");
  SDL_Surface *B_WPSE = IMG_Load("image_case/casemaronpieceblancheselected.png");

  SDL_Surface *Quit = SDL_LoadBMP("image_menu/Quit.bmp");
  SDL_Surface *Undo = SDL_LoadBMP("image_menu/Undo.bmp");
  SDL_Surface *Redo = SDL_LoadBMP("image_menu/Redo.bmp");
  SDL_Surface *Save = SDL_LoadBMP("image_menu/Save.bmp");
  SDL_Surface *Abandon = IMG_Load("image_menu/flag_white.bmp");

  SDL_Surface *Black_won = IMG_Load("image_menu/black_won.bmp");
  SDL_Surface *White_won = IMG_Load("image_menu/white_won.bmp");

  SDL_Surface *T = IMG_Load("image_case/last_move_trace.bmp");

  SDL_Init(SDL_INIT_VIDEO);
  SDL_EnableKeyRepeat(100, 100);
  SDL_Rect position;
  SDL_Event event;

  if (argc >= 2)
    SDL_WM_SetCaption(argv[1], NULL);
  else
    SDL_WM_SetCaption("Game", NULL);

//---------------------------SDL end init------------------------------------//


// Main loop
  int can_play = 1;
  int selected_x = -1;
  int selected_y = -1;
  int orig_x = -1;
  int orig_y = -1;
  int dest_x = -1;
  int dest_y = -1;
  int search_jumps = 1;
  int nb_orig = 0;
  int nb_captures = 0;

  int res, mandatory_jumps, possible_move;
  struct moves *moves_list = NULL;

  while (can_play)
  {
    res = -1;
    selected_x = -1;
    selected_y = -1;

    count_pieces(board);

    //test if someone has won
    if ((board->player == PLAYER_WHITE && board->nb_white == 0)
     || (board->player == PLAYER_BLACK && board->nb_black == 0))
    {
      can_play = 0;
      puts("No more pieces!");

      if (board->player == PLAYER_WHITE)
        printf("Black won!\n");
      else
        printf("White won!\n");
    }


    if (search_jumps && (cpu == 0 || board->player == human) && can_play)
    {
      if (moves_list)
        free_moves(moves_list);

      moves_list = build_moves(board);

      mandatory_jumps = list_len(moves_list);

      if (mandatory_jumps > 0)
      {
        nb_orig = set_orig_cases(board, moves_list, &orig_x, &orig_y);

        printf("You have %d mandatory moves\n", mandatory_jumps);
        list_print(moves_list);
      }

      else
      {
        free_moves(moves_list);
        moves_list = build_moves_not_mandatory(board);
        possible_move = list_len(moves_list);
        if (!possible_move)
        {
          can_play = 0;
          puts("No move is possible!");

          if (board->player == PLAYER_WHITE)
            printf("Black won!\n");
          else
            printf("White won!\n");
        }
        else
          printf("You have %d possible moves\n", possible_move);
      }

      search_jumps = 0;
      nb_captures = 0;
    }

//--------------------------SDL print board----------------------------------//
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
               else if (c.background == CROSSPOINT)
                   s = B_CRPT;
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
               break;
           case BK:
               if (c.background == SELECTED)
                 s = B_BKS;
               else if (c.background == ORIG)
                 s = B_BKSE;
               else
                 s = B_BK;
               SDL_BlitSurface(s, NULL, screen, &position);
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
               break;
           }
        }

        if (board->cells[i][j].last_move == 1)
          // mark last move
        {
          // use transparency
          // 255,0,0 = red
          SDL_SetColorKey(T, SDL_SRCCOLORKEY, SDL_MapRGB(T->format, 255, 0, 0));

          SDL_BlitSurface(T, NULL, screen, &position);
        }
     }
    }

    SDL_Rect pos5;
    pos5.x = 10;
    pos5.y = 760;

    SDL_Rect pos6;
    pos6.x = 253;
    pos6.y = 760;

    SDL_Rect pos7;
    pos7.x = 496;
    pos7.y = 760;

    SDL_Rect pos8;
    pos8.x = 253;
    pos8.y = 803;

    SDL_Rect pos9;
    pos9.x = 596;
    pos9.y = 803;

    SDL_BlitSurface(Undo, NULL, screen, &pos5);
    SDL_BlitSurface(Redo, NULL, screen, &pos6);
    SDL_BlitSurface(Save, NULL, screen, &pos7);
    SDL_BlitSurface(Quit, NULL, screen, &pos8);
    SDL_BlitSurface(Abandon, NULL, screen, &pos9);
    SDL_Flip(screen);
//--------------------------SDL end print------------------------------------//

//---------------------------- IA PLAYER-------------------------------------//
  if ((board->player) == cpu && can_play)
  {
    puts("IA is thinking...");
    //sleep(1);

    struct move_seq *ia_move;
    if (IA_WORKS && level != 0)
      ia_move = get_IA_move(board, human, deep);
    else
      ia_move = get_random_move(board);

    if (ia_move != NULL)
      exec_seq_IA(board, ia_move);

    else
    {
      undo_push(board, NULL);
      print_error("No move has been found by the IA");
      can_play = 0;
      puts("No move is possible!");

      if (board->player == PLAYER_WHITE)
        printf("Black won!\n");
      else
        printf("White won!\n");
    }

    board->player *= -1;
    continue;
  }

//--------------------------SDL handle input---------------------------------//
  SDL_WaitEvent(&event);

  switch (event.type)
  {
   case SDL_QUIT:
      res = QUIT;
      break;

    case SDL_MOUSEBUTTONDOWN:
       if (event.button.button == SDL_BUTTON_LEFT)
       {
          if (event.button.x > 10 && event.button.x < 243
           && event.button.y > 760 && event.button.y < 793)
          {
            res = UNDO;
          }
          else if (event.button.x > 253 && event.button.x < 486
                && event.button.y > 760 && event.button.y < 793)
          {
            res = REDO;
          }
          else if (event.button.x > 496 && event.button.x < 729
                && event.button.y > 760 && event.button.y < 793)
          {
            res = SAVE;
          }
          else if (event.button.x > 253 && event.button.x < 486
                && event.button.y > 803 && event.button.y < 836)
          {
            res = QUIT;
          }
          else if (event.button.x > 593 && event.button.x < 626
                && event.button.y > 803 && event.button.y < 836)
          {
            res = ABANDON;
          }
          else
          {
             // No mistake below: x is the ordinate (we write cells[x][y]).
             selected_x = event.button.y / BLOCK_SIZE;
             selected_y = event.button.x / BLOCK_SIZE;
             res = MOUSE;
             reset_last_move_trace(board);
          }
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
           res = SHELL;
           break;
         case SDLK_a:
           res = ABANDON;
           break;
         default:
           break;
       }
       break;
     default:
       break;
   }

    if (res == -1)
      continue; // nothing

// SHELL MODE
    if (res == SHELL) // press K
    {
      printBoard(board);

      if (mandatory_jumps > 0)
      {
        printf("You have %d mandatory moves\n", mandatory_jumps);
        list_print(moves_list);
        puts("Which sequence do you want to play?");
      }

      // parse input
      int i_seq, curLine, curCol, destLine, destCol;
      res = parse_input(&curLine, &curCol, &destLine, &destCol,
                        mandatory_jumps, &i_seq);
      if (res == -2)
        continue;
      while (res == -1) //error
      {
        print_error("Problem when reading your input");
        res = parse_input(&curLine, &curCol, &destLine, &destCol,
                        mandatory_jumps, &i_seq);
      }

      // play
      if (res == 0)
      {
        free_moves(board->redo);
        redo_init(board);
        int res2 = -1;
        if (mandatory_jumps > 0) // play the sequence moves_list[i]
          res2 = exec_seq_in_list(board, moves_list, i_seq);
        else // no capture possible
          res2 = move(board, curLine, curCol, destLine, destCol);

        if (res2 == 0) // success
          board->player *= -1; // change player

        i_seq = 0;
        search_jumps = 1;
        boardInitColor(board);
      }
    }

// QUIT
    if (res == QUIT)
      goto quit;

// HELP
    if (res == HELP)
    {
      puts("Left click: play\n"
           "u: undo\n"
           "r: redo\n"
           "s: save\n"
           "ESC: quit\n"
           "a: abandon\n"
           "h: help\n"
           "k: use shell (once):\n    "
           "Type 4 digits separated by space character:\n    "
           "current line and column, destination line and column\n    "
           "If you have a mandatory move, just type the corresponding number\n"
           "    You can also type undo, redo, save, quit or help");
    }
// ABANDON
    if (res == ABANDON)
    {
        can_play = 0;
        puts("Abandon!");

        if (board->player == PLAYER_WHITE)
          printf("Black won!\n");
        else
          printf("White won!\n");
    }

// SAVE
    if (res == SAVE)
    {
        char *filename = calloc(1024, 1);

        printf("Name of the file to write: ");
        scanf("%s", filename);

        if (0 != write_board_to_file(board, filename))
          print_error("Can not write board to file");

        free(filename);
    }

// UNDO and REDO
    if (res == UNDO || res == REDO)
    {
      if (res == UNDO && list_len(board->undo) > 0)
      {
        undo_move(board);
        if (cpu && (list_len(board->undo) > 0))
          undo_move(board);
        orig_x = -1;
        orig_y = -1;
        dest_x = -1;
        dest_y = -1;
        boardInitColor(board);
        search_jumps = 1;
      }
      else if (res == REDO && list_len(board->redo) > 0)
      {
        redo_move(board);
        if (cpu && (list_len(board->redo) > 0))
          redo_move(board);
        orig_x = -1;
        orig_y = -1;
        dest_x = -1;
        dest_y = -1;
        boardInitColor(board);
        search_jumps = 1;
      }
      else
        print_error("No previous move");
    }

    if (res != MOUSE)
      continue;

// UPDATE COLORS
    if (!mandatory_jumps)
    {
      if (is_same_color(board, selected_x, selected_y))
      {
        decolorize(board, SELECTED);
        set_background(board, selected_x, selected_y, SELECTED);
        orig_x = selected_x;
        orig_y = selected_y;
      }

      if (IS_VALID(orig_x, orig_y) && is_empty(board, selected_x, selected_y))
      {
        dest_x = selected_x;
        dest_y = selected_y;
      }

      if (IS_VALID(orig_x, orig_y) && IS_VALID(dest_x, dest_y))
      {
        // simple move
        if (move(board, orig_x, orig_y, dest_x, dest_y) == 0)
        {
          free_moves(board->redo);
          redo_init(board);
          orig_x = -1;
          orig_y = -1;
          search_jumps = 1;
          board->player *= -1;
        }
          dest_x = -1;
          dest_y = -1;
          decolorize(board, SELECTED);
      }
    }

    else
      // Playing sequence one jump at the time with the mouse
    {

      if (nb_orig > 1) // must choose the start of the sequence
      {
        if (get_background(board, selected_x, selected_y) == ORIG)
        {
          boardInitColor(board);
          set_orig_cases(board, moves_list, &orig_x, &orig_y);
          set_background(board, selected_x, selected_y, SELECTED);
          orig_x = selected_x;
          orig_y = selected_y;
        }
      }

      int sequence_is_finished = 0;
      int k = nb_captures;

      if (selected_x >= 0 && selected_y >= 0
          && get_background(board, selected_x, selected_y) != DEST
          && get_background(board, selected_x, selected_y) != SELECTED)
      {
        print_error("Invalid move");
        // reset
        boardInitColor(board);
        search_jumps = 1;
      }

      if (selected_x >= 0 && selected_y >= 0
          && get_background(board, selected_x, selected_y) == DEST)
      {
        struct moves *list = moves_list->next; // sentinel
        for (; list; list = list->next) // browse list of sequences
        {
          if (list->seq->can_be_played) // valid sequence
          {
            struct move_seq *seq = list->seq->next; // sentinel

            if (nb_captures == 0
              && (seq->orig.x != orig_x || seq->orig.y != orig_y))
              // first step: check if the starting coords corresponds
            {
              list->seq->can_be_played = 0; // if not, ignore this seq
              continue; // go to the next sequence
            }

            for (int i = 0; seq->next && i < k; seq = seq->next, ++i);
              // go to seq[k]

            if (seq->dest.x == selected_x && seq->dest.y == selected_y)
              // selected case is in the sequence
            {
              if (nb_captures == k) // no new capture yet
                nb_captures++;
              if (seq->next == NULL)
              {
                sequence_is_finished = 1;
                break;
              }
              else
                set_background(board, selected_x, selected_y, CROSSPOINT);
            }

            else // selected case not in the sequence
              list->seq->can_be_played = 0;
          }
        }

        if (sequence_is_finished)
        {
          printf("Successful sequence, %d pieces captured\n", nb_captures);

          exec_seq_if_playable(board, moves_list);

          free_moves(board->redo);
          redo_init(board);
          orig_x = -1;
          orig_y = -1;
          dest_x = -1;
          dest_y = -1;
          boardInitColor(board);
          search_jumps = 1;
          board->player *= -1;
        }

        else if (nb_captures == k) // no capture within the last step
        {
          print_error("Invalid move");
          // reset
          boardInitColor(board);
          search_jumps = 1;
        }

        else
          printf("Sequence ongoing: captured %d pieces\n", nb_captures);

      }
   }
}

  SDL_Rect pos10;
  pos10.x = 10;
  pos10.y = 803;

  if (board->player == PLAYER_BLACK)
    SDL_BlitSurface(White_won, NULL, screen, &pos10);
  else
    SDL_BlitSurface(Black_won, NULL, screen, &pos10);

  SDL_Flip(screen);
  puts("Press any key to quit...");

  for (;;)
  {
    SDL_WaitEvent(&event);
    switch (event.type)
    {
      case SDL_QUIT:
      case SDL_KEYDOWN:
        goto quit;
      default:
        break;
    }
  }

quit:
//-------free SDL------------------------------------------------------------//
  SDL_FreeSurface(screen);
  SDL_FreeSurface(W);
  SDL_FreeSurface(B);
  SDL_FreeSurface(B_WP);
  SDL_FreeSurface(B_BP);
  SDL_FreeSurface(B_WK);
  SDL_FreeSurface(B_BK);
  SDL_FreeSurface(Undo);
  SDL_FreeSurface(Redo);
  SDL_FreeSurface(Save);
  SDL_FreeSurface(Quit);
  SDL_FreeSurface(Abandon);
  SDL_FreeSurface(Black_won);
  SDL_FreeSurface(White_won);
  SDL_Quit();
//-------free SDL------------------------------------------------------------//

  free_moves(moves_list);
  free_moves(board->undo);
  free_moves(board->redo);
  free(board);

  main(argc, argv);
  return 0;
}
