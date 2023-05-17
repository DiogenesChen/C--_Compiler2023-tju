#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <math.h>
#include <queue>
#include <stack>
using namespace std;

typedef struct state{
	int now;
	char input;
	int next;
}state;

#define NFA_SIZE 26

vector<char> in_sym = {'S','_','I','0','Y','=','>','<','!','&','|','F'};
map<string,string> symbol_table;
map<int,char> nfamap = {{2,'S'},{4,'I'},{6,'Y'},{11,'F'}};
map<int,char> dfamap;
map<int,char> min_dfamap;

vector<int> NFA_ZT = {2,4,6,11};
map<string,pair<string,string>> usemap={
    {"int",{"KW","1"}},{"void",{"KW","2"}},{"return",{"KW","3"}},{"const",{"KW","4"}},
    {"main",{"KW","5"}},{"+",{"OP","6"}},{"-",{"OP","7"}},{"*",{"OP","8"}},
    {"/",{"OP","9"}},{"%",{"OP","10"}},{"=",{"OP","11"}},{">",{"OP","12"}},
    {"<",{"OP","13"}},{"==",{"OP","14"}},{"<=",{"OP","15"}},{">=",{"OP","16"}},
    {"!=",{"OP","17"}},{"&&",{"OP","18"}},{"||",{"OP","19"}},{"(",{"SE","20"}},
    {")",{"SE","21"}},{"{",{"SE","22"}},{"}",{"SE","23"}},{";",{"SE","24"}},
    {",",{"SE","25"}}
};
vector<state> DFA;
vector<state> MIN_DFA;
state NFA[NFA_SIZE]={
    {0,'S',1},{0,'_',1},{1,'S',1},{1,'_',1},{1,'I',1},{1,'0',1},{1,'$',2},
    {0,'0',3},{3,'$',4},{0,'I',5},{5,'0',5},{5,'I',5},{5,'$',4},
    {0,'Y',6},{0,'=',7},{7,'$',6},{7,'=',6},{0,'>',7},{0,'<',7},{0,'!',8},{8,'=',6},{0,'&',9},{9,'&',6},{0,'|',10},{10,'|',6},
    {0,'F',11}
    /*
    第一行为关键字和标识符
    第二行为整型数字
    第三行为运算符
    第四行为界符
    其中2，4，6，11为终态，初态为0
    */
};

char bewhich(char nowchar){
    if((nowchar >= 'A' && nowchar <= 'Z') || (nowchar >= 'a' && nowchar <= 'z')){
        return 'S';
    }else if(nowchar >= '1' && nowchar <= '9'){
        return 'I';
    }else if(nowchar == '0'){
        return '0';
    }else if(nowchar == '+' || nowchar == '-' || nowchar == '*' || nowchar == '/' || nowchar == '%'){
        return 'Y';
    }else if(nowchar == '(' || nowchar == ')' || nowchar == '{' || nowchar == '}' || nowchar == ';' || nowchar == ','){
        return 'F';
    }else if(nowchar == '=' || nowchar == '>' || nowchar == '<' || nowchar == '!' || nowchar == '&' || nowchar == '|' || nowchar == '_' || nowchar == ' ' || nowchar == '\t'){
        return nowchar;
    }else{
        return '~';
    }
}

set<int> closure(set<int> tem){
    set<int> res;
    set<int>::iterator i;
    for(i = tem.begin();i != tem.end();i++){
        res.insert(*i);
        int onestate = *i;
        while(true){
            int j;
            for( j = 0;j<NFA_SIZE;j++){
                if(NFA[j].now==onestate && NFA[j].input=='$'){
                    res.insert(NFA[j].next);
                    onestate=NFA[j].next;
                    break;
                }
            }
            if(j == NFA_SIZE){
                break;
            }
        }
    }
    return res;
}

set<int> moveby(set<int> state,char by){
    set<int> res;
    set<int>::iterator i;
    for(i = state.begin();i!=state.end();i++){
        for(int j = 0;j<NFA_SIZE;j++){
            if(NFA[j].now==*i && NFA[j].input==by){
                res.insert(NFA[j].next);
            }
        }
    }
    return closure(res);
}

