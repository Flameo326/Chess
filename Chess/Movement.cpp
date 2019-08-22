
#include "Movement.h"
#include "ChessCondition.h"

namespace Chess {

	std::bitset<64> Movement::getPawnMvmt(std::bitset<64> &pawnPos, bool isWhite) {
		if (pawnPos.count() != 1) { return 0; }

		std::bitset<64> pawnMvmt;
		
		auto XAndYPairs = board.Bit64ToXAndY(pawnPos);
		int pawnX = XAndYPairs.at(0).first, pawnY = XAndYPairs.at(0).second;

		auto ally = (isWhite ? board.getWhiteTeam() : board.getBlackTeam());
		auto enemy = (isWhite ? board.getBlackTeam() : board.getWhiteTeam());
		
		int bit;
		if (isWhite) {
			// Test if nothing is in front of
			bit = pawnX + (pawnY - 1) * 8;
			if (!(ally | enemy).test(bit)) {
				if (simulateMovement(pawnPos, bit, isWhite)) {
					pawnMvmt.set(bit);

					// Test if capable of moving 2
					bit = pawnX + (pawnY - 2) * 8;
					if (pawnY == 6 && !(ally | enemy).test(bit)) {
						pawnMvmt.set(bit);
					}
				}
			}

			// Test if Attack Right
			bit = pawnX + 1 + (pawnY - 1) * 8;
			if (pawnX + 1 < 8 && board.getBlackTeam().test(bit)) {
				if (simulateMovement(pawnPos, bit, isWhite)) { pawnMvmt.set(bit); }
			}

			// Test if Attack Left
			bit = pawnX - 1 + (pawnY - 1) * 8;
			if (pawnX - 1 >= 0 && board.getBlackTeam().test(bit)) {
				if (simulateMovement(pawnPos, bit, isWhite)) { pawnMvmt.set(bit); }
			}
		} else {
			// Test if can move in 
			bit = pawnX + (pawnY + 1) * 8;
			if (!(ally | enemy).test(bit)) {

				if (simulateMovement(pawnPos, bit, isWhite)) {
					pawnMvmt.set(bit);

					// Test if capable of moving 2
					bit = pawnX + (pawnY + 2) * 8;
					if (pawnY == 1 && !(ally | enemy).test(bit)) {
						// We do not need to test here because it can not cause a check by attack
						// - and it can only cause a check by moving.
						// We already check for Check by moving in the first
						pawnMvmt.set(bit);
					}
				}
			}

			// Test if Attack Left
			bit = pawnX + 1 + (pawnY + 1) * 8;
			if (pawnX + 1 < 8 && board.getWhiteTeam().test(bit)) {
				if (simulateMovement(pawnPos, bit, isWhite)) { pawnMvmt.set(bit); }
			}

			// Test if Attack Right
			bit = pawnX - 1 + (pawnY + 1) * 8;
			if (pawnX - 1 >= 0 && board.getWhiteTeam().test(bit)) {
				if (simulateMovement(pawnPos, bit, isWhite)) { pawnMvmt.set(bit); }
			}
		}

		return pawnMvmt;
	}

	std::bitset<64> Movement::getRookMvmt(std::bitset<64> &rookPos, bool isWhite) {
		if (rookPos.count() != 1) { return 0; }

		auto XAndY = board.Bit64ToXAndY(rookPos);
		int rookX = XAndY.at(0).first, rookY = XAndY.at(0).second;

		std::bitset<64> enemy = (isWhite ? board.getBlackTeam() : board.getWhiteTeam());
		std::bitset<64> ally = (isWhite ? board.getWhiteTeam() : board.getBlackTeam());

		std::bitset<64> rookMvmt;

		// Right
		int bit, i = rookX;
		while (++i < 8) {
			bit = i + rookY * 8;
			if (ally.test(bit)) {
				break;
			}
			if (enemy.test(bit)) {
				if (simulateMovement(rookPos, bit, isWhite)) { rookMvmt.set(bit); }
				break;
			}

			if (simulateMovement(rookPos, bit, isWhite)) { rookMvmt.set(bit); }
		}

		// Left
		i = rookX;
		while (--i >= 0) {
			bit = i + rookY * 8;
			if (ally.test(bit)) {
				break;
			}
			if (enemy.test(bit)) {
				if (simulateMovement(rookPos, bit, isWhite)) { rookMvmt.set(bit); }
				break;
			}

			if (simulateMovement(rookPos, bit, isWhite)) { rookMvmt.set(bit); }
		}

		// Forward
		i = rookY;
		while (--i >= 0) {
			bit = rookX + i * 8;
			if (ally.test(bit)) {
				break;
			}
			if (enemy.test(bit)) {
				if (simulateMovement(rookPos, bit, isWhite)) { rookMvmt.set(bit); }
				break;
			}

			if (simulateMovement(rookPos, bit, isWhite)) { rookMvmt.set(bit); }
		}

		// Down
		i = rookY;
		while (++i < 8) {
			bit = rookX + i * 8;
			if (ally.test(bit)) {
				break;
			}
			if (enemy.test(bit)) {
				if (simulateMovement(rookPos, bit, isWhite)) { rookMvmt.set(bit); }
				break;
			}

			if (simulateMovement(rookPos, bit, isWhite)) { rookMvmt.set(bit); }
		}

		return rookMvmt;
	}

