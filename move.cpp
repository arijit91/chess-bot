#include<iostream>
#include<cassert>
#include<string>

#include "move.h"
#include "board.h"
#include "utils.h"
using namespace std;

Move::Move(int from, int to, bool is_capture, int promoted) {
  this->from = (square_type) from;
  this->to = (square_type) to;
  this->is_capture = is_capture;
  this->promoted = (piece_type) promoted;
}

string Move::getStr() {
  assert(0 <= from && from < 64);
  assert(0 <= to && to < 64);

  string from = sqtostr(this->from);
  string to = sqtostr(this->to);

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

