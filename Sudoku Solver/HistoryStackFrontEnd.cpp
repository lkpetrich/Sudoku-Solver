//
//  HistoryStackFrontEnd.cpp
//  Sudoku Solver
//
//  Created by Loren Petrich on 1/10/17.
//  Copyright © 2017 Loren Petrich. All rights reserved.
//

#include "HistoryStackFrontEnd.hpp"

// This include file is not included in the front end's include file
// so as to isolate the details of this class from the outside world.
#include "HistoryStack.hpp"

HistoryStackFrontEnd::HistoryStackFrontEnd()
{
    // Create a history-stack instance
    // Use the smallest reasonable dimension
    HistoryStackPtr = new HistoryStack(1);
}

HistoryStackFrontEnd::~HistoryStackFrontEnd()
{
    // Clean up when done
    HistoryStack *HSP = (HistoryStack *)HistoryStackPtr;
    delete HSP;
}

void HistoryStackFrontEnd::SetSize(uint Size)
{
    HistoryStack *HSP = (HistoryStack *)HistoryStackPtr;
    delete HSP;
    HistoryStackPtr = new HistoryStack(Size);
}

uint HistoryStackFrontEnd::GetSize()
{
    HistoryStack *HSP = (HistoryStack *)HistoryStackPtr;
    return HSP->GetSize();
}

bool HistoryStackFrontEnd::CanGoBack()
{
    HistoryStack *HSP = (HistoryStack *)HistoryStackPtr;
    return HSP->CanGoBack();
}

bool HistoryStackFrontEnd::CanGoFwd()
{
    HistoryStack *HSP = (HistoryStack *)HistoryStackPtr;
    return HSP->CanGoFwd();
}

// For adding data: returns a pointer to add the data to
byte *HistoryStackFrontEnd::Add()
{
    HistoryStack *HSP = (HistoryStack *)HistoryStackPtr;
    return HSP->Add();
}

// Goes back and returns pointer to previous data, or else NULL
byte *HistoryStackFrontEnd::GoBack()
{
    HistoryStack *HSP = (HistoryStack *)HistoryStackPtr;
    return HSP->GoBack();
}

// Goes forward and returns pointer to next data, or else NULL
byte *HistoryStackFrontEnd::GoFwd()
{
    HistoryStack *HSP = (HistoryStack *)HistoryStackPtr;
    return HSP->GoFwd();
}

// Gets the currently-pointed-to data block
byte *HistoryStackFrontEnd::GetBlock()
{
    HistoryStack *HSP = (HistoryStack *)HistoryStackPtr;
    return HSP->GetBlock();
}

