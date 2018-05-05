# Makefile

CC=gcc #-fsanitize=address
CPPFLAGS= -MMD -D_XOPEN_SOURCE=500 `pkg-config --cflags sdl`
CFLAGS= -Wall -Wextra -std=c99 -O0 -g
LDFLAGS=-lSDL -lSDL_image #Linker
LDLIBS=`pkg-config --libs sdl SDL_image`

SRC= main.c \
     board.c \
     piece.c \
     list.c \
     IA.c\
     simple_move.c \
     find_move.c \
     exec_move.c \
     history.c \
     file.c \
     shell.c \
     menu.c

OBJ= ${SRC:.c=.o}
DEP= ${SRC:.c=.d}

all: main

main: ${OBJ}

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} main

-include ${DEP}

# END
