#ifndef SYNTAX_H
#define SYNTAX_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <unordered_map>
#include <iomanip>
#include "init.h"
#include "calculate.h"
#include "table.h"

extern void get_tokens(const char *file_name);
extern void grammar_analysis();

#endif