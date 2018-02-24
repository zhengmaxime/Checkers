#ifndef HISTORY_H_
#define HISTORY_H_

# include "list.h"
# include "board.h"

/* init the undo stack */
void undo_init(struct board *b);

/* init the redo stack */
void redo_init(struct board *b);

/*
 * fill the fields in move_seq structure.
 * if no captures, param is 0
 */
void seq_fill(struct move_seq *seq,
              int orig_x, int orig_y,
              int dest_x, int dest_y,
              struct coords captures[20],
              int nb_captures);

/* save the coords of the new king in the undo stack */
void save_king_coords(struct board *b, int x, int y);

/* push the sequence in the undo stack */
void undo_push(struct board *b, struct move_seq *seq);

/* push the sequence in the redo stack */
void redo_push(struct board *b, struct move_seq *seq);

/* undo the last move */
void undo_move(struct board *b);

/* exec a sequence in reverse */
void exec_seq_reverse(struct board *b, struct moves *m);

#endif /* HISTORY_H_ */
