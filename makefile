make: all

all:
	g++ main.cpp board.cpp move.cpp game.cpp utils.cpp values.cpp -O2 -o balrog.exe

