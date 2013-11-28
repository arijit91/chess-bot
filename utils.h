#ifndef UTILS_H_
#define UTILS_H

#include"constants.h"

string sqtostr(int);

int getManhattanDistance(int, int);
int getSq(int, int);
int getFile(int);
int getRank(int);
int getKnightVal(int);
int getPawnVal(int);
int getBishopVal(int);
int getRookVal(int);
int getQueenVal(int);
int getKingVal(int);

int opp(colour_type turn);

bool isValidSquare(int);
#endif
