# include <stdio.h>
# include <stdlib.h>
# include "board.h"
# include "piece.h"
# include "list.h"
# include "find_move.h"
# include "history.h"
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>

void boardInit(struct board *b)
{
  int line;
  int col;

  for(line = 0; line < 10; line++)
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
  b->nb_black = 20;
  b->nb_white = 20;
  b->undo = NULL;
  b->redo = NULL;
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

void count_pieces(struct board *b)
{
  b->nb_black = 0;
  b->nb_white = 0;

  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j < 10; j++)
    {
      if (is_white(b->cells[i][j].data))
        b->nb_white++;
      if (is_black(b->cells[i][j].data))
        b->nb_black++;
    }
  }
}
/*
void count_pieces_player(struct board *b, int player)
{
  b->nb_black = 0;
  b->nb_white = 0;

  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j < 10; j++)
    {
      if (is_white(b->cells[i][j].data))
        b->nb_white++;
      if (is_black(b->cells[i][j].data))
        b->nb_black++;
    }
  }
}
*/

void printBoard(struct board *b)
{
  b->nb_black = 0;
  b->nb_white = 0;

  if (b->player == PLAYER_WHITE)
    printf("\nWhite (x) play\n\n");
  else
    printf("\nBlack (o) play\n\n");

  char rep[] = {'X','x','.','o','O'};
  int i, j, piece;

  printf("       0   1   2   3   4   5   6   7   8   9\n");
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

      if (b->cells[i][j].background == LIGHT)
        printf("   |");
      else
        printf(" %c |",rep[piece + 2]);
    }
    printf("   %d\n", i);

  }
  printf("\n       0   1   2   3   4   5   6   7   8   9\n");
  printf("\nWhite: %d   Black: %d\n", b->nb_white, b->nb_black);
}

void print_error(const char *str)
{
  printf("Error: ");
  puts(str);
}

int pawn_to_king(struct board *b)
// called after a successful move
{
  int line, col, new_king, piece;
  new_king = 0;

  for (line = 0, col = 1; col < 10; col += 2)
  {
    piece = b->cells[line][col].data;
    if (is_pawn(piece) && PLAYER_WHITE == get_color(piece))
    {
      b->cells[line][col].data *= 2;
      new_king++;
      break;
    }
  }

  if (!new_king)
  {
    for (line = 9, col = 0; col < 10; col += 2)
    {
      piece = b->cells[line][col].data;
      if (is_pawn(piece) && PLAYER_BLACK == get_color(piece))
      {
        b->cells[line][col].data *= 2;
        new_king++;
        break;
      }
    }
  }

  if (new_king == 1) // save the coords of the new king
    save_king_coords(b, line, col);

  return new_king;
}

void decolorize(struct board *b, Color c)
{
  int line, col;

  for (line = 0; line < 10; line += 2)
  {
    for (col = 1; col < 10; col += 2)
    {
      if (b->cells[line][col].background == c)
        b->cells[line][col].background = DARK;
    }
  }

  for (line = 1; line < 10; line += 2)
  {
    for (col = 0; col < 10; col += 2)
    {
      if (b->cells[line][col].background == c)
        b->cells[line][col].background = DARK;
    }
  }
}

void set_last_move_trace(struct board *b, struct move_seq *seq)
{
  reset_last_move_trace(b);

  if (seq == NULL)
    return;

  for (seq = seq->next; seq->next; seq = seq->next)
    b->cells[seq->orig.x][seq->orig.y].last_move = 1;
  b->cells[seq->orig.x][seq->orig.y].last_move = 1;
  b->cells[seq->dest.x][seq->dest.y].last_move = 1;
}

void reset_last_move_trace(struct board *b)
{
  int line, col;

  for (line = 0; line < 10; line += 2)
  {
    for (col = 1; col < 10; col += 2)
    {
      if (b->cells[line][col].last_move)
        b->cells[line][col].last_move = 0;
    }
  }

  for (line = 1; line < 10; line += 2)
  {
    for (col = 0; col < 10; col += 2)
    {
      if (b->cells[line][col].last_move)
        b->cells[line][col].last_move = 0;
    }
  }
}

struct coords get_selected(struct board *b)
{
  int line, col;
  struct coords res;
  res.x = -1;
  res.y = -1;

  for (line = 0; line < 10; line += 2)
  {
    for (col = 1; col < 10; col += 2)
    {
      if (b->cells[line][col].background == SELECTED)
      {
        res.x = line;
        res.y = col;
        return res;
      }
    }
  }

  for (line = 1; line < 10; line += 2)
  {
    for (col = 0; col < 10; col += 2)
    {
      if (b->cells[line][col].background == SELECTED)
      {
        res.x = line;
        res.y = col;
        return res;
      }
    }
  }

  return res;
}
int is_same_color(struct board *b, int x, int y)
{
  if (is_out_of_board(x, y))
    return 0;
  return (b->cells[x][y].data * b->player > 0);
}

int is_empty(struct board *b, int x, int y)
{
  if (is_out_of_board(x, y))
    return 0;
  return (b->cells[x][y].data == 0);
}

void set_background(struct board *b, int x, int y, Color c)
{
  b->cells[x][y].background = c;
}

Color get_background(struct board *b, int x, int y)
{
  return b->cells[x][y].background;
}

int set_orig_cases(struct board *b, struct moves *list, int *orig_x, int *orig_y)
{
  int nb_orig = 0;
  list = list->next;

  for (; list; list = list->next)
  {
    struct move_seq *seq = list->seq->next;

    if (get_background(b, seq->orig.x, seq->orig.y) != ORIG)
    {
      set_background(b, seq->orig.x, seq->orig.y, ORIG);
      nb_orig++;
    }

    if (list->next == NULL && nb_orig == 1)
    {
      set_background(b, seq->orig.x, seq->orig.y, SELECTED);
      *orig_x = seq->orig.x;
      *orig_y = seq->orig.y;
    }
    set_background(b, seq->dest.x, seq->dest.y, DEST);

    if ( (seq = seq->next) )
    {
      for (; seq->next; seq = seq->next)
        set_background(b, seq->orig.x, seq->orig.y, DEST);
      set_background(b, seq->orig.x, seq->orig.y, DEST);
      set_background(b, seq->dest.x, seq->dest.y, DEST);
    }
  }

  return nb_orig;
}
