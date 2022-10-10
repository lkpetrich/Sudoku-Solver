# Sudoku-Solver
Solves Sudoku puzzles.

Standard Sudoku uses a square board with 3x3 square blocks of 3x3 cells each.
Each one can have a number from 1 to 9 (or any set of 9 distinct symbols).
Each row, column, and block can contain exactly one of each number.
A Sudoku puzzle consists of a Sudoku board with only some of the cells filled in.
To solve that puzzle, one must find the number values for the empty cells.

This application implements not only standard Sudoku, but variants with mxn blocks, each with size nxm.

It has the option of selecting solver parameters: which solver algorithms to use.
It implements:

- Visible sets, sizes 0, 1, 2, 3, 4, 5
- Hidden sets, sizes 0, 1, 2, 3, 4, 5
- Strip-block intersections (strip = row or column)

Visible and hidden sets are sets of candidate values in groups: rows, columns, and blocks.
- A visible set with size n has n cells with n distinct candidate values in them and no others.
The solver then clears all these candidate values from every group that contains all those cells.
- A hidden set with size n also has n cells with n distinct values,
values that are absent from the other cells in the group that these cells are in.
However, these cells may contain additional values, and the solver clears them.

For intersections, if a strip-block intersection contains some value that is not in the rest of the strip,
then the rest of the block is cleared of that value.
Likewise, if that value is not in the rest of the block,
then the rest of the strip is cleared of that value.

Written for macOS using Cocoa, but the solver part is in C++ and should be cross-platform.
