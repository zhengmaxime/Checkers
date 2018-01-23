# include "board.c"

int main()
{
  struct board *board = malloc(sizeof(struct board));
  boardInit(board);
  printBoard(board->cells);
  free(board);
  return 0;
}  
