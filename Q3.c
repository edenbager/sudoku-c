#include "Header.h"

int FillBoard(short board[][GRID_SIZE], Array*** possibilities) {

	int x = 0;
	int y = 0;
	int result = 0;

	//if oneStage returns NOT_FINISH -> 0
	//if oneStage returns FINISH_SUCCESS -> 1
	//if oneStage returns FINISH_FAILURE -> -1

	while ((result = OneStage(board, possibilities, &x, &y)) == 0) {
		//Check how many digits are in the smallest cell coordinates
		int howMany = possibilities[x][y]->size;
		printf("Cell number [%d, %d] currently holds the minimum number of possible values, select one of the below :\n", x, y);
		for (int i = 0; i < howMany; i++) 
			printf("%d. %d\n", i + 1, possibilities[x][y]->arr[i]);
		int chosenNum = 0;
		scanf("%d", &chosenNum);
		//insert the digit into the board
		board[x][y] = chosenNum;
		//change the possible digits array accordingly, by emptying the current cell
		free(possibilities[x][y]->arr);
		possibilities[x][y]->arr = NULL;
		possibilities[x][y]->size = 0;
		//Check the 3x3 cube by creating an index array of the starting position of each row and column
		int* CubePosArray = CheckCurrCube(x, y);
		//Then change the row, column and 3x3 cube of the possibilities array
		ChangePossibilitiesArr(possibilities, x, y, chosenNum, CubePosArray[0], CubePosArray[1]);
		//check correction of the board
		if (checkBoard(board, x, y, CubePosArray[0], CubePosArray[1]) == -1) {
			return -1;
		}
		free(CubePosArray);
	}
	return result;
}

