//生成预测分析表
#include <iostream>
#include <string>
#include <string.h>
#include <map>
#include <set>
#include <vector>
#include <float.h>
#include "Init.h"
#include "Set.h"
#include "Param.h"

using std::set;

string first[FIRST_NUM][FIRST_NUM];
string follow[FOLLOW_NUM][FOLLOW_NUM];

string table[TABLE_NUM][TABLE_NUM][TABLE_NUM];

set<int> set0;

void getTable() {
    
    // convert();
    set<string>::iterator it;
    int q = 0;
    first_set[0].insert("$");
    //为生成符号预测表，将"#"临时加入到终结符中
    ter.insert(map<string, int>::value_type("#",ter.size()));
    termin[25]="#";
    //set和二维数组之间的转化
    for (int i = 0; i < NONTER_NUM; i ++) {
        //cout << i << ": ";
        q = 0;
        for (it = first_set[i].begin(); it != first_set[i].end(); it ++) {
            first[i][q] = *it;
            //cout << first[i][q] << " ";
            q ++;
        }
        //cout << endl;
    }
    q = 0;
    for (int i = 0; i < NONTER_NUM; i ++) {
        q = 0;
        for (it = follow_set[i].begin(); it != follow_set[i].end(); it ++) {
            follow[i][q] = *it;
            q ++;
        }
    }
    /* table初始化
     * table的第一维为产生式序号
     * table第二维为终结符序号
     * table第三维为产生式的字符串下标
     */
    for(int i=0 ;i<TABLE_NUM; i++)
        for(int j=0; j<TABLE_NUM; j++)
            for(int k=0; k<TABLE_NUM; k++)
                table[i][j][k] = "";

    //遍历生成预测分析表
    for(int i=0; i<PROC_NUM && proc[i][0] != ""; i++){              //产生式
        for(int j=2; proc[i][j]!=""; j++){      //从首符号开始遍历
            if(proc[i][j-1]=="->" || proc[i][j-1]=="|"){
                if(n_ter.count(proc[i][j])!=0){        //if候选产生式的首部为非终结符
                    int firstIndex = n_ter[proc[i][j]];  //找到位置
                    for(int k=0; first[firstIndex][k]!=""; k++){ 
                            //First集所有非终结符 
                            if(first[firstIndex][k]!="$"){
                                int terIndex = ter[first[firstIndex][k]];
                                table[i][terIndex][0] = proc[i][0];
                                table[i][terIndex][1] = proc[i][1];
                                for(int n=j; proc[i][n]!=""; n++){
                                    if(proc[i][n]!="|")
                                        table[i][terIndex][n-j+2] = proc[i][n];
                                    else
                                        break;
                                }
                            }
                            else if(first[firstIndex][k]=="$"){         //FIRST集合包含空
                                for(int e=0; e<FOLLOW_NUM && follow[firstIndex][e]!=""; e++){
                                    if(ter.find(follow[firstIndex][e])!=ter.end() && first_set[firstIndex].find("$")!=first_set[firstIndex].end()){
                                        int terIndex = ter.find(follow[i][e])->second;
                                        table[i][terIndex][0] = proc[i][0];
                                        table[i][terIndex][1] = proc[i][1];
                                        for(int n = j; proc[i][n] != "";n++){
                                                if(proc[i][n] != "|"){
                                                    table[i][terIndex][n-j+2] = proc[i][n];
                                                }
                                                else
                                                    break;
                                        }
                                        set0.insert(i);
                                    }
                                }
                            }  
                    }
                }
                else if(ter.find(proc[i][j])!=ter.end()){       //if终结符
                    int terIndex = ter.find(proc[i][j])->second;
                    table[i][terIndex][0] = proc[i][0];     //P->
                    table[i][terIndex][1] = proc[i][1];
                    table[i][terIndex][2] = proc[i][j];
                    for(int m=j; proc[i][m]!=""; m++){
                        if(proc[i][m]!="|")
                            table[i][terIndex][m-j+2] = proc[i][m];
                        else
                            break;
                    }
                }
                else if(proc[i][j]=="$"){
                    for(int k=0; follow[i][k]!=""; k++){
                        if(ter.find(follow[i][k])!=ter.end() && first_set[i].find("$")!=first_set[i].end()){
                            int terIndex = ter.find(follow[i][k])->second;
                            table[i][terIndex][0] = proc[i][0];
                            table[i][terIndex][1] = proc[i][1];
                            for(int n = j; proc[i][n] != "";n++){
                                if(proc[i][n] != "|"){
                                    table[i][terIndex][n-j+2] = proc[i][n];
                                }
                                else
                                    break;
                            }
                        }
                    }
                }
            }
        }
    }
    cout <<"######Table Result######"<< endl;
    for(int i=0; termin[i]!=""; i++){
        cout<<" "<<termin[i];
    }
    cout<<endl;
    for(int i=0; non_ter[i]!=""; i++){
        cout<<non_ter[i]<<" ";
        for(int j=0; termin[j]!=""; j++){
            for(int k=0; table[i][j][k]!=""; k++){
                cout<<table[i][j][k];
            }
            cout<<" ";
        }
        cout<<endl;
    }
    //将"#"从终结符中移除
    termin[25]="";
    ter.erase(ter.find("#"));
}
