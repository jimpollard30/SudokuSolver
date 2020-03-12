# SudokuSolver
Solves Sudoku.  Like it says on the tin.

Reads a text file representation of a Sudoku puzzle, which should be 9 rows of text.  Each row consists of digits 1 through 9 or 'x' for the blank cells in the puzzle.  Example:

x,2,x,5,x,1,x,9,x

8,x,x,2,x,3,x,x,6

x,3,x,x,6,x,x,7,x

x,x,1,x,x,x,6,x,x

5,4,x,x,x,x,x,1,9

x,x,2,x,x,x,7,x,x

x,9,x,x,3,x,x,8,x

2,x,x,8,x,4,x,x,7

x,1,x,9,x,7,x,6,x

...currently, this file must be named 'puzzle_input.txt'.  The executable may be launched from the command line as follows:

>Sudokusolver.exe

...it will read the input and recursively solve the puzzle.

This implementation basically replicates how a human might solve the problem.  For each empty cell, examine all other cells in the same row/column or 'group of 9', eliminating digits that were either in the original puzzle or previously deduced.  You will then have cells with 'lists' of candidate digits, that you must mentally imagine being written in that cell, and then once again iterate through row, column, and 'group of 9' to see if that candidate digit will allow for a consistent puzzle.  

The implementation recurses and deduces until the puzzle array is completely filled in, with each digit appearing once and once only in each row, column, and 'group of 9'.

No doubt there are optimizations that could be done, but the initial intent was re-familiarization with visual studio, C++, the standard library, and so forth.  (Nor did I get sophisticated in terms of class design, etc.)

