
#ifndef HEADER_H
#define HEADER_H
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define SUBGRID_SIZE 3
#define GRID_SIZE 9
#define TOTAL_CELLS 81  // Total number of cells in the grid

// Structure to represent a node in the linked list of the random board
struct Node {
    int row;
    int column;
    struct Node* next;
};

typedef struct _Cell {
    int rowNum;
    int colNum;
} Cell;

typedef struct _Array {
    short* arr;
    unsigned short size;
} Array;

typedef struct _Player {
    char name[100];
    short board[GRID_SIZE][GRID_SIZE];
    Array*** PossibleDigits;
} Player;

typedef struct playerslistNode {
    Player player;
    struct playerslistNode* next;
    struct playerslistNode* prev;
}PlayersListNode;

typedef struct playerslist {
    PlayersListNode* head;
    PlayersListNode* tail;
}PlayersList;

typedef struct PlayersTreeNode {
    PlayersListNode* arrayCell;
    struct PlayersTreeNode* left;
    struct PlayersTreeNode* right;
} PlayersTreeNode;

typedef struct PlayersTree {
    PlayersTreeNode* root;
} PlayersTree;

Array*** PossibleDigits(short sudokuBoard[][GRID_SIZE]);
int* CheckTakenDigits(short sudokuBoard[][GRID_SIZE], int rowNum, int colNum, int* size);
int* CheckCurrCube(int rowNum, int colNum);
int OneStage(short board[][GRID_SIZE], Array*** possibilities, int* x, int* y);
void ChangePossibilitiesArr(Array*** possibilities, int x, int y, int insertedNum, int rowStart, int columnStart);
void removeNumberFromPossibilities(Array* possibilities, int num);
int FillBoard(short board[][GRID_SIZE], Array*** possibilities);
int countFilledCells(short board[][GRID_SIZE]);
int checkBoard(short board[][GRID_SIZE], int x, int y, int rowStart, int columnStart);
void sudokoPrintBoard(short board[][GRID_SIZE]);
void bubbleSortPlayers(PlayersListNode** array, int size);
void swap(PlayersListNode** array, int i, int j);
void makeEmptyPlayersList(PlayersList* lst);
void insertDataToEndPlayersList(PlayersList* lst, Player player);
PlayersListNode* createNewPlayersListNode(Player player, PlayersListNode* prev, PlayersListNode* next);
void insertNodeToEndPlayersList(PlayersList* lst, PlayersListNode* newTail);
bool isEmptyPlayersList(PlayersList lst);
void bubbleSortPlayers(PlayersListNode** array, int size);
void swap(PlayersListNode** array, int i, int j);
int findLogSize(int x);
PlayersTree BuildTreeFromArray(PlayersListNode** arr, int size);
PlayersTreeNode* BuildTreeFromArrayRec(PlayersListNode** arr, int start, int end);
PlayersTreeNode* createNewTreeNode(PlayersListNode* player, PlayersTreeNode* left, PlayersTreeNode* right);
void scanTreeInOrderRec(PlayersTreeNode* root, PlayersList* activePlayersList, PlayersList* winnersList);
void deleteListNode(PlayersList* list, PlayersListNode* nodeToDelete);
void freeList(PlayersList lst);
void freePlayerResources(Player* player);
void freeTree(PlayersTree tr);
void freeTreeRec(PlayersTreeNode* root);
void sudokuPrintBoardToFile(FILE* f, short board[][GRID_SIZE]);
struct Node* createNode(int row, int column);
void addNode(struct Node** head, int row, int column);
void freeNode(struct Node* node);
void shuffleArray(int array[], int size);
struct Node* removeRandomNode(struct Node** head, int index);
void fillSubGrid(short grid[GRID_SIZE][GRID_SIZE], int subGridRow, int subGridCol);
void fixConflicts(short grid[GRID_SIZE][GRID_SIZE], int row, int col, int n1);
int findValidReplacement(short grid[GRID_SIZE][GRID_SIZE], int row, int col);
void populateGrid(short grid[GRID_SIZE][GRID_SIZE], int N);
int isValid(short grid[GRID_SIZE][GRID_SIZE], int row, int col, int value);
int get_N_number(int min, int max);

#endif