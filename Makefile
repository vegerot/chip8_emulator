INCLUDES= -I./include -I/usr/local/opt/sdl2/include
#-L adds dirs to lib path, -l is the specific lib (from lib path) to link
LIBS= -L/usr/local/opt/sdl2/lib/ -lSDL2main -lSDL2

BIN= /usr/local/opt/sdl2/bin
CXXFLAGS= -O3 -std=c++2a
FLAGS= -g -Wall -Wextra -pedantic
all:
	#cc ${INCLUDES} ${LIBS} ${FLAGS} ./src/main.c -o ./bin/main
	c++ ${INCLUDES} ${LIBS} ${FLAGS} ${CXXFLAGS} ./src/main.cpp -o ./bin/main
compile_flags:
	rm compile_flags.txt
	touch compile_flags.txt
	$(foreach flag,\
		$(INCLUDES) $(LIBS) $(FLAGS) $(CXXFLAGS),\
		echo "$(flag)" >> compile_flags.txt;)
