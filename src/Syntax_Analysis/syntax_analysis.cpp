#include "include/syntax_analysis.h"

void syntax_analysis(){
    
    Init("../grammar.txt");

    compute_first_sets();
    std::cout << std::endl;

    compute_follow_sets();
    std::cout << std::endl;

    compute_select_sets();
    compute_table();
    std::cout << std::endl;

    analysis_table();
    std::cout << std::endl;

    get_tokens("../out/15lex.txt");
    std::cout << std::endl;

    grammar_analysis();
}