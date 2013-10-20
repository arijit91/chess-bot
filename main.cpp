#include<iostream>
#include<string>

#include"constants.h"
#include"board.h"
using namespace std;

string startFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq e6 0 1";

string fen2 = "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2";

int main() {
  Board b;
  b.setPositionFromFEN(startFEN);
  b.setPositionFromFEN(fen2);
  b.printBoard();
  return 0;
}
