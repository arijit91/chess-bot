#include<iostream>
#include<map>
#include<cassert>
#include<cstdlib>
#include<cstdio>

#include "board.h"
#include "utils.h"
using namespace std;

int dKnight[8] = {-17, -15, -10, -6, 6, 10, 15, 17};
int dKing[8] = {-9, -8, -7, -1, 1, 7, 8, 9};

int piece_values[13] = {1, 3, 3, 5, 9, 1000, 1, 3, 3, 5, 9, 1000, 0};

string bestmoves[1005];

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

int Board::getKingPos(int clr) {
  for (int i = a1; i <= a8; i+=8) {
    for (int j = 0; j < 8; j++) {
      if (board[i+j] == getKingVal(clr))
        return (i+j);
    }
  }
}

bool Board::isAttacked(int attackedSq, int attackingSq) {
  int attackingClr;
  assert(board[attackingSq] <= 11);
  attackingClr = 0;
  if (board[attackingSq] >= 6) {
    attackingClr = 1;
  }

  if (board[attackingSq] == getBishopVal(attackingClr) || board[attackingSq] == getQueenVal(attackingClr)) {
    for (int k = 0; k < 4; k++)
      for (int dist = 1; dist <= 7; dist++) {
        int rank = getRank(attackingSq);
        int file = getFile(attackingSq);

        if (k % 2 == 0) rank -= dist; else rank += dist; 
        if (k / 2 == 0) file -= dist; else file += dist; 

        if (rank < 0 || rank >= 8 || file < 0 || file >= 8) continue;
        int t = getSq(rank, file);

        if (isValidSquare(t) && (isEmpty(t) || isColour(t, 1-attackingClr))) {
          if (t == attackedSq) return true;
        }
        if (!isEmpty(t)) break;
      }
  }
  if (board[attackingSq] == getRookVal(attackingClr) || board[attackingSq] == getQueenVal(attackingClr)) {
    for (int k = 0; k < 4; k++)
      for (int dist = 1; dist <= 7; dist++) {
        int rank = getRank(attackingSq);
        int file = getFile(attackingSq);

        if (k == 0) file += dist;
        if (k == 1) rank += dist; 
        if (k == 2) file -= dist; 
        if (k == 3) rank -= dist; 

        if (rank < 0 || rank >= 8 || file < 0 || file >= 8) continue;
        int t = getSq(rank, file);

        if (isValidSquare(t) && (isEmpty(t) || isColour(t, 1-attackingClr))) {
          if (t == attackedSq) return true;
        }
        if (!isEmpty(t)) break;
      }
  }
  if (board[attackingSq] == getKingVal(attackingClr)) {
    for (int k = 0; k < 8; k++) {
      int t = attackingSq + dKing[k];
      if (getManhattanDistance(t, attackingSq) > 2) continue;
      if (isValidSquare(t) && (isEmpty(t) || isColour(t, 1-attackingClr))) {
        if (t == attackedSq) return true;
      }
    }
  }
  if (board[attackingSq] == getKnightVal(attackingClr)) {
    for (int k = 0; k < 8; k++) {
      int t = attackingSq + dKnight[k];
      if (getManhattanDistance(t, attackingSq) != 3) continue;
      if (isValidSquare(t) && (isEmpty(t) || isColour(t, 1-attackingClr))) {
        if (t == attackedSq) return true;
      }
    }
  }
  if (board[attackingSq] == getPawnVal(attackingClr)) {
    if (getFile(attackingSq) >= 1) {
      if (attackingClr == WHITE && (attackedSq == attackingSq + 7)) return true;
      if (attackingClr == BLACK && (attackedSq == attackingSq - 9)) return true;
    }
    if (getFile(attackingSq) < 7) {
      if (attackingClr == WHITE && (attackedSq == attackingSq + 9)) return true;
      if (attackingClr == BLACK && (attackedSq == attackingSq - 7)) return true;
    }
  }
  return false;
}

