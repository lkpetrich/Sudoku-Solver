//
//  BoardDimensionsGetter.h
//  Sudoku Solver
//
//  Created by Loren Petrich on 12/30/16.
//  Copyright © 2016 Loren Petrich. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface BoardDimensionsGetter : NSWindowController

// The block dimensions
- (NSUInteger)GetBlockRows;
- (NSUInteger)GetBlockCols;

@end
