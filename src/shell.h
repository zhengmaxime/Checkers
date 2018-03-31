#ifndef SHELL_H_
#define SHELL_H_

void fflush_stdin();


int parse_input(int *curLine, int *curCol,
                int *destLine, int *destCol,
                int seq, int *i_seq);
#endif
