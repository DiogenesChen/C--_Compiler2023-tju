#ifndef INIT_H
#define INIT_H
#include <iostream>
#include <map>
#include "Param.h"

using std::string;
using std::map;

extern string start;					    //开始符
extern int count;					        //产生式个数
extern string proc[PROC_NUM][PROC_NUM];    //产生式
extern string non_ter[NONTER_NUM];		    //非终结符
extern string termin[TERMIN_NUM];		    //终结符

extern map<string, int> n_ter;
extern map<string, int> ter;

extern void Init(const char *file_name);

#endif