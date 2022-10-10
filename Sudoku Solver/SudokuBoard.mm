//
//  SudokuBoard.m
//  Sudoku Solver
//
//  Created by Loren Petrich on 12/30/16.
//  Copyright © 2016 Loren Petrich. All rights reserved.
//


#import "SudokuConstants.h"
#import "SudokuBoard.h"
#import "SudokuBkgd.h"
#import "SolverParametersSetup.h"
#include "BoardManagerFrontEnd.hpp"
#include "HistoryStackFrontEnd.hpp"


static BOOL GetBoolReturn(NSInvocation *Invocation)
{
    [Invocation invoke];
    BOOL retcode;
    [Invocation getReturnValue:&retcode];
    return retcode;
}

// Approximate square root for the tooltips
// to make nearly square ones
// Rounded upward
static uint isqrt(uint a) {
    // Initial guess
    uint x = (a + 1) >> 1;
    
    // Improve with Newton's method
    for (uint iter=0; iter<16; iter++)
    {
        uint xnew = (x + a/x + 1) >> 1;
        if (xnew == x) break;
        x = xnew;
    }
    
    // All done
    return x;
}

@interface DummyUndoManager : NSUndoManager

{
    NSInvocation *CanUndoInvoke, *CanRedoInvoke;
    NSInvocation *UndoInvoke, *RedoInvoke;
}

@end

@implementation DummyUndoManager

-(void)GetCanUndoInvoke:(NSInvocation *)Invoke {CanUndoInvoke = Invoke;}
-(void)GetCanRedoInvoke:(NSInvocation *)Invoke {CanRedoInvoke = Invoke;}

-(void)GetUndoInvoke:(NSInvocation *)Invoke {UndoInvoke = Invoke;}
-(void)GetRedoInvoke:(NSInvocation *)Invoke {RedoInvoke = Invoke;}

-(BOOL)canUndo {return GetBoolReturn(CanUndoInvoke);}
-(BOOL)canRedo {return GetBoolReturn(CanRedoInvoke);}

-(void)undo {[UndoInvoke invoke];}
-(void)redo {[RedoInvoke invoke];}

@end

@interface SudokuBoard ()

{
    // Will insert the content programmatically to have full control over its insertion
    NSScrollView *ScrollCtnr;
    
    // The contents of the cells: one widget each
    NSArray<NSPopUpButton *> *CellContents;
    
    // Handles all the board details
    BoardManagerFrontEnd BMFE;
    
    // Handles the undoing and redoing
    HistoryStackFrontEnd HSFE;
    
    // Kludge to get around the undo and redo menu items not being handled like the others
    DummyUndoManager *UndoManager;
    
    // Tooltip stuff
    // For showing the available values with a mouseover
    
    // Number values for the tooltips
    NSMutableArray<NSString *> *TooltipValues;
    
    // Tooltip members, for the tooltip text
    NSMutableArray<NSString *> *TooltipMembers;
    
    // Tooltip final result
    NSMutableString *TooltipString;
}

- (IBAction)ShowSolverParameters:(id)sender;

@end

@implementation SudokuBoard

- (id)initWithRows:(NSUInteger)BlockRows Cols:(NSUInteger)BlockCols Params:(SolverParameters)Params
{
    self = [super initWithWindowNibName:@"SudokuBoard"];
    
    // Transmitting the block dimensions
    self->BMFE.SetBlockDimensions((uint)BlockRows, (uint)BlockCols, Params);
    
    // Undo-manager kludge
    UndoManager = [[DummyUndoManager alloc] init];
    
    // Doing invocations for method calls on this clase
    // so that the undo manager does not have to have
    // the details of this class
    
    SEL CanUndoSel = @selector(canUndo);
    SEL CanRedoSel = @selector(canRedo);
    SEL UndoSel = @selector(undo:);
    SEL RedoSel = @selector(redo:);
    
    NSInvocation *CanUndoInvoke = [self MakeInvocation:CanUndoSel];
    NSInvocation *CanRedoInvoke = [self MakeInvocation:CanRedoSel];
    NSInvocation *UndoInvoke = [self MakeInvocation:UndoSel];
    NSInvocation *RedoInvoke = [self MakeInvocation:RedoSel];
    
    [UndoInvoke setArgument:&self atIndex:2];
    [RedoInvoke setArgument:&self atIndex:2];
    
    [UndoManager GetCanUndoInvoke:CanUndoInvoke];
    [UndoManager GetCanRedoInvoke:CanRedoInvoke];
    [UndoManager GetUndoInvoke:UndoInvoke];
    [UndoManager GetRedoInvoke:RedoInvoke];
    
    return self;
}

