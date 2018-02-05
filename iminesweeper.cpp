#include "iminesweeper.h"
#include <cstdlib>

using namespace minesweeper;

// initialize the board and do stuff
void minesweeper::game::init(unsigned int startx, unsigned int starty) {
	if (width < 5 || height < 5) {
		width = 5;
		height = 5;
	}
	if (mines >= (width*height-1) || mines == 0) {
		mines = width*height/4;
	}

	// set up all mines
	for (unsigned int i = 0; i < mines; i++) {
		int x;
		int y;

		do {
			x = rand() % width;
			y = rand() % height;
		} while (board[x][y].mine || (x == startx || y == starty));

		board[x][y].mine = true;
	}

	// set up all other tiles
	for (unsigned int x = 0; x < width; x++) {
		for (unsigned int y = 0; y < height; y++) {
			if (!board[x][y].mine) {
				// left side
				if (x > 0) {
					if (y > 0) {
						// top left
						if (board[x-1][y-1].mine)
							board[x][y].mineCount++;
					}

					// left center
					if (board[x-1][y].mine)
						board[x][y].mineCount++;

					if (y < height-1) {
						// bottom left
						if (board[x-1][y+1].mine)
							board[x][y].mineCount++;
					}
				}
				// right side
				if (x < width-1) {
					if (y > 0) {
						// top right
						if (board[x+1][y-1].mine)
							board[x][y].mineCount++;
					}

					// right center
					if (board[x+1][y].mine)
						board[x][y].mineCount++;

					if (y < height-1) {
						// bottom right
						if (board[x+1][y+1].mine)
							board[x][y].mineCount++;
					}	
				}
				// top
				if (y > 0) {
					// top center
					if (board[x][y-1].mine)
						board[x][y].mineCount++;
				}
				// bottom
				if (y < height-1) {
					// bottom center
					if (board[x][y+1].mine)
						board[x][y].mineCount++;
				}
			}

#ifdef DEBUG
			board[x][y].revealed = true;
#endif
		}
	}
}

void minesweeper::game::reveal(unsigned int x, unsigned int y) {
	if (started) {
		board[x][y].revealed = true;
	} else {
		init(x, y);
		board[x][y].revealed = true;
		started = true;
	}
}

#ifdef DEBUG
void minesweeper::game::debugPrint() {
	// print each board space as a char
	for (unsigned int row = 0; row < height; row++) {
		for (unsigned int col = 0; col < width; col++) {
			if (board[row][col].mine)
				std::cout << 'M';
			else if (board[row][col].mineCount)
				std::cout << board[row][col].mineCount;
			else
				std::cout ' ';
		}
		std::cout << '\n';
	}
}
#endif
