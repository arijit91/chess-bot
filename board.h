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
  colour_type turn;
  int halfmove;
  int fullmove;
  vector<Move> possibleMovesList;
  vector<Board> undoMoveList;

  public:
  int getKingPos(int);
  bool isColour(int, int);
  bool isEmpty(int);

  bool inCheck();

  void setPositionFromFEN(string);
  void printBoard(bool castle = true, bool enp = true, bool moves = true);

  void generateMoveList();
  void generateKnightMoves();
  void generateBishopMoves();
  void generateRookMoves();
  void generateQueenMoves();
  void generatePawnMoves();
  void generateKingMoves();
  void makeMove(Move&);
  void undoMove();
  void printMoveList();
  void addMove(Move&);
  bool isMoveValid(Move&);
  string getMove();

};
#endif
