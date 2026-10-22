# sudoku-c

A multiplayer console Sudoku game in C. Each player gets a randomly generated board, the program fills every cell that has only one legal digit, and asks the player to choose when it gets stuck. A player wins by completing the board and is out if a choice breaks the rules.

Built with **Daniel Davidson** as the final project of an Advanced C Programming course. Originally developed in Visual Studio on Windows; it also compiles with gcc on macOS and Linux.

## Build and run

```bash
gcc -Wall -o sudoku *.c
./sudoku
```

Enter the number of players and their names. Winners' boards are written to `output.txt`.

## How it works

- **Board generation:** each player gets a board with 1 to 20 random clues, placed only where they are legal.
- **Possible digits:** for every empty cell, the program computes which digits 1-9 are still allowed by the row, column and 3x3 box.
- **Solving rounds:** cells with exactly one possibility are filled automatically, and the possibilities of related cells are updated. When no such cell is left, the player chooses a digit for the cell with the fewest options.
- **Turn order:** players are sorted by filled cells, then by name, and placed in a balanced binary tree built from the sorted array. Each round walks the tree in order.

## Data structures

- `Array***`: a 9x9 grid of dynamic arrays holding each cell's possible digits
- A doubly linked list of active players, and another of winners
- A balanced binary tree of players for turn order
- A singly linked list of free cells, used for random board generation

## Files

| File | Contents |
|---|---|
| `Header.h` | Types and declarations |
| `Q1.c` | Computing possible digits per cell |
| `Q2.c` | One solving round and board validation |
| `Q3.c` | Single-player fill loop |
| `RandomBoard.c` | Random board generation |
| `ListsFunc.c` | Player list operations |
| `TreeFunc.c` | Building and walking the player tree |
| `RealMain.c` | Game setup, sorting and output |

## Fixes after submission

- `swap()` in the player sort ignored its first parameter, so it swapped the wrong pair and read past the end of the array.
- Cleared compiler warnings (`int main`, pointer compared to `-1`) and removed leftover debug output.

## What I'd improve

- Seed the random generator from the clock instead of a fixed `srand(12345)`, so boards differ between runs.
- Validate user input: a non-number, or a digit that isn't in the offered list, isn't handled.
- Free memory on early-return paths (for example `CubePosArray` when a board check fails).
