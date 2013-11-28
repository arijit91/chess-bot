#ifndef BOARD_H
#define BOARD_H

#include<bitset>
#include<string>
#include<vector>

#include"move.h"
#include"constants.h"
using namespace std;

// class that does everything
// maybe i shouldn't have called it board
class Board {
  piece_type board[64];
  square_type enp_square;
  bitset<4> castle_rights;
  colour_type turn;
  int halfmove;
  int fullmove;
  vector<Move> possibleMovesList;
  vector<Board> undoMoveList;

  int max_depth;

  public:
  int getKingPos(int);
  bool isColour(int, int);
  bool isEmpty(int);

  bool isAttacked(int, int);
  bool inCheck(int);

  void setPositionFromFEN(string);
  void printBoard(bool castle = true, bool enp = true, bool moves = true);

  void generateMoveList();
  void generateKnightMoves();
  void generateBishopMoves();
  void generateRookMoves();
  void generateQueenMoves();
  void generatePawnMoves();
  void generateKingMoves();
  void generateCastlingMoves();
  void makeMove(Move&);
  void undoMove();
  void printMoveList();
  void addMove(Move&);
  bool isMoveValid(Move&);

  int evaluate();
  int alpha_beta(int, int, int);
  string iterativeDeepening();
  string getMove();

};
#endif
