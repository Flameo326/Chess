#pragma once

#include "ChessBoard.h"
#include "ChessCondition.h"

namespace Chess {
	class Display {
	private:
		const ChessBoard &board;
		
		// Find a way to convert long positions to Chars...
		//char display[8][8];
	public:
		Display(const ChessBoard &b) : board(b) {}

		void displayBoard(bool);
		void displayMvmt(std::bitset<64> &);
		void displayCondition(ChessCondition, bool);

		char getPromotionInput();
		std::bitset<64> getPieceSelection(std::bitset<64>&, bool);
		std::bitset<64> getCoord(bool allowExit);
	};
}
