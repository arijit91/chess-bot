#ifndef MOVE_H
#define MOVE_H

#include<string>

#include"constants.h"
using namespace std;

class Move {
  public:
  square_type from;
  square_type to;
  bool is_capture;
  piece_type promoted;

  int score;

  Move() {
    from = to = NO_SQUARE;
    is_capture = false;
    promoted = NO_PIECE;
  }
  Move(int, int, bool, int promoted=NO_PIECE);
  string getStr();
};
#endif
