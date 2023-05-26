CPP_FLAGS='-std=c++17'
target:Main.o init.o calculate.o table.o syntax_analysis.o
	g++ ${CPP_FLAGS} Main.o init.o calculate.o table.o syntax_analysis.o
Main.o:./Main.cpp
	g++ ${CPP_FLAGS} -c Main.cpp -I ./ -o Main.o
init.o:./init.cpp
	g++ ${CPP_FLAGS} -c init.cpp -I ./ -o init.o
calculate.o:./calculate.cpp
	g++ ${CPP_FLAGS} -c calculate.cpp -I ./ -o calculate.o
table.o:./table.cpp
	g++ ${CPP_FLAGS} -c table.cpp -I ./ -o table.o
syntax_analysis.o:./syntax_analysis.cpp
	g++ ${CPP_FLAGS} -c syntax_analysis.cpp -I ./ -o syntax_analysis.o
.PHONY:clean
clean:
	del *.o
	del *.exe
	del out.txt