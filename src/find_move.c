# include <stdio.h>
# include <stdlib.h>
# include "board.h"
# include "piece.h"
# include "list.h"
# include "find_move.h"
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>

int is_in_array(struct coords captures[], int nb_captures, int x, int y)
{
  //printf("nb_captures in array %d\n", nb_captures);
  for (int i = 0; i < nb_captures; ++i)
  {
    //printf("x %d y %d\n", captures[i].x, captures[i].y);
    if (captures[i].x == x && captures[i].y == y)
      return 1;
  }
  return 0;
}

int prise_simple_move(struct board *b, int cur_piece, int x, int y, int dx, int dy,
                      struct moves *moves,
                      struct move_seq *move_seq)
{
  if (is_out_of_board(x + dx, y + dy)
      || is_out_of_board(x + dx + dx, y + dy + dy))
    return -1;

   // int cur_piece = b->cells[x][y].data; it could be 0
  int jumped_piece = b->cells[x + dx][y + dy].data;
  int dest_piece = b->cells[x + 2*dx][y + 2*dy].data;

  if (move_seq == NULL)
  {
    move_seq = malloc(sizeof (struct move_seq));
    seq_init(move_seq);
    struct moves *elm_list = malloc(sizeof (struct moves));
    move_init(elm_list, move_seq);
    move_push_front(moves, elm_list);
  }
   if ((jumped_piece * cur_piece >= 0)
   || (dest_piece != 0)
   || (is_in_array(move_seq->captures, move_seq->nb_captures, x + dx, y + dy)))
    return -1;

  printf("Capture at %d %d from %d %d possible\n", x + dx, y + dy, x, y);

  struct move_seq *elm = malloc(sizeof (struct move_seq));
  elm->orig.x = x;
  elm->orig.y = y;
  elm->capt.x = x + dx;
  elm->capt.y = y + dy;
  elm->dest.x = x + 2*dx;
  elm->dest.y = y + 2*dy;

  for (int i = 0; i < 20; ++i)
  {
    elm->captures[i] = move_seq->captures[i];
  }

  elm->nb_captures = move_seq->nb_captures;
  elm->captures[elm->nb_captures].x = x + dx;
  elm->captures[elm->nb_captures].y = y + dy;
  elm->nb_captures++;

  move_seq->nb_captures = elm->nb_captures;
  for (int i = 0; i < 20; ++i)
  {
    move_seq->captures[i] = elm->captures[i];
  }
  //printf("move_seq->nb_captures: %d\n", move_seq->nb_captures);

  seq_push_front(move_seq, elm);

  build_move_seq(b, cur_piece, x + 2*dx, y + 2*dy, moves, move_seq);




//  b->cells[x + 2*dx][y + 2*dy].data = cur_piece;
//  b->cells[x][y].data = 0;

  return 0;
}

/*
int prise_around(struct board *b, int x, int y)
{
  int res;

  prise_simple_move(b, x);
  prise_simple_move();
  prise_simple_move();
  prise_simple_move();
}
*/

int build_move_seq(struct board *b, int cur_piece, int x, int y,
                   struct moves *moves,
                   struct move_seq *move_seq)

{
  //printf("build_move_seq %d %d\n", x, y);
  //if (move_seq != NULL)
  //  printf("build_move_seq nb_captures %d\n", move_seq->nb_captures);
  struct move_seq *seq_copy = copy(move_seq);
  //if (seq_copy != NULL)
  //  printf("build_move_seq nb_captures copy %d\n", seq_copy->nb_captures);
  int res;
  res = prise_simple_move(b, cur_piece, x, y, -1, -1, moves, seq_copy);

  if (res == 0)
    seq_copy = copy(move_seq);
  prise_simple_move(b, cur_piece, x, y, -1, 1, moves, seq_copy);
  if (res == 0)
    seq_copy = copy(move_seq);
  prise_simple_move(b, cur_piece, x, y, 1, -1, moves, seq_copy);
  if (res == 0)
    seq_copy = copy(move_seq);
  prise_simple_move(b, cur_piece, x, y, 1, 1, moves, seq_copy);
  return 0;
}

int build_moves(struct board *b)
{
  struct moves *moves = malloc(sizeof (struct moves));
  move_init(moves, NULL);

  for (int x = 0; x < 10; ++x)
  {
    for (int y = 0; y < 10; ++y)
    {
      if (get_color(b->cells[x][y].data) == b->player)
      {
        int cur_piece = b->cells[x][y].data;
        build_move_seq(b, cur_piece, x, y, moves, NULL);
      }
    }
  }

  return 0;
}
