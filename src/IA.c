# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <limits.h>

# include "board.h"
# include "piece.h"
# include "list.h"
# include "find_move.h"
# include "exec_move.h"
# include "simple_move.h"
# include "history.h"
# include "IA.h"
# include <time.h>

struct move_seq *get_IA_move(struct board *b)
{
  struct moves *moves_list = NULL;
  unsigned int mandatory_jumps;
	int move_choice, moves_list_len;

  moves_list = build_moves(b);
  
  mandatory_jumps = list_len(moves_list);

  if(mandatory_jumps > 0)
  {
  }
  else
  {
    free_moves(moves_list);
    moves_list = build_moves_not_mandatory(b);
	}
  moves_list_len = list_len(moves_list);

	if (moves_list_len == 0)
    return NULL;
  
  srand(time(NULL));
  move_choice = rand() % moves_list_len;

	moves_list = moves_list->next; //sentinel jumped
	
  for(int i = 0; i < move_choice; i++)
	{
		moves_list = moves_list->next;
	}
  
  if (moves_list == NULL)
    return NULL;
	
  return moves_list->seq;
}
