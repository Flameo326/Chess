
#include "ChessGame.h"
#include "Display.h"
#include <iostream>
#include <ConsoleIO.h>
#include <fstream>

namespace Chess {

	const char* ChessGame::AUTO_SAVE_FILE_PATH = "./.autosave.";

	ChessGame::ChessGame() : display{board}, movement{board} {
		board.resetBoard();

		if(load(AUTO_SAVE_FILE_PATH)){
			std::cout << "Loaded previous game\n";
		}
	}

	void ChessGame::run() {
		bool running = true;
		while (running) {
			running = this->playerTurn(isWhite);
			isWhite = !isWhite;
			
			if(!save(AUTO_SAVE_FILE_PATH)){
				std::cout << "Auto save failed\n";
			} 
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

	MapOfCoords ChessGame::GetAllPawnMovements(bool isWhite) {
		//if isWhite is true get all the white pawns, else get all the black pawns
		std::bitset<64> pawns = isWhite ? board.getPawns() & board.getWhiteTeam() : board.getPawns() & board.getBlackTeam();
		MapOfCoords movements;

		//find all the indivdual pawns
		for (size_t j = 0; j < 64; j++)
		{
			//if this index is active this is a piece
			if (pawns[j]) {
				//make this piece into an idvidual bitset
				std::bitset<64> piece;
				piece.set(j);

				//get the Coord of the piece
				Coord pieceCoord = board.Bit64ToXAndY(piece).at(0);

				//find all the movement bits of the piece
				std::bitset<64> movementBits = movement.getPawnMvmt(piece, isWhite);
				//check if there are valid moves
				if (movementBits.count() > 0) {
					//check if there are valid moves
					std::vector<Coord> movementCoordinates = board.Bit64ToXAndY(movementBits);
					//add the movements to the movement map
					movements[pieceCoord] = movementCoordinates;
				}
			}
		}
		return movements;
	}

	MapOfCoords ChessGame::GetAllRookMovements(bool isWhite) {
		//if isWhite is true get all the white rooks, else get all the black rooks
		std::bitset<64> rooks = isWhite ? board.getRooks() & board.getWhiteTeam() : board.getRooks() & board.getBlackTeam();
		MapOfCoords movements;

		//find all the indivdual rooks
		for (size_t j = 0; j < 64; j++)
		{
			//if this index is active this is a piece
			if (rooks[j]) {
				//make this piece into an idvidual bitset
				std::bitset<64> piece;
				piece.set(j);

				//get the Coord of the piece
				std::vector<Coord> pieceCoord = board.Bit64ToXAndY(piece);

				//find all the movement bits of the piece
				std::bitset<64> movementBits = movement.getRookMvmt(piece, isWhite);
				//check if there are valid moves
				if (movementBits.count() > 0) {
					//check if there are valid moves
					std::vector<Coord> movementCoordinates = board.Bit64ToXAndY(movementBits);

					//add the movements to the movement map
					movements[pieceCoord.at(0)] = movementCoordinates;
				}
			}
		}
		return movements;
	}

	MapOfCoords ChessGame::GetAllBishopMovements(bool isWhite) {
		//if isWhite is true get all the white bishops, else get all the black bishops
		std::bitset<64> bishops = isWhite ? board.getBishops() & board.getWhiteTeam() : board.getBishops() & board.getBlackTeam();
		MapOfCoords movements;

		//find all the indivdual bishops
		for (size_t j = 0; j < 64; j++)
		{
			//if this index is active this is a piece
			if (bishops[j]) {
				//make this piece into an idvidual bitset
				std::bitset<64> piece;
				piece.set(j);

				//get the Coord of the piece
				std::vector<Coord> pieceCoord = board.Bit64ToXAndY(piece);

				//find all the movement bits of the piece
				std::bitset<64> movementBits = movement.getBishopMvmt(piece, isWhite);
				//check if there are valid moves
				if (movementBits.count() > 0) {
					//check if there are valid moves
					std::vector<Coord> movementCoordinates = board.Bit64ToXAndY(movementBits);

					//add the movements to the movement map
					movements[pieceCoord.at(0)] = movementCoordinates;
				}
			}
		}
		return movements;
	}

	MapOfCoords ChessGame::GetAllKnightMovements(bool isWhite) {
		//if isWhite is true get all the white Kinghts, else get all the black Knights
		std::bitset<64> knights = isWhite ? board.getKnights() & board.getWhiteTeam() : board.getKnights() & board.getBlackTeam();
		MapOfCoords movements;

		//find all the indivdual Knights
		for (size_t j = 0; j < 64; j++)
		{
			//if this index is active this is a piece
			if (knights[j]) {
				//make this piece into an idvidual bitset
				std::bitset<64> piece;
				piece.set(j);

				//get the Coord of the piece
				std::vector<Coord> pieceCoord = board.Bit64ToXAndY(piece);

				//find all the movement bits of the piece
				std::bitset<64> movementBits = movement.getKnightMvmt(piece, isWhite);
				//check if there are valid moves
				if (movementBits.count() > 0) {
					//check if there are valid moves
					std::vector<Coord> movementCoordinates = board.Bit64ToXAndY(movementBits);

					//add the movements to the movement map
					movements[pieceCoord.at(0)] = movementCoordinates;
				}
			}
		}
		return movements;
	}

	MapOfCoords ChessGame::GetAllQueenMovements(bool isWhite) {
		//Check which team is active and grab all of that teams queens
		std::bitset<64> queens = isWhite ? board.getQueens() & board.getWhiteTeam() : board.getQueens() & board.getBlackTeam();
		MapOfCoords movements;

		//find all the "pieces in the bitset
		for (size_t j = 0; j < 64; j++)
		{
			//if this index is a 1 then it is a piece
			if (queens[j]) {
				std::bitset<64> piece;
				//set the new new bitset equal the the indivual "piece" location
				piece.set(j);

				//make a vector of Coords from the piece bitset
				std::vector<Coord> pieceCoord = board.Bit64ToXAndY(piece);
				
				//get all the potential moves bitset
				std::bitset<64> movementBits = movement.getQueenMvmt(piece, isWhite);
				//if there is a valid move
				if (movementBits.count() > 0) {
					//get the movement coordinates
					std::vector<Coord> movementCoordinates = board.Bit64ToXAndY(movementBits);

					//add the movements coordinates to the map with the key of the piece
					movements[pieceCoord.at(0)] = movementCoordinates;
				}
			}
		}
		return movements;
	}

	MapOfCoords ChessGame::GetAllKingMovements(bool isWhite) {
		//Chek which team is active and grab all of that teams kings
		std::bitset<64> kings = isWhite ? board.getQueens() & board.getWhiteTeam() : board.getQueens() & board.getBlackTeam();
		MapOfCoords map;

		//find all the "pieces" in the bitset
		for (size_t j = 0; j < 64; j++)
		{
			//if one of the elements is on(is = 1 or true)
			if (kings[j]) {
				std::bitset<64> piece;
				//set the new new bitset equal the the indivual "piece" location
				piece.set(j);

				//make a vector of Coords from the piece bitset
				std::vector<Coord> pieceCoord = board.Bit64ToXAndY(piece);

				//get all the potential moves bitset
				std::bitset<64> movementBits = movement.getKingMvmt(piece, isWhite);
				//if there is a valid move
				if (movementBits.count() > 0) {
					//get the movement coordinates
					std::vector<Coord> movementCoordinates = board.Bit64ToXAndY(movementBits);
					//add the movements coordinates to the map with the key of the piece
					map[pieceCoord.at(0)] = movementCoordinates;
				}
			}
		}
		return map;
	}

	MapOfCoords ChessGame::getAllAvailableMoves(bool isWhite)
	{
		MapOfCoords allPossibleMoves;

		//Get the potential Pawn movments
		MapOfCoords pawnMovements = GetAllPawnMovements(isWhite);
		//Get the potential Bishop movements
		MapOfCoords bishopMovments = GetAllBishopMovements(isWhite);
		//Get the potential Rook movments
		MapOfCoords rookMovments = GetAllRookMovements(isWhite);
		//Get the potential Knight movments
		MapOfCoords knightMovements = GetAllKnightMovements(isWhite);
		//Get the potential Queen movments
		MapOfCoords queenMovements = GetAllQueenMovements(isWhite);
		//Get the potential King movements
		MapOfCoords kingMovements = GetAllQueenMovements(isWhite);

		//add the pawn movements to the map
		allPossibleMoves.insert(pawnMovements.begin(), pawnMovements.end());
		//add the bishop movements to the map
		allPossibleMoves.insert(bishopMovments.begin(), bishopMovments.end());
		//add the rook movements to the map
		allPossibleMoves.insert(rookMovments.begin(), rookMovments.end());
		//add the knight movments to the map
		allPossibleMoves.insert(knightMovements.begin(), knightMovements.end());
		//add the queen movments to the map
		allPossibleMoves.insert(queenMovements.begin(), queenMovements.end());
		//add the king movements to the map
		allPossibleMoves.insert(kingMovements.begin(), kingMovements.end());

		//return a unordered map of all the possible moves
		return allPossibleMoves;
	}

	bool ChessGame::save(const char* filePath) {
		// save playerturn
		// save Board
		std::ofstream file(filePath, std::ios::binary | std::ios::trunc | std::ios::out);
		if (file) {
			file << isWhite; // Save Turn
			file << '\n'; // Add Newline Format
			file << board;// Save Board
			file.close();
			return true;
		} 
		return false;
	}

	bool ChessGame::load(const char* filePath) {
		std::ifstream file(filePath, std::ios::binary | std::ios::in);
		if (file) {
			file >> isWhite; // Load Turn
			file.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore newline
			file >> board; // Load Board
			file.close();
			return true;
		}
		return false;
	}
}