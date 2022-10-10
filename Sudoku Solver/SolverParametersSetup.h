//
//  SolverParametersSetup.h
//  Sudoku Solver
//
//  Created by Loren Petrich on 9/30/22.
//  Copyright © 2022 Loren Petrich. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include "SolverParameters.h"

@interface SolverParametersSetup : NSWindowController

- (id)initWithParams:(SolverParameters)Params_;

- (SolverParameters)GetSolverParameters;

@end
