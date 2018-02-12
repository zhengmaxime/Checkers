# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "board.h"
# include "piece.h"

void fflush_stdin()
{
  int c = 0;
  while (c != '\n' && c != EOF)
    c = getchar();
}

int parse_input(int *curLine, int *curCol, int *destLine, int *destCol)
{
  char input[20] = {0};

  if (fgets(input, 20, stdin) != NULL) // reads 18 char + \n, store to input
  {
    if (strchr(input, '\n') == NULL) // input too long, didn't find \n
      fflush_stdin(); // prevent overflow

    if (strncmp(input, "quit", 4) == 0)
      return 1;

    if (strncmp(input, "help", 4) == 0)
      return 2;

    if (input[0] >= 48 && input[0] <= 57)
    {
      sscanf(input, "%d %d %d %d", curLine, curCol, destLine, destCol);
      return 0;
    }

    return -1; // error
  }
  else
    return -1; // error
}

int main()
{
  struct board *board = malloc(sizeof(struct board));
  //init the board
  boardInit(board);
  boardInitColor(board);
  //print it
  printf("This is the start of the game\n");
  printBoard(board);

  int *curLine, *curCol, *destLine, *destCol;
  curLine  = malloc(sizeof (int));
  curCol   = malloc(sizeof (int));
  destLine = malloc(sizeof (int));
  destCol  = malloc(sizeof (int));

  int res;

  //main loop
  for (;;)
  {
    res = parse_input(curLine, curCol, destLine, destCol);

    if (res == -1) //error
    {
      puts("Problem when reading your input");
      continue;
    }

    if (res == 0) //normal
    {
      if (0 == deplacement(board, *curLine, *curCol, *destLine, *destCol))
        board->player *= -1;
      printBoard(board);
    }

    if (res == 1) //quit
      break;

    if (res == 2) //help
    {
      puts("Type 4 digits separated by space character:"
        " current line and column, destination line and column");
      continue;
    }
  }

  free(board);
  free(curLine);
  free(curCol);
  free(destLine);
  free(destCol);
  return 0;
}

/*
scanf("%d %d %d %d", &curLine, &curCol, &destLine, &destCol);
// consume all the char remaining in stdin buffer
// prevent overflow and infinite loop
while (getchar() != '\n');
*/
