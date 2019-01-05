//
//  BooleanVecOps.hpp
//  Sudoku Solver
//
//  Created by Loren Petrich on 1/6/17.
//  Copyright © 2017 Loren Petrich. All rights reserved.
//

#ifndef BooleanVecOps_hpp
#define BooleanVecOps_hpp
#include <algorithm>
using namespace std;


typedef unsigned char byte;
typedef unsigned int uint;


// Scalar Boolean operations
inline byte ByteNot(byte x) {return (x != 0) ? 0 : 1;}
inline byte ByteAnd(byte x, byte y) {return min(x,y);}
inline byte ByteOr(byte x, byte y) {return max(x,y);}
inline byte ByteSub(byte x, byte y) {return ByteAnd(x,ByteNot(y));}
inline byte ByteXor(byte x, byte y) {return (x == y) ? 0 : 1;}

// Vector Boolean operations
// Note: in all these functions, the result is the first operand and not the last one
// It is followed by the minimum and maximum vector indices to operate on (both inclusive)

void VecSet(byte *Vec, uint IndMin, uint IndMax, byte Val);
inline void VecFalse(byte *Vec, uint IndMin, uint IndMax) {VecSet(Vec,IndMin,IndMax,0);}
inline void VecTrue(byte *Vec, uint IndMin, uint IndMax) {VecSet(Vec,IndMin,IndMax,1);}

void VecCopy(byte *Vec, uint IndMin, uint IndMax, byte *Src);

void VecNot(byte *Vec, uint IndMin, uint IndMax, byte *Src);
void VecNotAt(byte *Vec, uint IndMin, uint IndMax);

void VecAnd(byte *Vec, uint IndMin, uint IndMax, byte *Src, byte *Srcx);
void VecAndTo(byte *Vec, uint IndMin, uint IndMax, byte *Src);

void VecOr(byte *Vec, uint IndMin, uint IndMax, byte *Src, byte *Srcx);
void VecOrTo(byte *Vec, uint IndMin, uint IndMax, byte *Src);

void VecSub(byte *Vec, uint IndMin, uint IndMax, byte *Src, byte *Srcx);
void VecSubFm(byte *Vec, uint IndMin, uint IndMax, byte *Src);

bool VecSubFmCheck(byte *Vec, uint IndMin, uint IndMax, byte *Src);

#endif /* BooleanVecOps_hpp */
