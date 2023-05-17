# 项目文件说明
1. test.sy为输入文件，从该文件中获取输入序列
2. lexcial.cpp: 对输入序列进行词法分析，输出token序列
3. Lexical_Result.txt文件存放词法分析结果，即token序列
4. Init.cpp: 语法分析初始化，通过读入文法获取文法产生式、终结符以及非终结符等
5. Grammar1.txt：C--文法
6. Set.cpp：生成非终结符的First集合和Follow集合
7. Table.cpp：根据First集合以及Follow集合生成预测分析表
8. syntax_analysis.cpp：预测分析算法，对token序列进行语法解析
9. target.exe：可执行文件，将其输出重定向到out.txt中
10. out.txt：输出结果，包含词法解析结果token序列，语法解析结果：first集合、follow集合、预测分析表、语法解析结果归约序列

# 项目运行说明
1. 编写了Makefile文件，可直接在终端通过`make`以及`make clean`命令对项目文件进行编译，采用C++-11标准
2. 通过`./target.exe > out.txt`命令将生成的可执行文件`target.exe`文件的输出重定向到`out.txt`文件中