bool Board::inCheck(int clr) {
  int kingPos = getKingPos(clr);

  for (int i = a1; i <= a8; i+=8)
    for (int j = 0; j < 8; j++)
      if (isColour(i+j, 1-clr) && isAttacked(kingPos, i+j)) 
        return true;

  return false;
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

  // special moves

  // enpassant
  if ((board[m.from] == getPawnVal(WHITE)) && 
      (m.to - m.from == 16)) {
    enp_square = (square_type) ((int)m.from + 8);
  }
  else if (board[m.from] == getPawnVal(BLACK) && 
      (m.to - m.from == -16)) {
    enp_square = (square_type) ((int)m.from - 8);
  }

  if (board[m.from] == getPawnVal(WHITE) && m.to == enp_square) {
    board[enp_square - 8] = NO_PIECE;
  }
  else if (board[m.from] == getPawnVal(BLACK) && m.to == enp_square) {
    board[enp_square + 8] = NO_PIECE;
  }

  board[m.to] = board[m.from];
  board[m.from] = NO_PIECE;

  // promotion
  if (m.promoted != NO_PIECE) board[m.to] = m.promoted;

  //castling
  if (board[m.to] == getKingVal(turn) && abs(m.from - m.to) == 2) {
    if (m.from == e1 && m.to == g1) { board[h1] = NO_PIECE; board[f1] = WR; }
    if (m.from == e1 && m.to == c1) { board[a1] = NO_PIECE; board[d1] = WR; }
    if (m.from == e8 && m.to == g8) { board[h8] = NO_PIECE; board[f8] = BR; }
    if (m.from == e8 && m.to == c8) { board[a8] = NO_PIECE; board[d8] = BR; }
  }

  if (turn == WHITE) turn = BLACK;
  else if (turn == BLACK) turn = WHITE;
}

bool Board::isMoveValid(Move& m) {
  makeMove(m);
  bool valid = inCheck(opp(turn));
  undoMove();
  return !valid;
}

bool Board::isEmpty(int sq) {
  return (board[sq] == NO_PIECE);
}

bool Board::isColour(int sq, int clr) {
  int c = board[sq];
  if (clr == WHITE && (c >= 0 && c <= 5)) return true;
  if (clr == BLACK && (c >= 6 && c <= 11)) return true;
  return false;
}

