#ifndef IA_H_
#define IA_H_

int isGameOver(struct board *board);

struct move_seq *get_IA_move(struct board *board, int cpu, int player);

long min(struct board *board, size_t deep, int cpu, int player);

long max(struct board *board, size_t deep, int cpu, int player);

long eval(struct board *board, int cpu, int player);

#endif
