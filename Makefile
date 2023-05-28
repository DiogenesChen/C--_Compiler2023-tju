##### global settings #####
.PHONY: clean all

CPP := g++
CPP_FLAGS := -std=c++17 -c -Werror

BIN_DIR := ./bin
INC_DIR := ./include
COM_DIR := ./src
LEX_DIR := ./src/Lex_Analysis
SYN_DIR := ./src/Syntax_Analysis
OUT_DIR := ./out
IN_DIR := ./src_in

SOURCE=$(wildcard $(COM_DIR)/*.cpp $(LEX_DIR)/*.cpp $(SYN_DIR)/*.cpp)
OBJS=$(patsubst %.cpp, %.o, $(notdir $(SOURCE)))

##### rules for building the project #####

all: mkdir target

mkdir:
	mkdir -p $(BIN_DIR)
	mkdir -p $(OUT_DIR)

ifeq ($(OS),"Windows_NT")
target:$(addprefix $(BIN_DIR)/, $(OBJS))
	$(CPP) $^ -o compiler_front_end.exe
else
target:$(addprefix $(BIN_DIR)/, $(OBJS))
	$(CPP) $^ -o compiler_front_end
endif

$(BIN_DIR)/%.o:$(COM_DIR)/%.cpp
	$(CPP) $(CPP_FLAGS) $< -I $(INC_DIR) -o $@
$(BIN_DIR)/%.o:$(LEX_DIR)/%.cpp
	$(CPP) $(CPP_FLAGS) $< -I $(INC_DIR) -o $@
$(BIN_DIR)/%.o:$(SYN_DIR)/%.cpp
	$(CPP) $(CPP_FLAGS) $< -I $(INC_DIR) -o $@

##### rules for cleaning the project #####

ifeq ($(OS),Windows_NT)
clean:
	del $(BIN_DIR)/*.o
	del Compiler_front_end.exe
	del $(OUT_DIR)/out.txt
else
clean:
	rm -rf $(BIN_DIR)
	rm -rf $(OUT_DIR)
	rm -f Compiler_front_end
endif