- (id)initWithRows:(NSUInteger)BlockRows Cols:(NSUInteger)BlockCols Params:(SolverParameters)Params Data:(unsigned char *)BoardData
{
    self = [self initWithRows:BlockRows Cols:BlockCols Params:Params];
    
    // Load its data
    BoardImport(BMFE,BoardData);
    
    return self;
}

- (NSInvocation *)MakeInvocation:(SEL)Selector {
    NSMethodSignature *Signature = [[self class] instanceMethodSignatureForSelector:Selector];
    NSInvocation *Invoke = [NSInvocation invocationWithMethodSignature:Signature];
    Invoke.target = self;
    Invoke.selector = Selector;
    return Invoke;
}

- (NSUndoManager *)windowWillReturnUndoManager:(NSWindow *)sender
{
    return UndoManager;
}

- (void)windowDidLoad {
    [super windowDidLoad];
    
    // Create the background view
    SudokuBkgd *Bkgd = [[SudokuBkgd alloc] initWithRows:BMFE.GetBlockRows() Cols:BMFE.GetBlockCols()];
    
    // Add all the cells' popup menus and their items
    uint SideLen = BMFE.GetSideLength();
    uint OffsetWd = BDCellMargin + ((BDCellSize - PopupWd)/2);
    uint OffsetHt = BDCellMargin + ((BDCellSize - PopupHt)/2);
    
    SEL Updater = @selector(UpdateBoardWithCellContents:);
    NSMutableArray<NSPopUpButton *> *TempCellContents = [NSMutableArray arrayWithCapacity:BMFE.GetBoardSize()];
    
    for (int irow=0; irow<SideLen; irow++)
    {
        NSUInteger ptrow = OffsetHt + irow*BDCellSize;
        for (int icol=0; icol<SideLen; icol++)
        {
            NSUInteger ptcol = OffsetWd + icol*BDCellSize;
            
            NSRect MenuFrame = NSMakeRect(ptcol,ptrow,PopupWd,PopupHt);
            NSPopUpButton *Menu = [[NSPopUpButton alloc] initWithFrame:MenuFrame pullsDown:NO];
            [Menu setBounds:MenuFrame];
            Menu.action = Updater;
            Menu.target = self;
            Menu.autoenablesItems = NO;
            
            NSPopUpButtonCell *MenuCell = (NSPopUpButtonCell *)[Menu cell];
            MenuCell.arrowPosition = NSPopUpNoArrow;
            
            for (unsigned int i=0; i<=SideLen; i++)
            {
                NSString *Title = (i > 0) ? [NSString stringWithFormat:@"%2d", i] : @"";
                [Menu addItemWithTitle:Title];
                NSMenuItem *Item = [Menu itemWithTitle:Title];
                Item.tag = i;
            }
            
            [TempCellContents addObject:Menu];
            [Bkgd addSubview:Menu];
        }
    }
    CellContents = [NSArray arrayWithArray:TempCellContents];
    
    // Initialize the history stack
    HSFE.SetSize(BMFE.GetBoardSize());
    BoardExport(BMFE,HSFE.Add());
    
    // Update the board with the initial data
    [self UpdateBoard:self];
    
    // Scroller init code pinched from some Apple sample code
    NSWindow *SelfWindow = [self window];
    NSScrollView *Scroller = [[NSScrollView alloc] initWithFrame:[[SelfWindow contentView] frame]];
    [Scroller setHasVerticalScroller:YES];
    [Scroller setHasHorizontalScroller:YES];
    [Scroller setBorderType:NSNoBorder];
    [Scroller setAutoresizingMask:NSViewWidthSizable|NSViewHeightSizable];
    [Scroller setDocumentView:Bkgd];
    [SelfWindow setContentView:Scroller];
    [SelfWindow makeKeyAndOrderFront:nil];
    
    // Tooltip stuff
    
    // Number values for the tooltips
    TooltipValues = [NSMutableArray arrayWithCapacity:(SideLen+1)];
    
    for (int ival=0; ival <= SideLen+1; ival++)
        [TooltipValues addObject:[NSString stringWithFormat:@"%d", ival]];
    
    // Tooltip members, for the tooltip text
    TooltipMembers = [NSMutableArray arrayWithCapacity:(SideLen)];
    
    // Tooltip final result
    TooltipString = [NSMutableString stringWithCapacity:(4*SideLen)];
    
    // Place the tooltips in the starting board
    [self UpdateBoard:self];
}

