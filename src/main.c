# include <stdio.h>
# include <stdlib.h>
# include "board.h"
# include "piece.h"


int main()
{
  struct board *board = malloc(sizeof(struct board));
  //init the board
  boardInit(board);
  //print it
  printf("This is the start of the game\n");
  printBoard(board);
  int curLine = 0;
  int curCol = 0;
  int destLine = 0;
  int destCol = 0;
  //main loop
  for(;;)
  {
    scanf("%d %d %d %d", &curLine, &curCol, &destLine, &destCol);
    if (0 == deplacement(board, curLine, curCol, destLine, destCol))
      board->player *= -1;
    printBoard(board);
  }
  free(board);
  return 0;
}
