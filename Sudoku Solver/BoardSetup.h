//
//  BoardDimensionsGetter.h
//  Sudoku Solver
//
//  Created by Loren Petrich on 12/30/16.
//  Copyright © 2016 Loren Petrich. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include "SolverParameters.h"

@interface BoardSetup : NSWindowController

// The block dimensions
- (NSUInteger)GetBlockRows;
- (NSUInteger)GetBlockCols;

// The solver parameters
- (SolverParameters)GetSolverParameters;

@end
