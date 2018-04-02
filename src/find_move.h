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
                   int old_dx, int old_dy,
                   struct moves *moves,
                   struct move_seq *move_seq);

/*
 * return a list of mandatory moves
 */
struct moves *build_moves(struct board *b);

/*
 * find_possible_move
 * If a possible move is found, it will be pushed into the list moves.
 * dx dy: relative moves (from -10 to 10, so it works with kings)
 */
void find_possible_move(struct board *b,
                        int x, int y,
                        int dx, int dy,
                        struct moves *moves);

/*
 * build a list of possible moves (if no mandatory jumps)
 */
struct moves *build_moves_not_mandatory(struct board *b);

# endif /* FIND_MOVE_H */