int NFA2DFA(int start,vector<int> end){
    queue<set<int>> myque;
    myque.push(closure({start}));
    set<int> nowstate,nextstate;
    set<set<int>> allstates;
    map<set<int>,int> state_map;
    map<pair<set<int>, char>, set<int>> use;
    int num = 0;
    while(!myque.empty()){
        nowstate = myque.front();
        myque.pop();
        if(nowstate.size()==0){
            continue;
        }
        if(allstates.count(nowstate)==0){
            allstates.insert(nowstate);
            vector<char>::iterator i;
            for(i = in_sym.begin();i != in_sym.end();i++){
                nextstate = moveby(nowstate,*i);
                if(nextstate.size()!=0){
                    use[{nowstate,*i}]=nextstate;
                    myque.push(nextstate);
                }
            }
            state_map[nowstate] = num++;
        }
    }
    map<pair<set<int>, char>, set<int>>::iterator it;
    for(it=use.begin();it!=use.end();it++){
        if(state_map[it->second]!=0){
            DFA.push_back({state_map[it->first.first],it->first.second,state_map[it->second]});
        }
    }
    for(set<int> onestate : allstates){
        char whi = 'n';
        for(int one : onestate){
            for(int another : NFA_ZT){
                if(one == another){
                    whi = nfamap[one];
                    break;
                }
            }
            if(whi!='n'){
                break;
            }
        }
        dfamap[state_map[onestate]] = whi;
    }

    return num;
}

bool checkZT(int state){
    if(state == -1){
        return false;
    }
    if(min_dfamap[state]=='I' || min_dfamap[state]=='S' || min_dfamap[state]=='F' ||min_dfamap[state]=='Y'){
        return true;
    }
    return false;
}

int checkNotEmpty(string use){
    int res = 0;
    while(use[res]==' ' || use[res]=='\t'){
        res++;
    }
    return res;
}

string Data_Preprocessing(string use){
    string res = use;
    int k;
    for(k = 0;k<res.length();k++){
        if(res[k]=='/' && res[k+1]=='/'){
            break;
        }
    }
    res = res.substr(0,k);
    return res;
}

string checkWhich(int state,string token){
    if(min_dfamap[state]=='I'){
        return "<INT,"+token+">";
    }else if(min_dfamap[state]=='Y' || min_dfamap[state]=='F'){
        return "<"+usemap[token].first+","+usemap[token].second+">";
    }else{
        string temp;
        for(int i=0;i<token.length();i++){
            if(token[i]>='A' && token[i]<='Z'){
                temp += token[i]+'a'-'A';
            }else{
                temp+=token[i];
            }
        }
        if(usemap[temp].first=="" && usemap[temp].second==""){
            symbol_table.insert(map<string,string>:: value_type(token,"int"));
            return "<IDN,"+token+">";
        }else{
            return "<"+usemap[temp].first+","+usemap[temp].second+">";
        }
    }
}

void symbol_table_output(){
    map<string,string>::iterator it;
    for(it = symbol_table.begin();it != symbol_table.end();it++){
        cout<<"标识符: "<<it->first<<"\t"<<"类型: "<<it->second<<endl;
    }
}

 int DFA_find_next_state(int now_num,char c,int DFA_SIZE){
                    int find_state = -1;
                    if((now_num ==0&& c =='!')) find_state =6;
                    else if((now_num ==0&& c =='&')) find_state =7;
                    else if((now_num ==0&& c =='0')) find_state =3;
                    else if((now_num ==0&& c =='<')) find_state =5;
                    else if((now_num ==0&& c =='=')) find_state =5;
                    else if((now_num ==0&& c =='>')) find_state =5;
                    else if((now_num ==0&& c =='F')) find_state =9;
                    else if((now_num ==0&& c =='I')) find_state =2;
                    else if((now_num ==0&& c =='S')) find_state =1;
                    else if((now_num ==0&& c =='Y')) find_state =4;
                    else if((now_num ==0&& c =='_')) find_state =1;
                    else if((now_num ==0&& c =='|')) find_state =8;
                    else if((now_num ==1&& c =='0')) find_state =1;
                    else if((now_num ==5&& c =='=')) find_state =4;
                    else if((now_num ==1&& c =='I')) find_state =1;
                    else if((now_num ==1&& c =='S')) find_state =1;
                    else if((now_num ==1&& c =='_')) find_state =1;
                    else if((now_num ==2&& c =='0')) find_state =1;
                    else if((now_num ==2&& c =='I')) find_state =2;
                    else if((now_num ==6&& c =='=')) find_state =4;
                    else if((now_num ==7&& c =='&')) find_state =4;
                    else if((now_num ==8&& c =='|')) find_state =4;
                    return find_state;
 }



