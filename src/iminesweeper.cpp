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
		} while (board[x][y].mine || ((x >= (int)startx-1 && x <= (int)startx+1) && (y >= (int)starty-1 && y <= (int)starty+1)));

		board[y][x].mine = true;
	}

	// set up all other tiles
	for (unsigned int c = 0; c < width; c++) {
		for (unsigned int r = 0; r < height; r++) {
			if (!board[c][r].mine) {
				// left side
				if (c > 0) {
					if (r > 0) {
						// top left
						if (board[c-1][r-1].mine)
							board[c][r].mineCount++;
					}

					// left center
					if (board[c-1][r].mine)
						board[c][r].mineCount++;

					if (r < height-1) {
						// bottom left
						if (board[c-1][r+1].mine)
							board[c][r].mineCount++;
					}
				}
				// right side
				if (c < width-1) {
					if (r > 0) {
						// top right
						if (board[c+1][r-1].mine)
							board[c][r].mineCount++;
					}

					// right center
					if (board[c+1][r].mine)
						board[c][r].mineCount++;

					if (r < height-1) {
						// bottom right
						if (board[c+1][r+1].mine)
							board[c][r].mineCount++;
					}	
				}
				// top
				if (r > 0) {
					// top center
					if (board[c][r-1].mine)
						board[c][r].mineCount++;
				}
				// bottom
				if (r < height-1) {
					// bottom center
					if (board[c][r+1].mine)
						board[c][r].mineCount++;
				}
			}
		}
	}
}

bool minesweeper::game::reveal(unsigned int x, unsigned int y) {
	if (x > width-1 || y > height-1)
		return false;
	if (started) {
		if (board[y][x].revealed || board[y][x].flagged)
			return false;
		if (board[y][x].mine) {
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

	board[y][x].revealed = true;

	// now we check if the revealed tile is blank, and if it is, recursively reveal all other ones around it
	if (!board[y][x].mine && board[y][x].mineCount == 0) {
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
	if (board[y][x].revealed) return false;
	board[y][x].flagged = !board[y][x].flagged;
	if (board[y][x].flagged)
		flagged_count++;
	else
		flagged_count--;
	return true;
}

bool minesweeper::game::hasWon() {
	unsigned int unrevealed = 0;
	for (auto row : board) {
		for (auto tile : row) {
			if (!tile.revealed) {
				unrevealed++;
				if (unrevealed > mines)
					return false;
			}
		}
	}

	for (auto row : board) {
		for (auto tile : row) {
			tile.revealed = true;
		}
	}

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
