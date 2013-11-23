#include<iostream>
#include<map>
#include<cassert>
#include<cstdlib>

#include "board.h"
#include "utils.h"
using namespace std;

int dKnight[8] = {-17, -15, -10, -6, 6, 10, 15, 17};
int dKing[8] = {-9, -8, -7, -1, 1, 7, 8, 9};

void Board::setPositionFromFEN(string fenString) {
  map<char, piece_type> M;
  char c;

  M['r'] = BR, M['n'] = BN, M['b'] = BB, M['q'] = BQ, M['k'] = BK, 
    M['p'] = BP;
  M['R'] = WR, M['N'] = WN, M['B'] = WB, M['Q'] = WQ, M['K'] = WK, 
    M['P'] = WP;
  int cur = 0;
  for (int i = a8; i >= a1; i -= 8) {
    int ind = 0;
    while (1) { 
      c = fenString[cur++];
      if (c == '/') break;

      if (c >= '1' && c <= '8') {
        for (int j = 1; j <= c - '0'; j++) {
          board[i + ind] = NO_PIECE;
          ind++;
        }
      }
      else {
        board[i + ind] = M[c];
        ind++;
      }
      if (i == a1 && ind == 8) break;
    }
  }

  assert(fenString[cur++] == ' ');
  c = fenString[cur++];
  if (c == 'w') turn = WHITE;
  else turn = BLACK;

  assert(fenString[cur++] == ' ');
  castle_rights.reset();
  while((c = fenString[cur++]) != ' ') {
    if (c == 'K') castle_rights.set(0);
    if (c == 'Q') castle_rights.set(1);
    if (c == 'k') castle_rights.set(2);
    if (c == 'q') castle_rights.set(3);
  }

  c = fenString[cur++];
  if (c == '-') enp_square = NO_SQUARE;
  else {
    int sq = (c - 'a');
    c = fenString[cur++];
    sq += 8 * (c - '1');
    enp_square = (square_type) sq;
  }

  assert(fenString[cur++] == ' ');

  int hm = 0, fm = 0;
  while ((c = fenString[cur++]) != ' ') {
    hm = (hm * 10) + (c - '0');
  }

  while ((cur < fenString.size())) {
    c = fenString[cur++];
    if (c < '0' || c > '9') break;
    fm = (fm * 10) + (c - '0');
  }

  halfmove = hm; 
  fullmove = fm;
}

void Board::printBoard(bool castle, bool enp, bool moves) {
  map<int, char> M;
  M[WP] = 'P', M[WN] = 'N', M[WB] = 'B', M[WR] = 'R', M[WQ] = 'Q',
    M[WK] = 'K';
  M[BP] = 'p', M[BN] = 'n', M[BB] = 'b', M[BR] = 'r', M[BQ] = 'q',
    M[BK] = 'k';
  M[NO_PIECE] = '.';

  for (int i = a8; i >= a1; i-=8) {
    for (int j = 0; j < 8; j++)
      cout<<M[board[i+j]];
    cout<<endl;
  }

  if (castle) {
    cout<<"Castling rights: ";
    if (castle_rights.test(0)) cout<<"WK ";
    if (castle_rights.test(1)) cout<<"WQ ";
    if (castle_rights.test(2)) cout<<"BK ";
    if (castle_rights.test(3)) cout<<"BQ ";
    cout<<endl;
  }

  if (enp) {
    cout<<"Enpassant square: "<<sqtostr(enp_square)<<endl;
  }
  if (moves) {
    cout<<"Halfmove: "<<halfmove<<endl;
    cout<<"Fullmove: "<<fullmove<<endl;
  }
}

int Board::getKingPos(int turn) {
  for (int i = a1; i <= a8; i++) {
    for (int j = 0; j < 8; j++) {
      if (board[i+j] == getKingVal(turn))
        return (i+j);
    }
  }
}

bool Board::inCheck() {
  return false;
  //int kingPos = getKingPos(turn);
  //if (turn == WHITE) {
  //  for (int i = 0; i < 8; i++) {

  //  }
  //}
}

void Board::undoMove() {
  assert(undoMoveList.size() > 0);
  *this = undoMoveList.back();
}

void Board::addMove(Move& m) {
  possibleMovesList.push_back(m);
}

void Board::makeMove(Move& m) {
  undoMoveList.push_back(*this);
}

bool Board::isMoveValid(Move& m) {
  makeMove(m);
  bool valid = inCheck();
  undoMove();
  return !valid;
}

bool Board::isEmpty(int sq) {
  return (board[sq] == NO_PIECE);
}

bool Board::isColour(int sq, int clr) {
  int c = board[sq];
  if (clr == WHITE && (c >= 0 && c<= 5)) return true;
  if (clr == BLACK && (c >= 6 && c <= 11)) return true;
  return false;
}

void Board::generateKnightMoves() {
  for (int i = a1; i <= a8; i++) {
    for (int j = 0; j < 8; j++) {
      if (board[i+j] == getKnightVal(turn)) {
        for (int k = 0; k < 8; k++) {
          int t = i + j + dKnight[k];
          if (getManhattanDistance(t, i+j) != 3) continue;
          if (isValidSquare(t) && (isEmpty(t) || isColour(t, 1-turn))) {
            bool is_capture = !isEmpty(t);
            Move m(i+j, t, is_capture);
            if (isMoveValid(m)) {
              addMove(m);
            }
          }
        }
      }
    }
  }
}

void Board::generatePawnMoves() {
  vector<Move> moves;
  for (int i = a1; i <= a8; i++) {
    for (int j = 0; j < 8; j++) {
      int sq = i+j;
      if (board[sq] == getPawnVal(turn)) {
        int newsq;
        if (turn == WHITE) newsq = sq + 8;
        else newsq = sq - 8;

        if (isValidSquare(newsq) && isEmpty(newsq)) {
          if ((turn == WHITE && newsq >= a8) || (turn == BLACK && newsq <= h1)) {
            moves.push_back(Move(sq, newsq, false, getKnightVal(turn)));
            moves.push_back(Move(sq, newsq, false, getBishopVal(turn)));
            moves.push_back(Move(sq, newsq, false, getRookVal(turn)));
            moves.push_back(Move(sq, newsq, false, getQueenVal(turn)));
          }
          else {
            moves.push_back(Move(sq, newsq, false));
          }
        }
      }
    }
  }
  for (int i = 0; i < moves.size(); i++) {
    if (isMoveValid(moves[i])) {
      addMove(moves[i]);
    }
  }
}

void Board::generateKingMoves() {
  for (int i = a1; i <= a8; i++) {
    for (int j = 0; j < 8; j++) {
      if (board[i+j] == getKingVal(turn)) {
        for (int k = 0; k < 8; k++) {
          int t = i + j + dKing[k];
          if (getManhattanDistance(t, i+j) > 2) continue;
          if (isValidSquare(t) && (isEmpty(t) || isColour(t, 1-turn))) {
            bool is_capture = !isEmpty(t);
            Move m(i+j, t, is_capture);
            if (isMoveValid(m)) {
              addMove(m);
            }
          }
        }
      }
    }
  }
}

void Board::generateMoveList() {
  possibleMovesList.clear();
  generateKnightMoves();
  generatePawnMoves();
  generateKingMoves();
}

void Board::printMoveList() {
  for (int i = 0; i < possibleMovesList.size() ; i++) {
    cout<<possibleMovesList[i].getStr()<<endl;
    cout<<endl;
  }
}

string Board::getMove() {
  generateMoveList();
  int sz = possibleMovesList.size();
  Move m = possibleMovesList[rand()%sz];
  return m.getStr();
}
