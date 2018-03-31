# include <stdio.h>
# include <stdlib.h>
# include "board.h"
# include "piece.h"
# include "list.h"
# include "find_move.h"
# include "history.h"
# include "file.h"
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>

int write_board_to_file(struct board *b, char filename[])
{
  FILE *f = fopen(filename, "w");
  int i, j, piece;
  char rep[] = {'X','x','.','o','O'};

  for (i = 0; i < 10; i++)
  {
    for (j = 0; j < 10; j++)
    {
      piece = b->cells[i][j].data;
      fputc(rep[piece + 2], f);
    }
    fputc('\n', f);
  }

  if (b->player == PLAYER_WHITE)
    fputc('w', f);
  if (b->player == PLAYER_BLACK)
    fputc('b', f);

  fputc('\n', f);

  fclose(f);
  return 0;
}

int open_board_from_file(struct board *b, char filename[])
{
  FILE *f = fopen(filename, "r");
  if (f == NULL)
    return -1;

  struct stat statbuf;
  stat(filename, &statbuf);
  if ((statbuf.st_size) != 112)
  {
    fclose(f);
    return -1;
  }

  int i, j, k, c;
  char rep[] = {'X','x','.','o','O'};

  for (i = 0; i < 10; i++)
  {
    for (j = 0; j < 10; j++)
    {
      c = fgetc(f);
      for (k = 0; k < 5; ++k)
      {
        if (rep[k] == c)
          break;
      }
      b->cells[i][j].data = k - 2;
    }
    fgetc(f); // \n
  }

  c = fgetc(f);

  if (c == 'w')
    b->player = PLAYER_WHITE;
  if (c == 'b')
    b->player = PLAYER_BLACK;

  fgetc(f); // \n

  fclose(f);
  return 0;
}
