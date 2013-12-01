#include<iostream>
#include<string>
#include<vector>


#include "game.h"

using namespace std;

Game::Game() {
  this->plies = 0;
  this->reward = 0;
}

Game::Game(int plies, int reward, vector<string> Movelist) {
  this->plies = plies;
  this->reward = reward;
  for(int i=0; i<plies; i++)
    this->Movelist.push_back(Movelist[i]);
}

int Game::getPlies() {
  return plies;
}

float Game::getReward() {
  return reward;
}

string Game::getPosition(int i) {
 return Movelist[i];
}
