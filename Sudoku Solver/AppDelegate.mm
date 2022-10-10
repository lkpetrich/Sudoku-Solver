//
//  AppDelegate.m
//  Sudoku Solver
//
//  Created by Loren Petrich on 7/30/16.
//  Copyright © 2016 Loren Petrich. All rights reserved.
//

#import "SudokuConstants.h"
#import "AppDelegate.h"
#import "BoardSetup.h"
#import "SudokuBoard.h"
#import "SolverParametersSetup.h"


bool ReadValue(FILE *f, uint &Value, uint MinVal, uint MaxVal, NSString *What)
{
    NSString *Desc;
    int rc = fscanf(f,"%u",&Value);
    
    if (rc < 1)
    {
        Desc = @"unable to read it";
        goto BadValue;
    }
    if (Value < MinVal || Value > MaxVal)
    {
        Desc = [NSString stringWithFormat:@"%u is outside %u to %u", Value, MinVal, MaxVal];
        goto BadValue;
    }
    return true;
    
BadValue:
    
    NSString *Msg = [NSString stringWithFormat:@"%@: %@", What, Desc];
    
    NSAlert *Alert = [[NSAlert alloc] init];
    [Alert addButtonWithTitle:@"OK"];
    Alert.messageText = Msg;
    Alert.informativeText = @"Could not create a Sudoku board.";
    Alert.alertStyle = NSAlertStyleCritical;
    
    [Alert runModal];
    
    return false;
}


@implementation AppDelegate

{
    // Keeps list of windows for retaining them
    NSMutableArray<SudokuBoard *> *WindowList;
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    WindowList = [NSMutableArray arrayWithCapacity:1024];
    
    // Make a viewer
    [self newDocument:self];
}

- (IBAction)newDocument:(id)sender
{
    // Make an empty board
    
    // Request the block dimensions: number of rows and columns (vertical, horizontal)
    BoardSetup *BDST = [[BoardSetup alloc] init];
    NSInteger IsOK = [NSApp runModalForWindow:BDST.window];
    if (!IsOK) return;
    
    NSUInteger BlockRows = [BDST GetBlockRows];
    NSUInteger BlockCols = [BDST GetBlockCols];
    SolverParameters SolverParams = [BDST GetSolverParameters];
    
    // Make a board
    SudokuBoard *Board = [[SudokuBoard alloc] initWithRows:BlockRows Cols:BlockCols Params:SolverParams];
    [Board showWindow:self];
    [WindowList addObject:Board];
}

 - (void)ReadFile:(NSURL *)FileRef {
     NSString *FilePath = FileRef.path;
     const char *FilePathCStr = [FilePath fileSystemRepresentation];
     
     // Read in the file and load up a data array
     
     FILE *f = fopen(FilePathCStr,"r");
     
     uint BlockRows, BlockCols, SideLength, BoardSize;
     
     if (!ReadValue(f, BlockRows, MinBlockSize, MaxBlockSize, @"Number of rows")) return;
     if (!ReadValue(f, BlockCols, MinBlockSize, MaxBlockSize, @"Number of columns")) return;
     SideLength = BlockRows * BlockCols;
     BoardSize = SideLength * SideLength;
     
     uint ix = 0;
     byte *Data = new byte[BoardSize];
     for (int irow=0; irow<SideLength; irow++)
     {
         for (int icol=0; icol<SideLength; icol++)
         {
             uint Value;
             if (!ReadValue(f, Value, 0, SideLength, @"Cell value"))
             {
                 delete []Data;
                 return;
             }
             Data[ix] = Value;
             ix++;
         }
     }
     
     fclose(f);
     
     SolverParameters SolverParams;
     SolverParametersSetup *SPS = [[SolverParametersSetup alloc] initWithParams:SolverParams];
     
     NSInteger SPIsOK = [NSApp runModalForWindow:SPS.window];
     if (SPIsOK != 0)
         SolverParams = [SPS GetSolverParameters];
     
     // Make a board
     SudokuBoard *Board = [[SudokuBoard alloc] initWithRows:BlockRows Cols:BlockCols Params:SolverParams Data:Data];
     [Board showWindow:self];
     [WindowList addObject:Board];
     
     delete []Data;
 }

- (IBAction)openDocument:(id)sender
{
    NSOpenPanel *OpenDialog = [NSOpenPanel openPanel];
    OpenDialog.canChooseFiles = YES;
    OpenDialog.canChooseDirectories = NO;
    OpenDialog.allowsMultipleSelection = NO;
    
    [OpenDialog beginWithCompletionHandler:^(NSInteger Result) {
        if (Result == NSModalResponseOK)
        {
            [self ReadFile:[[OpenDialog URLs] objectAtIndex:0]];
        }
    }];
}

- (SudokuBoard *)FindTopBoard
{
    // Finds the Sudoku-board window on top of the others,
    // returns nil if there is none
    
    // Check to see if there is a window on top
    NSWindow *TopWindow = [NSApp mainWindow];
    if (TopWindow == nil) return nil;
    
    // Check to see if it is a Sudoku-board window
    NSWindowController *TopWC = TopWindow.windowController;
    if (![TopWC isMemberOfClass:[SudokuBoard class]]) return nil;

    return (SudokuBoard *)TopWC;
}

- (IBAction)duplicateDocument:(id)sender
{
    // Running this action through here
    // to insure that the board's window gets made persistent
    // by adding it to this class's list WindowList
    SudokuBoard *TopBoard = [self FindTopBoard];
    if (TopBoard == nil) return;
    
    SudokuBoard *DupBoard = [TopBoard Duplicate];
    [DupBoard showWindow:self];
    [WindowList addObject:DupBoard];
}

- (BOOL)validateUserInterfaceItem:(id <NSValidatedUserInterfaceItem>)Item
{
    // Implemented to check on whether there is a Sudoku board that can be duplicated
    SEL ItemAction = [Item action];
    if (ItemAction == @selector(duplicateDocument:))
    {
        SudokuBoard *TopBoard = [self FindTopBoard];
        return (TopBoard != nil) ? YES : NO;
    }
    else
        return YES;
}

@end
