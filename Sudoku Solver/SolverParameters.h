//
//  SolverParameters.h
//  Sudoku Solver
//
//  Created by Loren Petrich on 9/29/22.
//  Copyright © 2022 Loren Petrich. All rights reserved.
//

#ifndef SolverParameters_h
#define SolverParameters_h

struct SolverParameters
{
    unsigned int MaxVisibleSetSize;
    unsigned int MaxHiddenSetSize;
    bool UseIntersections;
    
    // Default: maximum parameter values
    SolverParameters() noexcept:
        MaxVisibleSetSize(5),
        MaxHiddenSetSize(5),
        UseIntersections(true)
        {}
};

#endif /* SolverParameters_h */