	std::bitset<64> Movement::getBishopMvmt(std::bitset<64> &bishopPos, bool isWhite) {
		if (bishopPos.count() != 1) { return 0; }

		auto XAndY = board.Bit64ToXAndY(bishopPos);
		int bishopX = XAndY.at(0).first, bishopY = XAndY.at(0).second;

		std::bitset<64> enemy = (isWhite ? board.getBlackTeam() : board.getWhiteTeam());
		std::bitset<64> ally = (isWhite ? board.getWhiteTeam() : board.getBlackTeam());

		std::bitset<64> bishopMvmt;

		// Down Right
		int bit, x = bishopX, y = bishopY;
		while (++x < 8 && ++y < 8) {
			bit = x + y * 8;
			if (ally.test(bit)) {
				break;
			}
			if (enemy.test(bit)) {
				if (simulateMovement(bishopPos, bit, isWhite)) { bishopMvmt.set(bit); }
				break;
			}

			if (simulateMovement(bishopPos, bit, isWhite)) { bishopMvmt.set(bit); }
		}

		// Down Left
		x = bishopX;
		y = bishopY;
		while (--x >= 0 && ++y < 8) {
			bit = x + y * 8;
			if (ally.test(bit)) {
				break;
			}
			if (enemy.test(bit)) {
				if (simulateMovement(bishopPos, bit, isWhite)) { bishopMvmt.set(bit); }
				break;
			}

			if (simulateMovement(bishopPos, bit, isWhite)) { bishopMvmt.set(bit); }
		}

		// Up Left
		x = bishopX;
		y = bishopY;
		while (--x >= 0 && --y >= 0) {
			bit = x + y * 8;
			if (ally.test(bit)) {
				break;
			}
			if (enemy.test(bit)) {
				if (simulateMovement(bishopPos, bit, isWhite)) { bishopMvmt.set(bit); }
				break;
			}

			if (simulateMovement(bishopPos, bit, isWhite)) { bishopMvmt.set(bit); }
		}

		// Up Right
		x = bishopX;
		y = bishopY;
		while (++x < 8 && --y >= 0) {
			bit = x + y * 8;
			if (ally.test(bit)) {
				break;
			}
			if (enemy.test(bit)) {
				if (simulateMovement(bishopPos, bit, isWhite)) { bishopMvmt.set(bit); }
				break;
			}

			if (simulateMovement(bishopPos, bit, isWhite)) { bishopMvmt.set(bit); }
		}

		return bishopMvmt;
	}

