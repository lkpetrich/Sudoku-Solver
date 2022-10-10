//
//  Solver.cpp
//  Sudoku Solver
//
//  Created by Loren Petrich on 1/6/17.
//  Copyright © 2017 Loren Petrich. All rights reserved.
//

#include "Solver.hpp"
#include "BooleanVecOps.hpp"
#include <vector>
#include <algorithm>


static uint VectorSum(byte *Vec, uint IndMin, uint IndMax);

typedef bool (*DistRCBFunc)(byte **, uint);
static bool DistribRowColBlk(BoardManager &Board, DistRCBFunc Func, bool DoneIfOnce = true);

static bool VisOne(byte **Avail, uint Size);
static bool VisTwo(byte **Avail, uint Size);
static bool VisThree(byte **Avail, uint Size);
static bool VisFour(byte **Avail, uint Size);
static bool VisFive(byte **Avail, uint Size);

static bool HidOne(byte **Avail, uint Size);
static bool HidTwo(byte **Avail, uint Size);
static bool HidThree(byte **Avail, uint Size);
static bool HidFour(byte **Avail, uint Size);
static bool HidFive(byte **Avail, uint Size);

static bool ItscBlkRow(BoardManager &Board);
static bool ItscBlkCol(BoardManager &Board);
static bool Itsc(BoardManager &Board);


void Solve(BoardManager &Board, SolverParameters &Params)
{
next:
    if (Params.MaxVisibleSetSize >= 1 && DistribRowColBlk(Board,VisOne,false)) goto next;
    if (Params.MaxHiddenSetSize >= 1 && DistribRowColBlk(Board,HidOne)) goto next;
    
    if (Params.MaxVisibleSetSize >= 2 && DistribRowColBlk(Board,VisTwo)) goto next;
    if (Params.MaxHiddenSetSize >= 2 && DistribRowColBlk(Board,HidTwo)) goto next;
    
    if (Params.MaxVisibleSetSize >= 3 && DistribRowColBlk(Board,VisThree)) goto next;
    if (Params.MaxHiddenSetSize >= 3 && DistribRowColBlk(Board,HidThree)) goto next;
    
    if (Params.MaxVisibleSetSize >= 4 && DistribRowColBlk(Board,VisFour)) goto next;
    if (Params.MaxHiddenSetSize >= 4 && DistribRowColBlk(Board,HidFour)) goto next;
    
    if (Params.MaxVisibleSetSize >= 5 && DistribRowColBlk(Board,VisFive)) goto next;
    if (Params.MaxHiddenSetSize >= 5 && DistribRowColBlk(Board,HidFive)) goto next;
    
    if (Params.UseIntersections && Itsc(Board)) goto next;
}


uint VectorSum(byte *Vec, uint IndMin, uint IndMax)
{
    uint Sum = 0;
    for (uint i=IndMin; i<=IndMax; i++)
        Sum += Vec[i];
    
    return Sum;
}

bool DistribRowColBlk(BoardManager &Board, DistRCBFunc Func, bool DoneIfOnce)
{
    // Set up
    uint BlockRows = Board.GetBlockRows();
    uint BlockCols = Board.GetBlockCols();
    uint SideLength = Board.GetSideLength();
    vector<byte *> AvailSet(SideLength);
    byte **AvailPtr = &AvailSet[0];
    
    bool rctot = false;
    
    // Do rows
    for (uint irow=0; irow<SideLength; irow++)
    {
        for (uint icol=0; icol<SideLength; icol++)
            AvailPtr[icol] = &Board.Avail(irow,icol,0);
        
        if (Func(AvailPtr,SideLength))
        {
            if (DoneIfOnce) return true;
            rctot = true;
        }
    }
    
    // Do columns
    for (uint icol=0; icol<SideLength; icol++)
    {
        for (uint irow=0; irow<SideLength; irow++)
            AvailPtr[irow] = &Board.Avail(irow,icol,0);
        
        if (Func(AvailPtr,SideLength))
        {
            if (DoneIfOnce) return true;
            rctot = true;
        }
    }
    
    // Do blocks
    for (uint ibrow=0; ibrow<BlockCols; ibrow++)
    {
        for (uint ibcol=0; ibcol<BlockRows; ibcol++)
        {
            uint ix = 0;
            for (uint ixrow=0; ixrow<BlockRows; ixrow++)
            {
                for (uint ixcol=0; ixcol<BlockCols; ixcol++)
                {
                    AvailPtr[ix] =
                        &Board.Avail(ibrow*BlockRows+ixrow, ibcol*BlockCols+ixcol, 0);
                    ix++;
                }
            }
            
            if (Func(AvailPtr,SideLength))
            {
                if (DoneIfOnce) return true;
                rctot = true;
            }
        }
    }
    
    return rctot;
}

