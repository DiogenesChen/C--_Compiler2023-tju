//生成First集合和Follow集合

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <set>
#include "Init.h"

using std::set;
using std::cout;
using std::endl;

set<string> follow_set[NONTER_NUM];
set<string> first_set[NONTER_NUM];

string* pro(string* s);
void pri(string s[][NONTER_NUM]);
int getIndex(string s[], string str);
string* copy(string s1[], string s2[]);
string* pro(string s[]);
string* settoStr(set<string> set);
bool contains(set<string> sett, string str);
void convert(string s[][NONTER_NUM]); //将二维string数组转换为set
void print_follow();

//输出first集合
void pri(string s[][NONTER_NUM]){
    cout << "######FirstSet Result######" << endl;
	for(int i=0; i<count; i++){
        cout<<non_ter[i]+ ": ";
        for(int j=0; j<NONTER_NUM && s[i][j]!=""; j++){
            cout<<s[i][j]+" ";
        }
        cout<<endl;
    }
    cout<<endl;
}

//获得字符串在数组中的下标
int getIndex(string s[], string str){
	for(int i=0; s[i]!=""; i++){
        if(s[i]==str){	
            return i;
        }
    }
	return -1;
}

//将set变为string数组
string* settoStr(set<string> sett){
    std::set<string>::iterator ite;
    string* s = new string[STR_NUM];
    int num = 0;
    for(ite = sett.begin(); ite != sett.end(); ite ++){
        s[num++] = *ite;
    }
	return s;
} 

