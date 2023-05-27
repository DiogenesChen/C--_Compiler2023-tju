##### global settings #####
.PHONY: clean

CPP := g++
CPP_FLAGS='-std=c++17'

BIN_DIR := ./bin
LEX_DIR := ./src/Lex_Analysis
SYN_DIR := ./src/Syntax_Analysis
OUT_DIR := ./out
IN_DIR := ./src_in

##### rules for building the project #####

target:Compiler_front_end lexical_analysis syntax_analysis init calculate table \
		grammar_analysis
Compiler_front_end:Compiler_front_end.cpp
	$(CPP) $(CPP_FLAGS) Compiler_front_end.cpp -o $(BIN_DIR)/Compiler_front_end
lexical_analysis:$(LEX_DIR)/lexical_analysis.cpp
	$(CPP) $(CPP_FLAGS) $(LEX_DIR)/lexical_analysis.cpp -o $(BIN_DIR)/lexical_analysis
syntax_analysis:$(SYN_DIR)/syntax_analysis.cpp
	$(CPP) $(CPP_FLAGS) $(SYN_DIR)/syntax_analysis.cpp -o $(BIN_DIR)/syntax_analysis
init:$(SYN_DIR)/init.cpp
	$(CPP) $(CPP_FLAGS) $(SYN_DIR)/init.cpp -o $(BIN_DIR)/init
calculate:$(SYN_DIR)/calculate.cpp
	$(CPP) $(CPP_FLAGS) $(SYN_DIR)/calculate.cpp -o $(BIN_DIR)/calculate
table:$(SYN_DIR)/table.cpp
	$(CPP) $(CPP_FLAGS) $(SYN_DIR)/table.cpp -o $(BIN_DIR)/table
grammar_analysis:$(SYN_DIR)/grammar_analysis.cpp
	$(CPP) $(CPP_FLAGS) $(SYN_DIR)/grammar_analysis.cpp -o $(BIN_DIR)/grammar_analysis

##### rules for cleaning the project #####

clean:
	rm -rf $(BIN_DIR)/*
	rm -rf $(BIN_DIR)/*.exe
	rm -rf $(OUT_DIR)/*.txt

clean-out:
	rm -rf $(OUT_DIR)/*.txt

clean-bin:
	rm -rf $(BIN_DIR)/*
	rm -rf $(BIN_DIR)/*.exe


