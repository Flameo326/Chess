#include "ChessGame.h"
#include <iostream>

int main() {
	//std::cout << sizeof(std::bitset<64>) << std::endl;

	// This will create a new Chess Game
	Chess::newGame();

	// Manually construct a new board for playing...
	//Chess::newGame(new Board);

	std::cin.ignore();
	return 0;
}