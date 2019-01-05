//
//  SudokuConstants.h
//  Sudoku Solver
//
//  Created by Loren Petrich on 1/2/17.
//  Copyright © 2017 Loren Petrich. All rights reserved.
//

#ifndef SudokuConstants_h
#define SudokuConstants_h

typedef unsigned char byte;
typedef unsigned int uint;

// Uses C++ const keyword as a type-safe alternative to C-preprocessor #define
// Necessary to turn all the ObjC files that use it into ObjC++ files
// to avoid duplication of link-time symbols

// Maximum block size
const uint MinBlockSize = 1;
const uint MaxBlockSize = 9;

// Defaults: standard Sudoku board
const uint DfltNumRows = 3;
const uint DfltNumCols = 3;

// Board parameters
const uint BDCellSize = 48;
const uint BDCellMargin = 4;

// Popup-menu size
const uint PopupWd = 48;
const uint PopupHt = 22;


#endif /* SudokuConstants_h */
