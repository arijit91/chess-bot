#include<iostream>
#include<map>
#include<cassert>
#include "board.h"
using namespace std;

void Board::setPositionFromFEN(string fenString) {
    map<char, piece_type> M;
    char c;

    pawns[0].reset();
    pawns[1].reset();
    
    //pieces[WP] = pieces[BP] = pieces[NO_PIECE] = NO_SQUARE;

    M['r'] = BR, M['n'] = BN, M['b'] = BB, M['q'] = BQ, M['k'] = BK, 
    M['p'] = BP;
    M['R'] = WR, M['N'] = WN, M['B'] = WB, M['Q'] = WQ, M['K'] = WK, 
    M['P'] = WP;
    int cur = 0;
    for (int i = h1; i >= a1; i -= 8) {
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
                if (c == 'p') pawns[BLACK].set(i + ind);
                if (c == 'P') pawns[WHITE].set(i + ind);

                //if (c != 'p' && c != 'P') {
                //    pieces[M[c]] = (square_type) (i + ind);
                //}

                board[i + ind] = M[c];
                ind++;
            }
            if (i == a1 && ind == 8) break;
        }
    }

    assert(fenString[cur++] == ' ');
    c = fenString[cur++];
    if (c == 'w') turn = TURN_WHITE;
    else turn = TURN_BLACK;

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
        fm = (fm * 10) + (c - '0');
    }

    halfmove = hm; 
    fullmove = fm;
}

string Board::sqtostr(int num) {
    if (num == NO_SQUARE) return "None";
    string ret = "";
    ret += ('a' + (num % 8));
    ret += ('1' + (num / 8));
    return ret;
}

void Board::printBoard(bool castle, bool enp, bool moves) {
    map<int, char> M;
    M[WP] = 'P', M[WN] = 'N', M[WB] = 'B', M[WR] = 'R', M[WQ] = 'Q',
    M[WK] = 'K';
    M[BP] = 'p', M[BN] = 'n', M[BB] = 'b', M[BR] = 'r', M[BQ] = 'q',
    M[BK] = 'k';
    M[NO_PIECE] = '.';

    for (int i = h1; i >= a1; i-=8) {
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
