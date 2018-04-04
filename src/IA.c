# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <limits.h>
# include <string.h>
# include <time.h>

# include "board.h"
# include "piece.h"
# include "list.h"
# include "find_move.h"
# include "exec_move.h"
# include "simple_move.h"
# include "history.h"
# include "IA.h"

int isGameOver(struct board *board)
{ 
  struct moves *moves_list = NULL;
  struct moves *moves_not_mandatory = NULL;
 
  //test if someone has won
  if ((board->player == PLAYER_WHITE && board->nb_white == 0)
   || (board->player == PLAYER_BLACK && board->nb_black == 0))
  {
    return 1;
  }
  else
  {
    moves_list = build_moves(board);
    moves_not_mandatory = build_moves_not_mandatory(board);
    if(moves_list == NULL || moves_not_mandatory == NULL)
      return -1;//ERROR
    
    if(list_len(moves_list) == 0 && list_len(moves_not_mandatory) == 0)
    {
       return 1; 
    }
    else
    {
      return 0;
    }
  }
} 

struct move_seq *get_IA_move(struct board *board, int cpu, int player)
{
  size_t deep = 1; //deepest of the minimax calcul
  struct moves *moves_list = build_moves(board);
  struct board *board_copy = NULL;
  struct moves *best_move = malloc(sizeof(struct moves));
  
	int  best_move_val;
  int max_val = -2000000000;

  if(list_len(moves_list) == 0)
  {
    moves_list = build_moves_not_mandatory(board);
  }
  board_copy = malloc(sizeof(struct board));
  for(; moves_list->next != NULL;
        moves_list = moves_list->next)
  {
    board_copy = memcpy(board_copy, board, sizeof(struct board));
    if(exec_seq(board_copy, moves_list->seq) == -1)
      printf("error while exec_seq mandatory\n");
    best_move_val = min(board_copy, deep, cpu, player);

    if(best_move_val > max_val)
    {
      max_val = best_move_val;
      best_move = moves_list;
    }
  }
  /*else
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
	*/

  return best_move->seq;
}

long min(struct board *board, size_t deep, int cpu, int player)
{
  long min_val;
  long min;
  struct moves *moves_list = NULL;
  struct board *board_copy = NULL;

  if(deep == 0 || isGameOver(board))
    return eval(board, -1);// -1 -> adverse player
  min_val = 2000000000;//-2 millions

  moves_list = build_moves(board);
  if(list_len(moves_list) == 0)
  {
    moves_list = build_moves_not_mandatory(board);
  }
  moves_list = moves_list->next; //sentinel
  for(; moves_list->next != NULL;
        moves_list = moves_list->next)
  {
    board_copy = malloc(sizeof(struct board));
    board_copy = memcpy(board_copy, board, sizeof(struct board));
    if(exec_seq(board_copy, moves_list->seq) == -1)
      printf("error while exec_seq mandatory\n");
    min = max(board_copy, deep -1, cpu, player);

    if(min < min_val)
      min_val = min;
  }
  return min_val;
}

long max(struct board *board, size_t deep, int cpu, int player)
{
  long max_val;
  long max;
  struct moves *moves_list = NULL;
  struct board *board_copy = NULL;

  if(deep == 0 || isGameOver(board))
    return eval(board, 1);// 1 -> cpu
  max_val = -2000000000;//-2 millions
  
  moves_list = build_moves(board);
  if(list_len(moves_list) != 0)
  {
    moves_list = build_moves_not_mandatory(board);
  }
  moves_list = moves_list->next; //sentinel
  for(; moves_list->next != NULL;
        moves_list = moves_list->next)
  {     
    board_copy = malloc(sizeof(struct board));
    board_copy = memcpy(board_copy, board, sizeof(struct board));
    if(exec_seq(board_copy, moves_list->seq) == -1)
      printf("error while exec_seq mandatory (max)\n"); 
    max = min(board_copy, deep -1, cpu, player);

    if(max > max_val)
      max_val = max;
  }   
  return max_val;
}

long eval(struct board *board, int actual_player, int cpu, int player)
{
  //if actual_player
  if(gameIsOver
  if(cpu == PLAYER_WHITE && board->nb_white == 0
  || cpu == PLAYER_BLACK && board->nb_black == 0)
  {
    return -1000;
  }
  if(player == PLAYER_WHITE && board->nb_white == 0
  || player == PLAYER_BLACK && board->nb_black == 0)
  {
    return 1000;
  }


  return 1; //FIXME
}
