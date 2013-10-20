#ifndef BOARD_H
#define BOARD_H
#include<bitset>
#include<string>

#include"constants.h"
using namespace std;

class Board {
  piece_type board[64];
  //square_type pieces[13];
  square_type enp_square;
  bitset<64> pawns[2];
  bitset<4> castle_rights;
  turn_type turn;
  int halfmove;
  int fullmove;

  public:
  string sqtostr(int);

  void setPositionFromFEN(string);
  void printBoard(bool castle = true, bool enp = true, bool moves = true);
};
#endif
