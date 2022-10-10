//
//  BoardManagerFrontEnd.cpp
//  Sudoku Solver
//
//  Created by Loren Petrich on 8/4/16.
//  Copyright © 2016 Loren Petrich. All rights reserved.
//

#include "BoardManagerFrontEnd.hpp"

// This include file is not included in the front end's include file
// so as to isolate the details of this class from the outside world.
#include "BoardManager.hpp"

BoardManagerFrontEnd::BoardManagerFrontEnd()
{
    // Create a board-manager instance
    // Use the smallest reasonable dimension
    SolverParameters Params;
    BoardManagerPtr = new BoardManager(1,1,Params);
}

BoardManagerFrontEnd::~BoardManagerFrontEnd()
{
    // Clean up when done
    BoardManager *BMP = (BoardManager *)BoardManagerPtr;
    delete BMP;
}


// Set and get the dimensions and the solver parameters
void BoardManagerFrontEnd::SetBlockDimensions(uint BlockRows, uint BlockCols,
                                                SolverParameters &Params)
{
    // Replace the board manager with a new-sized one
    BoardManager *BMP = (BoardManager *)BoardManagerPtr;
    delete BMP;
    BoardManagerPtr = new BoardManager(BlockRows,BlockCols,Params);
}

// Get the sizes and parameters

uint BoardManagerFrontEnd::GetBlockRows()
{
    BoardManager *BMP = (BoardManager *)BoardManagerPtr;
    return BMP->GetBlockRows();
}

uint BoardManagerFrontEnd::GetBlockCols()
{
    BoardManager *BMP = (BoardManager *)BoardManagerPtr;
    return BMP->GetBlockCols();
}

uint BoardManagerFrontEnd::GetSideLength()
{
    BoardManager *BMP = (BoardManager *)BoardManagerPtr;
    return BMP->GetSideLength();
}

uint BoardManagerFrontEnd::GetBoardSize()
{
    BoardManager *BMP = (BoardManager *)BoardManagerPtr;
    return BMP->GetBoardSize();
}

SolverParameters &BoardManagerFrontEnd::GetParams()
{
    BoardManager *BMP = (BoardManager *)BoardManagerPtr;
    return BMP->GetParams();
}

// Set and get the values
byte &BoardManagerFrontEnd::Value(uint irow, uint icol)
{
    BoardManager *BMP = (BoardManager *)BoardManagerPtr;
    return BMP->Value(irow,icol);
}

// Find which values are available
byte BoardManagerFrontEnd::Avail(uint irow, uint icol, uint ival)
{
    BoardManager *BMP = (BoardManager *)BoardManagerPtr;
    return BMP->Avail(irow,icol,ival);
}


// Solve the board
void BoardManagerFrontEnd::SolveAndUpdate()
{
    BoardManager *BMP = (BoardManager *)BoardManagerPtr;
    BMP->SolveAndUpdate();
}
