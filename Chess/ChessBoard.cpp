
#include "ChessBoard.h"
#include <string>
#include <iterator>

namespace Chess {

	// 64 bits, ' indicates 4, '' indicates 8
	// 0b0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000;

	void ChessBoard::resetBoard() {
		// Set all values to 0
		whiteTeamPos.reset();
		blackTeamPos.reset();
		pawns.reset();
		bishops.reset();
		knights.reset();
		rooks.reset();
		queens.reset();
		kings.reset();

		// Black Pawn
		std::bitset<64> temp = 0b0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'1111'1111'0000'0000;
		pawns |= temp;
		blackTeamPos |= temp;

		// White Pawn
		temp = 0b0000'0000'1111'1111'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000;
		pawns |= temp;
		whiteTeamPos |= temp;

		// Black Knights
		temp = 0b0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0100'0010;
		knights |= temp;
		blackTeamPos |= temp;

		// White Knights
		temp = 0b0100'0010'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000;
		knights |= temp;
		whiteTeamPos |= temp;

		// Black Rooks
		temp = 0b0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'1000'0001;
		rooks |= temp;
		blackTeamPos |= temp;

		// White Rooks
		temp =  0b1000'0001'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000;
		rooks |= temp;
		whiteTeamPos |= temp;

		// Black Bishops
		temp = 0b0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0010'0100;
		bishops |= temp;
		blackTeamPos |= temp;

		// White Bishops
		temp = 0b0010'0100'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000;
		bishops |= temp;
		whiteTeamPos |= temp;

		// Black Queen
		temp = 0b0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'1000;
		queens |= temp;
		blackTeamPos |= temp;

		// White Queen
		temp = 0b0000'1000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000;
		queens |= temp;
		whiteTeamPos |= temp;

		// Black King
		temp = 0b0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0001'0000;
		kings |= temp;
		blackTeamPos |= temp;

		// White King
		temp = 0b0001'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000;
		kings |= temp;
		whiteTeamPos |= temp;
	}

	ChessBoard ChessBoard::simulateMovement(std::bitset<64> &piece, std::bitset<64> &dest) {
		ChessBoard temp = *this;

		temp.moveChessPiece(piece, dest);

		return temp;
	}

	void ChessBoard::moveChessPiece(std::bitset<64> &piece, std::bitset<64> &dest) {
		if (piece.count() != 1 || dest.count() != 1) { return; }

		// To Move Piece:
		// - Find what groups the piece is apart of 
		// - White/ Black && Pawn, Knight, Queen, King, etc.
		// - Modify both selections so that piece is in dest in both
		// - Cehck opposing team to see if piece exists there
		// - Remove piece on opposing team and in corresponding group

		// Piece is apart of White
		if ((piece & whiteTeamPos) != 0) { 

			// Remove piece if at dest on enemy team
			removeChessPiece(dest, true);

			// This should reverse the bits, making piece move to dest and leave origional
			whiteTeamPos ^= (dest | piece);

		// Piece is apart of Black
		} else if ((piece & blackTeamPos) != 0) { 

			// Remove piece if at dest on enemy team
			removeChessPiece(dest, false);

			// Should reverse bits
			blackTeamPos ^= (dest | piece);
		}

		std::string temp;
		if ((piece & pawns) != 0) {
			pawns ^= (dest | piece);
			temp += 'P';
		} else if ((piece & rooks) != 0) {
			rooks ^= (dest | piece);
			temp += 'R';
		} else if ((piece & knights) != 0) {
			knights ^= (dest | piece);
			temp += 'N';
		} else if ((piece & bishops) != 0) {
			bishops ^= (dest | piece);
			temp += 'B';
		} else if ((piece & queens) != 0) {
			queens ^= (dest | piece);
			temp += 'Q';
		} else if ((piece & kings) != 0) {
			kings ^= (dest | piece);
			temp += 'K';
		}

		auto sourcePos = Bit64ToXAndY(piece);
		auto destPos = Bit64ToXAndY(dest);

		temp += ('0' + sourcePos.at(0).first);
		temp += ('0' + sourcePos.at(0).second);
		temp += ('0' + destPos.at(0).first);
		temp += ('0' + destPos.at(0).second);
		history.push_back(temp);
	}

