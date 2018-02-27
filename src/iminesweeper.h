#ifndef IMINESWEEPER
#define IMINESWEEPER

#include <vector>

// NOTES: 4 bits from right to left (2^1-2^4) are the number of mines, i.e. a tile with 5 mines: 0101
// this will use int as the value since it will work fine in java with the JNI for sure
// 0001 0000 is if it's a mine, 1 yes, 0 no
// 0010 0000 is if it's flagged, 1 yes, 0 no
// 0100 0000 is if it's revealed, 1 yes 0 no
//
// to set mine count (with no other flags):
// t = <count>
// to flag as mine, flagged, or revealed:
// t |= MINE | FLAGGED | REVEALED
// to get the mine count without the flags:
// c = t 

// represents a tile on the board, notes above explain operation
typedef int tile_t;
// represents a board
typedef std::vector<std::vector<tile_t>> board_t;

// these are not written using binary constants, since that requires C++14
const int MINE = 16; // 0001 0000, 16
const int FLAGGED = 32; // 0010 0000, 32
const int REVEALED = 64; // 0100 0000, 64

namespace minesweeper {

	// these methods are for use mostly with the Java interface, but will work fine for normal use. They also are wrapped by the game class.
	// If you use these instead of the game class, more work is done manually, so it's not recommended.
	void initBoard(board_t&, unsigned int width, unsigned int height, unsigned int mines, unsigned int startx, unsigned int starty);
	bool reveal(board_t& board, unsigned int x, unsigned int y);
	bool revealAround(board_t& board, unsigned int x, unsigned int y);
	bool toggleflagged(board_t& board, unsigned int x, unsigned int y);
	bool hasLost(board_t& board, unsigned int x, unsigned int y);
	bool hasWon(board_t& board, unsigned int mines);
	void reset(board_t& board);
	unsigned int getWidth(board_t& board);
	unsigned int getHeight(board_t& board);
	unsigned int mineCount(board_t& board);
	int flaggedCount(board_t& board);
	void debugPrint(board_t& board);
	void revealAll(board_t& board);
	inline int getMineCount(tile_t t) { return (t & 15); };
	inline bool isMine(tile_t t) { return (t & MINE); };
	inline bool isFlagged(tile_t t) { return (t & FLAGGED); };
	inline bool isRevealed(tile_t t) { return (t & REVEALED); };
	inline void setMine(tile_t& t) { t |= MINE; };
	inline void setFlagged(tile_t& t, bool f = true) {
		if (f && !isFlagged(t)) t |= FLAGGED;
		else if (!f && isFlagged(t)) t -= FLAGGED;
	};
	inline void setRevealed(tile_t& t, bool r = true) {
		if (r && !isRevealed(t)) t |= REVEALED;
		else if (!r && isRevealed(t)) t -= REVEALED;
	};
	inline board_t boardFromIntArray(int **arr, int h, int w) {
		board_t b;
		for (int i = 0; i < h; i++) {
			b.push_back(std::vector<tile_t>(w));
		}
		return b;
	};

	// class that represents the whole game
	class game {
		private:
			board_t board;
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

			// reveals 8 tiles around the given tile IF it's revealed and if the number of flags around it is the same as the number of mines around it
			bool revealAround(unsigned int x, unsigned int y);
			
			// flags or unflags a tile, returns true if flagged and false if not flagged (i.e. tile is revealed, etc.)
			bool toggleflagged(unsigned int x, unsigned int y);

			// check if the user has lost (from clicking on a mine)
			bool hasLost() { return lost; };

			// check if the user has won (if there are no unrevealed tiles left that aren't mines
			bool hasWon();
			
			// prepares to start another game, after calling this, call reveal() to start a new game (generate the board and such)
			void reset();

			// getters
			board_t getBoard() { return board; };
			unsigned int getWidth() { return width; };
			unsigned int getHeight() { return height; };
			unsigned int getMines() { return mines; };
			int getFlagCount() { return flagged_count; };

			// prints the whole board, all revealed, for debugging purposes
			void debugPrint() { minesweeper::debugPrint(board); };
	};

};

#endif
