# ifndef BOARD_H_
# define BOARD_H_

#include "list.h"
# define BP  1
# define BK  2
# define WP -1
# define WK -2

# define PLAYER_WHITE -1
# define PLAYER_BLACK  1

typedef enum Color Color;
enum Color
{
  LIGHT,
  DARK, // DEFAULT
  SELECTED, // "player has selected one of his pieces"
  ORIG, // "player can play from this case"
  DEST, // "red point on a empty case"
  CROSSPOINT // "not the final dest"
};

struct cell
{
  int data;
  int last_move; // mark the last move, 1 if true
  Color background;
};

struct board
{
  struct cell cells[10][10];
  int nb_white;
  int nb_black;
  int player;
  int is_copy;
  struct moves *undo;
  struct moves *redo;
};

struct move_seq; // forward declaration

// Init the board with basic pawns
void boardInit(struct board *b);

// Init the background color of cells
void boardInitColor(struct board *b);

// Count the pieces
void count_pieces(struct board *b);

// Display the board
void printBoard(struct board *b);

// Print error
void print_error(const char *str);

// crowning
int pawn_to_king(struct board *b);

// if the selected piece belongs to the current player
int is_same_color(struct board *b, int x, int y);

// if the selected cell is empty
int is_empty(struct board *b, int x, int y);

// get/set color of a cell
void set_background(struct board *b, int x, int y, Color c);
Color get_background(struct board *b, int x, int y);

// reset cell if cell.background == c
void decolorize(struct board *b, Color c);

void reset_last_move_trace(struct board *b);
void set_last_move_trace(struct board *b, struct move_seq *list);

// get the coords of the selected cell
struct coords get_selected(struct board *b);

// set cells colors
// return the number of choices for the player ("ORIG" cells)
int set_orig_cases(struct board *b, struct moves *list, int *orig_x, int *orig_y);

# endif /* BOARD_H_ */
