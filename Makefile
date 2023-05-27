##### global settings #####
.PHONY: clean

CPP := g++
CPP_FLAGS='-std=c++17'
CFLAGS := -Wall -Werror -c

BIN_DIR := ./bin
SRC_DIR := ./src
OUT_DIR := ./out
IN_DIR := ./src_in

##### rules for building the project #####

target:Compiler_front_end.o lexcical_analysis.o syntax_analysis.o init.o calculate.o table.o \
		grammar_analysis.o
	$(CPP) $(CPP_FLAGS) Main.o init.o calculate.o table.o syntax_analysis.o
Compiler_front_end.o:$(SRC_DIR)/Compiler_front_end.cpp
	$(CPP) $(CPP_FLAGS) $(CFLAGS) $(SRC_DIR)/Main.cpp -o $(BIN_DIR)/Main.o
lexcical_analysis.o:$(SRC_DIR)/syntax_analysis.cpp
	$(CPP) $(CPP_FLAGS) $(CFLAGS) $(SRC_DIR)/lexcical_analysis.cpp -o $(BIN_DIR)/lexcical_analysis.o
syntax_analysis.o:$(SRC_DIR)/syntax_analysis.cpp
	$(CPP) $(CPP_FLAGS) $(CFLAGS) $(SRC_DIR)/syntax_analysis.cpp -o $(BIN_DIR)/syntax_analysis.o
init.o:$(SRC_DIR)/init.cpp
	$(CPP) $(CPP_FLAGS) $(CFLAGS) $(SRC_DIR)/init.cpp -o $(BIN_DIR)/init.o
calculate.o:$(SRC_DIR)/calculate.cpp
	$(CPP) $(CPP_FLAGS) $(CFLAGS) $(SRC_DIR)/calculate.cpp -o $(BIN_DIR)/calculate.o
table.o:$(SRC_DIR)/table.cpp
	$(CPP) $(CPP_FLAGS) $(CFLAGS) $(SRC_DIR)/table.cpp -o $(BIN_DIR)/table.o
grammar_analysis.o:$(SRC_DIR)/grammar_analysis.cpp
	$(CPP) $(CPP_FLAGS) $(CFLAGS) $(SRC_DIR)/grammar_analysis.cpp -o $(BIN_DIR)/grammar_analysis.o

##### rules for cleaning the project #####

clean:
	rm -rf $(BIN_DIR)/*.o
	rm -rf $(BIN_DIR)/*.exe
	rm -rf $(OUT_DIR)/*.txt

clean-out:
	rm -rf $(OUT_DIR)/*.txt

clean-bin:
	rm -rf $(BIN_DIR)/*.o
	rm -rf $(BIN_DIR)/*.exe


