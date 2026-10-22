#include "Header.h"

PlayersTree BuildTreeFromArray(PlayersListNode** arr, int size) {

	PlayersTree tr;

	//I create a recrusive function which calls the array, the first and last indexes of the array and puts it inside the the tree root, which is its first node
	tr.root = BuildTreeFromArrayRec(arr, 0, size - 1);

	return tr;

}

PlayersTreeNode* BuildTreeFromArrayRec(PlayersListNode** arr, int start, int end) {

	//If start is larger than end, we have reached before the first node of the array
	if (start > end)
		return NULL;

	//Get the middle of the 'new' changed array each time in a binary algorithem
	int mid = (start + end) / 2;

	//Check if the current node is -1 and ignore it if so
	if (arr[mid] == -1)
		return NULL;

	//Get the left node by decreasing the end index of the array to be a node smaller than the curr mid, so the new mid will be exactly half of the left side
	PlayersTreeNode* leftChild = BuildTreeFromArrayRec(arr, start, mid - 1);
	//Get the right node by increasing the start index of the array to be a node larger than the curr mid, so the new mid will be exactly half of the right side
	PlayersTreeNode* rightChild = BuildTreeFromArrayRec(arr, mid + 1, end);

	//Each time create a new node with the data of the elemt in the mid index of the array
	return createNewTreeNode(arr[mid], leftChild, rightChild);

}

PlayersTreeNode* createNewTreeNode(PlayersListNode* player, PlayersTreeNode* left, PlayersTreeNode* right) {

	PlayersTreeNode* node = (PlayersTreeNode*)malloc(sizeof(PlayersTreeNode));
	node->arrayCell = player;
	node->left = left;
	node->right = right;
	return node;
}

void scanTreeInOrderRec(PlayersTreeNode* root, PlayersList* activePlayersList, PlayersList* winnersList) {

	if (root == NULL)
		return;

	//If the current player still playes the game
	if (root->arrayCell != NULL) {
		scanTreeInOrderRec(root->left, activePlayersList, winnersList);
		PlayersListNode* currentNode = root->arrayCell;
		int x = 0;
		int y = 0;
		int result = OneStage(currentNode->player.board, currentNode->player.PossibleDigits, &x, &y);
		//If the player succeeded, create a new node in the winnersList
		if (result == 1)
			insertDataToEndPlayersList(winnersList, currentNode->player);
		//If the player failed or succeeded, delete from player list
		if (result == -1 || result == 1) {
			deleteListNode(activePlayersList, currentNode);
			//Set the player's pointer to NULL
			root->arrayCell = NULL;
		}
		//If the result is 0, it means that the player did not win nor lose
		else {
			//Print board for the player to see and react to
			sudokoPrintBoard(currentNode->player.board);
			int howMany = currentNode->player.PossibleDigits[x][y]->size;
			printf("Cell number [%d, %d] currently holds the minimum number of possible values, select one of the below :\n", x, y);
			for (int i = 0; i < howMany; i++)
				printf("%d. %d\n", i + 1, currentNode->player.PossibleDigits[x][y]->arr[i]);
			int chosenNum = 0;
			scanf("%d", &chosenNum);
			currentNode->player.board[x][y] = chosenNum;
			free(currentNode->player.PossibleDigits[x][y]->arr);
			currentNode->player.PossibleDigits[x][y]->arr = NULL;
			currentNode->player.PossibleDigits[x][y]->size = 0;
			int* CubePosArray = CheckCurrCube(x, y);
			ChangePossibilitiesArr(currentNode->player.PossibleDigits, x, y, chosenNum, CubePosArray[0], CubePosArray[1]);
			if (checkBoard(currentNode->player.board, x, y, CubePosArray[0], CubePosArray[1]) == -1) {
				deleteListNode(activePlayersList, root->arrayCell);
				//Set the player's pointer to NULL
				root->arrayCell = NULL;;
			}

			free(CubePosArray);
		}
		scanTreeInOrderRec(root->right, activePlayersList, winnersList);
	}
}

void freeTree(PlayersTree tr) {

	freeTreeRec(tr.root);
}

void freeTreeRec(PlayersTreeNode* root) {

	if (root == NULL)
		return;
	else {
		freeTreeRec(root->left);
		freeTreeRec(root->right);
		free(root);
	}
}