int findcollectnum(set<set<int>> con,int first_to_state,map<set<int>,int> bianhaomap){//找对应状态的编号
                int first_con_num = -1;

               for(set<set<int>> :: iterator ita = con.begin();ita != con.end();ita++){
                    set<int> tempset = *ita;
                    set<int> :: iterator itb = tempset.begin();
                    while(itb!=tempset.end()){
                        int tmpint = *itb;
                        if(tmpint == first_to_state ){
                            first_con_num = bianhaomap[tempset];
                            break;
                        }
                        else{
                            itb++;
                        }
                    }
                    if(first_con_num!=-1){
                         break;
                    }
                }
                return  first_con_num ;
}

int find_which_collection(int n,set<set<int>> con,map<set<int>,int> bianhaomap){
    int res=-1;
        set<set<int>> :: iterator it = con.begin();
        for(it; it!=con.end(); it++){
            set <int> s= *it;
            set<int> :: iterator itt = s.begin();
            for(itt;itt!= s.end();itt++){
                int f = *itt;
                if(f == n){
                    res = bianhaomap[s];
                    break;
                }
            }
            if(res!=-1){
                break;
            }
        }
        return res;
}

map<set<int>,int> bianhao(set<set<int>> con){//对集合进行编号
    map<set<int>,int> bianhaomap;
    set<set<int>> :: iterator it = con.begin();
    int num=0;
    for(it; it!=con.end();it++){
        set<int> s = *it;
        bianhaomap.insert(map<set<int>,int>:: value_type(s,num));
        num++;
    }
    return bianhaomap;
}


