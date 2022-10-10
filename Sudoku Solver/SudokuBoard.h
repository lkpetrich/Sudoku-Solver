//
//  SudokuBoard.h
//  Sudoku Solver
//
//  Created by Loren Petrich on 12/30/16.
//  Copyright © 2016 Loren Petrich. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include "SolverParameters.h"

@interface SudokuBoard : NSWindowController

// Needs horizontal and vertical block dimensions

- (id)initWithRows:(NSUInteger)BlockRows Cols:(NSUInteger)BlockCols Params:(SolverParameters)Params_;

- (id)initWithRows:(NSUInteger)BlockRows Cols:(NSUInteger)BlockCols Params:(SolverParameters)Params_ Data:(unsigned char *)BoardData;

// As it says

- (SudokuBoard *)Duplicate;

@end
