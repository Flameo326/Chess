#pragma once
#include "ChessBoard.h"
#include "Display.h"
#include "Movement.h"
#include "ChessCondition.h"

namespace Chess {

	class ChessGame {
	private:
		Movement movement;
		Display display;
		ChessBoard board;	
		bool isWhite = true;

		// Goes through each player util StaleMate or CheckMate occurs
		bool playerTurn(bool);

		// Get the Piece's mvmt and act accordingly
		bool move(std::bitset<64> &, bool, bool);
	public:
		static const char* AUTO_SAVE_FILE_PATH;

		ChessGame();

		void run();
		bool save(const char* file);
		bool load(const char* file);
	};
}