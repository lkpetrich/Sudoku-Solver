//
//  BoardDimensionsGetter.m
//  Sudoku Solver
//
//  Created by Loren Petrich on 12/30/16.
//  Copyright © 2016 Loren Petrich. All rights reserved.
//

#import "SudokuConstants.h"
#import "BoardSetup.h"
#import "SolverParametersSetup.h"

// Uses menu-item tags to get the dimension values,
// so as to avoid location in the menu or having to parse menu-item text

static void FillOutMenu(NSPopUpButton *Menu, unsigned int DefaultSize)
{
    for (unsigned int i=MinBlockSize; i<=MaxBlockSize; i++)
    {
        NSString *Title = [NSString stringWithFormat:@"%d", i];
        [Menu addItemWithTitle:Title];
        NSMenuItem *Item = [Menu itemWithTitle:Title];
        Item.tag = i;
        if (i == DefaultSize) [Menu selectItem:Item];
    }
}


@interface BoardSetup ()

{
    // Whether OK or canceled
    NSInteger IsOK;
    
    // The block dimensions
    NSUInteger BlockRows, BlockCols;
    
    // The algorithm parameters
    SolverParameters SolverParams;
    
    // The user-interface widgets
    IBOutlet NSPopUpButton *RowsPopup, *ColsPopup;
    IBOutlet NSButton *SquareCheckbox;
    IBOutlet NSTextField *SideLength, *BoardSize;
    IBOutlet NSSegmentedControl *VisibleSetSize, *HiddenSetSize;
    IBOutlet NSButton *UseIntersections;
}

- (IBAction)UpdateBlockDimensions: (id)sender;

@end

@implementation BoardSetup

// What we want
- (NSUInteger)GetBlockRows {return BlockRows;}
- (NSUInteger)GetBlockCols {return BlockCols;}
- (SolverParameters)GetSolverParameters {return SolverParams;}

- (IBAction)UpdateBlockDimensions: (id)sender {
    if (sender == RowsPopup)
    {
        BlockRows = RowsPopup.selectedTag;
        if (SquareCheckbox.state)
        {
            BlockCols = BlockRows;
            [ColsPopup selectItemWithTag:BlockCols];
        }
    }
    else if (sender == ColsPopup)
    {
        BlockCols = ColsPopup.selectedTag;
        if (SquareCheckbox.state)
        {
            BlockRows = BlockCols;
            [RowsPopup selectItemWithTag:BlockRows];
        }
    }
    else if (sender == SquareCheckbox)
    {
        if (SquareCheckbox.state)
        {
            [self UpdateBlockDimensions:ColsPopup];
        }
        
        // So won't recalculate the board size here
        return;
    }
    
    NSUInteger SideLengthValue = BlockRows * BlockCols;
    NSUInteger BoardSizeValue = SideLengthValue * SideLengthValue;
    
    [SideLength setIntegerValue:SideLengthValue];
    [BoardSize setIntegerValue:BoardSizeValue];
}

- (IBAction)GetSolverParameters:(NSButton *)sender
{
    SolverParametersSetup *SPS = [[SolverParametersSetup alloc] initWithParams:SolverParams];
    
    NSInteger SPIsOK = [NSApp runModalForWindow:SPS.window];
    if (SPIsOK != 0)
        SolverParams = [SPS GetSolverParameters];
}

// Startup code

- (id)init {
    self = [super initWithWindowNibName:@"BoardSetup"];
    
    // Cancel
    IsOK = 0;
    
    return self;
}

- (void)windowDidLoad {
    [super windowDidLoad];
    
    // Create the menu items programmatically
    FillOutMenu(RowsPopup, DfltNumRows);
    FillOutMenu(ColsPopup, DfltNumCols);
    
    // Read off the default selections
    [self UpdateBlockDimensions:RowsPopup];
    [self UpdateBlockDimensions:ColsPopup];
}

// Explicit because it also handles the close button in the window title
- (void)windowWillClose:(NSNotification*)notification
{
    [NSApp stopModalWithCode:IsOK];
}

- (IBAction)ButtonAction:(NSButton *)sender
{
    // All done
    IsOK = [sender tag];
    [self.window close];
}

@end
