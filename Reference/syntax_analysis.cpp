//通过预测分析算法对输入串进行分析

#include <iostream>
#include <map>
#include <fstream>
#include <string.h>
#include <vector>
#include "Param.h"
#include "Init.h"
#include "Set.h"
#include "Table.h"

using namespace std;

string symbol[FU_NUM];

/*
 * 创建Token类型便于后续尤其是变量的分析处理
 * Token类型对应着词法解析的结果   
 * name：token的名称
 * type：token的类型
 * content：token的内容
 */
typedef struct {
    string name;
    string type;
    string content;
}Token;

vector<Token> tokens;
/*从指定文件中读入token序列*/
bool read_token(const char *file_name) {
	ifstream source(file_name);
	if (!source.is_open())
		return false;
	vector<string> lines;
	string line;
	while (getline(source, line))
	{
		lines.push_back(line);
	}
	int len = lines.size();
	for (int i = 0; i < len; i++)
	{
		char* tokenSource = (char*)lines.at(i).c_str();	//读出一行作为一个token序列的来源
		Token token;	//拆分好的token
		char* cutChar1 = (char*)"<";
		char* cutChar2 = (char*)",";
		char* cutChar3 = (char*)">";
		char* nameAndSpace, * type, * content;
		nameAndSpace = strtok(tokenSource, cutChar1);
		string nameAndSpacee = nameAndSpace;
		token.name = nameAndSpacee.substr(0, nameAndSpacee.size() - 1);
		type = strtok(NULL, cutChar2);
		token.type = type;
		content = strtok(NULL, cutChar3);
		token.content = content;
		tokens.push_back(token);
	}
	return true;
}

void analysis() {
    //符号表下标
    int fuIndex = 1;
    cout << "######Sequence of Reduction######" << endl;
    bool readFlag = read_token("Lexical_Result.txt");
    if(!readFlag) {
        cout << "Error: Cannot read tokens from the file\n";
        return;
    }
    //给token序列加上"#"
    Token token;
    token.name = "#";
    token.type = " ";
    token.content = " ";
    tokens.push_back(token);
    //tokens序列长度
    int inNum = tokens.size();
    //读入序列下标
    int inIndex = 0;
    //符号表初始化
    symbol[0] = "#";
    symbol[1] = start;
    //num为输出归约序列序数
    int num = 1;

    while(!(inIndex == inNum - 1 && symbol[fuIndex] == "#")) {
        token = tokens[inIndex];
        string token_name = token.name;
        cout << num << "\t" << symbol[fuIndex] + "#" << token_name << "\t";
        num ++;
        //将变量等的名称改为终结符的名称，便于查找预测分析表
        if(token.type == "INT") token_name = "INT";
        if(token.type == "IDN") token_name = "Ident";
        if(token.type == "Ident") token_name = "Ident";
        if(token_name == "main" ) token_name = "Ident";
        //输入串结束
        if(inIndex == inNum - 1) {
            int i = n_ter[symbol[fuIndex]];
            int flag = 0;
            for(int j = 0; proc[i][j] != ""; j ++) {
                if(proc[i][j] == "$") flag = 1;
            }
            if(flag == 1) {
                cout << "reduction\n";
                fuIndex --;
            }
            else {
                cout << "error\n";
                break;
            }
        }
        else if(ter.count(symbol[fuIndex]) != 0) { //终结符，判断是否可以移进
            int x = ter[symbol[fuIndex]];
            int y = ter[token_name];
            if(x == y) {
                cout << "move\n";
                inIndex ++;
                fuIndex --; //相当于出栈操作
            }
            else {
                cout << "error\n";
                break;
            }
        }
        else {
            int x = n_ter[symbol[fuIndex]];
            int y = ter[token_name];
            if(table[x][y][0] == "") {
                cout << "error\n";
                break;
            }
            cout <<"reduction\n";
            if(table[x][y][2] == "$") fuIndex --;
            else {
                int k;
                for(k = 2; table[x][y][k] != ""; k ++);
                for(k = k - 1; k > 1; k --) {
                    symbol[fuIndex] = table[x][y][k];
                    fuIndex ++;
                }
                fuIndex --;
            }
        }
        //cout << fuIndex << endl;
    }
    //表示输入串可被接受
    if(inIndex == inNum - 1 && symbol[fuIndex] == "#") cout << num << "\t" << "EOF#EOF\t" <<"accept\n";
}
