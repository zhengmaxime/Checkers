// constants used by the main program

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#define BLOCK_SIZE          75 // square, in pixels
#define NB_BLOCKS_WIDTH     10
#define NB_BLOCKS_HEIGHT    10
#define SCREEN_WIDTH     BLOCK_SIZE * NB_BLOCKS_WIDTH
#define SCREEN_HEIGHT    BLOCK_SIZE * NB_BLOCKS_HEIGHT

#define QUIT 1
#define HELP 2
#define SAVE 3
#define UNDO 4
#define REDO 5
#define SWITCH_TO_MOUSE 6
#define MOUSE 7
#define SHELL 8

#endif
