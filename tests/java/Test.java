import java.util.Scanner;
import iminesweeper.Game;

public class Test {
	Game game;

	public Test() {
		// do game loop and stuff
		game = new Game(10, 10, 5);
		printBoard(game.getBoard());
		Scanner scan = new Scanner(System.in);

		String input;
		char o;
		int x = 0;
		int y = 0;
		boolean success = false;
		while (!game.hasLost()) {
			input = scan.nextLine();
			if (input.matches("[fra] \\d+ \\d+")) {
				o = input.split(" ")[0].charAt(0);
				x = Integer.parseInt(input.split(" ")[1]);
				y = Integer.parseInt(input.split(" ")[2]);

				switch (o) {
					case 'r':
						success = game.reveal(x, y);
						break;
					case 'f':
						success = game.toggleFlagged(x, y);
						break;
					case 'a':
						success = game.revealAround(x, y);
						break;
					default:
						success = false;
						break;
				}

				if (!success)
					System.out.println("Please enter a good thingy.");
				else {
					if (game.hasWon()) {
						printBoard(game.getBoard());
						System.out.println("You won!");
						break;
					}
				}
				printBoard(game.getBoard());
			}
		}

		System.out.println("You lose.");
	}
	
	void printBoard(int[][] board) {
		for (int r = 0; r < board.length; r++) {
			for (int c = 0; c < board[0].length; c++) {
				if (game.isRevealed(board[r][c])) {
					if (game.isMine(board[r][c]))
						System.out.print("M");
					else if (game.mineCount(board[r][c]) > 0)
						System.out.print(game.mineCount(board[r][c]));
					else
						System.out.print("_");
				} else {
					if (game.isFlagged(board[r][c]))
						System.out.print("F");
					else
						System.out.print("#");
				}
			}
			System.out.println();
		}
	}

	public static void main(String[] args) {
		new Test();
	}
}
