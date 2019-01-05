//
//  BoardManagerFrontEnd.hpp
//  Sudoku Solver
//
//  Created by Loren Petrich on 8/4/16.
//  Copyright © 2016 Loren Petrich. All rights reserved.
//
// This is for the purpose of isolating the details of BoardManager from the user interface
//

#ifndef BoardManagerFrontEnd_hpp
#define BoardManagerFrontEnd_hpp

typedef unsigned int uint;
typedef unsigned char byte;

class BoardManagerFrontEnd
{
private:
    void *BoardManagerPtr;
    
public:
    BoardManagerFrontEnd();
    ~BoardManagerFrontEnd();
    
    // Set and get the dimensions
    void SetBlockDimensions(uint BlockRows, uint BlockCols);
    
    uint GetBlockRows();
    uint GetBlockCols();
    uint GetSideLength();
    uint GetBoardSize();
    
    // Set and get the values
    byte &Value(uint irow, uint icol);
    
    // Find which values are available
    // Values for each cell have indices 0 to SideLength for convenience in indexing
    // Read-only here
    byte Avail(uint irow, uint icol, uint ival);
    
    // Solve the board
    void SolveAndUpdate();
};


// Import and export functions
// The export one assumes a pre-allocated data array

template<class T> void BoardImport(BoardManagerFrontEnd &BMFE, T *Data)
{
    uint ix = 0;
    uint SideLen = BMFE.GetSideLength();
    for (int irow=0; irow<SideLen; irow++)
    {
        for (int icol=0; icol<SideLen; icol++)
        {
            BMFE.Value(irow,icol) = Data[ix];
            ix++;
        }
    }
}

template<class T> void BoardExport(BoardManagerFrontEnd &BMFE, T *Data)
{
    uint ix = 0;
    uint SideLen = BMFE.GetSideLength();
    for (int irow=0; irow<SideLen; irow++)
    {
        for (int icol=0; icol<SideLen; icol++)
        {
            Data[ix] = BMFE.Value(irow,icol);
            ix++;
        }
    }
}

#endif /* BoardManagerFrontEnd_hpp */
