#include<iostream>
#include<fstream>
#include<cassert>
#include<string>
#include<cstdio>
#include<cstring>
#include<ctime>
#include<cstdlib>
#include<math.h>
#include<sstream>

#include"board.h"
#include"game.h"
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
bool quit_game = false;

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
    printf("id author %s\n", AUTHOR);
    printf("uciok\n");
	
	int MB = 64;

  //fstream of("of", ios::out);

	while (1) {
		memset(&line[0], 0, sizeof(line));
        if (!fgets(line, 500, stdin))
        continue;

        if (line[0] == '\n')
        continue;

        //of.write(line, 500);
        //of.flush();

        if (!strncmp(line, "isready", 7)) {
            printf("readyok\n");
        } 
        else if (!strncmp(line, "position startpos moves", 23)) {
            board.addMovesToPosition(startFEN, string(line));
        }
        else if (!strncmp(line, "position startpos", 17)) {
            board.setPositionFromFEN(startFEN);
        }
        else if (!strncmp(line, "position", 8)) {
            setupBoard(string(line));
        } else if (!strncmp(line, "ucinewgame", 10)) {
            board.setPositionFromFEN(startFEN);
        } else if (!strncmp(line, "go", 2)) {
            go(string(line));
        } else if (!strncmp(line, "makemove", 8)) {
            board.makeInputMove(string(line).substr(9));
        } else if (!strncmp(line, "quit", 4)) {
            quit_game = true;
            break;
        } else if (!strncmp(line, "print", 5)) {
            board.printBoard();
        }
        else if (!strncmp(line, "evalfen", 7)) {
            int ret = board.evalFEN(string(line));
            printf("Evaluation: %d\n", ret);
        } else if (!strncmp(line, "eval", 2)) {
            int ret = board.evaluate();
            printf("Evaluation: %d\n", ret);
        } else if (!strncmp(line, "uci", 3)) {
            printf("id name %s\n", NAME);
            printf("id author %s\n", AUTHOR);
            printf("uciok\n");
        } 
        if (quit_game) break;
		}
}

void tdUpdate(Board state, Board nextState, float reward, float wt[], float delV[], float eta, bool gameover) {
  int numFeatures = 5;
  float r=0;
  float features[] = {0,0,0,0,0};
  float V = state.evaluationFunction(wt);
  float value = 0;
 
  //Check if game is over
  if(gameover)
    r = reward - V;
  else
    r = reward + nextState.evaluationFunction(wt) - V;
  
  //cout<<"reward : "<<reward<<endl;
  //cout<<"V : "<<V<<endl;
  //cout<<"r : "<<r<<endl;
  
  state.extractFeatures(features);
  
  //cout<<state.getTurn();
  //cout<<" : ";
  
  for(int i=0; i<numFeatures; i++) {
    value += wt[i]*features[i];
    //cout<<features[i];
  }
  
//  cout<<"  w.f : "<<value<<endl;
//  cout<<" delV : ";
  
  //float delV[] = {0.0,0.0,0.0,0.0,0.0};
  for(int i=0; i<numFeatures; i++) {
    delV[i] = features[i]*exp(-1*value)*(V*V);
    //delV[i] += features[i]*V*(1-V);
    //cout<<delV[i]<<" ";
  }
  //cout<<endl;

  for(int i=0; i<numFeatures; i++)
    wt[i] += eta*r*delV[i];
}

vector<Game> extractGames(int numGames) {
  string data_path = "/home/aparna/quarter1/ai/chess-bot/games/";
  float reward;
  int plies;
  vector<Game> games;
  for(int i=0; i<numGames; i++) {
    
    vector<string> moveList;
    ifstream fin;
    ostringstream ss;
    ss<< (2*i);
    string filename = data_path + ss.str();
    const char* fname = filename.c_str();
    fin.open(fname);
    fin>>reward;
    fin>>plies;
    for(int j=0; j<=plies; j++) {
      char move[200];
      fin.getline(move, 200);
      if(j!=0)
        moveList.push_back(move);
    }
    fin.close();
    Game g(plies, reward, moveList);
    games.push_back(g);
    
  }
  return games;
}

void train() {
   int numGames = 1575;
   //int numGames = 2;
   vector<Game> games;
   float wt[] = {0.0,0.0,0.0,0.0,0.0};
   float eta = 1;
   games = extractGames(numGames);
   
   for(int g=0; g<numGames; g++) {
     Board state, nextState;
     float reward = games[g].getReward();
     //if(reward==0)
     //  reward=0.5;
     //else if(reward==-1)
     //  reward = 0;
     int plies = games[g].getPlies();
     float delV[] = {0.0,0.0,0.0,0.0,0.0};
     for(int i=0; i<plies-1; i++) {
       board.setPositionFromFEN(games[g].getPosition(i));
       state = board;
       board.setPositionFromFEN(games[g].getPosition(i+1));
       nextState = board;
       tdUpdate(state, nextState, 0, wt, delV, eta, 0);
     }
     //if(plies-1 >= 0) {
       board.setPositionFromFEN(games[g].getPosition(plies-1));
       tdUpdate(board, board, reward, wt, delV, eta, 1);
     //}

    //for(int i=0; i<5; i++)
    // cout<<wt[i]<<" ";

    //cout<<"-------------------------------------"<<endl;
   }
   for(int i=0; i<5; i++)
       cout<<wt[i]<<" ";

   }


int main() {
  train();
  
  /*  
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
  */
  return 0;
}
