#ifndef LEXICAL_H
#define LEXICAL_H
#include <vector>
#include <iostream>
using std::vector;
using std::string;

int NFA2DFA(int start, vector<int> end);
void min_DFA(int num);
void Lexical_analysis(const char* address);

#endif