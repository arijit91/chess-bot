#include<iostream>
#include<string>
#include<cstdio>
#include<cstring>
#include<cstdlib>

#include"constants.h"
#include"board.h"
using namespace std;

string startFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

Board b;

void UciLoop() {
	setbuf(stdin, NULL);
  setbuf(stdout, NULL);

	char line[500];
    printf("id name %s\n", "balrog");
    printf("id author temp\n");
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
            //ParsePosition(line, pos);
        } else if (!strncmp(line, "ucinewgame", 10)) {
            //ParsePosition("position startpos\n", pos);
        } else if (!strncmp(line, "go", 2)) {
            //printf("Seen Go..\n");
            printf("bestmove h2h4\n");
            //ParseGo(line, info, pos);
        } else if (!strncmp(line, "quit", 4)) {
            break;
        } else if (!strncmp(line, "uci", 3)) {
            printf("id name %s\n",NAME);
            printf("id author Bluefever\n");
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
