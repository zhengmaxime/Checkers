#ifndef IA_H_
#define IA_H_

int isGameOver(struct board *board);

struct move_seq *get_IA_move(struct board *board, int player, int deep);

long min(struct board *board, size_t deep, int player, int alpha, int beta);

long max(struct board *board, size_t deep, int player, int alpha, int beta);

long eval(struct board *board, int player);

#endif
