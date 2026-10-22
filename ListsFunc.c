#include "Header.h"

void makeEmptyPlayersList(PlayersList* lst) {
	lst->head = lst->tail = NULL;
}

void insertDataToEndPlayersList(PlayersList* lst, Player player) {
	PlayersListNode* newTail;
	newTail = createNewPlayersListNode(player, NULL, NULL);
	insertNodeToEndPlayersList(lst, newTail);
}

PlayersListNode* createNewPlayersListNode(Player player, PlayersListNode* prev, PlayersListNode* next) {
	PlayersListNode* node = (PlayersListNode*)malloc(sizeof(PlayersListNode));
	node->player = player;
	node->prev = prev;
	node->next = next;
	return node;
}

void insertNodeToEndPlayersList(PlayersList* lst, PlayersListNode* newTail) {
	if (isEmptyPlayersList(*lst))
		lst->head = lst->tail = newTail;
	else {
		newTail->prev = lst->tail;
		newTail->next = NULL;
		lst->tail->next = newTail;
		lst->tail = newTail;
	}
}

bool isEmptyPlayersList(PlayersList lst) {
	return (lst.head == NULL);
}

void deleteListNode(PlayersList* list, PlayersListNode* nodeToDelete) {

	if (nodeToDelete == NULL) {
		return;
	}

	// If it's the head of the list
	if (list->head == nodeToDelete) {
		list->head = nodeToDelete->next;
	}

	// If it's the tail of the list
	if (list->tail == nodeToDelete) {
		list->tail = nodeToDelete->prev;
	}

	// Correct new prev
	if (nodeToDelete->prev != NULL) {
		nodeToDelete->prev->next = nodeToDelete->next;
	}

	// Correct new next
	if (nodeToDelete->next != NULL) {
		nodeToDelete->next->prev = nodeToDelete->prev;
	}

	free(nodeToDelete);
}

void freeList(PlayersList lst) {
	PlayersListNode* curr = lst.head, * next;
	while (curr != NULL) {
		next = curr->next;
		freePlayerResources(&(curr->player));  // Free player's resources
		free(curr);
		curr = next;
	}
}

void freePlayerResources(Player* player) {
	// Free each cell's possible digits array
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (player->PossibleDigits[i][j] != NULL) {
				free(player->PossibleDigits[i][j]->arr);
				free(player->PossibleDigits[i][j]);
			}
		}
		free(player->PossibleDigits[i]);
	}
	free(player->PossibleDigits);  // Finally, free the outer array
}