#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <unordered_map>
#include <iomanip>
#include "include/init.h"
#include "include/calculate.h"
#include "include/table.h"

using namespace std;

vector<string> tokens;
vector<string> sym_stack;
set<string> type_sets;

void get_tokens(const char *file_name) {
    ifstream in(file_name);
    string line;

    cout << "######LEXCIAL TOKENS######" << endl;


    if(in) {
        while (getline(in, line))
        {
            cout << line << endl;
            size_t pos = line.find("	");
            string token = line.substr(0, pos);

            size_t begin_index = line.find("<");
            size_t end_index = line.find(">");

            string type = line.substr(begin_index+1, end_index-begin_index-1);
            if(type_sets.find(type)==type_sets.end()) {
                type_sets.insert(type);
            }

            if(is_terminal(token)) {
                tokens.push_back(token);
            }
            else {
                tokens.push_back(type);
            }

        }

        tokens.push_back("#");
        
    }
    else {
        cout << "Error! No Such File!" << endl;
    }
}

void grammar_analysis() {
    // # 和 开始符入栈
    sym_stack.push_back("#");
    sym_stack.push_back(start);
    int index = 0;
    string a = tokens[index];
    bool flag = true;
    
    cout << "######GRAMMAR ANALYSIS START!#######" << endl;

    while(flag) {
        string X = sym_stack.back();

        if(X=="#") {
            if(X==a) {
                cout << "EOF#EOF" << "  " << "accept" << endl;
                flag = false;
                break;
            }
            else {
                cout << X << "#" << a << "  " << "error" << endl;
                flag = false;
                break;
            }
        }
        else if(is_terminal(X)) {
            if(X==a) {
                cout << X << "#"  << a << "     " << "move" << endl;
                if(index < tokens.size()) {
                    index++;
                    a = tokens[index];
                    sym_stack.pop_back();
                }
                else {
                    cout << "Error! Wrong index size!" << endl;
                    flag = false;
                    break;
                }
            }
            else if(X!=a) {
                if(a=="#"){
                    cout << X << "#EOF  " << "error" << endl;
                }
                else {
                    cout << X << "#" << a << "  " << "error" << endl;
                }
                
                flag = false;
                break;
            }
            
        }
        else if(is_non_terminal(X)) {
            vector<string> tmp;
            tmp.push_back(X);
            tmp.push_back(a);

            if(!table[tmp].empty()) { // 分析表里有表达式
                if (find(table[tmp].begin(), table[tmp].end(), "$")!=table[tmp].end()) { // 是推空
                    sym_stack.pop_back();
                    if(a=="#") {
                        cout << X << "#"  <<"EOF" << "     " << "reduction" << endl;
                    }
                    else {
                        cout << X << "#"  << a << "     " << "reduction" << endl;
                    }
                    
                }
                else { // 有表达式，X出栈，表达式逆序进栈
                    sym_stack.pop_back();
                    vector<string> rev = table[tmp];
                    reverse(rev.begin(), rev.end());
                    for(const auto& t : rev) {
                        sym_stack.push_back(t);
                    }
                    if(a=="#") {
                        cout << X << "#"  <<"EOF" << "     " << "reduction" << endl;
                    }
                    else {
                        cout << X << "#"  << a << "     " << "reduction" << endl;
                    }
                }
                
            }
            else { // 分析表为空
                if(a=="#") {
                    cout << X << "#"  <<"EOF" << "     " << "error" << endl;
                }
                else {
                    cout << X << "#"  << a << "     " << "error" << endl;
                }
                flag = false;
                break;
            }

        }
        
    }
}