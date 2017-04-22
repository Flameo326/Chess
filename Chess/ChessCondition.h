#pragma once
#include "ChessBoard.h"

namespace Chess {
	enum ChessCondition {
		FINE,
		STALEMATE,
		CHECK,
		CHECKMATE
	};

	ChessCondition getCondition(ChessBoard &board, bool isWhite);
	bool checkForCheck(ChessBoard &board, bool isWhite);
	bool checkFor50TurnRule(const ChessBoard &board);
	bool checkFor3FoldRepetition(const ChessBoard &board);
}

