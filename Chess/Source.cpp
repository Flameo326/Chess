#include "ChessGame.h"
#include "ChessBoard.h"
#include <iostream>
#include <fstream>

int main() {
	Chess::ChessGame game;
	game.run();

	std::cin.ignore();
	return 0;
}