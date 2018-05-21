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
  size_t deep = 3; //deepest of the minimax calcul
  struct moves *moves_list = build_moves(board);
  struct board *board_copy = NULL;
  struct move_seq *best_move = malloc(sizeof(struct move_seq));

	int best_move_val;
  int max_val = INT_MIN;

  if (list_len(moves_list) == 0)
  {
    moves_list = build_moves_not_mandatory(board);
  }

  if (list_len(moves_list) == 0)
  {
    free_moves(moves_list);
    return NULL;
  }

  if (list_len(moves_list) == 1)
  {
    best_move = moves_list->next->seq;
    moves_list->next->seq = NULL;
    free_moves(moves_list);
    return best_move;
  }

  struct moves *head = moves_list;
  moves_list = moves_list->next; //sentinel
  for (; moves_list; moves_list = moves_list->next)
  {
    board_copy = malloc(sizeof(struct board));
    board_copy = memcpy(board_copy, board, sizeof(struct board));
    if (exec_seq(board_copy, moves_list->seq) == -1)
      printf("error while exec_seq mandatory\n");
    pawn_to_king(board_copy);
    board_copy->player *= -1;
    best_move_val = min(board_copy, deep, cpu, player);

    if (best_move_val >= max_val)
    {
      printf("%d\n", best_move_val);
      max_val = best_move_val;
      best_move = moves_list->seq;
      moves_list->seq = NULL;
    }
    free(board_copy);
  }

  free_moves(head);
  return best_move;
}

long min(struct board *board, size_t deep, int cpu, int player)
{
  long min_val;
  long min;
  struct moves *moves_list = NULL;
  struct board *board_copy = NULL;

  if(deep == 0 || isGameOver(board))
    return eval(board, player);
  min_val = INT_MAX;

  moves_list = build_moves(board);
  if (list_len(moves_list) == 0)
  {
    moves_list = build_moves_not_mandatory(board);
  }
  struct moves *head = moves_list;
  moves_list = moves_list->next; //sentinel
  for (; moves_list; moves_list = moves_list->next)
  {
    board_copy = malloc(sizeof(struct board));
    board_copy = memcpy(board_copy, board, sizeof(struct board));
    if (exec_seq(board_copy, moves_list->seq) == -1)
      printf("error while exec_seq mandatory\n");
    pawn_to_king(board_copy);
    board_copy->player *= -1;
    min = max(board_copy, deep -1, cpu, player);

    if (min < min_val)
      min_val = min;

    free(board_copy);
  }
  free_moves(head);
  return min_val;
}

long max(struct board *board, size_t deep, int cpu, int player)
{
  long max_val;
  long max;
  struct moves *moves_list = NULL;
  struct board *board_copy = NULL;

  if(deep == 0 || isGameOver(board))
    return eval(board, player);
  max_val = INT_MIN;

  moves_list = build_moves(board);
  if(list_len(moves_list) == 0)
  {
    moves_list = build_moves_not_mandatory(board);
  }
  struct moves *head = moves_list;
  moves_list = moves_list->next; //sentinel
  for (; moves_list; moves_list = moves_list->next)
  {
    board_copy = malloc(sizeof(struct board));
    board_copy = memcpy(board_copy, board, sizeof(struct board));
    if (exec_seq(board_copy, moves_list->seq) == -1)
      printf("error while exec_seq mandatory (max)\n");
    pawn_to_king(board_copy);
    board_copy->player *= -1;
    max = min(board_copy, deep -1, cpu, player);

    if (max > max_val)
      max_val = max;

    free(board_copy);
  }
  free_moves(head);
  return max_val;
}
#if 0
long eval(struct board *board/*, int actual_player*/, int cpu, int player)
{
  //if actual_player
  //cpu looses
  if((cpu == PLAYER_WHITE && board->nb_white == 0)
  || (cpu == PLAYER_BLACK && board->nb_black == 0))
    return -1000;
  //player looses
  if((player == PLAYER_WHITE && board->nb_white == 0)
  || (player == PLAYER_BLACK && board->nb_black == 0))
    return 1000;
  /*if(acual_player == PLAYER_WHITE)
  {
    return board->nb_white
  }*/
  return 0;
}
#endif
long eval(struct board *b, int player)
{
  int white_score = 0;
  int black_score = 0;
  int res = 0;

  int ev[10][10] = {
  {0, 80, 0, 100, 0, 125, 0, 100, 0, 75},
  {50, 0, 70, 0, 70, 0, 70, 0, 70, 0},
  {0, 25, 0, 20, 0, 20, 0, 20, 0, 40},
  {25, 0 ,10, 0, 10, 0, 10, 0, 25, 0},
  {0, 25, 0, 10, 0, 10, 0, 10, 0, 25},
  {25, 0 ,10, 0, 10, 0, 10, 0, 25, 0},
  {0, 25, 0, 10, 0, 10, 0, 10, 0, 25},
  {40, 0, 20, 0, 20, 0, 20, 0, 25, 0},
  {0, 70, 0, 70, 0, 70, 0, 70, 0, 50},
  {75, 0, 100, 0, 125, 0, 100, 0, 80, 0}
  };

  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j < 10; j++)
    {
      if (is_white(b->cells[i][j].data))
      {
        white_score += ev[i][j];
        if (is_king(b->cells[i][j].data))
          white_score += 300;
      }
      if (is_black(b->cells[i][j].data))
      {
        black_score += ev[i][j];
        if (is_king(b->cells[i][j].data))
          black_score += 300;
      }
    }
  }

  if (is_black(player))
    res = white_score - black_score;
  else
    res = black_score - white_score;

  return res;
}
