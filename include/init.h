#ifndef INIT_H
#define INIT_H
#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <map>

using namespace std;

extern set<string> non_terminals;  // 存储非终结符
extern set<string> terminals;      // 存储终结符
extern int proc_num;               // 产生式个数（一整行？）

extern map<string, vector<string>> prods; // 非终结符和对应的产生式集合的映射
extern string start; // 开始符

extern void Init(const char *file_name);

#endif