// Visible sets

bool VisOne(byte **Avail, uint Size)
{
    bool rc = false;
    
    for (uint i=0; i<Size; i++)
    {
        // The total visible set
        byte *AvailPtr = Avail[i];
        // Does it have the right size?
        if (VectorSum(AvailPtr,1,Size) != 1) continue;
        
        // Remove the visible set from the others in the row/column/block
        for (uint ix=0; ix<Size; ix++)
        {
            if (ix == i) continue;
            if (VecSubFmCheck(Avail[ix],1,Size,AvailPtr)) rc = true;
        }
    }
    
    return rc;
}

bool VisTwo(byte **Avail, uint Size)
{
    vector<byte> SetUnion(Size+1);
    byte *SetUnionPtr = &SetUnion[0];
    
    // Skipping over singles to save time
    for (uint i1=0; i1<Size; i1++)
    {
        byte *AvailPtr1 = Avail[i1];
        if (VectorSum(AvailPtr1,1,Size) <= 1) continue;
        
        for (uint i2=i1+1; i2<Size; i2++)
        {
            byte *AvailPtr2 = Avail[i2];
            if (VectorSum(AvailPtr2,1,Size) <= 1) continue;
            
            // The total visible set
            VecCopy(SetUnionPtr,1,Size,AvailPtr1);
            VecOrTo(SetUnionPtr,1,Size,AvailPtr2);
            // Does it have the right size?
            if (VectorSum(SetUnionPtr,1,Size) != 2) continue;
            
            // Remove the visible set from the others in the row/column/block
            bool rc = false;
            for (uint ix=0; ix<Size; ix++)
            {
                if (ix == i1) continue;
                if (ix == i2) continue;
                if (VecSubFmCheck(Avail[ix],1,Size,SetUnionPtr)) rc = true;
            }
            if (rc) return true;
        }
    }
    
    return false;
}

bool VisThree(byte **Avail, uint Size)
{
    vector<byte> SetUnion(Size+1);
    byte *SetUnionPtr = &SetUnion[0];
    
    // Skipping over singles to save time
    for (uint i1=0; i1<Size; i1++)
    {
        byte *AvailPtr1 = Avail[i1];
        if (VectorSum(AvailPtr1,1,Size) <= 1) continue;
        
        for (uint i2=i1+1; i2<Size; i2++)
        {
            byte *AvailPtr2 = Avail[i2];
            if (VectorSum(AvailPtr2,1,Size) <= 1) continue;
            
            for (uint i3=i2+1; i3<Size; i3++)
            {
                byte *AvailPtr3 = Avail[i3];
                if (VectorSum(AvailPtr3,1,Size) <= 1) continue;
                
                // The total visible set
                VecCopy(SetUnionPtr,1,Size,AvailPtr1);
                VecOrTo(SetUnionPtr,1,Size,AvailPtr2);
                VecOrTo(SetUnionPtr,1,Size,AvailPtr3);
                // Does it have the right size?
                if (VectorSum(SetUnionPtr,1,Size) != 3) continue;
                
                // Remove the visible set from the others in the row/column/block
                bool rc = false;
                for (uint ix=0; ix<Size; ix++)
                {
                    if (ix == i1) continue;
                    if (ix == i2) continue;
                    if (ix == i3) continue;
                    if (VecSubFmCheck(Avail[ix],1,Size,SetUnionPtr)) rc = true;
                }
                if (rc) return true;
            }
        }
    }
    
    return false;
}

