# include <stdio.h>
# include <stdlib.h>
# include "board.h"
# include "piece.h"
# include "list.h"


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
        b->cells[line][col].data = BP;
      if(line >= 4 && line <= 5)//middle of the board
        b->cells[line][col].data = 0;
      if(line > 5)//white pieces
        b->cells[line][col].data = WP;
      // 1/2 case colored
      if(col % 2 == 0)
        b->cells[line][col+1].data = 0;
      if(col % 2 == 1)
        b->cells[line][col-1].data = 0;

    }
  }
  b->player = PLAYER_WHITE;
}

void initCells(struct board *b)
{
  for(int i = 0; i < 10; i++)
  {
    for(int j = 0; j < 10; j++)
    {
      list_init(b->cells[i][j].l);
    }
  }
}

void printBoard(struct board *b)
{
  if (b->player == PLAYER_WHITE)
    printf("\nWhite (x) play\n\n");
  else
    printf("\nBlack (o) play\n\n");

  char rep[] = {'X','x',' ','o','O'};

  for (int i = 0; i < 10; i++)
  {
    printf("%d    |", i);
    for (int j = 0; j < 10; j++)
    {
      int piece = b->cells[i][j].data;
      printf("  %c  |",rep[piece + 2]);
    }
    printf("\n");
  }
  printf("\n        0     1     2     3     4     5     6     7     8     9\n");
}

void print_error(const char *str)
{
  printf("Error: ");
  puts(str);
}

int errManage(struct board *b, int curLine, int curCol, int destLine, int destCol)
{
  int curCell = b->cells[curLine][curCol].data;
  int destCell = b->cells[destLine][destCol].data;

  if (is_out_of_board(curLine, curCol) || is_out_of_board(destLine, destCol))
  {
    print_error("Out of the board");
    return -1;
  }

  if (curCell == 0)
  {
    print_error("Empty case");
    return -2;
  }

  if (curCell * b->player <= 0)
  {
    print_error("Not your piece");
    return -2;
  }

  if (is_pawn(curCell))
  {
    if ((abs(curLine - destLine) != 1) || (1 != abs(curCol - destCol)))
    {
      print_error("Pawn move one square diagonally");
      return -3;
    }
  }
  else
  {
    if (abs(curLine - destLine) != abs(curCol - destCol))
    {
      print_error("King move diagonally");
      return -3;
    }
  }

  if (destCell)
  {
    print_error("Destination cell is occupied");
    return -4;
  }

  if ( (curCell == WP && curLine <= destLine) ||
       (curCell == BP && curLine >= destLine))
  {
    print_error("Pawn can not move backward");
    return -5;
  }

  return 0;
}

//move the piece once we're sure the deplacement is valid
void move(struct board *b, int curLine, int curCol, int destLine, int destCol)
{
  int curPos = b->cells[curLine][curCol].data;
  b->cells[destLine][destCol].data = curPos;
  b->cells[curLine][curCol].data = 0;
}


int deplacement(struct board *b, int curLine, int curCol, int destLine, int destCol)
{
  int err = errManage(b, curLine, curCol, destLine, destCol);
  if (err == 0)
  {
    move(b, curLine, curCol, destLine, destCol);
  }
  return err;
}

void build_list_possible_moves(int x, int y)
{

}
