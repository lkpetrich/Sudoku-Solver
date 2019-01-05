//
//  BooleanVecOps.cpp
//  Sudoku Solver
//
//  Created by Loren Petrich on 1/6/17.
//  Copyright © 2017 Loren Petrich. All rights reserved.
//

#include "BooleanVecOps.hpp"


void VecSet(byte *Vec, uint IndMin, uint IndMax, byte Val)
{
    for (uint i=IndMin; i<=IndMax; i++)
        Vec[i] = Val;
}


void VecCopy(byte *Vec, uint IndMin, uint IndMax, byte *Src)
{
    for (uint i=IndMin; i<=IndMax; i++)
        Vec[i] = Src[i];
}


void VecNot(byte *Vec, uint IndMin, uint IndMax, byte *Src)
{
    for (uint i=IndMin; i<=IndMax; i++)
        Vec[i] = ByteNot(Src[i]);
}
void VecNotAt(byte *Vec, uint IndMin, uint IndMax)
{
    for (uint i=IndMin; i<=IndMax; i++)
        Vec[i] = ByteNot(Vec[i]);
}


void VecAnd(byte *Vec, uint IndMin, uint IndMax, byte *Src, byte *Srcx)
{
    for (uint i=IndMin; i<=IndMax; i++)
        Vec[i] = ByteAnd(Src[i],Srcx[i]);
}

void VecAndTo(byte *Vec, uint IndMin, uint IndMax, byte *Src)
{
    for (uint i=IndMin; i<=IndMax; i++)
        Vec[i] = ByteAnd(Vec[i],Src[i]);
}


void VecOr(byte *Vec, uint IndMin, uint IndMax, byte *Src, byte *Srcx)
{
    for (uint i=IndMin; i<=IndMax; i++)
        Vec[i] = ByteOr(Src[i],Srcx[i]);
}

void VecOrTo(byte *Vec, uint IndMin, uint IndMax, byte *Src)
{
    for (uint i=IndMin; i<=IndMax; i++)
        Vec[i] = ByteOr(Vec[i],Src[i]);
}


void VecSub(byte *Vec, uint IndMin, uint IndMax, byte *Src, byte *Srcx)
{
    for (uint i=IndMin; i<=IndMax; i++)
        Vec[i] = ByteSub(Src[i],Srcx[i]);
}

void VecSubFm(byte *Vec, uint IndMin, uint IndMax, byte *Src)
{
    for (uint i=IndMin; i<=IndMax; i++)
        Vec[i] = ByteSub(Vec[i],Src[i]);
}

bool VecSubFmCheck(byte *Vec, uint IndMin, uint IndMax, byte *Src)
{
    bool rc = false;
    for (uint i=IndMin; i<=IndMax; i++)
    {
        byte oldval = Vec[i];
        byte newval = ByteSub(oldval,Src[i]);
        if (newval != oldval) rc = true;
        Vec[i] = newval;
    }
    return rc;
}

