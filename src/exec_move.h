#ifndef EXEC_MOVE_H_
#define EXEC_MOVE_H_

# include "board.h"
# include "piece.h"
# include "list.h"
# include "find_move.h"
# include "history.h"

int exec_seq(struct board *b, struct move_seq *list);

int exec_seq_in_list(struct board *b, struct moves *list, int i);

/* exec a sequence in reverse */
void exec_seq_reverse(struct board *b, struct moves *m);

#endif /* EXEC_MOVE_H_ */