void min_DFA(int num){//num就是我的DFA_SIZE
    set<set<int> > con;
    set<int> end_ ;
    set<int> not_end;
    for(map<int,char>:: iterator dfai = dfamap.begin(); dfai!= dfamap.end() ;dfai++){
        if(dfai->second!= 'n'){
            end_.insert(dfai->first);
        }
        else{
            not_end.insert(dfai->first);
        }
    }
    con.insert(end_);
    con.insert(not_end);

    int oricnt=2;
    int nowcnt=0;
    //集合数目不变了就完事了
    int allflag = 0;
    while(allflag == 0 || allflag ==1){//都代表还没完事呢
        oricnt=con.size();
        allflag =0;//重新开始标识
        map<set<int>,int> bianhaomap = bianhao(con);
         //cout<<bianhaomap.size()<<endl;
        //考察每一个状态的集合
        set<set<int>> :: iterator it = con.begin();
        while(it!= con.end()){
            set<int> nowset = *it;//当前考察的集合
            //如果这个集合里面只有一个状态就不用考虑了！
            //考察每一个字符
            int charflag = 0;
            for(vector<char>::iterator m = in_sym.begin(); m !=in_sym.end(); m++ ){
                 map<pair<char,int>,set<int> > tempmap;//新揪出来的集合，谁经过了字符到达了哪个集合(编号): 是对某一个集合经历了某一个输入符号来说的。
                 set<set<int>> tmp;
                 tempmap.clear();//清空，重新记录
                char c= *m;//当前考察的符号
                //考察每一个集合中的每一个状态
                for(set<int> :: iterator s = nowset.begin();s !=nowset.end();s++){
                    int now_num = *s;//当前的这个状态对应的数字
                    int find_state;
                    find_state = DFA_find_next_state(now_num,c,num);
                    int now_con_num = findcollectnum(con,find_state,bianhaomap);
                        int flag = 0;
                        if(tempmap.size()>0){
                                for(map<pair<char,int>,set<int>>::iterator tempmapit = tempmap.begin();tempmapit!=tempmap.end();tempmapit++){
                                    if(tempmapit->first.first == c&&tempmapit->first.second == now_con_num){//之前有这个集合
                                        set<int> ss = tempmapit->second;
                                        pair<char,int> p(c,now_con_num) ;
                                         ss.insert(now_num);
                                        tempmap[p] = ss;
                                        flag = 1;
                                        break;
                                    }
                            }

                        }

                         if(flag == 0){//没这个集合，新建一个集合
                            set<int> ss;
                            //加入新集合
                            ss.insert(now_num);
                            pair<char,int> p(c,now_con_num) ;
                            tempmap.insert(map<pair<char,int>,set<int> >:: value_type(p,ss));//写入tempmap


                            //同时还要加入待加入的集合的那个集合

                        }
                }
        
                 if(c == 'F'){
                    charflag = 1;
                }
                if(tempmap.size()==1){//如果没拆分，我就不动了！！！！
                    continue;
                }
                //不等于1，被拆分了的话我们再删！
                else{
                    con.erase(nowset);
                    map<pair<char,int>,set<int> > :: iterator checkit = tempmap.begin();
                    for(checkit; checkit!= tempmap.end();checkit++){
                            set<int> tempmapset= checkit->second;
                            con.insert(tempmapset);
                    }
                    nowcnt = con.size();
                }

                if(nowcnt > oricnt){//如果集合变多了但还没到结束，就大退，重新算
                    allflag = 1;
                    bianhaomap = bianhao(con);//重新编号
                    break;
                }

            }
            if(allflag == 1){
                break;
            }
            it++;
            if(allflag ==0 &&  it==con.end()  && charflag ==1){//遍历到最后一个了，而且也没多集合
                allflag = 2;
                break;
            }
            if(allflag == 1){
                break;
            }
        }
        if(allflag ==2 ) {
            break;
        }



    }
    //判断每个多元素集合中没有输入的终态是否是一样的终态！
    set<set<int> >:: iterator mulit = con.begin();
    set<int> multmp;
    for(mulit;mulit!=con.end();mulit++){
        set<int> muls= *mulit;
        if(muls.size()!=1){
            set<int> :: iterator mulsetint = muls.begin();
            map<char,set<int> > tmpmap;//对应终态的字符，存放的集合
            int aa = * mulsetint;
            char cc = dfamap[aa];
            set<int> mulisett;
            tmpmap.insert(map<char,set<int> >::value_type(cc,mulisett) );
            for(mulsetint; mulsetint != muls.end();mulsetint++ ){
                int aaa = *mulsetint;
                    int aaflag = 0;
                    for( map<char,set<int> >:: iterator  aai = tmpmap.begin();aai!=tmpmap.end();aai++){
                        char ccc = aai->first;
                        if(ccc==dfamap[aaa] ){
                            aaflag ==1;
                            set<int> muliset = tmpmap[ccc];
                            muliset.insert(aaa);
                            tmpmap[ccc]=muliset;

                        }
                    }
                    if(aaflag ==0){
                        set<int> muliset;
                        muliset.insert(aaa);
                        tmpmap.insert(map<char,set<int> >::value_type(dfamap[aaa],muliset) );
                    }

            }
            con.erase(muls);
            map<char,set<int> > :: iterator ceckit = tmpmap.begin();
                    for(ceckit; ceckit!= tmpmap.end();ceckit++){
                            set<int> tempmapset= ceckit->second;
                            con.insert(tempmapset);

                    }
        }
    }

//开始通过每一个集合构造MIN_NFA了
//先编号
     map<set<int>,int> nummap = bianhao(con);
    set<set<int> >:: iterator conit = con.begin();
    //对每一个状态进行迭代！
    for(conit; conit !=con.end();conit++){
        set<int> collection = *conit;
        set<int>:: iterator collectionit = collection.begin();
        int collection_first_int = *collectionit;//取每个集合的第一个值作为代表进行计算
        vector<state> ::iterator stateit = DFA.begin();
        for(stateit;stateit!=DFA.end();stateit++){
            state sta = * stateit;//查到DFA的状态
            if(sta.now ==collection_first_int){
                int p1 = nummap[collection];//这个元素对应集合的序号（新的状态号）
                char c = sta.input;//输入的字符
                int whichcol =find_which_collection(sta.next, con,nummap) ;
                state inputstate;
                inputstate.now = p1;
                inputstate.input = c;
                inputstate.next = whichcol;
                MIN_DFA.push_back(inputstate);
            }
        }

    }
     for(map<int,char>:: iterator ztit = dfamap.begin(); ztit!= dfamap.end() ;ztit++){
            int putin = find_which_collection(ztit->first,con,nummap);
            min_dfamap.insert(map<int,char>:: value_type(ztit->first,ztit->second));


    }

}