bool VisFour(byte **Avail, uint Size)
{
    vector<byte> SetUnion(Size+1);
    byte *SetUnionPtr = &SetUnion[0];
    
    // Skipping over singles to save time
    for (uint i1=0; i1<Size; i1++)
    {
        byte *AvailPtr1 = Avail[i1];
        if (VectorSum(AvailPtr1,1,Size) <= 1) continue;
        for (uint i2=i1+1; i2<Size; i2++)
        {
            byte *AvailPtr2 = Avail[i2];
            if (VectorSum(AvailPtr2,1,Size) <= 1) continue;
            
            for (uint i3=i2+1; i3<Size; i3++)
            {
                byte *AvailPtr3 = Avail[i3];
                if (VectorSum(AvailPtr3,1,Size) <= 1) continue;
                
                for (uint i4=i3+1; i4<Size; i4++)
                {
                    byte *AvailPtr4 = Avail[i4];
                    if (VectorSum(AvailPtr4,1,Size) <= 1) continue;
                    
                    // The total visible set
                    VecCopy(SetUnionPtr,1,Size,AvailPtr1);
                    VecOrTo(SetUnionPtr,1,Size,AvailPtr2);
                    VecOrTo(SetUnionPtr,1,Size,AvailPtr3);
                    VecOrTo(SetUnionPtr,1,Size,AvailPtr4);
                    // Does it have the right size?
                    if (VectorSum(SetUnionPtr,1,Size) != 4) continue;
                    
                    // Remove the visible set from the others in the row/column/block
                    bool rc = false;
                    for (uint ix=0; ix<Size; ix++)
                    {
                        if (ix == i1) continue;
                        if (ix == i2) continue;
                        if (ix == i3) continue;
                        if (ix == i4) continue;
                        if (VecSubFmCheck(Avail[ix],1,Size,SetUnionPtr)) rc = true;
                    }
                    if (rc) return true;
                }
            }
        }
    }
    
    return false;
}

bool VisFive(byte **Avail, uint Size)
{
    vector<byte> SetUnion(Size+1);
    byte *SetUnionPtr = &SetUnion[0];
    
    // Skipping over singles to save time
    for (uint i1=0; i1<Size; i1++)
    {
        byte *AvailPtr1 = Avail[i1];
        if (VectorSum(AvailPtr1,1,Size) <= 1) continue;
        for (uint i2=i1+1; i2<Size; i2++)
        {
            byte *AvailPtr2 = Avail[i2];
            if (VectorSum(AvailPtr2,1,Size) <= 1) continue;
            
            for (uint i3=i2+1; i3<Size; i3++)
            {
                byte *AvailPtr3 = Avail[i3];
                if (VectorSum(AvailPtr3,1,Size) <= 1) continue;
                
                for (uint i4=i3+1; i4<Size; i4++)
                {
                    byte *AvailPtr4 = Avail[i4];
                    if (VectorSum(AvailPtr4,1,Size) <= 1) continue;
                    
                    for (uint i5=i4+1; i5<Size; i5++)
                    {
                        byte *AvailPtr5 = Avail[i5];
                        if (VectorSum(AvailPtr5,1,Size) <= 1) continue;
                       
                        // The total visible set
                        VecCopy(SetUnionPtr,1,Size,AvailPtr1);
                        VecOrTo(SetUnionPtr,1,Size,AvailPtr2);
                        VecOrTo(SetUnionPtr,1,Size,AvailPtr3);
                        VecOrTo(SetUnionPtr,1,Size,AvailPtr4);
                        VecOrTo(SetUnionPtr,1,Size,AvailPtr5);
                        // Does it have the right size?
                        if (VectorSum(SetUnionPtr,1,Size) != 5) continue;
                    
                        // Remove the visible set from the others in the row/column/block
                        bool rc = false;
                        for (uint ix=0; ix<Size; ix++)
                        {
                            if (ix == i1) continue;
                            if (ix == i2) continue;
                            if (ix == i3) continue;
                            if (ix == i4) continue;
                            if (ix == i5) continue;
                            if (VecSubFmCheck(Avail[ix],1,Size,SetUnionPtr)) rc = true;
                        }
                        if (rc) return true;
                    }
                }
            }
        }
    }
    
    return false;
}