- (void)UpdateBoardWithCellContents:(id)sender {

    // Read off of the cell-contents widgets
    uint ix = 0;
    uint SideLen = BMFE.GetSideLength();
    for (int irow=0; irow<SideLen; irow++)
    {
        for (int icol=0; icol<SideLen; icol++)
        {
            NSPopUpButton *Menu = [CellContents objectAtIndex:ix];
            ix++;
            BMFE.Value(irow,icol) = Menu.selectedTag;
        }
    }
    
    // Add to the history stack
    BoardExport(BMFE,HSFE.Add());
    
    [self UpdateBoard:sender];
}

- (void)UpdateBoard:(id)sender {
    // Run the solver
    BMFE.SolveAndUpdate();
    
    // Set the cell-contents widgets
    // Not only the values selected, but also the values' activity
    // Only enabled ones will be those that the solver allows
    // If the solution process finds none enabled, then the solution process has stalled
    uint ix = 0;
    uint SideLen = BMFE.GetSideLength();
    uint NumEmpty = 0, NumFull = 0;
    bool BadBoard = false;
    
    for (int irow=0; irow<SideLen; irow++)
    {
        for (int icol=0; icol<SideLen; icol++)
        {
            NSPopUpButton *Menu = [CellContents objectAtIndex:ix];
            ix++;
            byte Value = BMFE.Value(irow,icol);
            if (Value > 0)
                NumFull++;
            else
                NumEmpty++;
            [Menu selectItemWithTag:Value];
            
            // Start accumulating values for the tooltip
            [TooltipMembers removeAllObjects];
            
            // Set availability by enabling or disabling items
            // Check on whether there are any non-blank ones available
            // Also update the menu's tooltip to show available items
            NSUInteger NumItems = [Menu numberOfItems];
            bool BadSel = true;
            for (uint ival=0; ival<NumItems; ival++)
            {
                NSMenuItem *Item = [Menu itemAtIndex:ival];
                uint ItemVal = (uint)Item.tag;
                if (ItemVal > 0)
                {
                    bool IsAvail = BMFE.Avail(irow,icol,ItemVal);
                    Item.enabled = IsAvail;
                    if (IsAvail)
                    {
                        BadSel = false;
                        [TooltipMembers addObject:TooltipValues[ItemVal]];
                    }
                }
             }
            if (BadSel) BadBoard = true;
            
            // Set up the tooltip
            NSUInteger TMCount = [TooltipMembers count];
            if (TMCount >= 2)
            {
                // Clear the tooltip string
                [TooltipString setString:@""];
                // Find a good row size
                uint RowSize = isqrt((uint)TMCount);
                // Add the members
                // Need the indices, so can't use foreach
                for (uint ix=0; ix<TMCount; ix++) {
                    // Put in separator
                    if (ix == 0) {
                        // Beginning: no separator
                    }
                    else if ((ix % RowSize) == 0)
                    {
                        // Start a new line
                        [TooltipString appendString:@"\n"];
                    }
                    else
                    {
                        // Space on the same line
                        [TooltipString appendString:@" "];
                    }
                    // Add the value itself
                    [TooltipString appendString:TooltipMembers[ix]];
                }
                Menu.toolTip = TooltipString;
            }
            else
            {
                // Blank out the tooltip
                Menu.toolTip = nil;
           }
        }
    }
    
    // Use the window title as a status bar
    NSString *WindowTitle = [NSString stringWithFormat:@"Sudoku %u*%u %u %u/%u",
                             BMFE.GetBlockRows(), BMFE.GetBlockCols(), BMFE.GetSideLength(),
                             NumEmpty, NumFull];
    
    if (BadBoard)
        WindowTitle = [NSString stringWithFormat:@"%@ X", WindowTitle];
    
    if (NumEmpty == 0)
        WindowTitle = [NSString stringWithFormat:@"%@ \u2713", WindowTitle];
    
    NSWindow *SelfWindow = [self window];
    [SelfWindow setTitle:WindowTitle];
}