	std::bitset<64> Movement::getKnightMvmt(std::bitset<64> &knightPos, bool isWhite) {
		if (knightPos.count() != 1) { return 0; }

		auto XAndY = board.Bit64ToXAndY(knightPos);
		int knightX = XAndY.at(0).first, knightY = XAndY.at(0).second;

		std::bitset<64> ally = (isWhite ? board.getWhiteTeam() : board.getBlackTeam());

		std::bitset<64> knightMvmt;

		// Knight can either move 1 or 2 spaces once for a given position
		//int moveX = 2, moveY = 1;

		int bit;

		// Right Down
		if (knightX + 1 < 8 && knightY + 2 < 8) {
			bit = knightX + 1 + (knightY + 2) * 8;
			if (!ally.test(bit) && simulateMovement(knightPos, bit, isWhite)) {
				knightMvmt.set(bit);
			}
		}

		//bit = knightX + 2 + (knightY + 1) * 8;
		if (knightX + 2 < 8 && knightY + 1 < 8) {
			bit = knightX + 2 + (knightY + 1) * 8;
			if (!ally.test(bit) && simulateMovement(knightPos, bit, isWhite)) {
				knightMvmt.set(bit);
			}
		}

		// Left Down
		//bit = knightX - 1 + (knightY + 2) * 8;
		if (knightX - 1 >= 0 && knightY + 2 < 8) {
			bit = knightX - 1 + (knightY + 2) * 8;
			if (!ally.test(bit) && simulateMovement(knightPos, bit, isWhite)) {
				knightMvmt.set(bit);
			}
		}

		//bit = knightX - 2 + (knightY + 1) * 8;
		if (knightX - 2 >= 0 && knightY + 1 < 8) {
			bit = knightX - 2 + (knightY + 1) * 8;
			if (!ally.test(bit) && simulateMovement(knightPos, bit, isWhite)) {
				knightMvmt.set(bit);
			}
		}

		// Right Up
		//bit = knightX + 2 + (knightY - 1) * 8;
		if (knightX + 2 < 8 && knightY - 1 >= 0) {
			bit = knightX + 2 + (knightY - 1) * 8;
			if (!ally.test(bit) && simulateMovement(knightPos, bit, isWhite)) {
				knightMvmt.set(bit);
			}
		}

		//bit = knightX + 1 + (knightY - 2) * 8;
		if (knightX + 1 < 8 && knightY - 2 >= 0) {
			bit = knightX + 1 + (knightY - 2) * 8;
			if (!ally.test(bit) && simulateMovement(knightPos, bit, isWhite)) {
				knightMvmt.set(bit);
			}
		}

		// Left Up
		//bit = knightX - 1 + (knightY - 2) * 8;
		if (knightX - 1 >= 0 && knightY - 2 >= 0) {
			bit = knightX - 1 + (knightY - 2) * 8;
			if (!ally.test(bit) && simulateMovement(knightPos, bit, isWhite)) {
				knightMvmt.set(bit);
			}
		}

		//bit = knightX - 2 + (knightY - 1) * 8;
		if (knightX - 2 >= 0 && knightY - 1 >= 0) {
			bit = knightX - 2 + (knightY - 1) * 8;
			if (!ally.test(bit) && simulateMovement(knightPos, bit, isWhite)) {
				knightMvmt.set(bit);
			}
		}

		return knightMvmt;
	}
	
	std::bitset<64> Movement::getQueenMvmt(std::bitset<64> &queenPos, bool isWhite) {
		if (queenPos.count() != 1) { return 0; }

		std::bitset<64> queenMvmt;
		
		queenMvmt |= getRookMvmt(queenPos, isWhite);
		queenMvmt |= getBishopMvmt(queenPos, isWhite);

		return queenMvmt;
	}

	std::bitset<64> Movement::getKingMvmt(std::bitset<64> &kingPos, bool isWhite) {
		if (kingPos.count() != 1) { return 0; }
		
		std::bitset<64> kingMvmt;

		auto XAndYPairs = board.Bit64ToXAndY(kingPos);
		int kingX = XAndYPairs.at(0).first, kingY = XAndYPairs.at(0).second;

		std::bitset<64> enemy = (isWhite ? board.getBlackTeam() : board.getWhiteTeam());
		std::bitset<64> ally = (isWhite ? board.getWhiteTeam() : board.getBlackTeam());

		int bit;
		if (kingY > 0) {
			// Simulate Movement for Top Center
			bit = kingX + (kingY - 1) * 8;
			if (!ally.test(bit)) {
				if (simulateMovement(kingPos, bit, isWhite)) { kingMvmt.set(bit); }
			}
				
			// Top Left
			bit = kingX - 1 + (kingY - 1) * 8;
			if (kingX > 0 && !ally.test(bit)) {
				if (simulateMovement(kingPos, bit, isWhite)) { kingMvmt.set(bit); }
			}
				
			// top Right
			bit = kingX + 1 + (kingY - 1) * 8;
			if (kingX < 7 && !ally.test(bit)) {
				if (simulateMovement(kingPos, bit, isWhite)) { kingMvmt.set(bit); }
			}
		}
		if (kingY < 7) {
			// Bottom center
			bit = kingX + (kingY + 1) * 8;
			if (!ally.test(bit)) {
				if (simulateMovement(kingPos, bit, isWhite)) { kingMvmt.set(bit); }
			}
				
			// Bottom left
			bit = kingX - 1 + (kingY + 1) * 8;
			if (kingX > 0 && !ally.test(bit)) {
				if (simulateMovement(kingPos, bit, isWhite)) { kingMvmt.set(bit); }
			}

			// Bottom right
			bit = kingX + 1 + (kingY + 1) * 8;
			if (kingX < 7 && !ally.test(bit)) {
				if (simulateMovement(kingPos, bit, isWhite)) { kingMvmt.set(bit); }
			} 
		}

		// Center left
		bit = kingX - 1 + kingY * 8;
		if (kingX > 0 && !ally.test(bit)) {
			if (simulateMovement(kingPos, bit, isWhite)) { kingMvmt.set(bit); }
		}

		// Center Right 
		bit = kingX + 1 + kingY * 8;
		if (kingX < 7 && !ally.test(bit)) {
			if (simulateMovement(kingPos, bit, isWhite)) { kingMvmt.set(bit); }
		} 	

		return kingMvmt;
	}
	
