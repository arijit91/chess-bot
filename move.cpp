#include<iostream>
#include<cassert>
#include<string>

#include "move.h"
#include "board.h"
using namespace std;

Move::Move(square_type from, square_type to, bool is_capture, piece_type promoted) {
  this->from = from;
  this->to = to;
  this->is_capture = is_capture;
  this->promoted = promoted;
}

string Move::getStr() {
  assert(0 <= from && from < 64);
  assert(0 <= to && to < 64);

  string from = Board::sqtostr(this->from);
  string to = Board::sqtostr(this->to);

  string ret = "";

  ret += from;
  ret += to;
  //cout<<"Is captured: "<<is_capture<<endl;
  //cout<<"Promoted: "<<promoted<<endl;

  if (promoted != NO_PIECE) {
    if (promoted == WQ || promoted == BQ) ret += "q";
    if (promoted == WR || promoted == BR) ret += "r";
    if (promoted == WB || promoted == BB) ret += "b";
    if (promoted == WN || promoted == BN) ret += "n";
  }
  return ret;
}

