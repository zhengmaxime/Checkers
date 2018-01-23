# include <stdio.h>
# include <stdlib.h>
# include "board.h"

# define BP 1
# define WP -1


/*struct cell
{
  int piece;
  char print;
}*/


void boardInit(struct board *b)
{
  size_t line;
  size_t col;
  /*for(size_t i = 0; i < 10; i++)
  {
    for(size_t j = 0; j < 10; j++)
    {
      board->cells[i][j] = 0;
    }
  }*/
  for(;  line < 10; line++)
  {
    if(line % 2 == 0)
      col = 1;
    else
      col = 0;
    for(; col < 10; col+=2)
    {
      if(line < 4)//black
        b->cells[line][col] = BP;
      if(line >= 4 && line <= 5)
        b->cells[line][col] = 0;
      if(line > 5)//white
        b->cells[line][col] = WP;
      if(col % 2 == 0)
        b->cells[line][col+1] = 0;
      if(col % 2 == 1)
        b->cells[line][col-1] = 0;
    }
  }
}

void printBoard(int cells[10][10])
{
  char rep[] = {'X','x',' ','o','O'};
  for(size_t i = 0; i < 10; i++)
  {
    printf("%ld\t|",i);
    for(size_t j = 0; j < 10; j++)
    {
      int piece = cells[i][j];
      printf("%c|",rep[piece+2]);
    }
    printf("\n");
  }
}
