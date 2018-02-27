#include "iminesweeper.h"
#include <iostream>
#include <cstdio>
#include <vector>
#include <string>

using namespace minesweeper;

void printBoard(std::vector<std::vector<tile_t>> board) {
	std::cout << "     ";
	for (unsigned int i = 0; i < board[0].size(); i++)
		std::cout << i << "   ";
	std::cout << "\n   ┌───";
	for (unsigned int i = 0; i < board[0].size()-1; i++) {
		std::cout << "┬───";
	}
	std::cout << "┐\n";

	std::string toPrint = "■";
	// print each of the tiles in each row
	for (unsigned int r = 0; r < board.size(); r++) {
		std::cout << ' ' << r << " │";
		for (unsigned int c = 0; c < board[0].size(); c++) {
			if (isRevealed(board[r][c])) {
				if (!isMine(board[r][c])) {
					if (!getMineCount(board[r][c]))
						toPrint = " ";
					else
						toPrint = (char)('0' + getMineCount(board[r][c]));
				} else {
					toPrint = "✹";
				}
			} else {
				if (isFlagged(board[r][c]))
					toPrint = "✕";
				else
					toPrint = "■";
			}
			std::cout << ' ' << toPrint << " │";
			toPrint = "■";
		}
		if (r < board.size()-1) {
			std::cout << '\n';
			std::cout << "   ├───";
			for (unsigned int i = 0; i < board[0].size()-1; i++) {
				std::cout << "┼───";
			}
			std::cout << "┤\n";
		} else {
			std::cout << '\n';
			std::cout << "   └───";
			for (unsigned int i = 0; i < board[0].size()-1; i++) {
				std::cout << "┴───";
			}
			std::cout << "┘\n";
		}
	}
}

int main(void) {
	game g(10, 10, 5);

	printBoard(g.getBoard());

	while (!g.hasLost()) {
		char o;
		int x, y;
		std::cin >> o >> x >> y;

		bool success = false;
		switch (o) {
			case 'r':
				success = g.reveal((unsigned int)x, (unsigned int)y);
				break;
			case 'f':
				success = g.toggleflagged((unsigned int)x, (unsigned int)y);
				break;
			case 'a':
				success = g.revealAround((unsigned int)x, (unsigned int)y);
				break;
			default:
				success = false;
				break;
		}

		if (!success)
			std::cout << "please enter a good thingy\n";
		else {
			if (g.hasWon()) {
				printBoard(g.getBoard());
				std::cout << "You won!\n";
				break;
			}
		}
		printBoard(g.getBoard());
	}
	//g.debugPrint();

	return 0;
}
