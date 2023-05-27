#ifndef CALCULATE_H
#define CALCULATE_H
#include <set>
#include <map>
#include <iostream>

using namespace std;

extern map<string, set<string>> FirstSets;
extern map<string, set<string>> FollowSets;

extern void compute_first_sets();
extern void compute_follow_sets();

extern bool is_terminal(string x);
extern bool is_non_terminal(string x);
extern bool has_empty(string x);


#endif