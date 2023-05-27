#include "../../include/lex_analysis.h"
using namespace std;

typedef struct trans_map{
    char rec;
    int now;
    int next;
} transform_map;

bool operator<(const trans_map &a,const trans_map &b){
    return a.now + 100*int(a.rec) < b.now + 100*int(b.rec);
}

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

map<string, string> symbols_table = {
    {"int","KW"},{"void","KW"},{"return","KW"},{"const","KW"},{"main","KW"},
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


void printFA(FA FA){
    cout << "----input_symbols----" << endl;
    for(vector<char>::iterator i = FA.input_symbols.begin(); i != FA.input_symbols.end();i++){
        cout << *i << " " ;
    }
    cout << endl << "----states----" << endl;
    for(set<int>::iterator i = FA.states.begin(); i != FA.states.end();i++){
        cout << *i << " ";
    }
    cout << endl << "----start&final----" << endl << "start ";
    for(set<int>::iterator i = FA.start.begin();i != FA.start.end();i++){
        cout << *i << "    ";
    }
    cout <<"final ";
    for(set<int>::iterator i = FA.final.begin();i != FA.final.end();i++){
        cout << *i << ":" << FA.state_labels[*i] << " " ;
    }
    cout << endl << "labels ";
    for(map<int,string>::iterator i = FA.state_labels.begin();i != FA.state_labels.end();i++){
        cout << i->first << ":" << i->second << " " ;
    }
    cout << endl << "----trans_map----" << endl;
    for(set<transform_map>::iterator i = FA.trans_map.begin(); i != FA.trans_map.end();i++){
        transform_map trans = *i;
        cout << setw(2) << left << trans.now << setfill(' ') <<  " -- " << trans.rec << " ->  " << setw(2) << left << trans.next << setfill(' ') << endl ;
    }
}

void printSet(set<int> myset){
    for(set<int>::iterator i = myset.begin(); i != myset.end();i++) cout << *i << " "  ;
    cout << endl;
}

void printTrans(trans_map trans){
    cout << trans.now << " --" << trans.rec << "-> " << trans.next << endl;
}

void printTransSet(set<trans_map> trans){
    for(set<trans_map>::iterator i = trans.begin();i!=trans.end();i++){
        trans_map trans = *i;
        cout << trans.now << " --" << trans.rec << "-> " << trans.next << endl;
    }
    cout << endl;
}

void printMapForSet(map<set<int>,set<int>> state2set){
    for(map<set<int>,set<int>>::iterator i  = state2set.begin(); i != state2set.end();i++){
        set<int> tmp1 = i->first;
        set<int> tmp2 = i->second;
        cout << "first ";
        for(set<int>::iterator i = tmp1.begin();i != tmp1.end();i++){
            cout << *i << " ";
        }
        cout << "second ";
        for(set<int>::iterator i = tmp2.begin();i != tmp2.end();i++){
            cout << *i << " ";
        }
        cout << endl;
    }
}

set<int> getClosure(set<int> current_states, FA NFA){
    set<int> extended_states = current_states;
    while(true){
        int flag = 0;
        for(set<int>::iterator i = extended_states.begin();i != extended_states.end();i++){
            int now_state = *i;
            set<transform_map>::iterator j;
            transform_map tmp;
            for(j = NFA.trans_map.begin();j != NFA.trans_map.end();j++){
                tmp = *j;
                if(tmp.rec == '@' && now_state == tmp.now && !extended_states.count(tmp.next)){
                    extended_states.insert(tmp.next);
                    flag = 1;   //答辩一样的结构，想不出别的办法了，写了8个小时没吃晚饭写到这里人已经要散架了
                }
            }
        }
        if(!flag) break;
    }
    return extended_states;
}

set<int> getNextState(set<int> current_states, char input, FA NFA){
    set<int> extended_states;
    for(set<int>::iterator i = current_states.begin();i != current_states.end();i++){  //遍历状态集合
        int state = *i;
        trans_map trans;
        for(set<trans_map>::iterator j = NFA.trans_map.begin();j != NFA.trans_map.end();j++){
            trans = *j;
            if(trans.now == state && trans.rec == input){
            // cout << endl << " " << state << " " << input ;
            // printTrans(trans);
                extended_states.insert(trans.next);
            }
        }
    }
    return getClosure(extended_states, NFA);
}

char getInputChar(char input_symbol){
    if(input_symbol >= '1' && input_symbol <= '9')  return 'n';
    else if(input_symbol == '0')  return '0';
    else if((input_symbol >= 'A' && input_symbol <= 'Z')||(input_symbol >= 'a' && input_symbol <= 'z'))  return 'l';
    else if(input_symbol == '+' || input_symbol == '-' || input_symbol == '*' || input_symbol == '/' || input_symbol == '%')  return 'o';
    else if(input_symbol == '(' || input_symbol == ')' || input_symbol == '{' || input_symbol == '}' || input_symbol == ';' || input_symbol == ',')  return 's';
    // else if(input_symbol == '~')  return '~';
    else    return input_symbol;
}

FA NFA2DFA(FA NFA){
    FA DFA;
    queue<set<int>> processing_set;
    set<int> current_states, next_states;
    set<set<int>> processed_state;
    map<set<int>,int> trans_between_ND;
    int current_new_state;    //记录当前处理的
    current_states = getClosure(NFA.start,NFA); //将start进行@扩展
    processed_state.insert(current_states); //扩展完作为新的start
    trans_between_ND.insert({current_states,1}); //记录NFA和DFA集合之间关系
    DFA.start = {1};    //DFA有唯一的start
    DFA.states.insert(1);
    DFA.input_symbols = NFA.input_symbols;
    current_new_state = 1;
    processing_set.push(current_states);
    while(!processing_set.empty()){
        current_states = processing_set.front();
        current_new_state = trans_between_ND[current_states];   //根据当前的current_states找到对应在DFA中的state
        processing_set.pop();
        char ch;
        for(vector<char>::iterator i = NFA.input_symbols.begin(); i != NFA.input_symbols.end(); i++){  //遍历所有字符
            ch = *i;
            next_states = getNextState(current_states, ch, NFA); //找到下个集合
            set<int> tmp = {};
            if(next_states == tmp) continue;
            if(!processed_state.count(next_states)){
                processed_state.insert(next_states);    //处理完的集合没有，则插入
                int next_new_state = DFA.states.size() + 1;
                DFA.states.insert(next_new_state);  //更新DFA的states
                DFA.trans_map.insert({*i,current_new_state, next_new_state}); //更新DFA的trans_map
                for(set<int>::iterator i = next_states.begin();i != next_states.end();i++){
                    // DFA.state_labels.insert({next_new_state,NFA.state_labels[*i]});    //将对应终态的输出也写入
                    if(NFA.final.count(*i)){
                        DFA.final.insert(next_new_state);   //发现next_state中有终态，则在DFA中也相应的标记为终态
                        DFA.state_labels.insert({next_new_state,NFA.state_labels[*i]});    //将对应终态的输出也写入
                        break;
                    }
                }
                trans_between_ND.insert({next_states,next_new_state});//更新NFA到DFA的映射表
                processing_set.push(next_states);   //将下一个状态加入队列
            }
            DFA.trans_map.insert({*i,current_new_state, trans_between_ND[next_states]}); //更新DFA的
        }
    }
    
    return DFA; 
}

int findNewstate(map<set<int>,int> trans_between_DmD, int state){
    for(map<set<int>,int>::iterator i = trans_between_DmD.begin(); i != trans_between_DmD.end();i++ ){
        if(i->first.count(state))   return i->second;
    }
}

FA minimizeDFA(FA DFA){
    FA minDFA;
    minDFA.input_symbols = DFA.input_symbols;
    queue<set<int>> sets_que;   //处理队列
    set<set<int>> all_set;  //总划分
    set<int> current_set;   //当前处理的划分
    map<set<int>,int> trans_between_DmD;    //两个图之间的映射
    map<set<int>,set<int>> state2set;    //当前状态转移到了哪个集合，前面是转移到的状态集合，后者是当前状态集合
    set<int> tmp;
    for(set<int>::iterator i = DFA.states.begin();i!= DFA.states.end();i++){
        if(!DFA.final.count(*i)) tmp.insert(*i);
    }
    all_set.insert(tmp);
    set<string> label_set;
    for(set<int>::iterator i = DFA.final.begin(); i!= DFA.final.end();i++){
        if(!label_set.count(DFA.state_labels[*i])){
            string this_label = DFA.state_labels[*i];
            set<int> tmp;
            for(set<int>::iterator i = DFA.final.begin(); i!= DFA.final.end();i++){
                if(DFA.state_labels[*i] == this_label){
                    tmp.insert(*i);
                }
            }
            all_set.insert(tmp);
        }
    }
    // all_set.insert(DFA.final);
    sets_que.push(tmp);
    sets_que.push(DFA.final);
    int change_flag = 1;
    while(change_flag){
        for(set<set<int>>::iterator i = all_set.begin();i != all_set.end();i ++){
            change_flag = 0 ;
            current_set = *i;
            vector<char>::iterator k;
            for(k = minDFA.input_symbols.begin();k != minDFA.input_symbols.end();k++){  //遍历所有输入
                char input = *k;
                for(set<int>::iterator i = current_set.begin(); i != current_set.end();i++){   //遍历set中所有状态
                    int now_state = *i;
                    int next_state = -1;
                    set<int> target_set;
                    for(set<transform_map>::iterator j = DFA.trans_map.begin(); j != DFA.trans_map.end();j++){   //遍历所有规则
                        transform_map trans = *j;
                        if(now_state == trans.now && input == trans.rec){
                            next_state = trans.next;
                        }
                    }
                    for(set<set<int>>::iterator i = all_set.begin(); i != all_set.end();i++){   //遍历all_set寻找映射在的set
                        set<int> search_set = *i;
                        if(search_set.count(next_state)){
                            target_set = search_set;
                            break;
                        }
                    }
                    set<int> tmp = {};
                    if(!state2set.count(target_set)){
                        tmp.insert(now_state);
                        state2set.insert({target_set,tmp});
                        // state2set[target_set].insert(now_state);
                    }  
                    else{   //要么创建，要么插入
                        // state2set.insert({target_set,tmp});  
                        state2set[target_set].insert(now_state);
                    }
                    target_set.clear();
                }
                if(state2set.size() > 1){
                    change_flag = 1;
                    all_set.erase(current_set); //将当前的集合删除
                    for(map<set<int>,set<int>>::iterator i = state2set.begin(); i != state2set.end();i++){
                        all_set.insert(i->second);  //插入总划分新的集合
                        // sets_que.push(i->second);
                    }
                }
                state2set.clear();
                if(change_flag) break;
            }
            if(change_flag) break;
        }
    }
    for(set<set<int>>::iterator i = all_set.begin(); i != all_set.end();i++){   //遍历总划分
        set<int> new_state_set;
        new_state_set = *i;
        // cout << "this set is  ";
        // printSet(new_state_set);
        int new_state = minDFA.states.size()+1;
        minDFA.states.insert(new_state);    //minDFA写入新状态
        trans_between_DmD.insert({new_state_set,new_state});    //映射表中写入
        for(set<int>::iterator i = new_state_set.begin(); i != new_state_set.end();i++){    //遍历该划分下的所有状态
            if(DFA.start.count(*i)){    //如果是初始状态，放入新图的初始状态
                minDFA.start.insert(new_state);
                break;
            } else if(DFA.final.count(*i)){ //如果是终态，放入终态并把输出写入label
                // set<string> label_set;
                // label_set.insert(DFA.state_labels[*i]);
                // for(set<int>::iterator i = new_state_set.begin(); i != new_state_set.end();i++){
                //     if(!label_set.count(DFA.state_labels[*i])){
                //         label_set.insert(DFA.state_labels[*i]);
                //     } else{

                //     }
                // }
                // cout << "current state" << *i << " label " << DFA.state_labels[*i] << endl;
                minDFA.final.insert(new_state);
                minDFA.state_labels.insert({new_state,DFA.state_labels[*i]});
                break;
            }
        }
    }
    for(set<transform_map>::iterator i = DFA.trans_map.begin(); i != DFA.trans_map.end();i++){  //遍历原DFA的所有规则
        transform_map trans = *i;
        transform_map new_trans;
        new_trans.now = findNewstate(trans_between_DmD,trans.now);
        new_trans.next = findNewstate(trans_between_DmD,trans.next);
        new_trans.rec = trans.rec;
        if(!minDFA.trans_map.count(new_trans)){
            minDFA.trans_map.insert(new_trans);
        }
    }
    return minDFA;
}

string preProcess(string line){
    string res = line;
    int i;
    for(i = 0; i < line.length(); i++){
        if(line[i] == '/' && line[i+1] == '/'){
            break;
        }
    }
    res = res.substr(0,i);
    return res;
}

int getBC(string line){
    int first = 0;
    while(line[first] == ' ' || line[first] == '\t'){
        first++;
    }
    return first;
}

string getCatogory(int state, string str_token, FA DFA){
    if(DFA.state_labels[state]=="INT"){
        if(!processed_symbols_table.count(str_token)) processed_symbols_table.insert({str_token,DFA.state_labels[state]});
        return "<INT,"+str_token+">";
    } else if(DFA.state_labels[state] == "SE"){
        if(!processed_symbols_table.count(str_token)) processed_symbols_table.insert({str_token,DFA.state_labels[state]});
        return "<SE,"+str_token+">";
    } else if(DFA.state_labels[state] == "OP"){
        if(!processed_symbols_table.count(str_token)) processed_symbols_table.insert({str_token,DFA.state_labels[state]});
        return "<OP,"+str_token+">";
    } else if(DFA.state_labels[state] == "I&K" && symbols_table[str_token] == "KW"){
        if(!processed_symbols_table.count("KW")) processed_symbols_table.insert({str_token,DFA.state_labels[state]});
        return "<KW,"+str_token+">";
    } else{
        if(!processed_symbols_table.count(str_token)) processed_symbols_table.insert({str_token,"IDN"});
        return "<IDN,"+str_token+">";
    }
}

vector<string> erroProcessing(int state, char ch, FA DFA){
    vector<string> errolog;
    string error_category;

}

void generateFile(vector<string> str_token_vector){
    ofstream output;
    output.open("../out/tokens.txt", ios::out);
    for(int i=0;i<str_token_vector.size();i++){
        if(i == str_token_vector.size()-1){
            output << str_token_vector[i];
        } else{ 
            output << str_token_vector[i] << " ";
        }
    }
}

void lexcial(const char* file_address, FA DFA){
    char ch;
    string str_token;
    vector<string> str_token_vector;
    ofstream write;
	write.open("../out/15lex.txt", ios::out);
    ifstream file;
    file.open(file_address, ios_base::in);
    set<int>::iterator get_start;   //从DFA的start开始
    get_start = DFA.start.begin();
    string line;
    int errorflag;
    vector<string> errorlog;
    while(getline(file,line)){  //获取一行
        line = preProcess(line);    //预处理
        if(line == "")  continue;
        int front = getBC(line);
        int end = front;
        // cout << "---current line: " << line <<endl;  
        int state = *get_start; //当前的state
        int i;
        for(i=front; i < line.length();i++){    //遍历一行所有字符
            errorflag = 1;
            char proccessing_char = getInputChar(line[i]);
            char next_proccessing_char = getInputChar(line[i+1]);
            // cout << endl << "proccessing char " << line[i] << " postition " << i <<  " state " << state << endl;
            transform_map trans;
            for(set<transform_map>::iterator j = DFA.trans_map.begin(); j != DFA.trans_map.end();j++){   //遍历规则
                trans = *j;
                if((state == trans.now) && (proccessing_char == trans.rec)){   //如果规则成功匹配
                    // cout << "compare success " << proccessing_char << " current_state " << state <<endl;
                    // printTrans(trans);
                    errorflag = 0;
                    state = trans.next; //state转移，end+1
                    end = i;
                    int nextstate = -1;
                    set<transform_map>::iterator l;
                    transform_map next_trans;
                    for(l = DFA.trans_map.begin(); l != DFA.trans_map.end();l++){   //遍历规则
                        next_trans = *l;
                        if((state == next_trans.now) && (next_proccessing_char == next_trans.rec)) {
                            // cout << "trans " ;
                            // printTrans(next_trans);
                            nextstate = next_trans.next; //获取下一跳
                            break;
                        }
                    }
                    // cout << "next_char" << line[i+1] << " next_state " << nextstate << endl;
                    if(DFA.final.count(state) && !DFA.final.count(nextstate)){  //当前是终态，下一个不是终态，则走到头了
                        str_token = line.substr(front, end-front+1);    //获取字符
                        cout << str_token << "\t" << getCatogory(state,str_token,DFA) << endl;
                        write << str_token << "\t" << getCatogory(state,str_token,DFA) << endl; //写到输入流
                        str_token_vector.push_back(str_token);  //往字符组中写
                        state = *get_start; //状态机重新从start开始
                        int k = i+1;
                        while(line[k] ==' ' || line[k] == '\t') {
                            // cout << "k " << k << " char " <<  line[k] << endl;
                            k++;
                            i = k-1;
                        }
                        // cout << "i k " << i << " " << k << endl; 
                        front = k;  //如果有空格则除去空格，找到下一个词
                        end = front;
                    }
                    break;
                }
            }
            // cout << "###" <<endl;
            // errorlog = erroProcessing(state, proccessing_char, DFA); 
            // errorflag = 1;
            // break; 
            if(errorflag) break;
        }
        if(errorflag) break;
    }
    if(errorflag != 1){
        generateFile(str_token_vector); //没有错误，将tokens写入tokens.txt
    } else{
        string log = "error occur!";
        errorlog.push_back(log);
        generateFile(errorlog);
    }
}

void lex_analysis(){
    FA NFA, DFA, minDFA;
    NFA.input_symbols = lex_input_symbols;  //这里换成test1_~是课本样例，用于检验NFA2DFA和minimizeDFA的正确性
    NFA.states = lex_states;
    NFA.start = lex_start;
    NFA.final = lex_final;
    NFA.trans_map = lex_trans_map;
    NFA.state_labels = lex_state_labels;
    cout << "NFA" << endl;
    printFA(NFA);
    DFA = NFA2DFA(NFA);
    cout << endl << "DFA" << endl;
    printFA(DFA);
    minDFA = minimizeDFA(DFA);
    cout << endl << "minDFA" << endl;
    printFA(minDFA);
    cout << endl;
    lexcial("../src_in/test15.txt", minDFA);
}
