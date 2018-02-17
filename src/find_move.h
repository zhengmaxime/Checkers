# ifndef FIND_MOVE_H
# define FIND_MOVE_H

# include "board.h"
# include "list.h"

int prise_simple_move(struct board *b, int cur_piece,
                      int x, int y, int dx, int dy,
                      struct moves *moves,
                      struct move_seq *move_seq);


int build_move_seq(struct board *b, int cur_piece, int x, int y,
                   struct moves *moves,
                   struct move_seq *move_seq);

int build_moves(struct board *b);
# endif /* FIND_MOVE_H */
