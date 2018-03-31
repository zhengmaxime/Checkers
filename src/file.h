#ifndef FILE_H_
#define FILE_H_

// Save the pieces to a text file
int write_board_to_file(struct board *b, char filename[]);
// Init the board with specific pieces
int open_board_from_file(struct board *b, char filename[]);

#endif
