//
//  HistoryStackFrontEnd.hpp
//  Sudoku Solver
//
//  Created by Loren Petrich on 1/10/17.
//  Copyright © 2017 Loren Petrich. All rights reserved.
//

#ifndef HistoryStackFrontEnd_hpp
#define HistoryStackFrontEnd_hpp

typedef unsigned int uint;
typedef unsigned char byte;

class HistoryStackFrontEnd
{
private:
    void *HistoryStackPtr;
    
public:
    HistoryStackFrontEnd();
    ~HistoryStackFrontEnd();
    
    void SetSize(uint Size);
    uint GetSize();
    
    bool CanGoBack();
    bool CanGoFwd();
    
    // For adding data: returns a pointer to add the data to
    byte *Add();
    // Goes back and returns pointer to previous data, or else NULL
    byte *GoBack();
    // Goes forward and returns pointer to next data, or else NULL
    byte *GoFwd();
    // Gets the currently-pointed-to data block
    byte *GetBlock();
};

#endif /* HistoryStackFrontEnd_hpp */
