#include "iminesweeper_Game.h"
#include "iminesweeper.h"
#include <vector>
#include <iostream>

void setIntArrFromBoard(JNIEnv *env, jobjectArray& jarr, minesweeper::board_t& board) {
	for (unsigned int i = 0; i < board.size(); i++) {
		jintArray arr = env->NewIntArray(board[0].size());
		// add every item from the board row into this array
		env->SetIntArrayRegion(arr, (jsize) 0, (jsize) board[0].size(), board[i].data());
		env->SetObjectArrayElement(jarr, (jsize) i, arr);
	}
}

void setBoardFromIntArr(JNIEnv *env, minesweeper::board_t& board, jobjectArray& jarr) {
	int height = env->GetArrayLength(jarr);
	jintArray dim = (jintArray)env->GetObjectArrayElement(jarr, 0);
	int width = env->GetArrayLength(dim);
	env->DeleteLocalRef(dim);

	int **localArray = new int*[height];
	for (int i = 0; i < height; i++) {
		jintArray row = (jintArray)env->GetObjectArrayElement(jarr, i);
		jint *element = env->GetIntArrayElements(row, 0);


		localArray[i] = new int[width];
		for (int j = 0; j < width; j++) {
			localArray[i][j] = element[j];
		}

		env->ReleaseIntArrayElements(row, element, JNI_ABORT);
		env->DeleteLocalRef(row);
	}

	board = minesweeper::boardFromIntArray(localArray, height, width);

	// free up memory
	for (int i = 0; i < height; i++)
		delete localArray[i];
	delete [] localArray;
}

#ifdef __cplusplus
extern "C" {
#endif

	/*
	 * Class:     iminesweeper_Game
	 * Method:    reveal
	 * Signature: ([[III)[[I
	 */
	JNIEXPORT jobjectArray JNICALL Java_iminesweeper_Game_reveal(JNIEnv *env, jobject thisObj, jobjectArray jboard, jint x, jint y) {
		minesweeper::board_t board;
		setBoardFromIntArr(env, board, jboard);

		// reveall all the things
		minesweeper::reveal(board, (unsigned int)x, (unsigned int)y);

		jclass intArrClass = env->FindClass("[I");
		jobjectArray jObjArray = env->NewObjectArray((jsize)board.size(), intArrClass, NULL);

		setIntArrFromBoard(env, jObjArray, board);

		return jObjArray;
	}

	/*
	 * Class:     iminesweeper_Game
	 * Method:    revealAround
	 * Signature: ([[III)[[I
	 */
	JNIEXPORT jobjectArray JNICALL Java_iminesweeper_Game_revealAround(JNIEnv *env, jobject thisObj, jobjectArray jboard, jint x, jint y) {
		minesweeper::board_t board;
		setBoardFromIntArr(env, board, jboard);

		// reveall all the things
		minesweeper::revealAround(board, (unsigned int)x, (unsigned int)y);

		jclass intArrClass = env->FindClass("[I");
		jobjectArray jObjArray = env->NewObjectArray((jsize)board.size(), intArrClass, NULL);

		setIntArrFromBoard(env, jObjArray, board);

		return jObjArray;
	}

	/*
	 * Class:     iminesweeper_Game
	 * Method:    hasWon
	 * Signature: ([[II)Z
	 */
	JNIEXPORT jboolean JNICALL Java_iminesweeper_Game_hasWon(JNIEnv *env, jobject thisObj, jobjectArray jboard, jint mineCount) {
		minesweeper::board_t board;
		setBoardFromIntArr(env, board, jboard);
	
		return minesweeper::hasWon(board, (unsigned int)mineCount);
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
		jclass intArrClass = env->FindClass("[I"); // the 1D int array type

		minesweeper::board_t board; // board to populate
		minesweeper::initBoard(board, width, height, mines, startx, starty);

		jobjectArray jObjArray = env->NewObjectArray((jsize)height, intArrClass, NULL);
		setIntArrFromBoard(env, jObjArray, board);

		return jObjArray;
	}

	/*
	 * Class:     iminesweeper_Game
	 * Method:    revealAll
	 * Signature: ([[I)[[I
	 */
	JNIEXPORT jobjectArray JNICALL Java_iminesweeper_Game_revealAll(JNIEnv *env, jobject thisObj, jobjectArray jboard) {
		minesweeper::board_t board;
		setBoardFromIntArr(env, board, jboard);

		// reveall all the things
		minesweeper::revealAll(board);

		jclass intArrClass = env->FindClass("[I");
		jobjectArray jObjArray = env->NewObjectArray((jsize)board.size(), intArrClass, NULL);

		setIntArrFromBoard(env, jObjArray, board);

		return jObjArray;
	}

	/*
	 * Class:     iminesweeper_Game
	 * Method:    revealMines
	 * Signature: ([[I)[[I
	 */
	JNIEXPORT jobjectArray JNICALL Java_iminesweeper_Game_revealMines(JNIEnv *env, jobject thisObj, jobjectArray jboard) {
		minesweeper::board_t board;
		setBoardFromIntArr(env, board, jboard);

		// reveall all the things
		minesweeper::revealMines(board);

		jclass intArrClass = env->FindClass("[I");
		jobjectArray jObjArray = env->NewObjectArray((jsize)board.size(), intArrClass, NULL);
		setIntArrFromBoard(env, jObjArray, board);

		return jObjArray;
	}

#ifdef __cplusplus
}
#endif
