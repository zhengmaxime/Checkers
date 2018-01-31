# include "board.c"

int main()
{
  struct board *board = malloc(sizeof(struct board));
  //init the board
  boardInit(board);
  //print it
  printf("This is the start of the game\n");
  printBoard(board->cells);
  int curLine = 0;
  int curCol = 0;
  int destLine = 0;
  int destCol = 0;
  for(;;)
  {
    scanf("%d %d %d %d", &curLine, &curCol, &destLine, &destCol);
    deplacement(board, curLine, curCol, destLine, destCol);
    printBoard(board->cells);
  }
  free(board);
  return 0;
}  
