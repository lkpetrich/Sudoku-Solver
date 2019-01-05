//
//  HistoryStack.cpp
//  Sudoku Solver
//
//  Created by Loren Petrich on 1/10/17.
//  Copyright © 2017 Loren Petrich. All rights reserved.
//

#include "HistoryStack.hpp"

HistoryStack::HistoryStack(uint Size_)
{
    Size = Size_;
    Index = 0;
}


// For adding data: returns a pointer to add the data to
byte *HistoryStack::Add()
{
    // Initially: size 0 and index 0
    if (Stack.size() > 0) Index++;
    
    Stack.resize((Index+1)*Size);
    return &Stack[Index*Size];
}

// Goes back and returns pointer to previous data, or else NULL
byte *HistoryStack::GoBack()
{
    if (!CanGoBack()) return NULL;
    
    Index--;
    
    return &Stack[Index*Size];
}

// Goes forward and returns pointer to next data, or else NULL
byte *HistoryStack::GoFwd()
{
    if (!CanGoFwd()) return NULL;
    
    Index++;
    
    return &Stack[Index*Size];
}

// Gets the currently-pointed-to data block
byte *HistoryStack::GetBlock()
{
    if (Index*Size >= Stack.size()) return NULL;
    
    return &Stack[Index*Size];
}