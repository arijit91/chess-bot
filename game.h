#ifndef GAME_H
#define GAME_H

#include<string>
#include<vector>

using namespace std;

class Game {
  int plies;
  int reward;
  vector<string> Movelist;

  public:
    Game();
    Game(int, int, vector<string>);
    int getPlies();
    float getReward();
    string getPosition(int);
};
#endif