// Hidden sets

bool HidOne(byte **Avail, uint Size)
{
    vector<byte> SetUnion(Size+1);
    byte *SetUnionPtr = &SetUnion[0];
    vector<byte> SetDiff(Size+1);
    byte *SetDiffPtr = &SetDiff[0];
   
    for (uint i=0; i<Size; i++)
    {
        byte *AvailPtr = Avail[i];
        if (VectorSum(AvailPtr,1,Size) == 0) continue;
        
        // Get what's outside the set
        VecFalse(SetUnionPtr,1,Size);
        for (uint ix=0; ix<Size; ix++)
        {
            if (ix == i) continue;
            VecOrTo(SetUnionPtr,1,Size,Avail[ix]);
        }
        
        // Get the set's contents
        // Remove outside the set from inside it
        VecSub(SetDiffPtr,1,Size,AvailPtr,SetUnionPtr);
        // The right size?
        if (VectorSum(SetDiffPtr,1,Size) != 1) continue;
        
        // Trim down the set
        VecNotAt(SetDiffPtr,1,Size);
        if (VecSubFmCheck(AvailPtr,1,Size,SetDiffPtr)) return true;
    }
    
    return false;
}

bool HidTwo(byte **Avail, uint Size)
{
    vector<byte> SetUnion(Size+1);
    byte *SetUnionPtr = &SetUnion[0];
    vector<byte> SetDiff(Size+1);
    byte *SetDiffPtr = &SetDiff[0];
    
    // Skipping over singles to save time
    for (uint i1=0; i1<Size; i1++)
    {
        byte *AvailPtr1 = Avail[i1];
        if (VectorSum(AvailPtr1,1,Size) <= 1) continue;
        
        for (uint i2=i1+1; i2<Size; i2++)
        {
            byte *AvailPtr2 = Avail[i2];
            if (VectorSum(AvailPtr2,1,Size) <= 1) continue;
            
            // Get what's outside of the set
            VecFalse(SetUnionPtr,1,Size);
            for (uint ix=0; ix<Size; ix++)
            {
                if (ix == i1) continue;
                if (ix == i2) continue;
                VecOrTo(SetUnionPtr,1,Size,Avail[ix]);
            }
            
            // Get the set's contents
            VecCopy(SetDiffPtr,1,Size,AvailPtr1);
            VecOrTo(SetDiffPtr,1,Size,AvailPtr2);
            
            // Remove outside the set from inside it
            VecSubFm(SetDiffPtr,1,Size,SetUnionPtr);
            // The right size?
            if (VectorSum(SetDiffPtr,1,Size) != 2) continue;
            
            // Trim down the set
            VecNotAt(SetDiffPtr,1,Size);
            bool rc = false;
            if (VecSubFmCheck(AvailPtr1,1,Size,SetDiffPtr)) rc = true;
            if (VecSubFmCheck(AvailPtr2,1,Size,SetDiffPtr)) rc = true;
            if (rc) return true;
        }
    }
    
    return false;
}

