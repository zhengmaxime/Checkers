# include <stdio.h>
# include <stdlib.h>
# include "board.h"
# include "piece.h"
# include "list.h"
# include "find_move.h"
# include "history.h"

int errManage(struct board *b, int curLine, int curCol,
                               int destLine, int destCol,
                               int print)
{
  if (is_out_of_board(curLine, curCol) || is_out_of_board(destLine, destCol))
  {
    if (print)
      print_error("Out of the board");
    return -1;
  }

  int curCell = b->cells[curLine][curCol].data;
  int destCell = b->cells[destLine][destCol].data;

  if (curCell == 0)
  {
    if (print)
      print_error("Empty case");
    return -2;
  }

  if (curCell * b->player <= 0)
  {
    if (print)
      print_error("Not your piece");
    return -2;
  }

  if (destCell)
  {
    if (print)
      print_error("Destination cell is occupied");
    return -4;
  }

  if (is_pawn(curCell))
  {
    if ((abs(curLine - destLine) != 1) || (1 != abs(curCol - destCol)))
    {
      if (print)
        print_error("Pawn move one square diagonally");
      return -3;
    }
  }
  else
  {
    if (abs(curLine - destLine) != abs(curCol - destCol))
    {
      if (print)
        print_error("King move diagonally");
      return -3;
    }
    else // Check if the diagonal is empty (king can't jump over another piece)
    {
      int x = curLine;
      int y = curCol;
      int dx = ((destLine - curLine) > 0) ? 1 : -1;
      int dy = ((destCol - curCol) > 0) ? 1 : -1;

      while (x != destLine && y != destCol && b->cells[x][y].data == 0)
      {
        x += dx;
        y += dy;
      }

      if (b->cells[x + dx][y + dy].data != 0)
      {
        if (print)
          print_error("King is blocked by another piece");
        return -3;
      }
    }
  }

  if ( (curCell == WP && curLine <= destLine) ||
       (curCell == BP && curLine >= destLine))
  {
    if (print)
      print_error("Pawn can not move backward");
    return -5;
  }

  return 0;
}

//move the piece once we're sure the deplacement is valid
void __move(struct board *b, int curLine, int curCol, int destLine, int destCol)
{
  int curPos = b->cells[curLine][curCol].data;
  b->cells[destLine][destCol].data = curPos;
  b->cells[curLine][curCol].data = 0;
}


// check for errors, play the move, add move to history and create king
int move(struct board *b, int curLine, int curCol,
                          int destLine, int destCol)
{
  int err = errManage(b, curLine, curCol, destLine, destCol, 1);

  if (!err)
  {
     __move(b, curLine, curCol, destLine, destCol);
    struct move_seq *ms = malloc(sizeof (struct move_seq));
    seq_init(ms); // sentinel

    struct move_seq *elm = malloc(sizeof (struct move_seq));
    seq_fill(elm, curLine, curCol, destLine, destCol, 0, 0);
    seq_push_front(ms, elm);

    undo_push(b, ms);
    pawn_to_king(b);
    set_last_move_trace(b, ms);
  }

  return err;
}
