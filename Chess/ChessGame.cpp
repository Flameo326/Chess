
#include "ChessGame.h"
#include "Display.h"
#include <iostream>
#include <ConsoleIO.h>

namespace Chess {

/*	ChessGame::ChessGame() {
		// Initialize Board
		ChessBoard board;
		board.resetBoard();

		newGame(board);

		// Create a player struct
		// Constains Name, AILevel...
	}
	//ChessGame::ChessGame(ChessBoard &b);
	//ChessGame::ChessGame(ChessBoard &b, Display &d);*/
	
	void newGame() {
		ChessBoard board;
		board.resetBoard();

		newGame(board);

		std::cout << "Here" << std::endl;
	}

	void newGame(ChessBoard &board) {
		// Create Console Display
		Display display(board);
		newGame(board, display);
	}

	void newGame(ChessBoard &board, Display &d) {
		// Create Game
		ChessGame game{ board , d };

		// While loop for Player Turn
		bool isWhite = true;
		bool running = true;
		while (running) {
			running = game.playerTurn(isWhite);
			isWhite = !isWhite;
		}
	}

	bool ChessGame::playerTurn(bool isWhite) {
		// Get a chess piece from the player
		// Detect where its movement can be and display it
		// Have Player choose valid movement and move...
		
		bool inCheck = false;
		std::bitset<64> team = (isWhite ? board.getWhiteTeam() : board.getBlackTeam());
		std::bitset<64> piece;
		do {
			// Display the chess board
			display.displayBoard(isWhite);
			
			// Check Condition
			ChessCondition cond = getCondition(board, isWhite);
			if (cond != ChessCondition::FINE) {
				display.displayCondition(cond, isWhite);

				// Checkmate or Stalemate ends game...
				if (cond != ChessCondition::CHECK) {
					return false;
				}
				inCheck = true;
			} 

			// Display and get Chess Piece that will move
			piece = display.getPieceSelection(team, false);
				
		} while (!move(piece, isWhite, inCheck));		

		return true;
	}

	bool ChessGame::move(std::bitset<64> &piece, bool isWhite, bool inCheck) {
		std::bitset<64> king = board.getKings() & (isWhite ? board.getWhiteTeam() : board.getBlackTeam());

		// Display all possible movement
		std::bitset<64> mvmt;

		// Optional items
		std::bitset<64> enPassant, kingCastle, queenCastle;
		bool promotion = false;
		
		if ((piece & board.getPawns()) != 0) {
			mvmt = movement.getPawnMvmt(piece, isWhite);

			enPassant = movement.checkForEnPassant(king, piece, isWhite);
			mvmt |= enPassant;

			promotion = movement.checkForPromotion(piece, isWhite);
		} else if ((piece & board.getRooks()) != 0) {
			mvmt =  movement.getRookMvmt(piece, isWhite);
		} else if ((piece & board.getKnights()) != 0) {
			mvmt =  movement.getKnightMvmt(piece, isWhite);
		} else if ((piece & board.getBishops()) != 0) {
			mvmt =  movement.getBishopMvmt(piece, isWhite);
		} else if ((piece & board.getQueens()) != 0) {
			mvmt =  movement.getQueenMvmt(piece, isWhite);
		} else if ((piece & board.getKings()) != 0) {
			mvmt =  movement.getKingMvmt(piece, isWhite);

			kingCastle = movement.checkForCastle(piece, isWhite, true);
			mvmt |= kingCastle;

			queenCastle = movement.checkForCastle(piece, isWhite, false);
			mvmt |= queenCastle;
		}

		// I should check to make sure none of the movements will cause a check, otherwise remove them..
		
		// What if when we tested for mvmt, we act it out?
		// - For example, we slide the pieces one by one, simulating how it would end up on the board
		// - Then we check the board for ay checks...

		// Display Movement
		display.displayMvmt(mvmt);

		// Choose one space for mvmt
		mvmt = display.getPieceSelection(mvmt, true);
		if (mvmt == 0) {
			return false;
		}

		board.moveChessPiece(piece, mvmt);

		if (promotion) {
			char type = display.getPromotionInput();
			board.promote(mvmt, type);

		} else if (mvmt == enPassant) {
			auto OriginXAndY = board.Bit64ToXAndY(piece);
			auto NewXAndY = board.Bit64ToXAndY(mvmt);
			int newX = NewXAndY.at(0).first;
			int oriY = OriginXAndY.at(0).second;

			board.removeChessPiece(board.XAndYToBit64(newX, oriY), isWhite);
		} else if (mvmt == kingCastle) {
			auto newXAndY = board.Bit64ToXAndY(mvmt);
			auto kingY = newXAndY.at(0).second;

			// move rook
			board.moveChessPiece(board.XAndYToBit64(7, kingY), board.XAndYToBit64(5, kingY));
		} else if (mvmt == queenCastle) {
			auto newXAndY = board.Bit64ToXAndY(mvmt);
			auto kingY = newXAndY.at(0).second;

			board.moveChessPiece(board.XAndYToBit64(0, kingY), board.XAndYToBit64(3, kingY));
		}

		return true;
	}

	
}