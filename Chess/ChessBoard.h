#pragma once

//#include <utility>
#include <bitset>
#include <vector>

namespace Chess {

	class ChessBoard {
	public:
		const static int BOARD_LENGTH = 8;

	private:
		// In the Form "P6163" Type, PrevX, PrevY, NewX, NewY
		/*static*/ std::vector<std::string> history;

		// Board will be in the form of bitwise
		// bit 1 off = 0 = no pieces
		// bit 1 on = 1 = board[0][0] occupied
		// bit 7 on = 128 = 2^7 = board[7][0] occupied
		// bit 8 on = 256 = 2^8 = board[0][1] occupied
		// bit 9 on = 512 = 2^9 = board[1][1] occupied
		// - Looks like 0b0000'0000'1;
		std::bitset<64> whiteTeamPos;
		std::bitset<64> blackTeamPos;

		std::bitset<64> pawns;
		std::bitset<64> bishops;
		std::bitset<64> knights;
		std::bitset<64> rooks;
		std::bitset<64> queens;
		std::bitset<64> kings;
	public:

		// Resets all pieces and sets them to default
		void resetBoard();

		ChessBoard simulateMovement(std::bitset<64> &piece, std::bitset<64> &dest);

		void moveChessPiece(std::bitset<64> &piece, std::bitset<64> &dest);
		void removeChessPiece(std::bitset<64> &piece, bool isWhite);
		void promote(std::bitset<64> &pawnPos, char type);

		static std::vector<std::pair<int, int>> Bit64ToXAndY(std::bitset<64>&);
		static std::bitset<64> XAndYToBit64(int x, int y);
		static std::vector<std::bitset<64>> BitsToBit(std::bitset<64> &bits);

		std::bitset<64> getWhiteTeam() const { return whiteTeamPos; }
		std::bitset<64> getBlackTeam() const { return blackTeamPos; }
		std::bitset<64> getPawns() const { return pawns; }
		std::bitset<64> getRooks() const { return rooks; }
		std::bitset<64> getKnights() const { return knights; }
		std::bitset<64> getBishops() const { return bishops; }
		std::bitset<64> getQueens() const { return queens; }
		std::bitset<64> getKings() const { return kings; }
		const std::vector<std::string>& getHistory() const { return history; }

		bool operator==(const ChessBoard &temp) const;
	};
}
