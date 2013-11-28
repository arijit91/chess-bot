#include<iostream>
#include<cassert>
#include<string>
#include<cstdio>
#include<cstring>
#include<ctime>
#include<cstdlib>

#include"board.h"
using namespace std;

string startFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
string pos2FEN = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";
string pos3FEN = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1";
string pos4FEN = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";
string pos5FEN = "rnbqkb1r/pp1p1ppp/2p5/4P3/2B5/8/PPP1NnPP/RNBQK2R w KQkq - 0 6";
string pos6FEN = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10";
string pos7FEN = "8/3K4/2p5/p2b2r1/5k2/8/8/1q6 b - - 1 67";
string wideopenperftFEN = "rnbqkbnr/8/8/8/8/8/8/RNBQKBNR w KQkq - 0 1";

Board board;

void setupBoard(string s) {
  assert(s.substr(0, 13) == "position fen ");
  string fen = s.substr(13);
  board.setPositionFromFEN(fen);
  return ;
}

void go(string line) {
  printf("bestmove %s\n", board.getMove().c_str());
}

void UciLoop() {
	setbuf(stdin, NULL);
  setbuf(stdout, NULL);

	char line[500];
    printf("id name %s\n", NAME);
    printf("uciok\n");
	
	int MB = 64;

	while (1) {
		memset(&line[0], 0, sizeof(line));
        if (!fgets(line, 500, stdin))
        continue;

        if (line[0] == '\n')
        continue;

        if (!strncmp(line, "isready", 7)) {
            printf("readyok\n");
        } else if (!strncmp(line, "position", 8)) {
            setupBoard(string(line));
        } else if (!strncmp(line, "ucinewgame", 10)) {
            board.setPositionFromFEN(startFEN);
        } else if (!strncmp(line, "go", 2)) {
            go(string(line));
        } else if (!strncmp(line, "quit", 4)) {
            break;
        } else if (!strncmp(line, "evalfen", 7)) {
            int ret = board.evalFEN(string(line));
            cout<<"Evaluation: "<<ret<<endl;
            break;
        } else if (!strncmp(line, "uci", 3)) {
            printf("id name %s\n", NAME);
            printf("uciok\n");
        } else if (!strncmp(line, "debug", 4)) {
            //DebugAnalysisTest(pos,info);
        } 
		}
}

int main() {
  srand(time(0));
  
  //board.setPositionFromFEN(startFEN);
  //board.setPositionFromFEN(pos7FEN);
  //board.setPositionFromFEN(wideopenperftFEN);
  //cout<<board.perft(1)<<endl;
  //cout<<board.perft(2)<<endl;
  //cout<<board.perft(3)<<endl;
  //cout<<board.perft(4)<<endl;
  //cout<<board.perft(5)<<endl;

	char line[256];
	while (1) {
		memset(&line[0], 0, sizeof(line));
		if (!fgets(line, 256, stdin))
			continue;
		if (line[0] == '\n')
			continue;
		if (!strncmp(line, "uci",3)) {
			UciLoop();
      return -1;
    }
  }
  return 0;
}