//获得first集合
void firstSet(){
    string first[FIRST_NUM][FIRST_NUM];
    set<string> non_terset, terminset;
    for(int i=0; i<NONTER_NUM && non_ter[i]!=""; i++){
        non_terset.insert(non_ter[i]);
    }
    for(int i=0; i<TERMIN_NUM && termin[i]!=""; i++){
        terminset.insert(termin[i]);
    }
    /* 
     * judge1值,judge2值分别表示first集合初始个数以及一遍算法结束后first集合个数
     * 根据judge1值和judge2值比较结果，判断算法是否结束
     */
    //两个int型变量
    int judge1 = 1, judge2 = -1;
    int id[count]={0}; 
    //id[i]为第i个非终结符的first集个数
    while(judge1 != judge2){
        //获取初始judge1的值
        judge1 = 0;
        for(int i=0; i<count; i++){
            for(int j=0; j<FIRST_NUM && first[i][j]!=""; j++){
                judge1++;
            }
        }
        //枚举产生式
        for(int i=0; i<count; i++){
            for(int j=0; j<PROC_NUM && proc[i][j]!=""; j++){
				if(proc[i][j]=="->" || proc[i][j]=="|"){
                    //X为终结符
                    if(terminset.find(proc[i][j + 1])!=terminset.end()){
						first[i][id[i]++] = proc[i][j + 1];
                    }
                    //X为非终结符且能推出推出空字符$
                    else if(proc[i][j + 1]=="$" && (proc[i][j + 2] == "" || proc[i][j + 2]=="|")){
                        first[i][id[i]++] = proc[i][j + 1];
                    }
                    //X为非终结符
                    else{
                        //X能推出以非终结符开头
                        if(non_terset.find(proc[i][j + 1])!=non_terset.end()){
                        	int index = getIndex(non_ter, proc[i][j + 1]);
							int d;
							for(int c=0; c<FIRST_NUM && first[index][c]!=""; c++){
								int flag=false;
								for(d=0; d<FIRST_NUM && first[i][d]!=""; d++){
									if(first[i][d]==first[index][c]){
										flag=true;
										break;
									}	
								}
								if(!flag && first[index][c]!="$"){
									first[i][d]=first[index][c];
								}
							}
							int c3;
							for(c3=0; c3<FIRST_NUM && first[i][c3]!=""; c3++);
							id[i]=c3;
                        }
                        for(int k=j+1; k<PROC_NUM && proc[i][k]!=""; k++){
							bool haveend = false;
							int index = getIndex(non_ter, proc[i][k]);
							for(int n=0; n<FIRST_NUM && first[index][n]!=""; n++){
								if(first[index][n]=="$")
									haveend = true;
							}
							if(non_terset.find(proc[i][k])!=non_terset.end() && haveend){
								if(proc[i][k + 1]=="" || proc[i][k + 1]=="|"){
									int c3;
									for(c3=0; c3<FIRST_NUM && first[i][c3]!=""; c3++);
									id[i]=c3;
                                }
							}
                            else if(proc[i][k]=="|")
                                break;
                            else{
                                if(non_terset.find(proc[i][k])!=non_terset.end()) {
									int f;
									for(int e=0; e<FIRST_NUM && first[index][e]!=""; e++){
										int flag = false;
										for(f=0; f<FIRST_NUM && first[i][f]!=""; f++){
											if(first[i][f]==first[index][e]){
												flag = true;
												break;
											}
										}
										if(!flag && first[index][e]!="$")
											first[i][f]=first[index][e];
									}
									int c3;
									for(c3=0; c3<FIRST_NUM && first[i][c3]!=""; c3++);
									id[i]=c3;
									break;
                                }
                                else {
                                    first[i][id[i]++] = proc[i][k];
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
		for(int i=0; i<count; i++){
			//first集合去重
			set<string> firstset;	
            set<string>::iterator ite;		
            int cnt=0;
            for(int j=0; j<FIRST_NUM && first[i][j]!=""; j++){
				firstset.insert(first[i][j]);
				first[i][j]="";
			}
			for(ite=firstset.begin(); ite!=firstset.end(); ite++){
				first[i][cnt++] = *ite;
			}
            id[i] = cnt;
        }
        judge2 = 0;
        for (int i = 0; i < count; i++) {
            for (int j = 0; j < FIRST_NUM && first[i][j] != ""; j++) {
                judge2++;
            }
        }
    }
	pri(first);
    convert(first);
}

/*
 *@author hll
 *@date 2022/11/08
 */

//判断字符串集合sett中是否包含字符串str
bool contains(set<string> sett, string str) {
	if(sett.find(str) != sett.end()) return true;
	return false;
}

//将first二维字符串数组转换为集合
void convert(string s[][NONTER_NUM]) {
    for(int i = 0; i < count; i ++){
        for(int j = 0; j < NONTER_NUM && s[i][j] != ""; j ++){
            first_set[i].insert(s[i][j]);
        }
    }
}

//打印follow集合
void print_follow() {
    cout << "######FollowSet Result######" << endl;
    for(int i = 0; i < count; i ++){
        cout << non_ter[i] + ": ";
        set<string>::iterator ite;
        for(ite = follow_set[i].begin(); ite != follow_set[i].end(); ite ++) {
            cout << *ite + " ";
        }
        cout <<endl;
    }
}

//获取followSet
void followSet() {
    set<string> non_terset, terminset, tmp_set;
    for(int i = 0; i < NONTER_NUM && non_ter[i] != ""; i ++) {
        non_terset.insert(non_ter[i]);
    }
    for(int i = 0; i < TERMIN_NUM && termin[i] != ""; i ++){
        terminset.insert(termin[i]);
    }
    //judge1为初始follow集大小，judge2为一遍算法过后follow集大小
    int judge1 = 1, judge2 = -1, index_first = 0, index_follow;

    while(judge1 != judge2) {
        judge1 = 0;
        for(int i = 0; i < count; i ++){
            judge1 += follow_set[i].size();
        }

        //将"#"加入到开始符号中
        follow_set[getIndex(non_ter, start)].insert("#");

        for(int i = 0; i < count; i ++) {
            for(int j = 0; j < PROC_NUM && proc[i][j] != ""; j ++) {
                if(proc[i][j] == "->" || proc[i][j] == "|") {
                    for(int k = j + 1; k < PROC_NUM && proc[i][k] != ""; k ++) {
                        if(proc[i][k] == "|") break;
                        //当前为非终结符，对其进行Follow集计算
                        if(contains(non_terset, proc[i][k])) {
                            //判断当前非终结符在此产生式后是否还有符号
                            if(k + 1 < PROC_NUM && proc[i][k + 1] != "" && proc[i][k + 1] != "|") {
                                //其后为非终结符
                                if(contains(non_terset, proc[i][k + 1])) {
                                    index_first = getIndex(non_ter, proc[i][k + 1]);
                                    index_follow = getIndex(non_ter, proc[i][k]);
                                    tmp_set = first_set[index_first];
                                    //first集合不包含空字符"$"
                                    if(!contains(tmp_set, "$")) {
                                        follow_set[index_follow].insert(tmp_set.begin(), tmp_set.end());
                                    }
                                    else {
                                        for (int p = k + 1; p < PROC_NUM && proc[i][p] != "" && proc[i][p] != "|"; p ++) {
                                            index_first = getIndex(non_ter,proc[i][p]);
                                            tmp_set = first_set[index_first];
                                            if(contains(terminset, proc[i][p])) {
                                                follow_set[index_follow].insert(proc[i][p]);
                                            }
                                            else {
                                                follow_set[index_follow].insert(tmp_set.begin(),tmp_set.end());
                                                if(contains(tmp_set, "$")) {
                                                    follow_set[index_follow].erase(follow_set[index_follow].find("$"));
                                                }
                                            }
                                        } 
                                    }
                                }
                                else if(contains(terminset, proc[i][k + 1])) { //其后为终结符
                                    index_follow = getIndex(non_ter, proc[i][k]);
                                    follow_set[index_follow].insert(proc[i][k + 1]);
                                }
                                else if(proc[i][k + 1] == "$") {
                                    index_follow = getIndex(non_ter, proc[i][k]);
                                    follow_set[index_follow].insert(follow_set[i].begin(), follow_set[i].end());
                                }
                                tmp_set = first_set[getIndex(non_ter, proc[i][k + 1])];
								if(contains(non_terset, proc[i][k + 1]) && contains(tmp_set, "$")) {
									bool judge = true;
                                    index_follow = getIndex(non_ter, proc[i][k]);
									for(int l = k + 1; l < PROC_NUM && proc[i][l] != "" && proc[i][l] != "|"; l ++) {
										if(contains(non_terset, proc[i][l])) {
											if(!contains(tmp_set, "$")) {
												judge = false;
												break;
											}
										}
										else if(contains(terminset, proc[i][l])) {
											judge = false;
											break;
										}
									}
									if(judge) {
                                        follow_set[index_follow].insert(follow_set[i].begin(), follow_set[i].end());
									}
								}
                            }
                            else if(k + 1 >= PROC_NUM || proc[i][k + 1] == "" || proc[i][k + 1] == "|") {
                                index_follow = getIndex(non_ter, proc[i][k]);
                                follow_set[index_follow].insert(follow_set[i].begin(), follow_set[i].end());
                            }
                        }
                    }
                }
            }
        }
        judge2 = 0;
        for(int i = 0; i < count; i ++) {
            judge2 += follow_set[i].size();
        }
    }
    print_follow();
}