	void ChessBoard::promote(std::bitset<64> &pawnPos, char type) {
		if ((pawnPos & pawns) == 0) { return; }

		// Remove from Pawns
		pawns ^= pawnPos;

		switch (type) {
		case 'Q':
			queens |= pawnPos;
			break;
		case 'N':
			knights |= pawnPos;
			break;
		case 'R':
			rooks |= pawnPos;
			break;
		case 'B':
			bishops |= pawnPos;
			break;
		default:
			// Error occured
			break;
		}
	}

	void ChessBoard::removeChessPiece(std::bitset<64> &piece, bool isWhite) {
		// Check if piece is actually apart of 'Team'
		if (isWhite && (piece & blackTeamPos) != 0) {
			blackTeamPos ^= piece;
		} else if((piece & whiteTeamPos) != 0) {
			whiteTeamPos ^= piece;
		} else {
			// Is not part of the map
			return;
		}
		

		if ((piece & pawns) != 0) {
			pawns ^= piece;
		} else if ((piece & rooks) != 0) {
			rooks ^= piece;
		} else if ((piece & knights) != 0) {
			knights ^= piece;
		} else if ((piece & bishops) != 0) {
			bishops ^= piece;
		} else if ((piece & queens) != 0) {
			queens ^= piece;
		} else if ((piece & kings) != 0) {
			kings ^= piece;
		} else {
			// Is not a part of the map
			return;
		}
		auto sourcePos = Bit64ToXAndY(piece);
		std::string temp("C");
		temp += ('0' + sourcePos.at(0).first);
		temp += ('0' + sourcePos.at(0).second);
		temp += ('?'); // not important
		temp += ('?'); // not important
		history.push_back(temp);
		// If not then do nothing, No removing
	}

	bool ChessBoard::operator==(const ChessBoard &temp) const {
		if (whiteTeamPos != temp.whiteTeamPos) { return false; }
		if (blackTeamPos != temp.blackTeamPos) { return false; }
		if (pawns != temp.pawns) { return false; }
		if (rooks != temp.rooks) { return false; }
		if (bishops != temp.bishops) { return false; }
		if (knights != temp.knights) { return false; }
		if (queens != temp.queens) { return false; }
		if (kings != temp.kings) { return false; }
		return true;
	}

	std::vector<std::pair<int, int>> ChessBoard::Bit64ToXAndY(std::bitset<64> &l) {
		if (l == 0) { return { { -1, -1 } }; }

		std::vector<std::pair<int, int>> XandY;
		for (int i = 0; i < l.size(); i++) {
			if (l[i]) {
				XandY.push_back({ i % 8, i / 8 });
			}
		}
		return XandY;
	}
	
	std::bitset<64> ChessBoard::XAndYToBit64(int x, int y) {
		std::bitset<64> temp(0);
		temp.flip(x + y * 8);
		return temp;
	}

	std::vector<std::bitset<64>> ChessBoard::BitsToBit(std::bitset<64> &bits) {
		std::vector<std::bitset<64>> bitVect;
		for (int i = 0; i < bits.size(); i++) {
			if (bits[i]) {
				std::bitset<64> bit;
				bit.set(i);
				bitVect.push_back(bit);
			}
		}
		return bitVect;
	}

	std::ostream& operator<<(std::ostream& out, ChessBoard& board) {
		out << board.whiteTeamPos;
		out << board.blackTeamPos;
		out << board.pawns;
		out << board.bishops;
		out << board.knights;
		out << board.rooks;
		out << board.queens;
		out << board.kings;

		std::copy(board.history.begin(), board.history.end(), std::ostream_iterator<std::string>(out));

		return out;
	}

	std::istream& operator>>(std::istream& in, ChessBoard& board) {
		in >> board.whiteTeamPos;
		in >> board.blackTeamPos;
		in >> board.pawns;
		in >> board.bishops;
		in >> board.knights;
		in >> board.rooks;
		in >> board.queens;
		in >> board.kings;

		std::istream_iterator<std::string> eos; // End of Stream
		std::istream_iterator<std::string> iter(in);

		while (iter != eos) {
			board.history.push_back(*iter);
			iter++;
		}

		return in;
	}
}