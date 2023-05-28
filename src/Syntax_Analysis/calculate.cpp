#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include "init.h"

using namespace std;

map<string, set<string>> FirstSets;
map<string, set<string>> FollowSets;

bool changed = true;

bool is_terminal(string x) {
    return terminals.find(x) != terminals.end();
}

bool is_non_terminal(string x) {
    return non_terminals.find(x) != non_terminals.end();
}

void print_first() {
    ofstream out("out/first_set.txt");
    if(out.is_open()) {
        out << "######FIRST SETS######"<<endl;
        for (auto const & [symbol, first] : FirstSets) {
            if (is_non_terminal(symbol)) {
                out << symbol << ": ";
                for (auto const & t : first) {
                out << t << " ";
                }
                out << endl;
            }
        }
    }
    
}

void print_follow() {
    ofstream out("out/follow_set.txt");
    if(out.is_open()) {
        out << "######FOLLOW SETS######"<<endl;
        for (auto const & [symbol, follow] : FollowSets) {
            if (is_non_terminal(symbol)) {
                out << symbol << ": ";
                for (auto const & t : follow) {
                out << t << " ";
                }
                out << endl;
            }
        }
    }
    
}

// 判断非终结符的产生式里有没有空
bool has_empty(string x) {
    if(is_non_terminal(x)) {
        const auto & rhs = prods.find(x)->second;
        for (auto const & sym : rhs) {
            if (sym == "$") return true;
        }
    return false;
    }
    else return false;
    
}

// 将集合v中的符号加到集合s
void add_symbols_without_empty (set<string> & s, set<string> & v) {
    if(!v.empty()) {
         for (auto const & t : v) {
            if (t!="$") {
                if(s.find(t) == s.end()) {
                s.insert(t);
                // cout << "INSERT " << t << endl;
                changed = true;
                }
            }
            
        }
    }
}

// 初始化FIRST集合
void init_first() {
    for(auto const & t : non_terminals) {
        FirstSets.insert(make_pair(t, set<string>{}));
    }
    for(auto const & t : terminals) {
        FirstSets.insert(make_pair(t, set<string>{t}));
    }
}

// 初始化所有非终止符的FOLLOW集合
void init_follow() {
    for(auto const & t : non_terminals) {
        FollowSets.insert(make_pair(t, set<string>{}));
    }
}

// 计算非终止符a的FIRST集合
void compute_first_set_for_nonter(string a) {
    string lhs = a;
    vector<string> rhs = prods[a];

    // 如果A->$，则$->FIRST(A)
    if(has_empty(lhs)) {    
        if(FirstSets[lhs].find("$")==FirstSets[lhs].end()) {
            FirstSets[lhs].insert("$");
            changed = true;
        }
    }

    // 开始遍历所有非空的产生式
    for (auto const & prod : rhs) {
        if (prod != "$") {
            // 用空格将右边产生式里的候选符分离出来存入tokens便于计算
            vector<string> tokens;
            string tmp = prod;

            if(tmp.find(" ")!= string::npos) {
                string space_delimiter = " ";
                size_t pos = 0;
                while ((pos = tmp.find(space_delimiter)) != string::npos) {
                    tokens.push_back(tmp.substr(0, pos));
                    tmp.erase(0, pos + space_delimiter.length());
                }
                tokens.push_back(tmp);
            }
            else {
                tokens.push_back(tmp);
            }
            
            bool flag = false;
            for(int i = 0; i < tokens.size(); i++) {
                string t = tokens[i];
                if(has_empty(t)) {
                    flag = true; // 如果A的所有候选符都能推空，则$->FIRST(A)
                }
                else {
                    flag = false;
                }

                // A->a，a->FIRST(A)
                if (is_terminal(t)) {
                    if(FirstSets[lhs].find(t)==FirstSets[lhs].end()) {
                        FirstSets[lhs].insert(t); 
                        changed = true;
                    }
                    break;
                }
                else if(is_non_terminal(t)){
                    // A->B， FIRST(B)\{$}->FIRST(A)
                    add_symbols_without_empty(FirstSets[lhs], FirstSets[t]);
                    if(has_empty(t)) continue; // 能推空，继续分析下一个候选符，否则停止分析，终止循环
                    else break;
                }
            }

            // 如果A的所有候选符都能推空，则$->FIRST(A)
            if(flag && (FirstSets[lhs].find("$")==FirstSets[lhs].end())) {
                FirstSets[lhs].insert("$");
                changed = true;
            }
        }
    }
}

// 计算非终止符a的FOLLOW集合
void compute_follow_set_for_nonter(string a) {
    string lhs = a;
    vector<string> rhs = prods[a];

    for (auto const & prod : rhs) {
        
        if (prod != "$") {
            vector<string> tokens;
            string tmp = prod;

            if(tmp.find(" ")!= string::npos) {
                string space_delimiter = " ";
                size_t pos = 0;
                while ((pos = tmp.find(space_delimiter)) != string::npos) {
                    tokens.push_back(tmp.substr(0, pos));
                    tmp.erase(0, pos + space_delimiter.length());
                }
                tokens.push_back(tmp);
            }
            else {
                tokens.push_back(tmp);
            }

            for(int i = 0; i<tokens.size();i++) {
                string t = tokens[i];
                if(is_non_terminal(t)){
                    if(i!=tokens.size()-1) { // 没到句尾部

                        if(is_terminal(tokens[i+1])) {
                            if(FollowSets[t].find(tokens[i+1])==FollowSets[t].end()) {
                                FollowSets[t].insert(tokens[i+1]);
                                changed = true;
                            }
                        }
                        else if(is_non_terminal(tokens[i+1])) {
                            add_symbols_without_empty(FollowSets[t], FirstSets[tokens[i+1]]);
                            if(has_empty(tokens[i+1])) {
                                add_symbols_without_empty(FollowSets[t], FollowSets[lhs]);
                            }
                        }
                    }
                    else { // 尾部
                        add_symbols_without_empty(FollowSets[t], FollowSets[lhs]);
                        // 尾部是否要加#?
                    }

                }
                else continue;
            }
        }
    }
}

// 计算所有符号的FIRST集合
void compute_first_sets() {

        // 1 X是终结符，则X->FIRST(X)
        for(auto const & t : terminals) {
            FirstSets.insert(make_pair(t, set<string>{t}));
        }

        // 初始化所有非终结符的FIRST集合
        init_first();

        while(changed) {
            changed = false;

            for(auto const & p : prods) {
                string  lhs = p.first;
                vector<string> rhs = p.second;

                compute_first_set_for_nonter(lhs);
            }
        }
    
    print_first();
    changed = true;
}

// 计算所有符号的FOLLOW集合
void compute_follow_sets(){
    init_follow();
    FollowSets[start].insert("#");

    while(changed) {
        changed = false;

        for(auto const & p : prods) {
            string  lhs = p.first;
            vector<string> rhs = p.second;

            compute_follow_set_for_nonter(lhs);
        }
    }

    print_follow();
    changed = true;

}
