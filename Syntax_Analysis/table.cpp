// 生成LL(1)分析表
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <unordered_map>
#include <iomanip>
#include "init.h"
#include "calculate.h"

using namespace std;

typedef map<string, string> Production;        // 单条产生式
map<vector<string>, set<string>> select_sets;  // <non_ter, prod, vector<ter>>
map<vector<string>, vector<string>> table;     // 分析表：<<id, E>, <T, E1>>

void add_symbols(set<string> & s, set<string> & v, int i) {
    if(!v.empty()) {
        for (auto const & t : v) {
            if(i) {
                if (t!="$") {
                    if(s.find(t) == s.end()) {
                    s.insert(t);
                    }
                }
            }
            else {
                if(s.find(t) == s.end()) {
                    s.insert(t);
                }
            }
        }
    }
}

void print_select() {
    for(auto const& [lhs, rhs] : prods) {
        for(auto const& prod : rhs) {
            vector<string> proc;
            proc.push_back(lhs);
            proc.push_back(prod);

            cout << "SELECT(" << lhs << " -> " << prod << ") = { ";
            for(auto const& sym : select_sets[proc]) {
                cout << sym << " ";
            }
            cout << "}" << endl;
        }
    }
}


// 计算select集合，便于求分析表
void compute_select_sets() {
    // cout << "START SELCET: " << endl;
    for(auto const& [lhs, rhs] : prods) {
        // cout << "START lhs :" << lhs <<endl;

        for(auto const& prod: rhs) {
            vector<string> proc;
            proc.push_back(lhs);
            proc.push_back(prod);
           select_sets.insert(make_pair(proc, set<string>{}));

           string first_token;
           size_t pos = prod.find(" ");
           first_token = (pos==string::npos) ? prod : prod.substr(0, pos);

           if(first_token=="$") {
                add_symbols(select_sets[proc], FollowSets[lhs], 0);
           }
           else if (is_terminal(first_token)) {
                select_sets[proc].insert(first_token);
           } 
           else if (has_empty(first_token)) {
                add_symbols(select_sets[proc], FirstSets[first_token], 1);
                add_symbols(select_sets[proc], FollowSets[lhs], 0);
           }
           else if (!has_empty(first_token)) {
                add_symbols(select_sets[proc], FirstSets[first_token], 0);
           }
           
        }
    }
}

void compute_table() {
    cout << "######LL(1) ANALYSIS TABLE######" << endl;
    terminals.insert("#");
    cout << setw((terminals.size()*40 + 20)) << left << setfill('=') << "TABLE:" << endl;
    cout << setw(30) << left << setfill(' ') << "VN/VT";
    for (auto const & ter : terminals) {
        if (ter != "$") {
            cout << setw(40) << left << ter << setfill(' ');
        }
        
    }
    cout << endl;

    for(auto const& non_ter : non_terminals) { 
        cout << setw(30) << left  << non_ter << setfill(' ');
        

        for(auto const& ter : terminals) {
            if (ter != "$") {
                bool flag = 0;

                for(auto const& prod : prods[non_ter]) {
                    vector<string> proc;
                    proc.push_back(non_ter);
                    proc.push_back(prod);

                    if(select_sets[proc].find(ter)!=select_sets[proc].end()) {
                        // string tmp = non_ter + " -> " + prod;
                        cout << setw(40) << left << prod << setfill(' ') ;
                        flag = 1;
                    }
                }

                if(!flag) cout << setw(40) << left << "ERR" << setfill(' ');
            }
            
        }
        
        
        cout << endl; 
    }
    
}

void analysis_table() {
    for(auto const non_ter : non_terminals) {
        for(auto const prod : prods[non_ter]) {
            vector<string> proc;
            proc.push_back(non_ter);
            proc.push_back(prod);

            // 将产生式按照空格分割为单个token
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

            for(auto const ter : terminals) {
                vector<string> sets;
                sets.push_back(non_ter);
                sets.push_back(ter);
                table.insert(make_pair(sets, vector<string>{}));

                if(select_sets[proc].find(ter)!=select_sets[proc].end()) {
                    for(auto const & t : tokens) {
                        table[sets].push_back(t);
                    }
                }
            }
        }
    }
}

void print_ana_table() {
    for(auto const & non_ter : non_terminals) {
        for(auto const & ter : terminals) {
            vector<string> sets;
            sets.push_back(non_ter);
            sets.push_back(ter);

            cout << "(" << non_ter << ", " << ter << ") = { ";
            for(auto const & t : table[sets]) {
                cout << t << " ";
            }
            cout << " }" << endl;
        }
    }
}
