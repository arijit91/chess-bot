#include<cmath>
#include<cassert>
#include<string>

#include"utils.h"
using namespace std;

int strtosq(string s) {
  assert(s.size() == 2);
  return 8 * (s[1] - '1') + (s[0] - 'a');
}

string sqtostr(int num) {
  if (num == NO_SQUARE) return "None";
  string ret = "";
  ret += ('a' + (num % 8));
  ret += ('1' + (num / 8));
  return ret;
}

int opp(colour_type turn) {
  if (turn == WHITE) return (int)BLACK;
  return (int) WHITE;
}

int getSq(int rank, int file) {
  return 8 * rank + file;
}

int getFile(int sq) {
  return sq % 8;
}

int getRank(int sq) {
  return sq / 8;
}

int getManhattanDistance(int sq1, int sq2) {
  int f1 = getFile(sq1);
  int c1 = getRank(sq1);
  int f2 = getFile(sq2);
  int c2 = getRank(sq2);
  return abs(f1 - f2) + abs(c1 - c2);
}

int getKnightVal(int turn) {
  if (turn == WHITE) return WN;
  return BN;
}

int getPawnVal(int turn) {
  if (turn == WHITE) return WP;
  return BP;
}

int getBishopVal(int turn) {
  if (turn == WHITE) return WB;
  return BB;
}

int getRookVal(int turn) {
  if (turn == WHITE) return WR;
  return BR;
}

int getQueenVal(int turn) {
  if (turn == WHITE) return WQ;
  return BQ;
}

int getKingVal(int turn) {
  if (turn == WHITE) return WK;
  return BK;
}

bool isValidSquare(int sq) {
  return sq >= a1 && sq <= h8;
}