bool HidThree(byte **Avail, uint Size)
{
    vector<byte> SetUnion(Size+1);
    byte *SetUnionPtr = &SetUnion[0];
    vector<byte> SetDiff(Size+1);
    byte *SetDiffPtr = &SetDiff[0];
    
    // Skipping over singles to save time
    for (uint i1=0; i1<Size; i1++)
    {
        byte *AvailPtr1 = Avail[i1];
        if (VectorSum(AvailPtr1,1,Size) <= 1) continue;
        
        for (uint i2=i1+1; i2<Size; i2++)
        {
            byte *AvailPtr2 = Avail[i2];
            if (VectorSum(AvailPtr2,1,Size) <= 1) continue;
            
            for (uint i3=i2+1; i3<Size; i3++)
            {
                byte *AvailPtr3 = Avail[i3];
                if (VectorSum(AvailPtr3,1,Size) <= 1) continue;
                
                // Get what's outside of the set
                VecFalse(SetUnionPtr,1,Size);
                for (uint ix=0; ix<Size; ix++)
                {
                    if (ix == i1) continue;
                    if (ix == i2) continue;
                    if (ix == i3) continue;
                    VecOrTo(SetUnionPtr,1,Size,Avail[ix]);
                }
                
                // Get the set's contents
                VecCopy(SetDiffPtr,1,Size,AvailPtr1);
                VecOrTo(SetDiffPtr,1,Size,AvailPtr2);
                VecOrTo(SetDiffPtr,1,Size,AvailPtr3);
                
                // Remove outside the set from inside it
                VecSubFm(SetDiffPtr,1,Size,SetUnionPtr);
                // The right size?
                if (VectorSum(SetDiffPtr,1,Size) != 3) continue;
                
                // Trim down the set
                VecNotAt(SetDiffPtr,1,Size);
                bool rc = false;
                if (VecSubFmCheck(AvailPtr1,1,Size,SetDiffPtr)) rc = true;
                if (VecSubFmCheck(AvailPtr2,1,Size,SetDiffPtr)) rc = true;
                if (VecSubFmCheck(AvailPtr3,1,Size,SetDiffPtr)) rc = true;
                if (rc) return true;
            }
        }
    }
    
    return false;
}

bool HidFour(byte **Avail, uint Size)
{
    vector<byte> SetUnion(Size+1);
    byte *SetUnionPtr = &SetUnion[0];
    vector<byte> SetDiff(Size+1);
    byte *SetDiffPtr = &SetDiff[0];
    
    // Skipping over singles to save time
    for (uint i1=0; i1<Size; i1++)
    {
        byte *AvailPtr1 = Avail[i1];
        if (VectorSum(AvailPtr1,1,Size) <= 1) continue;
        
        for (uint i2=i1+1; i2<Size; i2++)
        {
            byte *AvailPtr2 = Avail[i2];
            if (VectorSum(AvailPtr2,1,Size) <= 1) continue;
            
            for (uint i3=i2+1; i3<Size; i3++)
            {
                byte *AvailPtr3 = Avail[i3];
                if (VectorSum(AvailPtr3,1,Size) <= 1) continue;
                
                for (uint i4=i3+1; i4<Size; i4++)
                {
                    byte *AvailPtr4 = Avail[i4];
                    if (VectorSum(AvailPtr4,1,Size) <= 1) continue;
                    
                    // Get what's outside of the set
                    VecFalse(SetUnionPtr,1,Size);
                    for (uint ix=0; ix<Size; ix++)
                    {
                        if (ix == i1) continue;
                        if (ix == i2) continue;
                        if (ix == i3) continue;
                        if (ix == i4) continue;
                        VecOrTo(SetUnionPtr,1,Size,Avail[ix]);
                    }
                    
                    // Get the set's contents
                    VecCopy(SetDiffPtr,1,Size,AvailPtr1);
                    VecOrTo(SetDiffPtr,1,Size,AvailPtr2);
                    VecOrTo(SetDiffPtr,1,Size,AvailPtr3);
                    VecOrTo(SetDiffPtr,1,Size,AvailPtr4);
                    
                    // Remove outside the set from inside it
                    VecSubFm(SetDiffPtr,1,Size,SetUnionPtr);
                    // The right size?
                    if (VectorSum(SetDiffPtr,1,Size) != 4) continue;
                    
                    // Trim down the set
                    VecNotAt(SetDiffPtr,1,Size);
                    bool rc = false;
                    if (VecSubFmCheck(AvailPtr1,1,Size,SetDiffPtr)) rc = true;
                    if (VecSubFmCheck(AvailPtr2,1,Size,SetDiffPtr)) rc = true;
                    if (VecSubFmCheck(AvailPtr3,1,Size,SetDiffPtr)) rc = true;
                    if (VecSubFmCheck(AvailPtr4,1,Size,SetDiffPtr)) rc = true;
                    if (rc) return true;
                }
            }
        }
    }
    
    return false;
}

