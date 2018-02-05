#ifndef IMINESWEEPER
#define IMINESWEEPER

#define DEBUG

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
			game(unsigned int w, unsigned int h, unsigned int mine_count): width(w), height(h), mines(mine_count) {};
			// reveal a tile
			void reveal(unsigned int x, unsigned int y);
			// flags or unflags a tile, returns true if flagged and false if not flagged (i.e. tile is revealed, etc.)
			bool toggleflagged(int x, int y);
			bool hasLost() { return lost; };
#ifdef DEBUG
			void debugPrint();
#endif
	};
};

#endif
