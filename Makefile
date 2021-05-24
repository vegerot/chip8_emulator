INCLUDES= -I./include -I/usr/local/opt/sdl2/include
#-L adds dirs to lib path, -l is the specific lib (from lib path) to link
LIBS= -L/usr/local/opt/sdl2/lib/ -lSDL2main -lSDL2

BIN= /usr/local/opt/sdl2/bin
CXXFLAGS= -std=c++2a
FLAGS= -g -Wall -Wextra -pedantic

OBJECTS= ./build/chip8memory.o ./build/chip8stack.o ./build/chip8keyboard.o ./build/chip8.o
all: ${OBJECTS}
	c++ ${INCLUDES} ${LIBS} ${FLAGS} ${CXXFLAGS} ${OBJECTS} ./src/main.cpp -o ./bin/main

./build/chip8memory.o:src/chip8memory.cpp
	c++ ${INCLUDES} ${FLAGS} ${CXXFLAGS} ./src/chip8memory.cpp -c -o ./build/chip8memory.o
./build/chip8stack.o:src/chip8stack.cpp
	c++ ${INCLUDES} ${FLAGS} ${CXXFLAGS} ./src/chip8stack.cpp -c -o ./build/chip8stack.o
./build/chip8keyboard.o:src/chip8keyboard.cpp
	c++ ${INCLUDES} ${FLAGS} ${CXXFLAGS} ./src/chip8keyboard.cpp -c -o ./build/chip8keyboard.o
./build/chip8.o:src/chip8.cpp
	c++ ${INCLUDES} ${FLAGS} ${CXXFLAGS} ./src/chip8.cpp -c -o ./build/chip8.o

clean:
	rm -rfv build/*

compile_flags:
	rm compile_flags.txt
	touch compile_flags.txt
	$(foreach flag,\
		$(INCLUDES) $(LIBS) $(FLAGS) $(CXXFLAGS),\
		echo "$(flag)" >> compile_flags.txt;)
