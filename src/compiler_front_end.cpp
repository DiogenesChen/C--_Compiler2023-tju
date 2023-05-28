#include "init.h"
#include "calculate.h"
#include "table.h"
#include "syntax_analysis.h"

extern void lexical_analysis();

int main() {
    lexical_analysis();
    syntax_analysis();
    return 0;
}