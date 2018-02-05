#include "iminesweeper.h"
#include <iostream>
#include <vector>
#include <string>

using namespace minesweeper;

void printBoard(std::vector<std::vector<tile_t>> board) {
	std::cout << "┌───";
	for (unsigned int i = 0; i < board[0].size()-1; i++) {
		std::cout << "┬───";
	}
	std::cout << "┐\n";

	std::string toPrint = "■";
	// print each of the tiles in each row
	for (unsigned int r = 0; r < board.size(); r++) {
		std::cout << "│";
		for (unsigned int c = 0; c < board[0].size(); c++) {
			if (board[r][c].revealed) {
				if (!board[r][c].mine) {
					if (!board[r][c].mineCount)
						toPrint = " ";
					else
						toPrint = (char)('0' + board[r][c].mineCount);
				}
			} else {
				toPrint = "■";
			}
			std::cout << ' ' << toPrint << " │";
			toPrint = "■";
		}
		if (r < board.size()-1) {
			std::cout << '\n';
			std::cout << "├───";
			for (unsigned int i = 0; i < board[0].size()-1; i++) {
				std::cout << "┼───";
			}
			std::cout << "┤\n";
		} else {
			std::cout << '\n';
			std::cout << "└───";
			for (unsigned int i = 0; i < board[0].size()-1; i++) {
				std::cout << "┴───";
			}
			std::cout << "┘\n";
		}
	}
}

int main(void) {
	game g(10, 10, 20);

	printBoard(g.getBoard());

	int x, y;
	std::cin >> x >> y;

	g.reveal(x, y);
	printBoard(g.getBoard());
	//g.debugPrint();

	return 0;
}