bool HidFive(byte **Avail, uint Size)
{
    vector<byte> SetUnion(Size+1);
    byte *SetUnionPtr = &SetUnion[0];
    vector<byte> SetDiff(Size+1);
    byte *SetDiffPtr = &SetDiff[0];
    
    // Skipping over singles to save time
    for (uint i1=0; i1<Size; i1++)
    {
        byte *AvailPtr1 = Avail[i1];
        if (VectorSum(AvailPtr1,1,Size) <= 1) continue;
        
        for (uint i2=i1+1; i2<Size; i2++)
        {
            byte *AvailPtr2 = Avail[i2];
            if (VectorSum(AvailPtr2,1,Size) <= 1) continue;
            
            for (uint i3=i2+1; i3<Size; i3++)
            {
                byte *AvailPtr3 = Avail[i3];
                if (VectorSum(AvailPtr3,1,Size) <= 1) continue;
                
                for (uint i4=i3+1; i4<Size; i4++)
                {
                    byte *AvailPtr4 = Avail[i4];
                    if (VectorSum(AvailPtr4,1,Size) <= 1) continue;
                    
                    for (uint i5=i4+1; i5<Size; i5++)
                    {
                        byte *AvailPtr5 = Avail[i5];
                        if (VectorSum(AvailPtr5,1,Size) <= 1) continue;

                        // Get what's outside of the set
                        VecFalse(SetUnionPtr,1,Size);
                        for (uint ix=0; ix<Size; ix++)
                        {
                            if (ix == i1) continue;
                            if (ix == i2) continue;
                            if (ix == i3) continue;
                            if (ix == i4) continue;
                            if (ix == i5) continue;
                            VecOrTo(SetUnionPtr,1,Size,Avail[ix]);
                        }
                    
                        // Get the set's contents
                        VecCopy(SetDiffPtr,1,Size,AvailPtr1);
                        VecOrTo(SetDiffPtr,1,Size,AvailPtr2);
                        VecOrTo(SetDiffPtr,1,Size,AvailPtr3);
                        VecOrTo(SetDiffPtr,1,Size,AvailPtr4);
                        VecOrTo(SetDiffPtr,1,Size,AvailPtr5);
                        
                        // Remove outside the set from inside it
                        VecSubFm(SetDiffPtr,1,Size,SetUnionPtr);
                        // The right size?
                        if (VectorSum(SetDiffPtr,1,Size) != 5) continue;
                        
                        // Trim down the set
                        VecNotAt(SetDiffPtr,1,Size);
                        bool rc = false;
                        if (VecSubFmCheck(AvailPtr1,1,Size,SetDiffPtr)) rc = true;
                        if (VecSubFmCheck(AvailPtr2,1,Size,SetDiffPtr)) rc = true;
                        if (VecSubFmCheck(AvailPtr3,1,Size,SetDiffPtr)) rc = true;
                        if (VecSubFmCheck(AvailPtr4,1,Size,SetDiffPtr)) rc = true;
                        if (VecSubFmCheck(AvailPtr5,1,Size,SetDiffPtr)) rc = true;
                        if (rc) return true;
                    }
                }
            }
        }
    }
    
    return false;
}


// Intersections between blocks, rows, and columns
// Pointing sets

