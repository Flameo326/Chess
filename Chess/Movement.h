#pragma once

#include "ChessBoard.h"
#include "Display.h"

namespace Chess {

	class Movement {
	private:
		ChessBoard &board;
		bool checkForCheck = true;
	public:
		Movement(ChessBoard &b) : board(b) {}
		Movement(ChessBoard &b, bool check) : board(b), checkForCheck(check) {}

		// I may need to pass in the ally team as well
		std::bitset<64> getPawnMvmt(std::bitset<64> &pawnPos, bool isWhite);
		std::bitset<64> getRookMvmt(std::bitset<64> &rookPos, bool isWhite);
		std::bitset<64> getBishopMvmt(std::bitset<64> &bishopPos, bool isWhite);
		std::bitset<64> getKnightMvmt(std::bitset<64> &knightPos, bool isWhite);
		std::bitset<64> getQueenMvmt(std::bitset<64> &queenPos, bool isWhite);
		std::bitset<64> getKingMvmt(std::bitset<64> &kingPos, bool isWhite);
		std::bitset<64> getTeamMvmt(bool isWhite);

		//getting movements of individual peices on the board
		MapOfCoords getAllPawnMovements(bool isWhite);
		MapOfCoords getAllRookMovements(bool isWhite);
		MapOfCoords getAllBishopMovements(bool isWhite);
		MapOfCoords getAllKnightMovements(bool isWhite);
		MapOfCoords getAllQueenMovements(bool isWhite);
		MapOfCoords getAllKingMovements(bool isWhite);

		//Get all Available Moves
		MapOfCoords getAllAvailableMoves(bool isWhite);

		// Checks position of Pawns,
		// Then Checks History
		std::bitset<64> checkForEnPassant(std::bitset<64> &king, std::bitset<64> &pawnPos, bool isWhit);

		// Checks positions to make sure thay are valid
		// Checks History to make sure haven't moved
		// - Long to X and Y
		std::bitset<64> checkForCastle(std::bitset<64> &kingPos, bool isWhite, bool kingSide);

		bool checkForPromotion(std::bitset<64> &pawnPos, bool isWhite);

		// Pass in all values to simulate movement to that square and detect if a check will occur
		bool simulateMovement(std::bitset<64> &piece, int bit, bool isWhite);

		void setCheckForCheck(bool value) { checkForCheck = value; }
	};
}
