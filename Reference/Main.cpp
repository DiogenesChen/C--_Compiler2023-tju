#include <iostream>
#include <vector>
#include "lexical.h"
#include "Init.h"
#include "Set.h"
#include "Table.h"
#include "syntax_analysis.h"
using namespace std;

int main() {
	//lexical analysis
	cout << "######Token Sequence######" << endl;
	vector<int> tmp;
	int a[5] = {2,4,6,11};
	for(int i = 0; i < 4; i ++) {
		tmp.push_back(a[i]);
	}
	int num = NFA2DFA({0},tmp);
    min_DFA(num);
    Lexical_analysis("test1.sy");

	//syntax analysis
	cout << "######Start Syntax Analysis######\n";
	Init("Grammar1.txt");

	firstSet();
	followSet();

    getTable();

	analysis();

	return 0;
}