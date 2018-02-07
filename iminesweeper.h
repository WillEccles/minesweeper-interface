#ifndef IMINESWEEPER
#define IMINESWEEPER

#include <vector>

namespace minesweeper {
	// A struct that represents a tile on the board.
	struct tile_t {
		// number of adjacent mines (only needs to be set once!)
		int mineCount = 0;
		// whether or not there is a mine on this space
		bool mine = false;
		// whether or not the tile is flagged
		bool flagged = false;
		// whether or not the tile is actually visible
		bool revealed = false;
	};

	// class that represents the whole game
	class game {
		private:
			std::vector<std::vector<tile_t>> board;
			int flagged_count = 0; // number flagged
			unsigned int width = 0;
			unsigned int height = 0;
			unsigned int mines = 0;
			bool started = false;
			bool lost = false;
			void init(unsigned int startx, unsigned int starty);
		public:
			// minimum size is 5x5, mines must be < w*h and > 0
			game(unsigned int w, unsigned int h, unsigned int mine_count): width(w), height(h), mines(mine_count) {
				for (size_t i = 0; i < height; i++) {
					board.push_back(std::vector<tile_t>(width));
				}
			};
			
			// reveal a tile. returns true if successful, false if the tile was already revealed
			// still returns true after the user clicks a mine, so use hasLost() to figure out if the user has actually lost
			// if the user has lost, this will also reveal all tiles
			bool reveal(unsigned int x, unsigned int y);
			
			// flags or unflags a tile, returns true if flagged and false if not flagged (i.e. tile is revealed, etc.)
			bool toggleflagged(unsigned int x, unsigned int y);

			// check if the user has lost (from clicking on a mine)
			bool hasLost() { return lost; };

			// check if the user has won (if there are no unrevealed tiles left that aren't mines
			bool hasWon();
			
			// prepares to start another game, after calling this, call reveal() to start a new game (generate the board and such)
			void reset();

			// getters
			std::vector<std::vector<tile_t>> getBoard() { return board; };
			unsigned int getWidth() { return width; };
			unsigned int getHeight() { return height; };
			unsigned int getMines() { return mines; };
			int getFlagCount() { return flagged_count; };

			// prints the whole board, all revealed, for debugging purposes
			void debugPrint();
	};
};

#endif
