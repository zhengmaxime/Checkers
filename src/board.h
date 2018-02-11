#include "list.h"

# define BP  1
# define BK  2
# define WP -1
# define WK -2

# define PLAYER_WHITE -1
# define PLAYER_BLACK  1

struct cell
{
  struct list *l;
  int data;
};

struct board
{
  struct cell cells[10][10];
  int nb_white;
  int nb_black;
  int player;
};

void cellInit(struct cell c);

//Init the board with basic pawns
void boardInit(struct board *b);

//Display the board
void printBoard(struct board *b);

int errManage(struct board *b, int curLine, int curCol, int destLine, int
destCol);


void move(struct board *b, int curLine, int curCol, int destLine, int destCol);


int deplacement(struct board *b, int curLine, int curCol, int destLine, int
destCol);
