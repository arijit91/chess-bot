#include<iostream>
#include<cassert>
#include<string>
#include<cstdio>
#include<cstring>
#include<cstdlib>

#include"constants.h"
#include"board.h"
using namespace std;

string startFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

Board b;

void setupBoard(string s) {
  assert(s.substr(0, 13) == "position fen ");
  string fen = s.substr(14);
  b.setPositionFromFEN(fen);
  return ;
}

void go(string line) {
  printf("bestmove %s\n", b.getMove().c_str());
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
        } else if (!strncmp(line, "go", 2)) {
            go(string(line));
        } else if (!strncmp(line, "quit", 4)) {
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
