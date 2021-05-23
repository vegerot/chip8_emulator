INCLUDES= -I./include -I/usr/local/opt/sdl2/include
#-L adds dirs to lib path, -l is the specific lib (from lib path) to link
LIBS= -L/usr/local/opt/sdl2/lib/ -lSDL2main -lSDL2

BIN= /usr/local/opt/sdl2/bin/
FLAGS= -g -Wall -Wextra -pedantic -O3
all:
	#cc ${INCLUDES} ${LIBS} ${FLAGS} ./src/main.c -o ./bin/main
	c++ ${INCLUDES} ${LIBS} ${FLAGS} ./src/main.cpp -o ./bin/main
compile_flags:
	rm compile_flags.txt
	touch compile_flags.txt
	$(foreach flag,\
		$(INCLUDES) $(LIBS) $(FLAGS),\
		echo "$(flag)" >> compile_flags.txt;)
