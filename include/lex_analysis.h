#ifndef LEX_ANAYLYSIS_H
#define LEX_ANAYLYSIS_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <math.h>
#include <queue>
#include <stack>

using namespace std;
#define NFA_SIZE 30

// type define

typedef struct trans_map{
    char rec;
    int now;
    int next;
} transform_map;

typedef struct finite_automata{
    vector<char> input_symbols;    //可以输入的字符集
    set<int> states;   //状态集合  
    map<int, string> state_labels;   //非终态的label是描述，终态的label是输出
    // transform_map trans_map[30] = {};    //子集映射规则，A接受t到B
    set<transform_map> trans_map;    //子集映射规则，A接受t到B
    // set<pair<char,pair<int,int>>> trans_map;    //子集映射规则，A接受t到B
    set<int> start;    //初始集
    set<int> final;   //终态集
} FA;  //FA定义 五元组

// constant

map<string, string> symbols_table = {
    {"int","KW"},{"void","KW"},{"return","KW"},{"const","KW"},
    {"+","OP"},{"-","OP"},{"*","OP"},{"/","OP"},{"%","OP"},{"=","OP"},{">","OP"},
    {"<","OP"},{"==","OP"},{"<=","OP"},{">=","OP"},{"!=","OP"},{"&&","OP"},{"||","OP"},
    {"(","SE"},{")","SE"},{"{","SE"},{"}","SE"},{";","SE"},{",","SE"}
};
map<string, string> processed_symbols_table = {};

vector<char> lex_input_symbols = {'n','l','o','s','_','0','=','>','<','!','&','|','-'};
set<int> lex_states = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18};
map<int,string> lex_state_labels = {{1,"n"},{2,"l"},{3,"o"},{4,"s"},{5,"_"},{6,"0"},{7,"="},{8,">"},{9,"<"},{10,"!"},{11,"&"},{12,"|"},{13,"INT"},{14,"SE"},{15,"I&K"},{16,"OP"},{17,"none"},{18,"OP"}}; //#表示
set<int> lex_start = {17};
set<int> lex_final = {13,14,15,16,18};
// set<transform_map> lex_trans_map = {{'n',1,13},{'@',1,13},{'0',1,13},{'n',13,13},{'0',13,13},{'@',2,15},{'n',2,15},{'0',2,15},{'_',2,15},{'l',2,15},{'_',15,15},{'n',15,15},{'0',15,15},{'l',15,15},{'@',3,16},{'S',4,14},{'@',7,16},{'=',7,16},{'@',8,16},{'=',8,16},{'@',9,16},{'=',9,16},{'=',10,16},{'&',11,16},{'|',12,16},{'n',17,1},{'l',17,2},{'o',17,3},{'s',17,4},{'_',17,5},{'0',17,6},{'=',17,7},{'>',17,8},{'<',17,9},{'!',17,10},{'&',17,11},{'|',17,12}};
set<transform_map> lex_trans_map = {{'@',1,13},{'0',13,13},{'n',13,13},{'@',2,15},{'l',15,15},{'0',15,15},{'_',15,15},{'n',15,15},{'@',4,14},{'@',3,16},{'@',5,15},{'@',7,16},{'@',8,16},{'=',8,16},{'@',9,16},{'=',9,16},{'=',10,16},{'&',11,16},{'|',12,16},{'n',18,13},{'n',17,1},{'l',17,2},{'o',17,3},{'s',17,4},{'_',17,5},{'0',17,6},{'=',17,7},{'>',17,8},{'<',17,9},{'!',17,10},{'&',17,11},{'|',17,12},{'-',17,18}};

vector<char> test1_input_symbols = {'a','b'};
set<int> test1_states = {1,2,3,4,5,6,7,8};
map<int,string> test1_state_labels = {{1,"n"},{2,"l"},{3,"o"},{4,"s"},{5,"_"},{6,"0"},{7,"="},{8,">"}}; //#表示
set<int> test1_start = {8};
set<int> test1_final = {7};
set<transform_map> test1_trans_map = {{'@',8,1},{'@',1,2},{'@',5,6},{'@',6,7},{'a',1,1},{'b',1,1},{'a',2,3},{'a',3,5},{'b',2,4},{'b',4,5},{'a',6,6},{'b',6,6},};

// funtion

bool operator<(const trans_map &a,const trans_map &b);
int findNewstate(map<set<int>,int>, int);
void generateFile(vector<string>);
string preProcess(string);

void printFA(FA);
void printSet(set<int>);
void printTrans(trans_map);
void printTransSet(set<trans_map>);
void printMapForSet(map<set<int>,set<int>>);

set<int> getClosure(set<int>, FA);
set<int> getNextState(set<int>, char, FA);
char getInputChar(char);
int getBC(string);
string getCatogory(int, string, FA);

FA NFA2DFA(FA);
FA minimizeDFA(FA);
void lexcial(const char*, FA);

void lexical_analysis();

#endif