	std::bitset<64> Movement::getTeamMvmt(bool isWhite) {
		std::bitset<64> mvmt;
		auto team = (isWhite ? board.getWhiteTeam() : board.getBlackTeam());

		{
			auto pawns = board.BitsToBit(board.getPawns() & team);
			for (int i = 0; i < pawns.size(); i++) {
				mvmt |= getPawnMvmt(pawns[i], isWhite);
			}
		}

		{
			auto rooks = board.BitsToBit(board.getRooks() & team);
			for (int i = 0; i < rooks.size(); i++) {
				mvmt |= getRookMvmt(rooks[i], isWhite);
			}
		}

		{
			auto knights = board.BitsToBit(board.getKnights() & team);
			for (int i = 0; i < knights.size(); i++) {
				mvmt |= getKnightMvmt(knights[i], isWhite);
			}
		}

		{
			auto bishops = board.BitsToBit(board.getBishops() & team);
			for (int i = 0; i < bishops.size(); i++) {
				mvmt |= getBishopMvmt(bishops[i], isWhite);
			}
		}

		{
			auto queens = board.BitsToBit(board.getQueens() & team);
			for (int i = 0; i < queens.size(); i++) {
				mvmt |= getQueenMvmt(queens[i], isWhite);
			}
		}

		{
			auto king = board.getKings() & team;
			mvmt |= getKingMvmt(king, isWhite);
		}
		return mvmt;
	}

