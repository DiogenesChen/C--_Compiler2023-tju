//获取文法产生式、非终结符、终结符

#include <iostream>
#include <fstream>
#include <string.h>
#include <algorithm>
#include <set>
#include <map>
#include "Param.h"

using std::set;
using std::map;
using std::cout;
using std::string;
using std::ifstream;
using std::getline;

string start;					    //开始符
int count;					        //产生式个数
string proc[PROC_NUM][PROC_NUM];    //产生式
string non_ter[NONTER_NUM];		    //非终结符
string termin[TERMIN_NUM];		    //终结符

map<string, int> n_ter; 			//<非终结符,在非终结符数组中下标>
map<string, int> ter; 				//<终结符，在终结符数组中下标>

int check(string *s, string str, int index);

//判断字符数组s是否包含字符串str
int check(string *s, string str, int index)
{
	for (int i = index; i < count; i++)
	{	
		if (s[i]==str)
		{
			return i; //返回下标
		}
	}
	return -1;
}

//读取文法文件并进行初始化操作
void Init(const char *file_name) {
    count = 0;
	ifstream in(file_name);
	string filename;
	string line;
	if (in)
	{
		//while按行读入产生式，记录开始符号，产生式中->左侧的符号为非终结符，将所有符号放到一个数组里
		while (getline(in, line))
		{
			string word[WORD_NUM];
			int cnt = 0;
			char *charline = new char[line.size() + 1];
			//将string line转化为char* charline
			strcpy(charline, line.c_str()); 
			char *wordtmp = strtok(charline, " ");
			if(wordtmp){
				word[0] = wordtmp;
				cnt++;
			}
			while (wordtmp = strtok(NULL, " "))
			{
				word[cnt++] = wordtmp;
			}
			if (count == 0)
			{
				start = word[0];
			}
			non_ter[count] = word[0];
			for (int j = 0; j < WORD_NUM && word[WORD_NUM] != ""; j++)
			{
				proc[count][j] = word[j];
			}
			count++;
		}
		int num = 0;
		//数组里不是非终极符的为终结符
		for (int i = 0; i < count; i++)
		{
			for (int j = 0; j < PROC_NUM && proc[i][j] != ""; j++)
			{
				if (check(non_ter, proc[i][j], 0) == -1 && proc[i][j] != "->" && proc[i][j] != "$" && proc[i][j] != "|")
				{
					termin[num++] = proc[i][j];
				}
			}
		}
		//终结符去重
		set<string> termin_set;
		//set<string> non_termin_set;
		for(int i=0; i<TERMIN_NUM && termin[i]!=""; i++){
			termin_set.insert(termin[i]);
		}
		for(int i=0; i<TERMIN_NUM && termin[i]!=""; i++){
			termin[i]="";
		}
		std::set<string>::iterator ite;
		int cnt=0, ncnt=0;
		for(ite=termin_set.begin(); ite!=termin_set.end(); ite++){
			termin[cnt++] = *ite;
		}
        for(int i=0; i<NONTER_NUM && non_ter[i]!=""; i++){
            n_ter.insert({non_ter[i], i});
        }
        for(int i=0; i<TERMIN_NUM && termin[i]!=""; i++){
            ter.insert({termin[i], i});
        }
	}
	else
	{
		cout << "Error! No Such File!\n";
	}
}
