#ifndef BOARD_H
#define BOARD_H
#include<bitset>
#include<string>
#include<vector>

#include"constants.h"
#include"move.h"
using namespace std;

class Board {
  piece_type board[64];
  square_type enp_square;
  bitset<4> castle_rights;
  turn_type turn;
  int halfmove;
  int fullmove;
  vector<Move> possibleMovesList;
  vector<Board> undoMoveList;

  public:
  static string sqtostr(int);

  void setPositionFromFEN(string);
  void printBoard(bool castle = true, bool enp = true, bool moves = true);
  void generateMoveList();
  void generateKnightMoves();
  void makeMove(Move&);
  void undoMove();
  void printMoveList();
  void addMove(Move&);

  bool isMoveValid(Move&);
  bool inCheck();
  string getMove();
};
#endif
