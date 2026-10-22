#include "Header.h"

int main(void) {

	srand(12345);  // Seed for random number generator

	int x = 0;
	printf("please insert amount of players: \n");
	scanf("%d", &x);

	PlayersList activePlayersList;

	makeEmptyPlayersList(&activePlayersList);

	//I loop and create the list with the names of the players and their random boards
	for (int i = 0; i < x; i++) {

		Player currPlayer;
		printf("please insert the name: \n");
		scanf("%s", currPlayer.name);

		int N;
		// If we want a random number between 1 to 20:
		N = 1 + rand() % 20;

		//Fill the current player's board with zeros
		for (int j = 0; j < GRID_SIZE; j++) {
			for (int l = 0; l < GRID_SIZE; l++){
				currPlayer.board[j][l] = 0;
			}
		}
		populateGrid(currPlayer.board, N);
		// Change the ilegal 0 into -1 from the populateGrid function
		for (int l = 0; l < GRID_SIZE; l++) {
			for (int m = 0; m < GRID_SIZE; m++) {
				if (currPlayer.board[l][m] == 0)
					currPlayer.board[l][m] = -1;
			}
		}
		sudokoPrintBoard(currPlayer.board);
		currPlayer.PossibleDigits = PossibleDigits(currPlayer.board);
		insertDataToEndPlayersList(&activePlayersList, currPlayer);
	}

	//I create an array of pointers so that every element is pointed to a list node
	PlayersListNode** playersArray = (PlayersListNode**)malloc(x * sizeof(PlayersListNode*));
	PlayersListNode* curr = activePlayersList.head;
	for (int i = 0; i < x; i++) {
		playersArray[i] = curr;
		curr = curr->next;
	}

	//Sort the array
	bubbleSortPlayers(playersArray, x);

	//Find the new 'n' size
	int n = findLogSize(x);

	//I relocate the size of the array to the new size n and then I put NULL in the new array cells
	playersArray = realloc(playersArray, n * sizeof(PlayersListNode*));
	for (int i = x; i < n; i++) 
		playersArray[i] = NULL;

	//I build the tree using the new sized array
	PlayersTree playersTree;
	playersTree = BuildTreeFromArray(playersArray, n);

	PlayersList winnersList;

	makeEmptyPlayersList(&winnersList);

	//Finally, I begin the game by iterating for every player in the tree!
	while (activePlayersList.head != NULL) 
		scanTreeInOrderRec(playersTree.root, &activePlayersList, &winnersList);
	
	//Print winners into a txt file
	FILE* f;
	PlayersListNode* currWinner = winnersList.head;
	f = fopen("output.txt", "w");
	if (f == NULL){
		printf("Failed opening the file. Exiting!\n");
		return 1;
	}
	while (currWinner != NULL) {
		fprintf(f, "Player Name: %s\n", currWinner->player.name);
		sudokuPrintBoardToFile(f, currWinner->player.board);
		currWinner = currWinner->next;
	}
	fclose(f);

	//Free everything
	freeTree(playersTree);
	freeList(activePlayersList);
	freeList(winnersList);
	free(playersArray);
	return 0;
}

void bubbleSortPlayers(PlayersListNode** array, int size) {
	
	for (int i = 0; i < size - 1; i++) {
		for (int j = 0; j < size - 1 - i; j++) {
			//Check how many filled cells are in the players boards
			int firstPlayerBoard = countFilledCells(array[j]->player.board);
			int secondPlayerBoard = countFilledCells(array[j + 1]->player.board);
			if (firstPlayerBoard > secondPlayerBoard) 
				swap(array, j, j + 1);
			else if (firstPlayerBoard == secondPlayerBoard) {
				if (strcmp(array[j]->player.name, array[j + 1]->player.name) > 0)
					swap(array, j, j + 1);
			}
		}
	}
}

void swap(PlayersListNode** array, int i, int j) {
	
	PlayersListNode* temp;

	// Swap pointers if out of order
	temp = array[i];
	array[i] = array[j];
	array[j] = temp;
}

int findLogSize(int x) {

	//I find the size of n by increasing it by 1 and then checking the correct power of two
	//that is the same or larger than n
	int n = x + 1;
	int powersOfTwo = 2;
	int i = 1;
	while (i < n) {
		if (powersOfTwo >= n)
			break;
		else
			powersOfTwo = powersOfTwo * 2;
		i++;
	}

	n = 2;
	for (int j = 1; j < i; j++)
		n = n * 2;

	n = n - 1;

	return n;
}

void sudokuPrintBoardToFile(FILE* f, short board[][GRID_SIZE]) {
	
	fprintf(f, "  | 0 1 2| 3 4 5| 6 7 8\n");
	fprintf(f, "-----------------------\n");
	for (int i = 0; i < 9; i++) {
		fprintf(f, "%d| ", i);  // Row number
		// Printing the board columns
		for (int j = 0; j < 9; j++) {
			if (j == 2 || j == 5) {
				if (board[i][j] == -1)
					fprintf(f, " | ");
				else
					fprintf(f, "%d| ", board[i][j]);
			}
			else {
				if (board[i][j] == -1)
					fprintf(f, "  ");
				else
					fprintf(f, "%d ", board[i][j]);
			}
		}
		// New line after each row
		fprintf(f, "\n");
		// Print separator after certain rows
		if (i == 2 || i == 5 || i == 8) {
			fprintf(f, "-----------------------\n");
		}
	}
	fprintf(f, "\n");
}

void sudokoPrintBoard(short board[][GRID_SIZE]) {

	printf(" | 0 1 2| 3 4 5| 6 7 8\n");
	printf("----------------------\n");
	for (int i = 0; i < GRID_SIZE; i++) {
		printf("%d| ", i);
		for (int j = 0; j < GRID_SIZE; j++) {
			if (j == 2 || j == 5) {
				if (board[i][j] == -1)
					printf(" | ");
				else
					printf("%d| ", board[i][j]);
			}
			else {
				if (board[i][j] == -1)
					printf("  ");
				else
					printf("%d ", board[i][j]);
			}
		}
		printf("\n");
		if (i == 2 || i == 5 || i == 8) {
			printf("-----------------------\n");
		}
	}
}