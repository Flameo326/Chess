#pragma once
#include "ChessBoard.h"
#include "Display.h"
#include "Movement.h"
#include "ChessCondition.h"

namespace Chess {

	class ChessGame {
	private:
		Movement movement;
		Display &display;
		ChessBoard &board;

		ChessGame(ChessBoard &b, Display &d) : board(b), display(d), movement { b } {}

		// Goes through each player util StaleMate or CheckMate occurs
		bool playerTurn(bool);

		// Get the Piece's mvmt and act accordingly
		bool move(std::bitset<64> &, bool, bool);
	public:
		//ChessGame();
	//	ChessGame(ChessBoard &b);
//		ChessGame(ChessBoard &b, Display &d);
		friend void newGame();
		friend void newGame(ChessBoard &b);
		friend void newGame(ChessBoard &b, Display &d);
	};
}