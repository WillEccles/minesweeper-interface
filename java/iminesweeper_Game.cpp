#include "iminesweeper.h"
#include <vector>

void setIntArrFromBoard(JNIEnv *env, jobjectArray& jarr, minesweeper::board_t& board) {
	for (unsigned int i = 0; i < height; i++) {
		jintArray arr = (*env)->NewIntArray(env, width);
		// add every item from the board row into this array
		for (unsigned int j = 0; i < width; j++) {
			(*env)->SetIntArrayRegion(env, arr, (jsize) i, (jsize) 1, board[i].data());
		}
		(*env)->SetObjectArrayElement(env, jarr, (jsize) i, arr);
	}
}

#ifdef __cplusplus
extern "C" {
#endif

	/*
	 * Class:     iminesweeper_Game
	 * Method:    reveal
	 * Signature: ([[III)[[I
	 */
	JNIEXPORT jobjectArray JNICALL Java_iminesweeper_Game_reveal(JNIEnv *, jobject, jobjectArray, jint, jint) {
	}

	/*
	 * Class:     iminesweeper_Game
	 * Method:    revealAround
	 * Signature: ([[III)[[I
	 */
	JNIEXPORT jobjectArray JNICALL Java_iminesweeper_Game_revealAround(JNIEnv *, jobject, jobjectArray, jint, jint) {
	}

	/*
	 * Class:     iminesweeper_Game
	 * Method:    hasWon
	 * Signature: ([[II)Z
	 */
	JNIEXPORT jboolean JNICALL Java_iminesweeper_Game_hasWon(JNIEnv *, jobject, jobjectArray, jint) {
	}

	/*
	 * Class:     iminesweeper_Game
	 * Method:    isMine
	 * Signature: (I)Z
	 */
	JNIEXPORT jboolean JNICALL Java_iminesweeper_Game_isMine(JNIEnv *env, jclass thisObj, jint tile) {
		return minesweeper::isMine(tile);
	}

	/*
	 * Class:     iminesweeper_Game
	 * Method:    isFlagged
	 * Signature: (I)Z
	 */
	JNIEXPORT jboolean JNICALL Java_iminesweeper_Game_isFlagged(JNIEnv *env, jclass thisObj, jint tile) {
		return minesweeper::isFlagged(tile);
	}

	/*
	 * Class:     iminesweeper_Game
	 * Method:    isRevealed
	 * Signature: (I)Z
	 */
	JNIEXPORT jboolean JNICALL Java_iminesweeper_Game_isRevealed(JNIEnv *env, jclass thisObj, jint tile) {
		return minesweeper::isRevealed(tile);
	}

	/*
	 * Class:     iminesweeper_Game
	 * Method:    mineCount
	 * Signature: (I)I
	 */
	JNIEXPORT jint JNICALL Java_iminesweeper_Game_mineCount(JNIEnv *env, jclass thisObj, jint tile) {
		return minesweeper::getMineCount(tile);
	}

	/*
	 * Class:     iminesweeper_Game
	 * Method:    initboard
	 * Signature: (IIIII)[[I
	 */
	JNIEXPORT jobjectArray JNICALL Java_iminesweeper_Game_initboard(JNIEnv *env, jobject thisObj, jint width, jint height, jint mines, jint startx, jint starty) {
		jclass intArrClass = (*env)->FindClass(env, "[I"); // the 1D int array type

		minesweeper::board_t board; // board to populate
		minesweeper::initBoard(board, width, height, mines, startx, starty);

		jobjectArray jObjArray = (*env)->NewObjectArray(env, (jsize)height, intArrClass, NULL);
		setIntArrFromBoard(env, jObjArray, board);

		return jObjArray;
	}

	/*
	 * Class:     iminesweeper_Game
	 * Method:    revealAll
	 * Signature: ([[I)[[I
	 */
	JNIEXPORT jobjectArray JNICALL Java_iminesweeper_Game_revealAll(JNIEnv *, jobject, jobjectArray) {
	}

#ifdef __cplusplus
}
#endif
