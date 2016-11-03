COMPILE = g++
FLAGS = -Wall -Werror -ansi -pedantic
OBJS = rshell.o

 all: rshell

 rshell:
		@mkdir -p bin
		$(COMPILE) $(FLAGS) src/Command.cpp src/TokenTest.cpp src/RShell.cpp src/main.cpp -o bin/rshell
		
 clean:
		rm -rf *~ *.o a.out	