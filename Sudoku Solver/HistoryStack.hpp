//
//  HistoryStack.hpp
//  Sudoku Solver
//
//  Created by Loren Petrich on 1/10/17.
//  Copyright © 2017 Loren Petrich. All rights reserved.
//

#ifndef HistoryStack_hpp
#define HistoryStack_hpp

#include <vector>
using namespace std;

typedef unsigned int uint;
typedef unsigned char byte;

class HistoryStack
{
    uint Size;
    uint Index;
    
    vector<byte> Stack;
    
public:
    // Create with a block size that will stay fixed
    HistoryStack(uint Size_);

    uint GetSize() {return Size;}
    
    // Index is zero-based
    bool CanGoBack() {return (Index > 0);}
    // Index ends at ((# data blocks) - 1)
    bool CanGoFwd() {return (Stack.size() > (Index+1)*Size);}
    
    // For adding data: returns a pointer to add the data to
    byte *Add();
    // Goes back and returns pointer to previous data, or else NULL
    byte *GoBack();
    // Goes forward and returns pointer to next data, or else NULL
    byte *GoFwd();
    // Gets the currently-pointed-to data block
    byte *GetBlock();
};

#endif /* HistoryStack_hpp */