bool checkNUM(int pos,string senten,char whi){
    int j = pos;
    int res = 0;
    while(senten[j]==whi){
        j--;
    }
    j++;
    while(senten[j]==whi){
        res++;
        j++;
    }
    if(res % 2 == 0){
        return true;
    }else{
        return false;
    }
}

void final_tokens(vector<string> tokens){
    ofstream outuse;
	outuse.open("tokens.txt", ios::out);
    for(int i = 0;i<tokens.size();i++){
        if(i == tokens.size()-1){
            outuse<<tokens[i];
        }
        else{
            outuse<<tokens[i]<<" ";
        }
    }
}

void Lexical_analysis(const char* address){
    vector<string> tokens;
    ofstream outwrite;
	outwrite.open("Lexical_Result.txt", ios::out);
    ifstream file;
	file.open(address, ios_base::in);
    string use;
    int errorflag = 0;
    while(getline(file,use)){
        use = Data_Preprocessing(use);
        if(use==""){
            continue;
        }
        int front = checkNotEmpty(use);
        int end = front;
        int state = 0;
        errorflag = 0;
        for(int i = 0;i<use.length();i++){
            if(use[i]=='&' || use[i]=='|'){
                if(!checkNUM(i,use,use[i])){
                    cout<<"The input: "<<use[i]<<" out of range!"<<endl;
                    errorflag = 1;
                    break;
                }
            }
            if(use[i]=='!' && use[i+1]!='='){
                cout<<"The input: "<<use[i]<<" out of range!"<<endl;
                errorflag = 1;
                break;
            }
            if(bewhich(use[i]) == '~'){
                cout<<"The input: "<<use[i]<<" out of range!"<<endl;
                errorflag = 1;
                break;
            }
            for(int j = 0;j<MIN_DFA.size();j++){
                if(MIN_DFA[j].now==state && MIN_DFA[j].input==bewhich(use[i])){
                    state = MIN_DFA[j].next;
                    end = i;
                    break;
                }
            }
            if(state == 3 && (bewhich(use[i+1])=='I' || use[i+1]=='0')){
                cout<<"A number beginning with 0!"<<endl;
                errorflag = 1;
                break;
            }
            if(min_dfamap[state]=='I' && bewhich(use[i+1])=='S'){
                cout<<"Error keyword or identifier beginning with a number!"<<endl;
                errorflag = 1;
                break;
            }
            int nextstate = -1;
            for(int j = 0;j<MIN_DFA.size();j++){
                if(i+1 == use.length()){
                    break;
                }
                if(MIN_DFA[j].now==state && MIN_DFA[j].input==bewhich(use[i+1])){
                    nextstate = MIN_DFA[j].next;
                }
            }
            string token;
            if(checkZT(state) && !checkZT(nextstate)){
                token = use.substr(front,end-front+1);
                cout<<token<<"\t"<<checkWhich(state,token)<<endl;
                outwrite<<token<<"\t"<<checkWhich(state,token)<<endl;
                tokens.push_back(token);
                state = 0;
                int k = i+1;
                while(use[k]==' ' || use[k]=='\t'){
                    k++;
                }
                front = k;
                end = k;
            }
        }
        if(errorflag==1){
            break;
        }
    }
    if(errorflag != 1){
        final_tokens(tokens);
    }else{
        vector<string> noneobj;
        final_tokens(noneobj);
    }
    return;
}
