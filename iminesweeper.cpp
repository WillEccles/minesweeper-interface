#include "iminesweeper.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

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

	srand(time(NULL));
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
		}
	}
}

bool minesweeper::game::reveal(unsigned int x, unsigned int y) {
	if (x > width-1 || y > height-1)
		return false;
	if (started) {
		if (board[x][y].revealed || board[x][y].flagged)
			return false;
		if (board[x][y].mine) {
			std::cout << "lost\n";
			lost = true;
			// set all tiles to be revealed
			for (unsigned int r = 0; r < height; r++)
				for (unsigned int c = 0; c < width; c++)
					board[r][c].revealed = true;
			return true;
		}
	} else {
		init(x, y);
		started = true;
	}

	board[x][y].revealed = true;

	// now we check if the revealed tile is blank, and if it is, recursively reveal all other ones around it
	if (!board[x][y].mine && board[x][y].mineCount == 0) {
		if (x > 0 && y > 0)
			reveal(x-1, y-1); // top left
		if (x > 0)
			reveal(x-1, y); // left center
		if (y > 0)
			reveal(x, y-1); // top center
		if (y < height-1)
			reveal(x, y+1); // bottom center
		if (x < width-1)
			reveal(x+1, y); // right center
		if (x > 0 && y < height-1)
			reveal(x-1, y+1); // bottom left
		if (x < width-1 && y > 0)
			reveal(x+1, y-1); // top right
		if (x < width-1 && y < height-1)
			reveal(x+1, y+1); // bottom right
	}

	return true;
}

bool minesweeper::game::toggleflagged(unsigned int x, unsigned int y) {
	if (board[x][y].revealed) return false;
	board[x][y].flagged = !board[x][y].flagged;
	if (board[x][y].flagged)
		flagged_count++;
	else
		flagged_count--;
	return true;
}

void minesweeper::game::reset() {
	board.clear();
	for (size_t i = 0; i < height; i++) {
		board.push_back(std::vector<tile_t>(width));
	}
	flagged_count = 0;
	started = false;
	lost = false;
}

void minesweeper::game::debugPrint() {
	// print each board space as a char
	for (unsigned int row = 0; row < height; row++) {
		for (unsigned int col = 0; col < width; col++) {
			if (board[row][col].mine)
				std::cout << 'M';
			else if (board[row][col].mineCount)
				std::cout << board[row][col].mineCount;
			else
				std::cout << ' ';
		}
		std::cout << '\n';
	}
}
