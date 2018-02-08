#include "iminesweeper.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace minesweeper;

void minesweeper::initBoard(board_t& board, unsigned int width, unsigned int height, unsigned int mines, unsigned int startx, unsigned int starty) {
	if (board.empty()) {
		for (size_t i = 0; i < height; i++) {
			board.push_back(std::vector<tile_t>(width));
		}
	}

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

// initialize the board and do stuff
void minesweeper::game::init(unsigned int startx, unsigned int starty) {
	initBoard(board, width, height, mines, startx, starty);
}

void minesweeper::revealAll(board_t& board) {
	for (auto row : board) {
		for (auto tile : row) {
			tile.revealed = true;
		}
	}
}

bool minesweeper::reveal(board_t& board, unsigned int x, unsigned int y) {
	unsigned int height = board.size();
	unsigned int width = board[0].size();
	if (x > width-1 || y > height-1)
		return false;
	if (board[y][x].revealed || board[y][x].flagged)
		return false;

	board[y][x].revealed = true;

	// now we check if the revealed tile is blank, and if it is, recursively reveal all other ones around it
	if (!board[y][x].mine && board[y][x].mineCount == 0) {
		if (x > 0 && y > 0)
			minesweeper::reveal(board, x-1, y-1); // top left
		if (x > 0)
			minesweeper::reveal(board, x-1, y); // left center
		if (y > 0)
			minesweeper::reveal(board, x, y-1); // top center
		if (y < height-1)
			minesweeper::reveal(board, x, y+1); // bottom center
		if (x < width-1)
			minesweeper::reveal(board, x+1, y); // right center
		if (x > 0 && y < height-1)
			minesweeper::reveal(board, x-1, y+1); // bottom left
		if (x < width-1 && y > 0)
			minesweeper::reveal(board, x+1, y-1); // top right
		if (x < width-1 && y < height-1)
			minesweeper::reveal(board, x+1, y+1); // bottom right
	}

	return true;
}

bool minesweeper::game::reveal(unsigned int x, unsigned int y) {
	if (started) {
		if (board[y][x].mine) {
			lost = true;
			// set all tiles to be revealed
			minesweeper::revealAll(board);
			return true;
		}
	} else {
		init(x, y);
		started = true;
	}
	return minesweeper::reveal(board, x, y);
}

bool minesweeper::toggleflagged(board_t& board, unsigned int x, unsigned int y) {
	if (board[y][x].revealed) return false;
	board[y][x].flagged = !board[y][x].flagged;
	return true;
}

bool minesweeper::game::toggleflagged(unsigned int x, unsigned int y) {
	bool success = minesweeper::toggleflagged(board, x, y);
	if (!success) return false;
	if (board[y][x].flagged)
		flagged_count++;
	else
		flagged_count--;
	return true;
}

bool minesweeper::hasLost(board_t& board, unsigned int x, unsigned int y) {
	return board[y][x].mine;
}

bool minesweeper::hasWon(board_t& board, unsigned int mines) {
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

bool minesweeper::game::hasWon() {
	return minesweeper::hasWon(board, mines);
}

void minesweeper::reset(board_t& board) {
	unsigned int height = board.size();
	unsigned int width = board[0].size();
	board.clear();
	for (size_t i = 0; i < height; i++) {
		board.push_back(std::vector<tile_t>(width));
	}
}

void minesweeper::game::reset() {
	minesweeper::reset(board);
	flagged_count = 0;
	started = false;
	lost = false;
}

void minesweeper::debugPrint(board_t& board) {
	unsigned int height = board.size();
	unsigned int width = board[0].size();
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
