
#include "Display.h"
#include <iostream>
#include <ConsoleIO.h>

namespace Chess {

	void setAsMvmtText(std::string&, int);

	void Display::displayBoard(bool isWhite) {
		static const int PIECE_LENGTH = 10;
		static const int CHAR_IN_LINE = ChessBoard::BOARD_LENGTH * PIECE_LENGTH + ChessBoard::BOARD_LENGTH + 1;
		std::bitset<64> pieceBoard = board.getBlackTeam() | board.getWhiteTeam();

		printf("\t");
		for (int i = 0; i < ChessBoard::BOARD_LENGTH; i++) {
			printf("%6d%5c", i + 1, ' ');
		}
		printf("\n");

		char row = 'A';
		for (int i = 0; i < ChessBoard::BOARD_LENGTH; i++, row++) {

			// Print line of "---"
			printf("\t%s\n", std::string(CHAR_IN_LINE, '-').c_str());

			// Print the Row Value, 'A', 'B', etc.
			printf("%c\t", row);
			for (int y = 0; y < ChessBoard::BOARD_LENGTH; y++) {
				std::bitset<64> currPos = ChessBoard::XAndYToBit64(y, i);

				printf("|");

				// Print formatted to center
				std::string line(10, ' ');
				if ((board.getBlackTeam() & currPos) != 0) {
					line = "Black";
				} else if ((board.getWhiteTeam() & currPos) != 0) {
					line = "White";
				}

				int diff = PIECE_LENGTH - line.size();
				if (diff > 0) {
					if (diff % 2 == 1) {
						printf("%*s%*c", line.size() + diff / 2 + 1, line.c_str(), diff / 2, ' ');
					} else {
						printf("%*s%*c", line.size() + diff / 2, line.c_str(), diff / 2, ' ');
					}
				} else {
					printf(line.c_str());
				}
			}
			// End of Line for Piece Color
			printf("|\n\t");

			for (int y = 0; y < ChessBoard::BOARD_LENGTH; y++) {
				std::bitset<64> currPos = ChessBoard::XAndYToBit64(y, i);

				printf("|");

				// Format printing to print in the center
				std::string line(10, ' ');
				if ((board.getPawns() & currPos) != 0) {
					line = "Pawn";
				} else if ((board.getRooks() & currPos) != 0) {
					line = "Rook";
				} else if ((board.getKnights() & currPos) != 0) {
					line = "Knight";
				} else if ((board.getBishops() & currPos) != 0) {
					line = "Bishop";
				} else if ((board.getQueens() & currPos) != 0) {
					line = "Queen";
				} else if ((board.getKings() & currPos) != 0) {
					line = "King";
				}

				int diff = PIECE_LENGTH - line.size();
				if (diff > 0) {
					if (diff % 2 == 1) {
						printf("%*s%*c", line.size() + diff / 2 + 1, line.c_str(), diff / 2, ' ');
					} else {
						printf("%*s%*c", line.size() + diff / 2, line.c_str(), diff / 2, ' ');
					}
				} else {
					printf(line.c_str());
				}
			}
			// End of Line
			printf("|\n");
		}
		// Print a  Line of "----"
		printf("\t%s\n", std::string(CHAR_IN_LINE, '-').c_str());

		std::cout << "It is " << (isWhite ? "White" : "Black") << "'s turn." << std::endl;
	}

	void Display::displayMvmt(std::bitset<64> &mvmt) {
		static const int PIECE_LENGTH = 10;
		static const int CHAR_IN_LINE = ChessBoard::BOARD_LENGTH * PIECE_LENGTH + ChessBoard::BOARD_LENGTH + 1;
		std::bitset<64> pieceBoard = board.getBlackTeam() | board.getWhiteTeam();

		printf("\t");
		for (int i = 0; i < ChessBoard::BOARD_LENGTH; i++) {
			printf("%6d%5c", i + 1, ' ');
		}
		printf("\n");

		char row = 'A';
		for (int i = 0; i < ChessBoard::BOARD_LENGTH; i++, row++) {

			// Print line of "---"
			printf("\t%s\n", std::string(CHAR_IN_LINE, '-').c_str());

			// Print the Row Value, 'A', 'B', etc.
			printf("%c\t", row);
			for (int y = 0; y < ChessBoard::BOARD_LENGTH; y++) {
				std::bitset<64> currPos = ChessBoard::XAndYToBit64(y, i);

				printf("|");

				// Print formatted to center
				std::string line(10, ' ');
				if ((board.getBlackTeam() & currPos) != 0) {
					line = "Black";
				} else if ((board.getWhiteTeam() & currPos) != 0) {
					line = "White";
				}

				if ((currPos & mvmt) != 0) {
					setAsMvmtText(line, PIECE_LENGTH);
					printf(line.c_str());
				} else {
					int diff = PIECE_LENGTH - line.size();
					if (diff > 0) {
						if (diff % 2 == 1) {
							printf("%*s%*c", line.size() + diff / 2 + 1, line.c_str(), diff / 2, ' ');
						} else {
							printf("%*s%*c", line.size() + diff / 2, line.c_str(), diff / 2, ' ');
						}
					} else {
						printf(line.c_str());
					}
				}

			}
			// End of Line for Piece Color
			printf("|\n\t");

			for (int y = 0; y < ChessBoard::BOARD_LENGTH; y++) {
				std::bitset<64> currPos = ChessBoard::XAndYToBit64(y, i);

				printf("|");

				// Format printing to print in the center
				std::string line(10, ' ');
				if ((board.getPawns() & currPos) != 0) {
					line = "Pawn";
				} else if ((board.getRooks() & currPos) != 0) {
					line = "Rook";
				} else if ((board.getKnights() & currPos) != 0) {
					line = "Knight";
				} else if ((board.getBishops() & currPos) != 0) {
					line = "Bishop";
				} else if ((board.getQueens() & currPos) != 0) {
					line = "Queen";
				} else if ((board.getKings() & currPos) != 0) {
					line = "King";
				}

				if ((currPos & mvmt) != 0) {
					setAsMvmtText(line, PIECE_LENGTH);
					printf(line.c_str());
				} else {
					int diff = PIECE_LENGTH - line.size();
					if (diff > 0) {
						if (diff % 2 == 1) {
							printf("%*s%*c", line.size() + diff / 2 + 1, line.c_str(), diff / 2, ' ');
						} else {
							printf("%*s%*c", line.size() + diff / 2, line.c_str(), diff / 2, ' ');
						}
					} else {
						printf(line.c_str());
					}
				}

			}
			// End of Line
			printf("|\n");
		}
		// Print a  Line of "----"
		printf("\t%s\n", std::string(CHAR_IN_LINE, '-').c_str());
	}

