//
//  BoardManager.hpp
//  Sudoku Solver
//
//  Created by Loren Petrich on 8/3/16.
//  Copyright © 2016 Loren Petrich. All rights reserved.
//

#ifndef BoardManager_hpp
#define BoardManager_hpp

#include "SolverParameters.h"

#include <vector>
using namespace std;

typedef unsigned char byte;
typedef unsigned int uint;

class BoardManager
{
    uint BlockRows, BlockCols, SideLength, BoardSize;
    
    vector<byte> BoardVals;
    vector<byte> AvailVals;
    
    SolverParameters Params;

public:
    // Create with a block size that will stay fixed
    BoardManager(uint BlockRows_, uint BlockCols_, SolverParameters &Params_);
    
    // Get the dimensions and the solver parameters
    uint GetBlockRows() {return BlockRows;}
    uint GetBlockCols() {return BlockCols;}
    uint GetSideLength() {return SideLength;}
    uint GetBoardSize() {return BoardSize;}
    SolverParameters &GetParams() {return Params;}
    
    // Set and get the values
    byte &Value(uint irow, uint icol) {return BoardVals[SideLength*irow+icol];}
    
    // Find which values are available
    // Values for each cell have indices 0 to SideLength for convenience in indexing
    // Made writable here for convenience in passing to a solver function
    byte &Avail(uint irow, uint icol, uint ival)
    {return AvailVals[(SideLength+1)*(SideLength*irow+icol)+ival];}
    
    // Solve the board
    void SolveAndUpdate();
};

#endif /* BoardManager_hpp */
