//
//  SudokuBoard.h
//  Sudoku Solver
//
//  Created by Loren Petrich on 12/30/16.
//  Copyright © 2016 Loren Petrich. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface SudokuBoard : NSWindowController

// Needs horizontal and vertical block dimensions

- (id)initWithRows:(NSUInteger)BlockRows Cols:(NSUInteger)BlockCols;

- (id)initWithRows:(NSUInteger)BlockRows Cols:(NSUInteger)BlockCols Data:(unsigned char *)BoardData;

- (SudokuBoard *)Duplicate;

@end
