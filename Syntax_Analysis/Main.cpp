#include <iostream>
#include <vector>
#include "init.h"
#include "calculate.h"
#include "table.h"
#include "syntax_analysis.h"

using namespace std;

int main() {
    Init("grammar.txt");

    compute_first_sets();
    cout << endl;
    compute_follow_sets();

    cout << endl;

    compute_select_sets();
    compute_table();

    cout << endl;

    analysis_table();

    cout << endl;
    get_tokens("10_lexical.txt");
    cout << endl;
    syntax_analysis();

    return 0;
}