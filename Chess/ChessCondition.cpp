
#include "ChessCondition.h"
#include "Movement.h"

namespace Chess {
	ChessCondition getCondition(ChessBoard &board, bool isWhite) {
		// SUGGESTION: Should we pass this in?
		// - What about display?
		Movement movement(board);

		if (checkForCheck(board, isWhite)) {

			auto teamMvmt = movement.getTeamMvmt(isWhite);
			if (teamMvmt == 0) {
				return ChessCondition::CHECKMATE;
			}
			return ChessCondition::CHECK;
		}

		auto teamMvmt = movement.getTeamMvmt(isWhite);
		if (teamMvmt == 0) {
			return ChessCondition::STALEMATE;
		}

		// These rules must be claimed by a player to work...
		// Could always pass in dispplay..
		if (checkFor50TurnRule(board)) { return ChessCondition::STALEMATE; }
		if (checkFor3FoldRepetition(board)) { return ChessCondition::STALEMATE; }

		// Check for insufficient material, i.e. not enough pieces to checkmate

		return ChessCondition::FINE;
	}

	bool checkForCheck(ChessBoard &board, bool isWhite) {
		// Construct movement based on th board
		Movement movement(board, false);

		// Get enemy movement
		auto enemyMvmt = movement.getTeamMvmt(!isWhite);
		auto king = board.getKings() & (isWhite ? board.getWhiteTeam() : board.getBlackTeam());
		
		// If enemy mvmt can land on king, he is in check...
		return (enemyMvmt & king) != 0;
	}

	bool checkFor50TurnRule(const ChessBoard &board) {
		auto history = board.getHistory();
		// Check for 100 because turns consist of both player actions
		if (history.size() < 100) { return false; }

		for (int i = history.size() - 1; i >= history.size() - 100; i--) {
			auto move = history.at(i);
			if (move.at(i) == 'P' || move.at(i) == 'C') {
				return false;
			}
		}
		// 50 Black and 50 White moves have occured without pawn or Capture
		return true;
	}

	bool checkFor3FoldRepetition(const ChessBoard &board) {
		// 3Fold Repeption occurs if the board has been in the same position as previously
		// without any pawn mvmt or captures.

		// Because we will simply be testing this every time, we only need to test the current
		// board position for repetition.

		// SUGGESTION: If we incorporate the request of drawing, we could always store a static bool
		// - and Test that to see if they can withdraw.

		auto history = board.getHistory();

		// Copy of Board
		ChessBoard historyBoard = board;
		int amount = 0;

		// Check for repeition of the initial state
		for (int y = history.size() - 1; y >= 0; y--) {
			auto move = history.at(y);
			if (move.at(0) == 'P' || move.at(0) == 'C') {
				break;
			}
			int prevX = move.at(3) - '0';
			int prevY = move.at(4) - '0';
			int toX = move.at(1) - '0';
			int toY = move.at(2) - '0';
			historyBoard.moveChessPiece(historyBoard.XAndYToBit64(prevX, prevY),
				historyBoard.XAndYToBit64(toX, toY));

			if (historyBoard == board) {
				if (++amount >= 3) {
					return true;
				}
			}
		}

		return false;
	}
}