	MapOfCoords Movement::getAllPawnMovements(bool isWhite) {
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
				std::bitset<64> movementBits = getPawnMvmt(piece, isWhite);
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

	MapOfCoords Movement::getAllRookMovements(bool isWhite) {
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
				std::bitset<64> movementBits = getRookMvmt(piece, isWhite);
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

	MapOfCoords Movement::getAllBishopMovements(bool isWhite) {
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
				std::bitset<64> movementBits = getBishopMvmt(piece, isWhite);
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

	MapOfCoords Movement::getAllKnightMovements(bool isWhite) {
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
				std::bitset<64> movementBits = getKnightMvmt(piece, isWhite);
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

	MapOfCoords Movement::getAllQueenMovements(bool isWhite) {
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
				std::bitset<64> movementBits = getQueenMvmt(piece, isWhite);
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

	MapOfCoords Movement::getAllKingMovements(bool isWhite) {
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
				std::bitset<64> movementBits = getKingMvmt(piece, isWhite);
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

	MapOfCoords Movement::getAllAvailableMoves(bool isWhite)
	{
		MapOfCoords allPossibleMoves;

		//Get the potential Pawn movments
		MapOfCoords pawnMovements = getAllPawnMovements(isWhite);
		//Get the potential Bishop movements
		MapOfCoords bishopMovments = getAllBishopMovements(isWhite);
		//Get the potential Rook movments
		MapOfCoords rookMovments = getAllRookMovements(isWhite);
		//Get the potential Knight movments
		MapOfCoords knightMovements = getAllKnightMovements(isWhite);
		//Get the potential Queen movments
		MapOfCoords queenMovements = getAllQueenMovements(isWhite);
		//Get the potential King movements
		MapOfCoords kingMovements = getAllQueenMovements(isWhite);

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

	// Checks position of Pawns,
	// Then Checks History
	std::bitset<64> Movement::checkForEnPassant(std::bitset<64> &king, std::bitset<64> &pawnPos, bool isWhite) {
		// Check if Last move was pawn
		auto history = board.getHistory();
		if (history.size() == 0) { return 0; }

		auto lastMove = history.at(history.size() - 1);
		if (lastMove.at(0) == 'P') {

			// Check if It moved 2 spaces
			if (abs(lastMove.at(2) - lastMove.at(4)) == 2) {
				auto XAndY = board.Bit64ToXAndY(pawnPos);
				int pawnY = XAndY.at(0).second;
				int moveY = lastMove.at(4) - '0';
				int pawnX = XAndY.at(0).first;
				int moveX = lastMove.at(3) - '0';

				// Check if they are now at the same Y value, 
				if (moveY == pawnY && abs(moveX - pawnX) == 1) {
					// '1' - '0' - ('1' - '3')/2
					// 1 - (-2)/2
					// 1 - -1
					// 1 + 1 = 2 !

					// '6' - '0' - ('6' - '4')/2
					// 6 - (2)/2
					// 6 - 1 = 5 !
					int x = lastMove.at(1) - '0' + (lastMove.at(3) - lastMove.at(1)) / 2;
					int y = lastMove.at(2) - '0' + (lastMove.at(4) - lastMove.at(2)) / 2;

					// Simulate movement of pawn
					ChessBoard tempBoard = board.simulateMovement(pawnPos, board.XAndYToBit64(x, y));

					// Remove enPassant piece
					tempBoard.removeChessPiece(tempBoard.XAndYToBit64(x, pawnY), isWhite);

					if (checkForCheck && Chess::checkForCheck(tempBoard, isWhite)) {
						return 0;
					} else {
						return board.XAndYToBit64(x, y);
					}
				}
			}
		}
		return 0;
	}

	// Checks positions to make sure thay are valid
	// Checks History to make sure haven't moved
	// - Long to X and Y
	std::bitset<64> Movement::checkForCastle(std::bitset<64> &kingPos, bool isWhite, bool kingSide) {

		// check if King is in original space
		int kingX = 4;
		int kingY = (isWhite ? 7 : 0);

		if ((kingPos & board.XAndYToBit64(kingX, kingY)) == 0) {
			// King not in original pos...
			return 0;
		}

		// Check if rook is in original space
		int rookX = (kingSide ? 7 : 0);
		int rookY = (isWhite ? 7 : 0);

		auto rooks = board.getRooks() & (isWhite ? board.getWhiteTeam() : board.getBlackTeam());
		if ((rooks & board.XAndYToBit64(rookX, rookY)) == 0) {
			// Not in original pos...
			return 0;
		}

		// Check space in between them...
		int mvmtX = (kingSide ? kingX : rookX);
		for (int i = (kingSide ? kingX : rookX) + 1; i < (kingSide ? rookX : kingX); i++) {
			auto pos = board.XAndYToBit64(i, (isWhite ? 7 : 0));
			if ((pos & (board.getBlackTeam() | board.getWhiteTeam())) != 0) {
				return 0;
			}
		}

		// We've assured that the rook and King are in the original spaces, we need to check to see if
		// - If they moved from them. The quickest way to do that would be to check the dest of move
		auto history = board.getHistory();
		for (int i = 0; i < history.size(); i++) {
			auto move = history.at(i);
			int moveX = move.at(3) - '0';
			int moveY = move.at(4) - '0';
			// If either has moved then 
			if ((rookX == moveX && rookY == moveY) || (kingX == moveX && kingY == moveY)) {
				return 0;
			}
		}

		// Y will be original value
		int toX = (kingSide ? 6 : 2);

		// Check if the king would go into check as a result of moving...
		for (int i = (kingSide ? kingX : toX) + 1; i < (kingSide ? toX : kingX); i++) {
			// Simulate movement
			ChessBoard tempBoard = board.simulateMovement(kingPos, board.XAndYToBit64(i, kingY));

			// King would go into check
			if (checkForCheck && Chess::checkForCheck(tempBoard, isWhite)) {
				return 0;
			}
		}
		// Simulate final movement of king
		std::bitset<64> dest = board.XAndYToBit64((kingSide ? toX : kingX), kingY);
		ChessBoard tempBoard = board.simulateMovement(kingPos, dest);

		// Move rook
		if (kingSide) {
			tempBoard.moveChessPiece(tempBoard.XAndYToBit64(7, kingY), tempBoard.XAndYToBit64(5, kingY));
		} else {
			tempBoard.moveChessPiece(tempBoard.XAndYToBit64(0, kingY), tempBoard.XAndYToBit64(3, kingY));
		}

		if (checkForCheck && Chess::checkForCheck(tempBoard, isWhite)) {
			return 0;
		} else {
			return board.XAndYToBit64(toX, kingY);
		}
	}

	bool Movement::checkForPromotion(std::bitset<64> &pawnPos, bool isWhite) {
		bool promotion = false;
		auto XAndY = board.Bit64ToXAndY(pawnPos);
		int y = XAndY.at(0).second;
		if (isWhite) {
			promotion = y == 1;
		} else {
			promotion = y == 6;
		}
		return promotion;
	}

	bool Movement::simulateMovement(std::bitset<64> &piece, int bit, bool isWhite) {
		if (checkForCheck) {
			std::bitset<64> dest;
			dest.set(bit);
			ChessBoard tempBoard = board.simulateMovement(piece, dest);
			if (!Chess::checkForCheck(tempBoard, isWhite)) {
				return true;
			}
		} else {
			return true;
		}
		return false;
	}
}