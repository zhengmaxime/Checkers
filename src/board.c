# include <stdio.h>
# include <stdlib.h>
# include "board.h"
# include "piece.h"
# include "list.h"
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>

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

void boardInitColor(struct board *b)
{
  int line, col;

  for (line = 0; line < 10; line += 2)
  {
    for (col = 0; col < 10; col += 2)
    {
      b->cells[line][col].background = LIGHT;
      b->cells[line][col + 1].background = DARK;
    }
  }

  for (line = 1; line < 10; line += 2)
  {
    for (col = 0; col < 10; col += 2)
    {
      b->cells[line][col].background = DARK;
      b->cells[line][col + 1].background = LIGHT;
    }
  }
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
  b->nb_black = 0;
  b->nb_white = 0;

  if (b->player == PLAYER_WHITE)
    printf("\nWhite (x) play\n\n");
  else
    printf("\nBlack (o) play\n\n");

  char rep[] = {'X','x',' ','o','O'};
  int i, j, piece;

  for (i = 0; i < 10; i++)
  {
    printf("%d    |", i);
    for (j = 0; j < 10; j++)
    {
      piece = b->cells[i][j].data;

      if (is_white(piece))
        b->nb_white++;
      if (is_black(piece))
        b->nb_black++;

      printf(" %c |",rep[piece + 2]);
    }
    printf("\n");
  }
  printf("\n       0   1   2   3   4   5   6   7   8   9\n");
  printf("\nWhite: %d   Black: %d\n", b->nb_white, b->nb_black);
}

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
void print_error(const char *str)
{
  printf("Error: ");
  puts(str);
}

int errManage(struct board *b, int curLine, int curCol, int destLine, int destCol)
{
  if (is_out_of_board(curLine, curCol) || is_out_of_board(destLine, destCol))
  {
    print_error("Out of the board");
    return -1;
  }

  int curCell = b->cells[curLine][curCol].data;
  int destCell = b->cells[destLine][destCol].data;

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

int pawn_to_king(struct board *b)
// called after a successful move
{
  int line, col, res, piece;
  res = 0;

  if (b->player == PLAYER_WHITE)
  {
    line = 0;
    col = 1;
  }

  else
  {
    line = 9;
    col = 0;
  }

  for (; col < 10; col += 2)
  {
    piece = b->cells[line][col].data;
    if (is_pawn(piece) && b->player == get_color(piece))
    {
      b->cells[line][col].data *= 2;
      res++;
      break;
    }
  }

  return res;
}

int prise_simple_move(struct board *b, int x, int y, int dx, int dy)
{
  int cur_piece = b->cells[x][y].data;
  int jumped_piece = b->cells[x + dx][y + dy].data;
  int dest_piece = b->cells[x + 2*dx][y + 2*dy].data;

  if (is_out_of_board(x + dx, y + dy)
   || is_out_of_board(x + dx + dx, y + dy + dy)
   || (jumped_piece * cur_piece >= 0)
   || (dest_piece != 0))
    return -1;

  b->cells[x + 2*dx][y + 2*dy].data = cur_piece;
  b->cells[x][y].data = 0;

  return 0;
}

/*
int prise_around(struct board *b, int x, int y)
{
  int res;

  prise_simple_move(b, x);
  prise_simple_move();
  prise_simple_move();
  prise_simple_move();
}
*/

void build_list_possible_moves()
{

}
