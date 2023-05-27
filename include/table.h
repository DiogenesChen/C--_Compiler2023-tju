#ifndef TABLE_H
#define TABLE_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include "init.h"
#include "calculate.h"
using namespace std;

extern map<string, set<string>> select_sets; 
extern map<vector<string>, vector<string>> table;     // 分析表：<<id, E>, <T, E1>>

extern void print_select();
extern void compute_select_sets();
extern void compute_table();
extern void analysis_table();
extern void print_ana_table();

#endif