bool ItscBlkRow(BoardManager &Board)
{
    // Set up
    uint BlockRows = Board.GetBlockRows();
    uint BlockCols = Board.GetBlockCols();
    uint SideLength = Board.GetSideLength();
    
    vector<byte> SetSeg(SideLength+1);
    byte *SetSegPtr = &SetSeg[0];
    vector<byte> SetRow(SideLength+1);
    byte *SetRowPtr = &SetRow[0];
    vector<byte> SetBlk(SideLength+1);
    byte *SetBlkPtr = &SetBlk[0];
    
    vector<byte> SetDiffRow(SideLength+1);
    byte *SetDiffRowPtr = &SetDiffRow[0];
    vector<byte> SetDiffBlk(SideLength+1);
    byte *SetDiffBlkPtr = &SetDiffBlk[0];
    
    // Iterate over the blocks
    for (uint ibkrow=0; ibkrow<BlockCols; ibkrow++)
    {
        uint ibkrwmin = BlockRows*ibkrow;
        uint ibkrwmax = ibkrwmin + BlockRows;
        for (uint ibkcol=0; ibkcol<BlockRows; ibkcol++)
        {
            uint ibkclmin = BlockCols*ibkcol;
            uint ibkclmax = ibkclmin + BlockCols;
            
            // Iterate over rows in the block
            for (uint irow = ibkrwmin; irow<ibkrwmax; irow++)
            {
                // Find the segment availability
                VecFalse(SetSegPtr,1,SideLength);
                uint numseg = 0;
                for (uint icol=ibkclmin; icol<ibkclmax; icol++)
                {
                    byte *AvailPtr = &Board.Avail(irow,icol,0);
                    if (VectorSum(AvailPtr,1,SideLength) <= 1) continue;
                    VecOrTo(SetSegPtr,1,SideLength,AvailPtr);
                    numseg++;
                }
                if (numseg == 0) continue;
                
                // Find the row availability
                VecFalse(SetRowPtr,1,SideLength);
                for (uint icol=0; icol<SideLength; icol++)
                {
                    if (icol>=ibkclmin && icol<ibkclmax) continue;
                    byte *AvailPtr = &Board.Avail(irow,icol,0);
                    if (VectorSum(AvailPtr,1,SideLength) <= 1) continue;
                    VecOrTo(SetRowPtr,1,SideLength,AvailPtr);
                }
                
                // Find the block availability
                VecFalse(SetBlkPtr,1,SideLength);
                for (uint irwx=ibkrwmin; irwx<ibkrwmax; irwx++)
                {
                    if (irwx == irow) continue;
                    for (uint icol=ibkclmin; icol<ibkclmax; icol++)
                    {
                        byte *AvailPtr = &Board.Avail(irwx,icol,0);
                        if (VectorSum(AvailPtr,1,SideLength) <= 1) continue;
                        VecOrTo(SetBlkPtr,1,SideLength,AvailPtr);
                    }
                }
                
                // Find the block-clearing availability
                VecSub(SetDiffRowPtr,1,SideLength,SetSegPtr,SetRowPtr);
                uint dfrowlen = VectorSum(SetDiffRowPtr,1,SideLength);
                
                // Clear the non-segment part of the block
                if (dfrowlen > 0 && dfrowlen <= numseg)
                {
                    bool rc = false;
                    for (uint irwx=ibkrwmin; irwx<ibkrwmax; irwx++)
                    {
                        if (irwx == irow) continue;
                        for (uint icol=ibkclmin; icol<ibkclmax; icol++)
                        {
                            byte *AvailPtr = &Board.Avail(irwx,icol,0);
                            if (VecSubFmCheck(AvailPtr,1,SideLength,SetDiffRowPtr)) rc = true;
                        }
                    }
                    if (rc) return true;
                }
               
                // Find the row-clearing availability
                VecSub(SetDiffBlkPtr,1,SideLength,SetSegPtr,SetBlkPtr);
                uint dfblklen = VectorSum(SetDiffBlkPtr,1,SideLength);
                
                // Clear the non-segment part of the row
                if (dfblklen > 0 && dfblklen <= numseg)
                {
                    bool rc = false;
                    for (uint icol=0; icol<SideLength; icol++)
                    {
                        if (icol>=ibkclmin && icol<ibkclmax) continue;
                        byte *AvailPtr = &Board.Avail(irow,icol,0);
                        if (VecSubFmCheck(AvailPtr,1,SideLength,SetDiffBlkPtr)) rc = true;
                    }
                    if (rc) return true;
                }
            }
        }
    }
    
    return false;
}

