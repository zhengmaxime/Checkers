#ifndef IA_H_
#define IA_H_

int isGameOver(struct board *board);

struct move_seq *get_IA_move();

long min(struct board *board, size_t deep);

long max(struct board *board, size_t deep);

long eval(struct board *board);

#endif
