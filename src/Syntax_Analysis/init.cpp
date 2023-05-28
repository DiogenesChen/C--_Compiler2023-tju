// 处理文法，并获取文法产生式、非终结符、终结符

#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <map>

using namespace std;

string start; // 开始符
int proc_num; // 产生式个数
set<string> non_terminals;  // 存储非终结符
set<string> terminals;  // 存储终结符

typedef map<string, vector<string>> Productions; // 非终结符和对应的产生式集合的映射
Productions prods;
Productions temp;


// void elimate_left_recursion() { // 消除直接左递归
//     for (auto const& [lhs, rhs] : prods) {
//         for (auto const& prod : rhs) {
//             size_t pos = prod.find(" ");
//             string first_token;
//             if(pos!=string::npos) {
//                 first_token = prod.substr(0, pos); 
//             }
//             else {
//                 first_token = prod;
//             }

//             if(lhs == first_token) { // 存在直接左递归 A→Aα|β
//                 string prime = lhs + "'"; // 新的非终止符 A'
//                 // A→βA'
//                 for(auto const & n_prod : prods[lhs]) {
//                     if(is_terminal(n_prod)) {
//                         string new_line = n_prod + prime;
//                         prods[lhs].push_back(new_line);
//                     }
//                 }
                
                
//                 // A'→αA'|ε
//                 string last = prod.substr(pos+first_token.size());
//                 last = last + " " + prime; // αA'

//                 // A'→αA'|ε
//                 prods.insert(make_pair(prime, vector<string>{last}));
//                 prods[prime].push_back("$");

//                 // 

//             }
//         }
//     }

//      for (auto const& [lhs, rhs] : prods) {
//         for (auto const& prod : rhs) {
//             size_t pos = prod.find(" ");
//             string first_token;
//             if(pos!=string::npos) {
//                 first_token = prod.substr(0, pos); 
//             }
//             else {
//                 first_token = prod;
//             }

//             if(lhs == first_token) { // 存在直接左递归 A→Aα|β
//                 auto it = find(prods[lhs].begin(), prods[lhs].end(), prod);
//                 while (it != prods[lhs].end()) {
//                 prods[lhs].erase(it);
//             }
//         }
//     }
//      }
// }



int Init(const char *file_name) {
    // 读取原文法文件，将每个非终结符和对应的产生式存入映射中
    ifstream in(file_name);
    string line;

    if(in) {
        while(getline(in, line)) {
            size_t pos = line.find(" -> ");
            string lhs = line.substr(0, pos); // 左部，非终止符
            string rhs = line.substr(pos+4); // 右部

            if(prods.empty()) {
                start = lhs;
            }

            if(prods.count(lhs)) {
                // 如果非终结符在映射中已经存在，则添加新的产生式
                prods[lhs].push_back(rhs);
            }
            else {
                // 添加新的键值对
                prods.insert(make_pair(lhs, vector<string>{rhs}));
            }
        }

        // 合并相同非终结符的产生式，用 | 隔开，并将处理结果输出到文本文件grammar_result.txt
        ofstream out("out/grammar_result.txt");

        if (out.is_open()) {
            for (auto it = prods.begin(); it!=prods.end(); it++) {
            string lhs = it->first;
            vector<string> rhs = it->second;
            out << lhs << " -> ";
            for (size_t i = 0; i < rhs.size(); i++) {
                out << rhs[i];
                if (i < rhs.size() - 1) {
                    out << " | ";
                }
            }
            out << endl;
            }
        }

        // 统计终结符与非终结符
        for (auto const & [lhs, rhs] : prods) {
            non_terminals.insert(lhs); // 将所有左侧符号放入非终结符集合
        }

        // 没出现在左侧的符号就是终结符
        for (auto const & [lhs, rhs] : prods) {
            for (auto const & prod : rhs) {
                if(prod.find(" ")!= string::npos) {
                    vector<string> tokens;
                    string tmp = prod;
                    string space_delimiter = " ";
                    size_t pos = 0;
                    while ((pos = tmp.find(space_delimiter)) != string::npos) {
                        tokens.push_back(tmp.substr(0, pos));
                        tmp.erase(0, pos + space_delimiter.length());
                    }
                    tokens.push_back(tmp);

                    for(int i = 0; i < tokens.size(); i++) {
                        string t = tokens[i];
                        if(non_terminals.find(t) == non_terminals.end()) {
                            terminals.insert(t);
                        }
                    }
                }
                else {
                    string tmp = prod;
                    if(non_terminals.find(tmp) == non_terminals.end() && tmp != "$") {
                            terminals.insert(tmp);
                        }
                }
            }
        }

        

    }
    else
	{
		cout << "Error! No Such File!" << endl;
	}

    proc_num = non_terminals.size(); // 产生式个数
    return 0;
}