bool ItscBlkCol(BoardManager &Board)
{
    // Set up
    uint BlockRows = Board.GetBlockRows();
    uint BlockCols = Board.GetBlockCols();
    uint SideLength = Board.GetSideLength();
    
    vector<byte> SetSeg(SideLength+1);
    byte *SetSegPtr = &SetSeg[0];
    vector<byte> SetCol(SideLength+1);
    byte *SetColPtr = &SetCol[0];
    vector<byte> SetBlk(SideLength+1);
    byte *SetBlkPtr = &SetBlk[0];
    
    vector<byte> SetDiffCol(SideLength+1);
    byte *SetDiffColPtr = &SetDiffCol[0];
    vector<byte> SetDiffBlk(SideLength+1);
    byte *SetDiffBlkPtr = &SetDiffBlk[0];
    
    // Iterate over the blocks
    for (uint ibkrow=0; ibkrow<BlockCols; ibkrow++)
    {
        uint ibkrwmin = BlockRows*ibkrow;
        uint ibkrwmax = ibkrwmin + BlockRows;
        for (uint ibkcol=0; ibkcol<BlockRows; ibkcol++)
        {
            uint ibkclmin = BlockCols*ibkcol;
            uint ibkclmax = ibkclmin + BlockCols;
            
            // Iterate over columns in the block
            for (uint icol = ibkclmin; icol<ibkclmax; icol++)
            {
                // Find the segment availability
                VecFalse(SetSegPtr,1,SideLength);
                uint numseg = 0;
                for (uint irow=ibkrwmin; irow<ibkrwmax; irow++)
                {
                    byte *AvailPtr = &Board.Avail(irow,icol,0);
                    if (VectorSum(AvailPtr,1,SideLength) <= 1) continue;
                    VecOrTo(SetSegPtr,1,SideLength,AvailPtr);
                    numseg++;
                }
                if (numseg == 0) continue;
               
                // Find the column availability
                VecFalse(SetColPtr,1,SideLength);
                for (uint irow=0; irow<SideLength; irow++)
                {
                    if (irow>=ibkrwmin && irow<ibkrwmax) continue;
                    byte *AvailPtr = &Board.Avail(irow,icol,0);
                    if (VectorSum(AvailPtr,1,SideLength) <= 1) continue;
                    VecOrTo(SetColPtr,1,SideLength,AvailPtr);
                }
                
                // Find the block availability
                VecFalse(SetBlkPtr,1,SideLength);
                for (uint iclx = ibkclmin; iclx<ibkclmax; iclx++)
                {
                    if (iclx == icol) continue;
                    for (uint irow=ibkrwmin; irow<ibkrwmax; irow++)
                    {
                        byte *AvailPtr = &Board.Avail(irow,iclx,0);
                        if (VectorSum(AvailPtr,1,SideLength) <= 1) continue;
                        VecOrTo(SetBlkPtr,1,SideLength,AvailPtr);
                    }
                }
                
                // Find the block-clearing availability
                VecSub(SetDiffColPtr,1,SideLength,SetSegPtr,SetColPtr);
                uint dfcollen = VectorSum(SetDiffColPtr,1,SideLength);
                
                // Clear the non-segment part of the block
                if (dfcollen > 0 && dfcollen <= numseg)
                {
                    bool rc = false;
                    for (uint iclx = ibkclmin; iclx<ibkclmax; iclx++)
                    {
                        if (iclx == icol) continue;
                        for (uint irow=ibkrwmin; irow<ibkrwmax; irow++)
                        {
                            byte *AvailPtr = &Board.Avail(irow,iclx,0);
                            if (VecSubFmCheck(AvailPtr,1,SideLength,SetDiffColPtr)) rc = true;
                        }
                        if (rc) return true;
                    }
                }
                
                // Find the column-clearing availability
                VecSub(SetDiffBlkPtr,1,SideLength,SetSegPtr,SetBlkPtr);
                uint dfblklen = VectorSum(SetDiffBlkPtr,1,SideLength);
                
                // Clear the non-segment part of the column
                if (dfblklen > 0 && dfblklen <= numseg)
                {
                    bool rc = false;
                    for (uint irow=0; irow<SideLength; irow++)
                    {
                        if (irow>=ibkrwmin && irow<ibkrwmax) continue;
                        byte *AvailPtr = &Board.Avail(irow,icol,0);
                        if (VecSubFmCheck(AvailPtr,1,SideLength,SetDiffBlkPtr)) rc = true;
                    }
                    if (rc) return true;
                }
            }
        }
    }
    
    return false;
}

bool Itsc(BoardManager &Board)
{
    if (ItscBlkRow(Board)) return true;
    if (ItscBlkCol(Board)) return true;
    
    return false;
}
