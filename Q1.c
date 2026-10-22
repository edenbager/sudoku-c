#include "Header.h"

Array*** PossibleDigits(short sudokuBoard[][GRID_SIZE]) {

    //First, I create the array of arrays of pointers, using the size of the sudoku board (9x9)
    //So i multiply by 9, the size of the rows
    Array*** possible_solutions = (Array***)malloc(GRID_SIZE * sizeof(Array**));

    //Check for error
    if (possible_solutions == NULL) {
        printf("Memory alloaction failed!\n");
        exit(1);
    }
    
    for (int i = 0; i < GRID_SIZE; i++) {
        //Then, I loop for all the rows, and create each column, and there are 9 collums
        possible_solutions[i] = (Array**)malloc(GRID_SIZE * sizeof(Array*));

        //Check for error
        if (possible_solutions[i] == NULL) {
            for (int k = 0; k < i; k++)
                free(possible_solutions[k]);
            free(possible_solutions);
            printf("Memory alloaction failed!\n");
            exit(1);
        }
        for (int j = 0; j < GRID_SIZE; j++) {
            //Finally I create the single cell array which will contain all the possible digits
            possible_solutions[i][j] = (Array*)malloc(sizeof(Array));
            //If there is already a digit in the board, put NULL
            if (sudokuBoard[i][j] != -1) {
                possible_solutions[i][j]->arr = NULL; 
                possible_solutions[i][j]->size = 0;
            }
            else {
                //I create an array which holds all of the taken digits that could NOT be in the
                //possible solutions array
                int takenDigArrSize = 0;
                int* takenDigArr = CheckTakenDigits(sudokuBoard, i, j, &takenDigArrSize);
                //The size of the possible solution array would hold all the REMAINING digits from 1-9
                int possibleSolutionsSize = GRID_SIZE - takenDigArrSize;
                possible_solutions[i][j]->arr = (short*)malloc(possibleSolutionsSize * sizeof(short));
                possible_solutions[i][j]->size = possibleSolutionsSize;
               
                //I create an index array to keep track of which numbers are already taken and mark them by 1
                int indexArr[GRID_SIZE] = { 0 };
                for (int k = 0; k < takenDigArrSize; k++) 
                    indexArr[takenDigArr[k] - 1] = 1;

                free(takenDigArr);
       
                //I loop and check for zeros inside the indexArr, and insert the matching digit
                //to the possible solutions array
                int n = 0;
                while (n < possibleSolutionsSize){
                    for (int k = 0; k < GRID_SIZE; k++) {
                        if (indexArr[k] == 0) {
                            possible_solutions[i][j]->arr[n] = k + 1;
                            n++;
                        }
                    }
                }
            }
        }
    }
    return possible_solutions;
}

int* CheckTakenDigits(short sudokuBoard[][GRID_SIZE], int rowNum, int colNum, int* size) {

    *size = 0;  // Ensure size is initialized

    int* takenDigArr = (int*)malloc(GRID_SIZE * sizeof(int));
    //Check for error
    if (takenDigArr == NULL) {
        printf("Memory alloaction failed!\n");
        exit(1);
    }
    
    int i = 0;
    //Check the entire row for digits
    for (int j = 0; j < GRID_SIZE; j++) {
        if (sudokuBoard[rowNum][j] != -1 && j != colNum) {
            takenDigArr[i] = sudokuBoard[rowNum][j];
            i++;
            *size = *size + 1;
        }
    }
    //Check the entire column for digits and check if another same digit is in the row
    bool foundSame = false;
    for (int j = 0; j < GRID_SIZE; j++) {
        if (sudokuBoard[j][colNum] != -1 && j != rowNum) {
            foundSame = false;
            for (int k = 0; k < i + 1; k++) {
                if (sudokuBoard[j][colNum] == takenDigArr[k]) {
                    foundSame = true;
                    break;
                }
            }
            if (foundSame == false) {
                takenDigArr[i] = sudokuBoard[j][colNum];
                i++;
                *size = *size + 1;
            }
        }
    }
    //Check the 3x3 cube by creating an index array of the starting position of each row and column
    foundSame = false;
    int* CubePosArray = CheckCurrCube(rowNum, colNum);
    int k = CubePosArray[0];
    while (k <= CubePosArray[0] + 2) {
        int l = CubePosArray[1];
        while (l <= CubePosArray[1] + 2) {
            foundSame = false;
            //Check if our current position is either on the specific row or column, and skip it if so
            if (k == rowNum) 
                break;
            else {
                if (sudokuBoard[k][l] != -1 && l != colNum) {
                    for (int h = 0; h < i + 1; h++) {
                        if (sudokuBoard[k][l] == takenDigArr[h]) {
                            foundSame = true;
                            break;
                        }
                    }
                    if (foundSame == false) {
                        takenDigArr[i] = sudokuBoard[k][l];
                        i++;
                        *size = *size + 1;
                    }
                }
                l++;
            }
        }
        k++;
    }
    free(CubePosArray);
    takenDigArr = realloc(takenDigArr, *size * sizeof(int));
    return takenDigArr;
}

int* CheckCurrCube(int rowNum, int colNum) {

    //A 2 sized array where the first element is the staritng postion of the row,
    //and the second is the starting position of the column
    int* CubePosArray = (int*)malloc(2 * sizeof(int));
    if (CubePosArray == NULL) {
        printf("Memory alloaction failed!\n");
        exit(1);
    }

    //Check which is the starting position of the cube's row
    if (rowNum >= 0 && rowNum <= 2)
        CubePosArray[0] = 0;
    else if (rowNum >= 3 && rowNum <= 5)
        CubePosArray[0] = 3;
    else 
        CubePosArray[0] = 6;

    //Check which is the starting position of the cube's column
    if (colNum >= 0 && colNum <= 2)
        CubePosArray[1] = 0;
    else if (colNum >= 3 && colNum <= 5)
        CubePosArray[1] = 3;
    else
        CubePosArray[1] = 6;

    return CubePosArray;
}