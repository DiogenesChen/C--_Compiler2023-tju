#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

int main() {

    string line = "funcType IDN <funcFParams> ) block";
    string tmp1 = "btype";

    size_t begin_index = line.find("<");
    size_t end_index = line.find(">");

    string type = line.substr(begin_index+1, end_index-begin_index-1);
    cout << type << endl;

    return 0;
    // vector<string> tokens;
    // string space_delimiter = " ";
    // size_t pos = 0;
    // while ((pos = tmp.find(space_delimiter)) != string::npos) {
    //     tokens.push_back(tmp.substr(0, pos));
    //     tmp.erase(0, pos + space_delimiter.length());
    //     // cout << tmp << endl;
    // }
    // tokens.push_back(tmp);

    // for (auto const & t : tokens) {
    //     cout << t <<endl;
    // }

    // cout << "TOKENS SIZE : " << tokens.size() << endl;

    
}