//
//  BoardManager.cpp
//  Sudoku Solver
//
//  Created by Loren Petrich on 8/3/16.
//  Copyright © 2016 Loren Petrich. All rights reserved.
//
// This class manages a Sudoku board.
// It checks for legitimate additions

#include <algorithm>
#include "BoardManager.hpp"
#include "Solver.hpp"


static uint VectorSum(byte *Vec, uint IndMin, uint IndMax)
{
    uint Sum = 0;
    for (uint i=IndMin; i<=IndMax; i++)
        Sum += Vec[i];
    
    return Sum;
}


BoardManager::BoardManager(uint BlockRows_, uint BlockCols_)
{
    BlockRows = BlockRows_;
    BlockCols = BlockCols_;
    SideLength = BlockRows * BlockCols;
    BoardSize = SideLength * SideLength;
    
    // Initialize the board as empty
    BoardVals.resize(BoardSize);
    fill(BoardVals.begin(),BoardVals.end(),0);
    
    // Initialize the availability as all possible
    uint AvailSize = (SideLength+1)*BoardSize;
    AvailVals.resize(AvailSize);
    fill(AvailVals.begin(),AvailVals.end(),1);
}


// Solve the board
void BoardManager::SolveAndUpdate()
{
    // Set cell availabilities from the cell values
    for (uint irow=0; irow<SideLength; irow++)
    {
        for (uint icol=0; icol<SideLength; icol++)
        {
            byte CellVal = Value(irow,icol);
            byte *CellAvail = &Avail(irow,icol,0);
            for (uint ival=1; ival<=SideLength; ival++)
            {
                CellAvail[ival] = (CellVal > 0) ? ((ival == CellVal) ? 1 : 0) : 1;
            }
        }
    }
    
    // Check on changes using availability array
    vector<byte> PrevAvailVals(AvailVals);
    
    // Iterate over the solution code
    // Check for changes at each iteration
    bool ContinueSolving = false;
    do {
        Solve(*this);
        
        // Check on whether AvailVals has changed
        ContinueSolving = !equal(AvailVals.begin(),AvailVals.end(),PrevAvailVals.begin());
        
        // Push down the current value for the next iteration
        PrevAvailVals = AvailVals;
        
    } while(ContinueSolving);

    // Set cell values from the cell availabilities
    for (uint irow=0; irow<SideLength; irow++)
    {
        for (uint icol=0; icol<SideLength; icol++)
        {
            byte *CellAvail = &Avail(irow,icol,0);
            uint Sum = VectorSum(CellAvail,1,SideLength);
            if (Sum == 1)
            {
                for (uint ival=1; ival<=SideLength; ival++)
                {
                    if (CellAvail[ival] != 0)
                    {
                        Value(irow,icol) = ival;
                    }
                }
                
            }
            else
            {
                Value(irow,icol) = 0;
            }
        }
    }

}