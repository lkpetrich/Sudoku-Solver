//
//  SolverParametersSetup.m
//  Sudoku Solver
//
//  Created by Loren Petrich on 9/30/22.
//  Copyright © 2022 Loren Petrich. All rights reserved.
//

#import "SolverParametersSetup.h"

static NSInteger TagOfSelection(NSSegmentedControl *SegWidget)
{
    NSInteger ix = SegWidget.indexOfSelectedItem;
    return [SegWidget tagForSegment:ix];
}

@interface SolverParametersSetup ()

{
    IBOutlet NSSegmentedControl *MaxVisibleSetSizeWidget;
    IBOutlet NSSegmentedControl *MaxHiddenSetSizeWidget;
    IBOutlet NSButton *UseIntersectionWidget;
}

@end

@implementation SolverParametersSetup

{
    SolverParameters Params;
    
    // Whether OK or canceled
    NSInteger IsOK;
}

- (id)initWithParams:(SolverParameters)Params_
{
    self = [super initWithWindowNibName:@"SolverParametersSetup"];
    
    Params = Params_;
    
    return self;
}

- (void)windowDidLoad {
    [super windowDidLoad];
    
    [MaxVisibleSetSizeWidget selectSegmentWithTag:((NSInteger)(Params.MaxVisibleSetSize))];
    [MaxHiddenSetSizeWidget selectSegmentWithTag:((NSInteger)(Params.MaxHiddenSetSize))];
    UseIntersectionWidget.state = (Params.UseIntersections ? NSControlStateValueOn : NSControlStateValueOff);
}

- (SolverParameters)GetSolverParameters
{
    Params.MaxVisibleSetSize = (unsigned int)TagOfSelection(MaxVisibleSetSizeWidget);
    Params.MaxHiddenSetSize = (unsigned int)TagOfSelection(MaxHiddenSetSizeWidget);
    Params.UseIntersections = (UseIntersectionWidget.state != NSControlStateValueOff);
    
    return Params;
}

- (NSInteger)GetUseIntersections
{
    return Params.UseIntersections ? NSControlStateValueOn : NSControlStateValueOff;
}

- (void)SetUseIntersections:(NSInteger)UseIntersections_
{
    Params.UseIntersections = (UseIntersections_ == NSControlStateValueOn);
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
