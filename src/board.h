#include "list.h"

# ifndef BOARD_H_
# define BOARD_H_

# define BP  1
# define BK  2
# define WP -1
# define WK -2

# define PLAYER_WHITE -1
# define PLAYER_BLACK  1

typedef enum Color Color;
enum Color
{
  LIGHT, DARK
};

struct cell
{
  struct list *l;
  int data;
  Color background;
};

struct board
{
  struct cell cells[10][10];
  int nb_white;
  int nb_black;
  int player;
};

struct coords
{
  int x;
  int y;
};

struct simple_move
{
  struct coords orig;
  struct coords captures[20];
  struct coords dest;
  int nb_captures;
};



void cellInit(struct cell c);

//Init the board with basic pawns
void boardInit(struct board *b);

// Init the board with specific pieces
int open_board_from_file(struct board *b, char filename[]);
//
// Init the background color of cells
void boardInitColor(struct board *b);

//Display the board
void printBoard(struct board *b);

// Save the pieces to a text file
int write_board_to_file(struct board *b, char filename[]);

//Print error
void print_error(const char *str);

int errManage(struct board *b, int curLine, int curCol, int destLine, int
destCol);


void move(struct board *b, int curLine, int curCol, int destLine, int destCol);


int deplacement(struct board *b, int curLine, int curCol, int destLine, int
destCol);

int pawn_to_king(struct board *b);

int prise_simple_move(struct board *b, int x, int y, int dx, int dy);
# endif /* BOARD_H_ */
