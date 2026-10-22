#include "Header.h"

int OneStage(short board[][GRID_SIZE], Array*** possibilities, int* x, int* y) {

	//this function will return -1 if FINISH FAILURE, 1 if FINISH SUCCESSFUL, and 0 if NOT FINISH
	
	//I count the filled cells in the existing board
	int filledCells = countFilledCells(board);

	//I declare the minimal array size as 50, so every other array has to be smaller than this (max is 9)
	int minimalArraySize = 50;
	
	//I use a 'changed' bool, to check if the iteration changed the possibilities array
	//and thus I check from the start again to make sure I dont miss any 'new' one digit arrays
	bool changed = true;

	while (changed) {
		changed = false;
		//I loop for the whole board, checking for an empty cell
		for (int i = 0; i < GRID_SIZE; i++) {
			for (int j = 0; j < GRID_SIZE; j++) {
				if (board[i][j] == -1){
					if (possibilities[i][j]->size == 1) {
						//put the number in the board
						board[i][j] = possibilities[i][j]->arr[0];
						filledCells++;
						//remove the digit from the possibilities array
						free(possibilities[i][j]->arr);
						possibilities[i][j]->arr = NULL; 
						possibilities[i][j]->size = 0;
						//Check the 3x3 cube by creating an index array of the starting position of each row and column
						int* CubePosArray = CheckCurrCube(i, j);
						//Then change the row, column and 3x3 cube of the possibilities array
						ChangePossibilitiesArr(possibilities, i, j, board[i][j], CubePosArray[0], CubePosArray[1]);
						changed = true;
						//Lastly I check if the inserted digit is LEGAL
						if (checkBoard(board, i, j, CubePosArray[0], CubePosArray[1]) == -1) {
							return -1;
						}
						free(CubePosArray);
					}
					else {
						if (possibilities[i][j]->size != 0 && possibilities[i][j]->size < minimalArraySize) {
							minimalArraySize = possibilities[i][j]->size;
							*x = i;
							*y = j;
						}
						//If the size of a current possibility is 0, and the board cell is empty
						//that means we cannot put ANY digit in the cell, thus FAILURE
						if (possibilities[i][j]->size == 0 && board[i][j] == -1) {
							return -1;
						}
					}
				}
			}
		}
	}
	//if the filledCells are 81, then the entire board is filled, return 1
	if (filledCells == TOTAL_CELLS) {
		return 1;
	}
	else {
		return 0;
	}
		
}

void ChangePossibilitiesArr(Array*** possibilities, int x, int y, int insertedNum, int rowStart, int columnStart) {
	
	// Change row
	for (int i = 0; i < GRID_SIZE; i++) {
		if (i != y && possibilities[x][i]->arr != NULL)
			removeNumberFromPossibilities(possibilities[x][i], insertedNum);
	}

	// Change column
	for (int i = 0; i < GRID_SIZE; i++) {
		if (i != x && possibilities[i][y]->arr != NULL) 
			removeNumberFromPossibilities(possibilities[i][y], insertedNum);
	}

	// Change 3x3 cube
	for (int i = rowStart; i < rowStart + 3; i++) {
		for (int j = columnStart; j < columnStart + 3; j++) {
			if ((i != x || j != y) && possibilities[i][j]->arr != NULL) 
				removeNumberFromPossibilities(possibilities[i][j], insertedNum);
		}
	}
}

void removeNumberFromPossibilities(Array* possibilities, int num) {
	
	int size = possibilities->size;
	for (int i = 0; i < size; i++) {
		if (possibilities->arr[i] == num) {
			for (int j = i; j < size - 1; j++) {
				possibilities->arr[j] = possibilities->arr[j + 1];
			}
			possibilities->arr = realloc(possibilities->arr, (size - 1) * sizeof(short));
			possibilities->size--;
			break;
		}
	}
}

int countFilledCells(short board[][GRID_SIZE]) {

	int filledCells = 0;

	for (int i = 0; i < GRID_SIZE; i++) {
		for (int j = 0; j < GRID_SIZE; j++) {
			if (board[i][j] != -1)
				filledCells++;
		}
	}
	return filledCells;
}

int checkBoard(short board[][GRID_SIZE], int x, int y, int rowStart, int columnStart) {

	//I check the entire row for the same number
	for (int i = 0; i < GRID_SIZE; i++) {
		for (int j = i + 1; j < GRID_SIZE; j++) {
			if (board[x][i] != -1 && board[x][i] == board[x][j])
				return -1;
		}	
	}

	//I check the entire column for the same number
	for (int i = 0; i < GRID_SIZE; i++) {
		for (int j = i + 1; j < GRID_SIZE; j++) {
			if (board[i][y] != -1 && board[i][y] == board[j][y])
				return -1;
		}
	}

	//I check the entire cube for the same number
	for (int i = rowStart; i < rowStart + 3; i++) {
		for (int j = columnStart; j < columnStart + 3; j++) {
			if ((i != x || j != y) && board[i][j] == board[x][y])
				return -1;
		}
	}
	return 0;
}