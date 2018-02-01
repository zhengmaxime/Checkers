# include "piece.h"
# include "board.h"

int is_pawn(int piece){
  return (piece == BP || piece == WP);
};

int is_king(int piece){
  return (piece == BK || piece == WK);
};
