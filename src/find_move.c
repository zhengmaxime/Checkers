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

/*
 * Return 0 if no jump possible, else return 1
 * dx dy: relative moves (-1 or 1)
 */
int simple_jump(struct board *b,
                int cur_piece,
                int x, int y,
                int dx, int dy,
                struct moves *moves,
                struct move_seq *move_seq)
{
  if (is_out_of_board(x + dx, y + dy)
   || is_out_of_board(x + 2*dx, y + 2*dy))
    return 0;

  int jumped_piece = b->cells[x + dx][y + dy].data;
  int dest_piece   = b->cells[x + 2*dx][y + 2*dy].data;

  int i = 1;
  if (is_king(cur_piece) && jumped_piece == 0)
  {
    while (!is_out_of_board(x + (i+2)*dx, y + (i+2)*dy)
          && b->cells[x + i*dx][y + i*dy].data == 0)
      i++;

    jumped_piece = b->cells[x + i * dx][y + i * dy].data;
    dest_piece   = b->cells[x + (i+1)*dx][y + (i+1)*dy].data;
  }

  if ((jumped_piece * cur_piece >= 0)
   || (dest_piece != 0)
   || (is_in_array(move_seq->captures,
                   move_seq->nb_captures,
                   x + i * dx, y + i*dy)))
  {
    return 0;
  }

  //printf("Capture at %d %d from %d %d possible\n", x + i*dx, y + i*dy, x, y);

  struct move_seq *elm = malloc(sizeof (struct move_seq));
  elm->orig.x = x;
  elm->orig.y = y;
  elm->capt.x = x + i * dx;
  elm->capt.y = y + i * dy;
  elm->dest.x = x + (i + 1) * dx;
  elm->dest.y = y + (i + 1) * dy;

  seq_push_front(move_seq, elm);

  move_seq->captures[move_seq->nb_captures].x = x + i * dx;
  move_seq->captures[move_seq->nb_captures].y = y + i * dy;
  move_seq->captures[move_seq->nb_captures].data = jumped_piece;
  move_seq->nb_captures++;

  if (is_pawn(cur_piece))
  {
    if (0 == build_move_seq(b, cur_piece, x + 2*dx, y + 2*dy, dx, dy,
                            moves, move_seq)
       && (move_seq->nb_captures > 0)) // end of sequence
    {
      moves_insert(moves, move_seq);
    }
  }

  if (is_king(cur_piece))
  {
    struct move_seq *seq_copy;

    // go through all free cells after the jump
    while (!is_out_of_board(x + (i + 1) * dx, y + (i + 1) * dy)
        && b->cells[x + (i + 1) * dx][y + (i + 1)* dy].data == 0)
    {
      seq_copy = copy(move_seq);

      if (0 == build_move_seq(b, cur_piece,
                              x + (i + 1) * dx,
                              y + (i + 1) * dy,
                              dx, dy,
                              moves, seq_copy) // end of sequence
          && (seq_copy->nb_captures > 0))
      {
        // reminder: push_front is used so the last move is the first
        seq_copy->next->dest.x = x + (i + 1) * dx; // update final destination
        seq_copy->next->dest.y = y + (i + 1) * dy;

        moves_insert(moves, seq_copy);
      }
      else
      {
        free_seq(seq_copy); // copy not used
      }

      i++;
    }
  }

  return 1;
}

/*
 * return 1 if a sequence is found, else 0
 */
int build_move_seq(struct board *b,
                   int cur_piece,
                   int x, int y,
                   int old_dx, int old_dy,
                   struct moves *moves,
                   struct move_seq *move_seq)

{
  int res1, res2, res3, res4;
  res1 = 0;
  res2 = 0;
  res3 = 0;
  res4 = 0;

  if (old_dx != 1 || old_dy != 1) // avoid backwards step
  {
    struct move_seq *seq_copy1 = copy(move_seq); // save move_seq
    res1 = simple_jump(b, cur_piece, x, y, -1, -1, moves, seq_copy1);
    // if res == 0 copy not modified, not used
    if (res1 == 0)
    {
      free_seq(seq_copy1);
    }
  }

  if (old_dx != 1 || old_dy != -1)
  {
    struct move_seq *seq_copy2 = copy(move_seq);
    res2 = simple_jump(b, cur_piece, x, y, -1, 1, moves, seq_copy2);
    if (res2 == 0)
    {
      free_seq(seq_copy2);
    }
  }

  if (old_dx != -1 || old_dy != 1)
  {
    struct move_seq *seq_copy3 = copy(move_seq);
    res3 = simple_jump(b, cur_piece, x, y, 1, -1, moves, seq_copy3);
    if (res3 == 0)
    {
      free_seq(seq_copy3);
    }
  }

  if (old_dx != -1 || old_dy != -1)
  {
    struct move_seq *seq_copy4 = copy(move_seq);
    res4 = simple_jump(b, cur_piece, x, y, 1, 1, moves, seq_copy4);
    if (res4 == 0)
    {
      free_seq(seq_copy4);
    }
  }

  if (res1 == 0 && res2 == 0 && res3 == 0 && res4 == 0)
    return 0;

  return 1;
}

/*
 * return a list of moves
 */
struct moves *build_moves(struct board *b)
{
  struct moves *moves = malloc(sizeof (struct moves));
  moves_init(moves, NULL);

  struct move_seq *move_seq;

  for (int x = 0; x < 10; ++x)
  {
    for (int y = 0; y < 10; ++y)
    {
      if (get_color(b->cells[x][y].data) == b->player)
      {
        move_seq = malloc(sizeof (struct move_seq));
        seq_init(move_seq);
        build_move_seq(b, b->cells[x][y].data, x, y, 0, 0, moves, move_seq);
        free_seq(move_seq);
      }
    }
  }

  return moves;
}
