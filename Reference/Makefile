#目标文件：依赖关系
CPP_FLAGS='-std=c++11'
target:Main.o lexical.o Init.o Set.o Table.o syntax_analysis.o
	g++ ${CPP_FLAGS} Main.o lexical.o Init.o Set.o Table.o syntax_analysis.o -o target
Main.o:./Main.cpp
	g++ ${CPP_FLAGS} -c Main.cpp -I ./ -o Main.o
lexical.o:./lexical.cpp
	g++ ${CPP_FLAGS} -c lexical.cpp -I ./ -o lexical.o
Init.o:./Init.cpp
	g++ ${CPP_FLAGS} -c Init.cpp -I ./ -o Init.o
Set.o:./Set.cpp
	g++ ${CPP_FLAGS} -c ./Set.cpp -I ./ -o Set.o
Table.o:./Table.cpp
	g++ ${CPP_FLAGS} -c ./Table.cpp -I ./ -o Table.o
syntax_analysis.o:./syntax_analysis.cpp
	g++ ${CPP_FLAGS} -c ./syntax_analysis.cpp -I ./ -o syntax_analysis.o
.PHONY:clean
clean:
	rm -rf *.o target