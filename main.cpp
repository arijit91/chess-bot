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

  printf("id name %s\n", NAME);
  printf("uciok\n");

  char line[500];

  while (1) {
    memset(&line[0], 0, sizeof(line));
    fflush(stdout);
    if (!fgets(line, 500, stdin))
      continue;

    if (line[0] == '\n')
      continue;

    if (!strncmp(line, "isready", 7)) {
      printf("readyok\n");
    } else if (!strncmp(line, "ucinewgame", 10)) {
      //ParsePosition("position startpos\n", pos);
      b.setPositionFromFEN(startFEN);
    } else if (!strncmp(line, "go", 2)) {
      //ParseGo(line, info, pos);
      //string s = b.getMove();
      //printf("readyok\n");
      //printf("info depth 2\n");
      printf("bestmove e2e4\n");
      fflush(stdout);
      //printf("bestmove %s\n", s.c_str());
    } else if (!strncmp(line, "uci", 3)) {
      printf("id name %s\n", NAME);
      printf("uciok\n");
    }
  }

}

int main() {
  UciLoop();
  return 0;
}
