package iminesweeper;

import java.util.Vector;

/**
 * Java class that emulates the C++ minesweeper::game class.
 * @author Will Eccles
 * @version 02-07-2018
 */
public class Game {

	static {
		System.loadLibrary("minesweeper"); // load lib from libminesweeper.jnilib
	}

	// in native code, we can basically just convert our unsigned ints to longs
	// (jlong)(unsigned long long)i

	private int[][] board; // used to store the board's values
	private int flagged_count = 0;
	private boolean started = false;
	private boolean lost = false;
	private int _height = 0;
	private int _width = 0;
	private int _mines = 0;

	private static final int FLAGGED = 32;

	/* Java methods that wrap native ones */

	public Game(int width, int height, int minecount) {
		_width = width;
		_height = height;
		_mines = minecount;
		// just make sure the array is not null
		board = new int[height][width];
	}

	/**
	 * Get the current state of the board.
	 * @return The board as an array of ints.
	 */
	public int[][] getBoard() {
		return board;
	}

	/**
	 * Reveals a tile on the board.
	 * @param x Horizontal coordinate (column) zero-indexed
	 * @param y Vertical coordinate (row) zero-indexed
	 * @return Whether or not the operation was successful, i.e. false if it was already revealed or flagged.
	 */
	public boolean reveal(int x, int y) {
		if (started) {
			if (!isFlagged(board[y][x]) && isMine(board[y][x])) {
				lost = true;
				board = revealAll(board);
				return true;
			}
		} else {
			board = initboard(_width, _height, _mines, x, y);
			started = true;
		}
		int[][] newBoard = reveal(board, x, y);
		if (newBoard != board) {
			board = newBoard;
			return true;
		} else return false;
	}

	/**
	 * Reveals around a revealed tile on the board if and only if the number of adjacent tiles that are flagged is the same as the number of mines around the tile.
	 * @param x Horizontal coordinate of the tile.
	 * @param y Vertical coordinate of the tile.
	 * @return Whether or not the operation was successful, i.e. false if the tile is unrevealed, or if the number of tiles flagged around the tile is not the right amount.
	 */
	public boolean revealAround(int x, int y) {
		if (!started) return false;

		int[][] newBoard = revealAround(board, x, y);
		if (newBoard != board) {
			board = newBoard;
			return true;
		} else return false;
	}

	/**
	 * Reveal all of the tiles on the board.
	 */
	public void revealAll() {
		board = revealAll(board);
	}

	/**
	 * Reveal all mines on the board.
	 */
	public void revealMines() {
		board = revealMines(board);
	}

	/**
	 * Toggle the flagged status of a tile.
	 * @param x The horizontal coordinate of the tile.
	 * @param y The vertical coordinate of the tile.
	 * @return Whether or not the operation worked, i.e. false if the tile was already revealed.
	 */
	public boolean toggleFlagged(int x, int y) {
		if (!started) return false;

		if (!isRevealed(board[y][x]) && isFlagged(board[y][x])) {
			board[y][x] -= FLAGGED;
			return true;
		} else if (!isRevealed(board[y][x]) && !isFlagged(board[y][x])) {
			board[y][x] |= FLAGGED;
			return true;
		} else {
			return false;
		}
	}

	/**
	 * Determines if the player has lost.
	 * @return Whether or not the player has lost.
	 */
	public boolean hasLost() {
		return lost;
	}

	/**
	 * Determines whether or not the player has won (i.e. the only tiles left unrevealed are mines).
	 * @return Whether or not the player has won.
	 */
	public boolean hasWon() {
		return hasWon(board, _mines);
	}

	/**
	 * Reveal the given tile in the given board and return the new board. If the board is different from the old one, you can tell it was changed. {@link #reveal(int, int) reveal(int, int) uses this.}
	 * @param board The board to reveal from.
	 * @param x The column of the tile.
	 * @param y The row of the tile.
	 * @return The board after revealing the tile.
	 * @see #reveal(int, int)
	 */
	private native int[][] reveal(int[][] board, int x, int y);

	/**
	 * Reveal around a given tile on the board assuming it's possible to do so. This is the backend for {@link #revealAround(int, int) reveal(int, int)}.
	 * @param board The board to use.
	 * @param x The horizontal coordinate of the tile.
	 * @param y The vertical coordinate of the tile.
	 * @see #revealAround(int, int)
	 */
	private native int[][] revealAround(int[][] board, int x, int y);

	/**
	 * Determine whether or not the given board is won or not, given the board and the number of mines in it (this is the native version, {@link #hasWon() hasWon() is the public wrapper}).
	 * @param board The board to be checked.
	 * @param mines The number of mines in the board.
	 * @return Whether or not the player has won.
	 * @see #hasWon()
	 */
	private native boolean hasWon(int[][] board, int mines); 

	/**
	 * Determine whether or not a given tile is a mine.
	 * @param tile The tile to be checked (as an int).
	 * @return Whether or not the tile is a mine.
	 */
	public static native boolean isMine(int tile);

	/**
	 * Determine whether or not a given tile is flagged.
	 * @param tile The tile to be checked (as an int).
	 * @return Whether or not the tile is flagged.
	 */
	public static native boolean isFlagged(int tile);
	
	/**
	 * Determine whether or not a given tile is revealed.
	 * @param tile The tile to be checked (as an int).
	 * @return Whether or not the tile is revealed.
	 */
	public static native boolean isRevealed(int tile);

	/**
	 * Get the number of mines adjacent to a given tile.
	 * @param tile The tile in question (as an int)
	 * @return The number of mines adjacent to the tile.
	 */
	public static native int mineCount(int tile);

	/**
	 * Initialize a board with the given dimensions, number of mines, and starting position.
	 * @param w The width of the board.
	 * @param h The height of the board.
	 * @param m The number of mines in the board.
	 * @param startx The X coordinate (column) the player started in.
	 * @param starty The Y coordinate (row) the player started in.
	 * @return The generated board.
	 */
	private native int[][] initboard(int w, int h, int m, int startx, int starty);

	/**
	 * Reveal all tiles on a board. This is the backend for {@link #revealAll() revealAll()}.
	 * @param board The board to reveal all tiles of.
	 * @return The board after revealing all tiles.
	 * @see #revealAll()
	 */
	private native int[][] revealAll(int[][] board);

	/**
	 * Reveal all mines on the board. This is the backend for {@link #revealMines() revealMines()}.
	 * @param board The board to reveal the mines on.
	 * @return The board post-reveal.
	 * @see #revealMines()
	 */
	private native int[][] revealMines(int[][] board);
}
