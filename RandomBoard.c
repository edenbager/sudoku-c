#include "Header.h"

struct Node* createNode(int row, int column) {
	struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
	newNode->row = row;
	newNode->column = column;
	newNode->next = NULL;
	return newNode;
}

void addNode(struct Node** head, int row, int column) {
	struct Node* newNode = createNode(row, column);
	newNode->next = *head;
	*head = newNode;
}

// Function to remove a node and return its value (randomly chosen)
struct Node* removeRandomNode(struct Node** head, int index) {
	struct Node* temp = *head;
	struct Node* prev = NULL;

	// Handle empty list case
	if (*head == NULL) {
		return NULL;
	}

	if (index == 0) {
		*head = temp->next;
		return temp;
	}

	// Traverse the list safely
	for (int i = 0; temp != NULL && i < index; i++) {
		prev = temp;
		temp = temp->next;
	}

	// Check if we reached the end of the list
	if (temp == NULL) {
		return NULL;
	}

	prev->next = temp->next;
	return temp;
}

// Properly free the node and update the linked list
void freeNode(struct Node* node) {
	if (node != NULL) {
		free(node);
	}
}

void shuffleArray(int array[], int size) {
	for (int i = size - 1; i > 0; i--) {
		int j = rand() % (i + 1);
		int temp = array[i];
		array[i] = array[j];
		array[j] = temp;
		//printf("shuffle\n");
	}
}

// Function to check if the number n2 conflicts with any other number in the row, column or 3x3 grid
void fixConflicts(short grid[GRID_SIZE][GRID_SIZE], int row, int col, int n1) {
	// Loop through the row and column to find conflicts
	for (int i = 0; i < GRID_SIZE; i++) {
		if (grid[row][i] == n1 && i != col) { // Conflict in the same row
			grid[row][i] = findValidReplacement(grid, row, i); // Replace with valid number
		}
		if (grid[i][col] == n1 && i != row) { // Conflict in the same column
			grid[i][col] = findValidReplacement(grid, i, col); // Replace with valid number
		}
		printf("check row and column\n");
	}

	// Check the 3x3 sub-grid for conflicts
	int subGridRow = (row / SUBGRID_SIZE) * SUBGRID_SIZE;
	int subGridCol = (col / SUBGRID_SIZE) * SUBGRID_SIZE;
	for (int i = 0; i < SUBGRID_SIZE; i++) {
		for (int j = 0; j < SUBGRID_SIZE; j++) {
			int currentRow = subGridRow + i;
			int currentCol = subGridCol + j;
			if (grid[currentRow][currentCol] == n1 && currentRow != row && currentCol != col) {
				grid[currentRow][currentCol] = findValidReplacement(grid, currentRow, currentCol); // Replace with valid number
			}
			//printf("checked cube\n");
		}
	}
}

// Helper function to find a valid replacement for a conflicting number
int findValidReplacement(short grid[GRID_SIZE][GRID_SIZE], int row, int col) {
	for (int value = 1; value <= GRID_SIZE; value++) {
		if (isValid(grid, row, col, value)) {
			return value; // Return the first valid number found
		}
		//printf("found replacement\n");
	}
	// was 0
	return -1; // Default to 0 if no valid number is found (this should not happen in a valid Sudoku grid)
}

// Check if a value is valid for the given position (row, column)
int isValid(short grid[GRID_SIZE][GRID_SIZE], int row, int col, int value) {
	for (int i = 0; i < GRID_SIZE; i++) {
		if (grid[row][i] == value || grid[i][col] == value) {
			return 0;
		}
		//printf("is valid\n");
	}

	int startRow = (row / SUBGRID_SIZE) * SUBGRID_SIZE;
	int startCol = (col / SUBGRID_SIZE) * SUBGRID_SIZE;

	for (int i = 0; i < SUBGRID_SIZE; i++) {
		for (int j = 0; j < SUBGRID_SIZE; j++) {
			if (grid[startRow + i][startCol + j] == value) {
				return 0;
			}
			//printf("is valid\n");
		}
	}

	return 1;
}

// Fill a sub-grid with shuffled values
void fillSubGrid(short grid[GRID_SIZE][GRID_SIZE], int subGridRow, int subGridCol) {
	int validValues[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	shuffleArray(validValues, 9);

	int index = 0;
	for (int row = subGridRow; row < subGridRow + SUBGRID_SIZE; row++) {
		for (int col = subGridCol; col < subGridCol + SUBGRID_SIZE; col++) {
			grid[row][col] = validValues[index++];
			fixConflicts(grid, row, col, grid[row][col]);
			//printf("fill\n");
		}
	}
}

void populateGrid(short grid[GRID_SIZE][GRID_SIZE], int N) {
	struct Node* head = NULL;
	int filledCells = 0; // Start with zero filled cells

	// Populate linked list with all available cells
	for (int i = 0; i < GRID_SIZE; i++) {
		for (int j = 0; j < GRID_SIZE; j++) {
			addNode(&head, i, j);
		}
	}

	// We need to place exactly N numbers randomly in the grid
	while (filledCells < N) {
		int index = rand() % (GRID_SIZE * GRID_SIZE);  // Randomly select a cell
		struct Node* chosenNode = removeRandomNode(&head, index);

		if (chosenNode != NULL) {
			int row = chosenNode->row;
			int col = chosenNode->column;

			// Try to place a random number between 1 and 9 in the chosen cell
			int number = get_N_number(1, GRID_SIZE);  // Generate a random number between 1 and 9

			if (isValid(grid, row, col, number)) {
				grid[row][col] = number;  // Place the number in the grid
				filledCells++;
			}

			free(chosenNode);  // Free the chosen node after using it
		}

		// If we run out of cells and haven't placed N numbers, we may need to retry
		if (head == NULL && filledCells < N) {
			printf("Error: Not enough valid cells to place %d numbers.\n", N);
			break;
		}
	}
}

int get_N_number(int min, int max) {
	return rand() % (max - min + 1) + min;
}