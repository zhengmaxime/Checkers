# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "board.h"
# include "piece.h"
# include "list.h"
# include "find_move.h"
# include "history.h"
# include "exec_move.h"

/* init the undo stack */
void undo_init(struct board *b)
{
  struct moves *undo = malloc(sizeof (struct moves));
  moves_init(undo, NULL);
  b->undo = undo;
}

/* init the redo stack */
void redo_init(struct board *b)
{
  struct moves *redo = malloc(sizeof (struct moves));
  moves_init(redo, NULL);
  b->redo = redo;
}

/*
 * fill the fields in move_seq structure.
 * if no captures, param is 0
 */
void seq_fill(struct move_seq *seq,
              int orig_x, int orig_y,
              int dest_x, int dest_y,
              struct coords captures[20],
              int nb_captures)
{
  seq->orig.x = orig_x;
  seq->orig.y = orig_y;
  seq->dest.x = dest_x;
  seq->dest.y = dest_y;
  if (captures)
    memcpy(seq->captures, &captures, 20 * sizeof (struct coords));
  seq->nb_captures = nb_captures;
}

/* save the coords of the new king in the undo stack */
void save_king_coords(struct board *b, int x, int y)
{
  b->undo->next->crowned.x = x;
  b->undo->next->crowned.y = y;
}

/* push the sequence in the undo stack */
void undo_push(struct board *b, struct move_seq *seq)
{
  struct moves *elm = malloc(sizeof (struct moves));
  moves_init(elm, seq);
  moves_push_front(b->undo, elm);
}

/* push the sequence in the redo stack */
void redo_push(struct board *b, struct moves *m)
{
  moves_push_front(b->redo, m);
}

/* undo the last move */
void undo_move(struct board *b)
{
  struct moves *m = moves_pop_front(b->undo);

  b->player *= -1;
  redo_push(b, m);

  if (m->seq == NULL) // empty move
    return;

  if (!is_out_of_board(m->crowned.x, m->crowned.y)) // undo crowning
    b->cells[m->crowned.x][m->crowned.y].data /= 2;

  exec_seq_reverse(b, m);
  set_last_move_trace(b, m->seq);
}

/* redo the last move */
void redo_move(struct board *b)
{
  struct moves *m = moves_pop_front(b->redo);

  b->player *= -1;
  undo_push(b, m->seq);

  if (m->seq == NULL) // empty move
    return;

  exec_seq(b, m->seq);
  set_last_move_trace(b, m->seq);
}