void Board::generateKnightMoves() {
  for (int i = a1; i <= a8; i+=8) {
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

void Board::generateBishopMoves() {
  for (int i = a1; i <= a8; i+=8)
    for (int j = 0; j < 8; j++)
      if (board[i+j] == getBishopVal(turn))
        for (int k = 0; k < 4; k++)
          for (int dist = 1; dist <= 7; dist++) {
            int rank = getRank(i+j);
            int file = getFile(i+j);

            if (k % 2 == 0) rank -= dist; else rank += dist; 
            if (k / 2 == 0) file -= dist; else file += dist; 

            if (rank < 0 || rank >= 8 || file < 0 || file >= 8) continue;
            int t = getSq(rank, file);

            if (isValidSquare(t) && (isEmpty(t) || isColour(t, 1-turn))) {
              bool is_capture = !isEmpty(t);
              Move m(i+j, t, is_capture);
              if (isMoveValid(m)) {
                addMove(m);
              }
            }
            if (!isEmpty(t)) break;
          }
}

void Board::generateRookMoves() {
  for (int i = a1; i <= a8; i+=8)
    for (int j = 0; j < 8; j++)
      if (board[i+j] == getRookVal(turn))
        for (int k = 0; k < 4; k++)
          for (int dist = 1; dist <= 7; dist++) {
            int rank = getRank(i+j);
            int file = getFile(i+j);

            if (k == 0) file += dist;
            if (k == 1) rank += dist; 
            if (k == 2) file -= dist; 
            if (k == 3) rank -= dist; 

            if (rank < 0 || rank >= 8 || file < 0 || file >= 8) continue;
            int t = getSq(rank, file);

            if (isValidSquare(t) && (isEmpty(t) || isColour(t, 1-turn))) {
              bool is_capture = !isEmpty(t);
              Move m(i+j, t, is_capture);
              if (isMoveValid(m)) {
                addMove(m);
              }
            }
            if (!isEmpty(t)) break;
          }
}

void Board::generateQueenMoves() {
  for (int i = a1; i <= a8; i+=8)
    for (int j = 0; j < 8; j++)
      if (board[i+j] == getQueenVal(turn))
        for (int k = 0; k < 4; k++)
          for (int dist = 1; dist <= 7; dist++) {
            int rank = getRank(i+j);
            int file = getFile(i+j);

            if (k == 0) file += dist;
            if (k == 1) rank += dist; 
            if (k == 2) file -= dist; 
            if (k == 3) rank -= dist; 

            if (rank < 0 || rank >= 8 || file < 0 || file >= 8) continue;
            int t = getSq(rank, file);

            if (isValidSquare(t) && (isEmpty(t) || isColour(t, 1-turn))) {
              bool is_capture = !isEmpty(t);
              Move m(i+j, t, is_capture);
              if (isMoveValid(m)) {
                addMove(m);
              }
            }
            if (!isEmpty(t)) break;
          }
  for (int i = a1; i <= a8; i+=8)
    for (int j = 0; j < 8; j++)
      if (board[i+j] == getQueenVal(turn))
        for (int k = 0; k < 4; k++)
          for (int dist = 1; dist <= 7; dist++) {
            int rank = getRank(i+j);
            int file = getFile(i+j);

            if (k % 2 == 0) rank -= dist; else rank += dist; 
            if (k / 2 == 0) file -= dist; else file += dist; 

            if (rank < 0 || rank >= 8 || file < 0 || file >= 8) continue;
            int t = getSq(rank, file);

            if (isValidSquare(t) && (isEmpty(t) || isColour(t, 1-turn))) {
              bool is_capture = !isEmpty(t);
              Move m(i+j, t, is_capture);
              if (isMoveValid(m)) {
                addMove(m);
              }
            }
            if (!isEmpty(t)) break;
          }
}

void Board::generatePawnMoves() {
  vector<Move> moves;
  for (int i = a1; i <= a8; i+=8) {
    for (int j = 0; j < 8; j++) {
      int sq = i+j;
      if (board[sq] == getPawnVal(turn)) {
        int newsq;
        if (turn == WHITE) newsq = sq + 8;
        else newsq = sq - 8;

        // pushes
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

        if (turn == WHITE && getRank(sq) == 1 && isValidSquare(sq + 16) && isEmpty(sq + 8) && isEmpty(sq + 16)) {
          moves.push_back(Move(sq, sq + 16, false));
        }
        if (turn == BLACK && getRank(sq) == 6 && isValidSquare(sq - 16) && isEmpty(sq - 8) && isEmpty(sq - 16)) {
          moves.push_back(Move(sq, sq - 16, false));
        }

        // captures
        if (turn == WHITE && getFile(sq) >= 1 && isValidSquare(sq + 7) && isColour(sq + 7, BLACK)) {
          if (sq + 7 >= a8) {
            moves.push_back(Move(sq, sq + 7, true, getKnightVal(turn)));
            moves.push_back(Move(sq, sq + 7, true, getBishopVal(turn)));
            moves.push_back(Move(sq, sq + 7, true, getRookVal(turn)));
            moves.push_back(Move(sq, sq + 7, true, getQueenVal(turn)));
          }
          else {
            moves.push_back(Move(sq, sq + 7, true));
          }
        }
        if (turn == WHITE && getFile(sq) < 7 && isValidSquare(sq + 9) && isColour(sq + 9, BLACK)) {
          if (sq + 9 >= a8) {
            moves.push_back(Move(sq, sq + 9, true, getKnightVal(turn)));
            moves.push_back(Move(sq, sq + 9, true, getBishopVal(turn)));
            moves.push_back(Move(sq, sq + 9, true, getRookVal(turn)));
            moves.push_back(Move(sq, sq + 9, true, getQueenVal(turn)));
          }
          else {
            moves.push_back(Move(sq, sq + 9, true));
          }
        }
        if (turn == BLACK && getFile(sq) < 7 && isValidSquare(sq - 7) && isColour(sq - 7, WHITE)) {
          if (sq - 7 <= h1) {
            moves.push_back(Move(sq, sq - 7, true, getKnightVal(turn)));
            moves.push_back(Move(sq, sq - 7, true, getBishopVal(turn)));
            moves.push_back(Move(sq, sq - 7, true, getRookVal(turn)));
            moves.push_back(Move(sq, sq - 7, true, getQueenVal(turn)));
          }
          else {
            moves.push_back(Move(sq, sq - 7, true));
          }
        }
        if (turn == BLACK && getFile(sq) >= 1 && isValidSquare(sq - 9) && isColour(sq - 9, WHITE)) {
          if (sq - 9 <= h1) {
            moves.push_back(Move(sq, sq - 9, true, getKnightVal(turn)));
            moves.push_back(Move(sq, sq - 9, true, getBishopVal(turn)));
            moves.push_back(Move(sq, sq - 9, true, getRookVal(turn)));
            moves.push_back(Move(sq, sq - 9, true, getQueenVal(turn)));
          }
          else {
            moves.push_back(Move(sq, sq - 9, true));
          }
        }

        // en passant
        if (turn == WHITE && getRank(sq) == 4 && ((enp_square == sq + 9) || (enp_square == sq + 7))) {
          moves.push_back(Move(sq, enp_square, true));
        }

        if (turn == BLACK && getRank(sq) == 3 && ((enp_square == sq - 9) || (enp_square == sq - 7))) {
          moves.push_back(Move(sq, enp_square, true));
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
  for (int i = a1; i <= a8; i+=8) {
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

void Board::generateCastlingMoves() {
  int pos = getKingPos(turn);

  if (turn == WHITE) {
    if (castle_rights.test(0)) {
      int kPos = getKingPos(turn);
      assert(kPos == e1);
      // king side castling
      bool f = true;
      if (!(isEmpty(f1) && isEmpty(g1))) f = false;
      for (int i = a1; i <= a8; i+=8)
        for (int j = 0; j < 8; j++)
          if (isColour(i+j, 1-turn))
            if (isAttacked(e1, i+j) || isAttacked(f1, i+j) || isAttacked(g1, i+j)) {
              f = false;
              break;
            }
      if (f) {
        Move m(e1, g1, false);
        bool ok = isMoveValid(m);
        assert(ok);
        if (isMoveValid(m)) {
          addMove(m);
        }
      }
    }
    if (castle_rights.test(1)) {
      int kPos = getKingPos(turn);
      assert(kPos == e1);
      bool f = true;
      if (!(isEmpty(d1) && isEmpty(c1) && isEmpty(b1))) f = false;
      for (int i = a1; i <= a8; i+=8)
        for (int j = 0; j < 8; j++)
          if (isColour(i+j, 1-turn))
            if (isAttacked(e1, i+j) || isAttacked(d1, i+j) || isAttacked(c1, i+j)) {
              f = false;
              break;
            }
      if (f) {
        Move m(e1, c1, false);
        bool ok = isMoveValid(m);
        assert(ok);
        if (isMoveValid(m)) {
          addMove(m);
        }
      }
    }
  }
  else {
    if (castle_rights.test(2)) {
      int kPos = getKingPos(turn);
      assert(kPos == e8);
      // king side castling
      bool f = true;
      if (!(isEmpty(f8) && isEmpty(g8))) f = false;
      for (int i = a1; i <= a8; i+=8)
        for (int j = 0; j < 8; j++)
          if (isColour(i+j, 1-turn))
            if (isAttacked(e8, i+j) || isAttacked(f8, i+j) || isAttacked(g8, i+j)) {
              f = false;
              break;
            }
      if (f) {
        Move m(e8, g8, false);
        bool ok = isMoveValid(m);
        assert(ok);
        if (isMoveValid(m)) {
          addMove(m);
        }
      }
    }
    if (castle_rights.test(3)) {
      int kPos = getKingPos(turn);
      assert(kPos == e8);
      bool f = true;
      if (!(isEmpty(d8) && isEmpty(c8) && isEmpty(b8))) f = false;
      for (int i = a1; i <= a8; i+=8)
        for (int j = 0; j < 8; j++)
          if (isColour(i+j, 1-turn))
            if (isAttacked(e8, i+j) || isAttacked(d8, i+j) || isAttacked(c8, i+j)) {
              f = false;
              break;
            }
      if (f) {
        Move m(e8, c8, false);
        bool ok = isMoveValid(m);
        assert(ok);
        if (isMoveValid(m)) {
          addMove(m);
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
  generateBishopMoves();
  generateRookMoves();
  generateQueenMoves();
  generateCastlingMoves();
}

void Board::printMoveList() {
  for (int i = 0; i < possibleMovesList.size() ; i++) {
    cout<<possibleMovesList[i].getStr()<<endl;
    cout<<endl;
  }
}

int Board::evaluate() {
  int ret = 0;
  for (int i = a1; i <= a8; i+=8) {
    for (int j = 0; j < 8; j++) {
      int sign = 1;
      if (board[i+j] >= 0 && board[i+j] <= 5 && turn == BLACK) sign = -1;
      if (board[i+j] >= 6 && board[i+j] <= 11 && turn == WHITE) sign = -1;
      ret += sign * piece_values[board[i+j]];
    }
  }
  return ret;
}

int Board::alpha_beta(int alpha, int beta, int depth) {
  if (depth == 0) {
    return evaluate();
  }
  generateMoveList();
  vector<string> good_moves;
  for (int i = 0; i < possibleMovesList.size(); i++) {
    Move m = possibleMovesList[i];
    makeMove(m);
    int score = -alpha_beta(-beta, -alpha, depth - 1);
    undoMove();

    if (score >= alpha) {
      if (score > alpha) {
        bestmoves[depth] = m.getStr();
        good_moves.clear();
      }
      else {
        good_moves.push_back(m.getStr());
      }
      alpha = score;
    }
  }
  // choose a random move if all moves are equally good
  int sz = good_moves.size();
  if (sz > 0) {
    bestmoves[depth] = good_moves[rand()%sz];
  }
  return alpha;
}

string Board::iterativeDeepening() {
  max_depth = 1;
  for (int i = 1; i <= max_depth; i++) {
    int score = alpha_beta(-INF, INF, i);
  }
  return bestmoves[max_depth];
}

string Board::getMove() {
  return iterativeDeepening();
  //generateMoveList();
  //int sz = possibleMovesList.size();
  //Move m = possibleMovesList[rand()%sz];
  //return m.getStr();
}
