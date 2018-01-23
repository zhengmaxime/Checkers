struct board
{
  int cells[10][10];
};


//Init the board with basic pawns
void boardInit(struct board *b);

//Display the board
void printBoard(int cells[10][10]);
