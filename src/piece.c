# include "piece.h"
# include "board.h"

int is_pawn(int piece){
  return (piece == BP || piece == WP);
};

int is_king(int piece){
  return (piece == BK || piece == WK);
};

int is_white(int piece){
  return (piece == WP || piece == WK);
}

int is_black(int piece){
  return (piece == BP || piece == BK);
}

int is_out_of_board(int x, int y){
  return (x < 0 || y < 0 || x > 9 || y > 9);
}