- (SudokuBoard *)Duplicate
{
    byte *Data = new byte[BMFE.GetBoardSize()];
    BoardExport(BMFE,Data);
    
    SolverParameters SolverParams = BMFE.GetParams();
    SolverParametersSetup *SPS = [[SolverParametersSetup alloc] initWithParams:SolverParams];
    
    NSInteger SPIsOK = [NSApp runModalForWindow:SPS.window];
    if (SPIsOK != 0)
        SolverParams = [SPS GetSolverParameters];
    
    SudokuBoard *DupBoard = [[SudokuBoard alloc] initWithRows:BMFE.GetBlockRows()
                                                         Cols:BMFE.GetBlockCols() Params:SolverParams Data:Data];
    
    delete []Data;
    return DupBoard;
}

- (void)WriteFile:(NSURL *)FileRef {
    NSString *FilePath = FileRef.path;
    const char *FilePathCStr = [FilePath fileSystemRepresentation];
    
    FILE *f = fopen(FilePathCStr,"w");
    fprintf(f, "%u\t%u\n", BMFE.GetBlockRows(), BMFE.GetBlockCols());
    
    uint SideLen = BMFE.GetSideLength();
    for (int irow=0; irow<SideLen; irow++)
    {
        for (int icol=0; icol<SideLen; icol++)
        {
            fprintf(f, "%u\t", (uint)BMFE.Value(irow,icol));
        }
        fprintf(f, "\n");
    }
    
    fclose(f);

}

- (IBAction)saveDocument:(id)sender {
    
    NSString *FileName = [NSString stringWithFormat:@"Saved Sudoku %ux%u.txt",
                          BMFE.GetBlockRows(), BMFE.GetBlockCols()];
    
    NSSavePanel* SaveDialog = [NSSavePanel savePanel];
    [SaveDialog setNameFieldStringValue:FileName];
    
    NSWindow *SelfWindow = [self window];
    [SaveDialog beginSheetModalForWindow:SelfWindow
                       completionHandler:^(NSInteger Result) {
        
        if (Result == NSModalResponseOK)
        {
            [self WriteFile:[SaveDialog URL]];
        }
    }];
}

-(BOOL)canUndo {
    return HSFE.CanGoBack();
}

-(BOOL)canRedo {
    return HSFE.CanGoFwd();
}

-(IBAction)undo:(id)sender {
    // Go backward in the history stack
    byte *HistoryBlock = HSFE.GoBack();
    if (HistoryBlock == NULL) return;
    
    BoardImport(BMFE,HistoryBlock);
    [self UpdateBoard:self];
}

-(IBAction)redo:(id)sender {
    // Go forward in the history stack
    byte *HistoryBlock = HSFE.GoFwd();
    if (HistoryBlock == NULL) return;
    
    BoardImport(BMFE,HistoryBlock);
    [self UpdateBoard:self];
}

- (BOOL)validateUserInterfaceItem:(id <NSValidatedUserInterfaceItem>)Item
{
    // Can one undo or redo?
    SEL ItemAction = [Item action];
    
    if (ItemAction == @selector(undo:))
        return [self canUndo];
    else if (ItemAction == @selector(redo:))
        return [self canRedo];
    else
        return YES;
}

- (IBAction)ShowSolverParameters:(id)sender
{
    SolverParameters Params = BMFE.GetParams();
    SolverParametersSetup *SPS = [[SolverParametersSetup alloc] initWithParams:Params];
    
    [NSApp runModalForWindow:SPS.window];
}

@end
