# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "board.h"
# include "piece.h"
# include "list.h"
# include "find_move.h"
# include "history.h"
# include "exec_move.h"
# include "constants.h"

void fflush_stdin()
{
  int c = 0;
  while (c != '\n' && c != EOF)
    c = getchar();
}

int parse_input(int *curLine, int *curCol,
                int *destLine, int *destCol,
                int seq, int *i_seq)
{
  printf("Type: ");
  char input[20] = {0};

  if (fgets(input, 20, stdin) != NULL) // reads 18 char + \n, store to input
  {
    if (strchr(input, '\n') == NULL) // input too long, didn't find \n
      fflush_stdin(); // prevent overflow

    if (strncmp(input, "quit", 4) == 0)
      return QUIT;

    if (strncmp(input, "help", 4) == 0)
      return HELP;

    if (strncmp(input, "undo", 4) == 0)
      return UNDO;

    if (strncmp(input, "redo", 4) == 0)
      return REDO;

    if (strncmp(input, "save", 4) == 0)
      return SAVE;

    if (input[0] >= 48 && input[0] <= 57) // digit
    {
      if (seq) // > 0
      {
        sscanf(input, "%d", i_seq);
        if (*i_seq > seq || *i_seq <= 0)
          return -1;
        else
          return 0;
      }

      sscanf(input, "%d %d %d %d", curLine, curCol, destLine, destCol);
      return 0;
    }

      return -1; // error
    }
    else
      return -1; // error
  }

