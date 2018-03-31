#ifndef SIMPLE_MOVE_H_
#define SIMPLE_MOVE_H_

// In this file, handling of moves with no captures

// check for errors and print them
int errManage(struct board *b, int curLine, int curCol, int destLine, int
destCol);


//move the piece once we're sure the deplacement is valid
void __move(struct board *b, int curLine, int curCol,
                             int destLine, int destCol);

// check for errors, play the move, add move to history and create king
int move(struct board *b, int curLine, int curCol, int destLine, int destCol);

#endif
