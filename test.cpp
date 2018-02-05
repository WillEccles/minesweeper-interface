#include "iminesweeper.h"
#include <iostream>
#include <vector>

using namespace minesweeper;

void printBoard(std::vector<std::vector<tile_t>> board) {
	std::cout << "┌───";
	for (unsigned int i; i < board[0].size()-1; i++) {
		std::cout << "┬───";
	}
	std::cout << "┐";

	// print each 
}

int main(void) {
	game g(5, 5, 2);

	int x, y;
	std::cin >> x >> y;

	g.reveal(x, y);

	return 0;
}
