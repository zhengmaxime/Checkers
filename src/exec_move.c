# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "board.h"
# include "piece.h"
# include "list.h"
# include "find_move.h"
# include "history.h"
# include "exec_move.h"

/*
 * Functions in this file use structures for moves.
 * Basic move that does not need struct is in simple_move.
 */

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

  pawn_to_king(b);
  return 0;
}

int exec_seq_in_list(struct board *b, struct moves *list, int i)
{
  for (; list; list = list->next)
  {
    if (i == 0)
    {
      if (0 == exec_seq(b, list->seq))
      {
        undo_push(b, list->seq);
        set_last_move_trace(b, list->seq);
        list->seq = NULL; // seq has been pushed to undo
        return 0;
      }
    }
    i--;
  }
  return -1;
}

int exec_seq_if_playable(struct board *b, struct moves *list)
{
  for (list = list->next; list; list = list->next)
  {
    if (list->seq->can_be_played)
    {
      if (0 == exec_seq(b, list->seq))
      {
        undo_push(b, list->seq);
        set_last_move_trace(b, list->seq);
        list->seq = NULL; // seq has been pushed to undo
        return 0;
      }
    }
  }
  return -1;
}

int exec_seq_IA(struct board *b, struct move_seq *seq)
{
  if (0 == exec_seq(b, seq))
  {
    undo_push(b, seq);
    set_last_move_trace(b, seq);
    seq = NULL; // seq has been pushed to undo
    return 0;
  }
  return -1;
}

/* exec a sequence in reverse (used for undoing) */
void exec_seq_reverse(struct board *b, struct move_seq *ms)
{
  struct move_seq *seq = ms->next; // sentinel

  int orig_x = seq->orig.x; // save origin cell coords
  int orig_y = seq->orig.y;

  for (; seq->next; seq = seq->next); // go to the last node

  // undo
  b->cells[orig_x][orig_y].data = b->cells[seq->dest.x][seq->dest.y].data;
  b->cells[seq->dest.x][seq->dest.y].data = 0;

  // undo the captures
  for (int i = 0; i < ms->nb_captures; ++i)
  {
    b->cells[ms->captures[i].x][ms->captures[i].y].data
   = ms->captures[i].data;
  }
}
