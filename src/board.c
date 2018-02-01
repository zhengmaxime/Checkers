# include <stdio.h>
# include <stdlib.h>
# include "board.h"

# define BP 1
# define BR 2
# define WP -1
# define WR -2

/*struct cell
{
  int piece;
  char print;
}*/


void boardInit(struct board *b)
{
  int line;
  int col;

  for(; line < 10; line++)
  {
    if(line % 2 == 0)
      col = 1;
    else
      col = 0;
    for(; col < 10; col+=2)
    {
      if(line < 4)//black pieces
        b->cells[line][col] = BP;
      if(line >= 4 && line <= 5)//middle of the board
        b->cells[line][col] = 0;
      if(line > 5)//white pieces
        b->cells[line][col] = WP;
      // 1/2 case colored
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
  for(int i = 0; i < 10; i++)
  {
    printf("%d    |",i);
    for(int j = 0; j < 10; j++)
    {
      int  piece = cells[i][j];
      printf("  %c  |",rep[piece+2]);
      //printf("%d|",piece);
    }
    printf("\n");
  }
  printf("\n        0     1     2     3     4     5     6     7     8     9");
  printf("\n\n");
}

int errManage(struct board *b, int curLine, int curCol, int destLine, int destCol)
{
  int curPos = b->cells[curLine][curCol];
  int destPos = b->cells[destLine][destCol];
  printf("curPos = %d ; destPos = %d \n", curPos, destPos);
  //out of the board
  if(curLine < 0 || curLine > 9 || curCol < 0 || curCol > 9 || destLine < 0 \
     || destLine > 9 || destCol < 0 || destCol > 9)
    return -1;
  //doesn't respect rules
  if(abs(curLine - destLine) != abs(curCol - destCol))
    return -2;
  //if dest position is a ally
  if(destPos == curPos /*|| abs(destPos) == (abs(curPos) + 1)*/)
    return -3;
  return 0;
}

//move the piece once we're sure the deplacement is valid
void move(struct board *b, int curLine, int curCol, int destLine, int destCol)
{
  int curPos = b->cells[curLine][curCol];
  b->cells[destLine][destCol] = curPos;
  b->cells[curLine][curCol] = 0;
}

int deplacement(struct board *b, int curLine, int curCol, int destLine, int destCol)
{
  //error magagement
  int errno = errManage(b, curLine, curCol, destLine, destCol);
  if(errno == -1)
  {
    printf("Deplacement out of the board\n");
    return -1;//error, to figure in the main loop
  }
  if(errno == -2)
  {
    printf("You don't respect rules\n");
    return -1;//error, to figure in the main loop
  }
  if(errno == -3)
  {
    printf("ally at the position\n");
    return -1;//error, to figure in the main loop
  }
  int curPos = b->cells[curLine][curCol];
  int destPos = b->cells[destLine][destCol];
  //PIONS
  if(curPos == BP || curPos == WP)
  {
    //if destPos is empty
    if(destPos == 0)
    {
      errManage(b, curLine, curCol, destLine, destCol);
      move(b, curLine, curCol, destLine, destCol);
    }

  }
  //DAMES
  if(curPos == WR || curPos == BR)
  {
    //FIX ME
  }
  return 0;
}
