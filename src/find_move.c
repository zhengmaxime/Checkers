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
  for (int i = 0; i < nb_captures; ++i)
  {
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
  {
    moves_insert(moves, move_seq); // end of sequence
    return 0;
  }

  int jumped_piece = b->cells[x + dx][y + dy].data;
  int dest_piece = b->cells[x + 2*dx][y + 2*dy].data;

  if ((jumped_piece * cur_piece >= 0)
   || (dest_piece != 0)
   || (is_in_array(move_seq->captures, move_seq->nb_captures, x + dx, y + dy)))
  {
     moves_insert(moves, move_seq); // end of sequence
     return 0;
  }

  printf("Capture at %d %d from %d %d possible\n", x + dx, y + dy, x, y);

  struct move_seq *elm = malloc(sizeof (struct move_seq));
  elm->orig.x = x;
  elm->orig.y = y;
  elm->capt.x = x + dx;
  elm->capt.y = y + dy;
  elm->dest.x = x + 2*dx;
  elm->dest.y = y + 2*dy;

  seq_push_front(move_seq, elm);

  move_seq->captures[move_seq->nb_captures].x = x + dx;
  move_seq->captures[move_seq->nb_captures].y = y + dy;
  move_seq->nb_captures++;

  build_move_seq(b, cur_piece, x + 2*dx, y + 2*dy, moves, move_seq);

  return 1;
}

void build_move_seq(struct board *b, int cur_piece, int x, int y,
                   struct moves *moves,
                   struct move_seq *move_seq)

{
  if (move_seq == NULL) // create a new sequence
  {
    move_seq = malloc(sizeof (struct move_seq));
    seq_init(move_seq);
  }

  struct move_seq *seq_copy = copy(move_seq); // save move_seq

  // if True seq_copy has been modified, so a copy is created
  if (prise_simple_move(b, cur_piece, x, y, -1, -1, moves, seq_copy))
    seq_copy = copy(move_seq);

  if (prise_simple_move(b, cur_piece, x, y, -1, 1, moves, seq_copy))
    seq_copy = copy(move_seq);

  if (prise_simple_move(b, cur_piece, x, y, 1, -1, moves, seq_copy))
    seq_copy = copy(move_seq);

  prise_simple_move(b, cur_piece, x, y, 1, 1, moves, seq_copy);
}

struct moves *build_moves(struct board *b)
{
  struct moves *moves = malloc(sizeof (struct moves));
  moves_init(moves, NULL);

  for (int x = 0; x < 10; ++x)
  {
    for (int y = 0; y < 10; ++y)
    {
      if (get_color(b->cells[x][y].data) == b->player)
      {
        build_move_seq(b, b->cells[x][y].data, x, y, moves, NULL);
      }
    }
  }

  return moves;
}
