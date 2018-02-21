# ifndef FIND_MOVE_H
# define FIND_MOVE_H

# include "board.h"
# include "list.h"

/*
 * Return 0 if no jump possible, else return 1
 * dx dy: relative moves (-1 or 1)
 */
int simple_jump(struct board *b,
                int cur_piece,
                int x, int y,
                int dx, int dy,
                struct moves *moves,
                struct move_seq *move_seq);

/*
 * return 1 if a sequence is found, else 0
 */
int build_move_seq(struct board *b,
                   int cur_piece,
                   int x, int y,
                   struct moves *moves,
                   struct move_seq *move_seq);

/*
 * return a list of moves
 */
struct moves *build_moves(struct board *b);

# endif /* FIND_MOVE_H */
