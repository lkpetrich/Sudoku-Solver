//
//  SudokuBkgd.m
//  Sudoku Solver
//
//  Created by Loren Petrich on 1/2/17.
//  Copyright © 2017 Loren Petrich. All rights reserved.
//

#import "SudokuConstants.h"
#import "SudokuBkgd.h"

// The hue varies from 0 to 1
inline NSColor *GetBkgdColor(CGFloat Hue)
{
     return [NSColor colorWithCalibratedHue:Hue saturation:1 brightness:1 alpha:0.2];
}

@implementation SudokuBkgd

- (id)initWithRows:(NSUInteger)BlockRows Cols:(NSUInteger)BlockCols
{
    // Set up the sizes before doing initing proper
    NSUInteger Rows = BlockRows;
    NSUInteger Cols = BlockCols;
    NSUInteger SideLength = Rows * Cols;
    NSUInteger BoardSideLen = SideLength*BDCellSize + 2*BDCellMargin;
    NSRect BoardRect = NSMakeRect(0,0,BoardSideLen,BoardSideLen);
    
    self = [super initWithFrame:BoardRect];
    [self setBounds:BoardRect];
    
    // Draw the board background
    self.wantsLayer = YES;
    
    // The colors to use
    NSColor *EvenEvenColor = GetBkgdColor(0.0);
    NSColor *EvenOddColor = GetBkgdColor(0.1);
    NSColor *OddEvenColor = GetBkgdColor(0.6);
    NSColor *OddOddColor = GetBkgdColor(0.5);
    NSColor *LineColor = [NSColor grayColor];
    
    BOOL (^BackgroundDrawer)(NSRect) = ^BOOL (NSRect DestRect) {
        // Draw!
        for (uint ir=0; ir<SideLength; ir++)
        {
            // The cell backgrounds
            for (uint ic=0; ic<SideLength; ic++)
            {
                // the coordinates
                uint ibr = ir / Rows;
                uint ibc = ic / Cols;
                
                // The color
                bool CellParity = ((ir+ic) % 2) == 0;
                bool BlockParity = ((ibr+ibc) % 2) == 0;
                NSColor *BkgdColor = (BlockParity) ?
                ((CellParity) ? EvenEvenColor : EvenOddColor) :
                ((CellParity) ? OddEvenColor : OddOddColor);
                [BkgdColor set];
                
                // The shape
                NSUInteger HorizOrigin = ic*BDCellSize + BDCellMargin;
                NSUInteger VertOrigin = ir*BDCellSize + BDCellMargin;
                NSRect BkgdRect = NSMakeRect(HorizOrigin, VertOrigin, BDCellSize, BDCellSize);
                NSBezierPath *BkgdPath = [NSBezierPath bezierPathWithRect:BkgdRect];
                [BkgdPath fill];
            }
            
            // The cell boundary lines
            // Thick for major and thin for minor
            [LineColor set];
            for (int i=0; i<=SideLength; i++)
            {
                NSRect LineRect;
                NSUInteger LWFac;
                NSUInteger LineOrigin, LineWidth, LineOffset, LineLength;
                NSBezierPath *LinePath;
                
                // Horizontal
                LWFac = ((i % Rows) == 0) ? 2 : 1;
                LineOrigin = BDCellMargin - LWFac;
                LineOffset = LineOrigin + i*BDCellSize;
                LineWidth = 2*LWFac;
                LineLength = SideLength*BDCellSize + LineWidth;
                LineRect = NSMakeRect(LineOrigin,LineOffset,LineLength,LineWidth);
                LinePath = [NSBezierPath bezierPathWithRect:LineRect];
                [LinePath fill];
                
                // Vertical
                LWFac = ((i % Cols) == 0) ? 2 : 1;
                LineOrigin = BDCellMargin - LWFac;
                LineOffset = LineOrigin + i*BDCellSize;
                LineWidth = 2*LWFac;
                LineLength = SideLength*BDCellSize + LineWidth;
                LineRect = NSMakeRect(LineOffset,LineOrigin,LineWidth,LineLength);
                LinePath = [NSBezierPath bezierPathWithRect:LineRect];
                [LinePath fill];
            }
        }
        return YES;
    };
    
    // Use offscreen buffer
    NSImage *BkgdImg = [NSImage imageWithSize:NSMakeSize(BoardSideLen, BoardSideLen)
                                             flipped:YES drawingHandler:BackgroundDrawer];
    
    // Use this background image
    self.layer.contents = BkgdImg;
    
    return self;
}

// Flip the vertical direction
- (BOOL)isFlipped { return YES; }

@end