	void setAsMvmtText(std::string &line, int length) {
		int diff = length - line.size();
		bool uneven = line.size() % 2 == 1;
		for (int i = 0; i < length; i++) {
			if (i < diff / 2 + (uneven ? 1 : 0)) {
				line.insert(i, "X");
			}

			if (i >= length - diff / 2) {
				line.append("X");
			}

			if (line.at(i) == ' ') {
				line.at(i) = 'X';
			}
		}
	}

	void Display::displayCondition(ChessCondition c, bool isWhite) {
		if (c == ChessCondition::STALEMATE) { // No valid moves remaining

			std::cout << "\nThe game is a draw. No one wins." << std::endl;
		} else if (c == ChessCondition::CHECKMATE) { // King can not move and can be attacked

			std::cout << "\nCheckmate. " << (isWhite ? "White" : "Black") << " loses." << std::endl;
		} else if (c == ChessCondition::CHECK) { // King can move but can be attacked

			std::cout << "\nYou are Checked." << std::endl;
		}
	}

	char Display::getPromotionInput() {
		int selection = ConsoleIO::promptForMenuSelection({"Queen", "Knight","Bishop","Rook"}, false);
		switch (selection) {
		case 1:
			return 'Q';
		case 2:
			return 'N';
		case 3:
			return 'B';
		case 4:
			return 'R';
		default:
			std::cerr << "Error in choisng unit: " << selection << std::endl;
			return 0;
		}
	}

	std::bitset<64> Display::getPieceSelection(std::bitset<64> &possSelection, bool allowExit) {
		// Get coordinate
		// Test if the coordinate is valid spot
		// return coord as bitset<64>
		// return 0 if going back

		bool validSelection = false;
		std::bitset<64> selection;
		do {
			selection = getCoord(allowExit);

			if ((possSelection & selection) != 0) {
				validSelection = true;
			} else {
				if (allowExit && selection == 0) {
					validSelection = true;
				} else {
					std::cout << "\nThat is not a valid Coordinate" << std::endl;
				}
			}
		} while (!validSelection);
		
		return selection;
	}

	// SUGGESTION:: Split when a number is occured
	// - Check for the value of the char before
	// - Parse for the value of the int after
	// - Check if valid...
	std::bitset<64> Display::getCoord(bool allowExit) {
		char tempC;
		int tempI;
		std::string input;
		bool incorrectInput = true;

		do {
			// Get input
			std::string prompt("Choose a Coordinate");
			if (allowExit) { prompt += " or 'b' to go Back."; }
			input = ConsoleIO::promptForInput(prompt, false);
			
			// Check if input has 2 values
			if (input.size() == 2) {
				tempI = input.at(1);
				tempC = input.at(0);
				
				// Check if second value is number
				if (tempI >= '1' && tempI <= '1' + ChessBoard::BOARD_LENGTH - 1) {
					tempI -= 49;
					
					// Check if first value is char
					if (tempC >= 'a' && tempC <= 'a' + ChessBoard::BOARD_LENGTH - 1) {
						tempC -= 97;
						incorrectInput = false;
					} else if (tempC >= 'A' && tempC <= 'A' + ChessBoard::BOARD_LENGTH - 1) {
						tempC -= 65;
						incorrectInput = false;
					} else {
						std::cout << "\nCoordinate should start with a letter between 'a' and '" <<
							static_cast<char>('a' + ChessBoard::BOARD_LENGTH - 1) << "'" << std::endl;
					}
				} else {
					std::cout << "\nPlease input a valid number." << std::endl;
				}
			} else {
				// Check if value is b, indicating back
				if (input.size() == 1 && input.at(0) == 'b') { return 0; } 
				else { std::cout << "\nCoordinates should be in the form of 'A1'" << std::endl; }
			}
		} while(incorrectInput);
		
		return board.XAndYToBit64(tempI, tempC);
	}
}