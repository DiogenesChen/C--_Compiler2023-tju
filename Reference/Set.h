#ifndef SET_H
#define SET_H
#include <set>
#include <iostream>
#include "Param.h"

using namespace std;

extern set<string> follow_set[NONTER_NUM];
extern set<string> first_set[NONTER_NUM];

extern void firstSet();
extern void followSet();

#endif

