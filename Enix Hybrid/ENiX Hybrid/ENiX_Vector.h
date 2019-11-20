/*

This file is part of ENiXHybrid1.

ENiXHybrid1 is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

ENiXHybrid1 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ENiXHybrid1.  If not, see <http://www.gnu.org/licenses/>.

Programmed By M.S.Braithwaite December 2008.

*/

#ifndef ENiX_VEC
#define ENiX_VEC

#include<stdlib.h>

#define EQSIZE 4

/* return a new vector of length Size */
int *ENiXDefVecInt(int Size);

/* return a new vector of interpolated differences of Vector */
int *ENiXDifVecInt(int *Vector);

/* return 1 if Vector is uniform */
int ENiXIsEqVecInt(int *Vector);

/* return Vector filled with ints, Num */
int *ENiXFillVecInt(int Num,int *Vector);

/* return a new vector Sub subtracted from From */
int *ENiXSubVecInt(int *From,int *Sub);

/* return a new vector identical to Src */
int *ENiXCpyVecInt(int *Src);

/* return a new vector of length Size */
float *ENiXDefVecFlt(int Size);

/* return a new vector of interpolated differences of Vector */
float *ENiXDifVecFlt(float *Vector);

/* return a new vector of interpolated divisions of Vector */
float *ENiXDivVecFlt(float *Vector);

/* return 1 if Vector is uniform */
int ENiXIsEqVecFlt(float *Vector);

/* return Vector filled with floats, Num */
float *ENiXFillVecFlt(float Num,float *Vector);

/* return a new vector Sub subtracted from From */
float *ENiXSubVecFlt(float *From,float *Vector);

/* return a new vector identical to Src */
float *ENiXCpyVecFlt(float *Src);

/* return a float vector converted from int Vector */
float *ENiXConvertI2F(int *Vector);

/* return an int vector converted from float Vector */
int *ENiXConvertF2I(float *Vector);

/* return a new vector of Vector truncated to size Size */
float *ENiXCpySubFlt(int Size,float *Vector);

/* return a new vector from start to finish of Vector */
int *ENiXCpySecInt(int Start,int Finish,int *Vector);

/* return a new vector from start to finish of Vector */
float *ENiXCpySecFlt(int Start,int Finish,float *Vector);

/* return a blank vector equation */
float *ENiXNewBlankEq();

#endif

/*
                                                                                                               
    _/_/_/_/  _/      _/  _/  _/      _/      _/    _/            _/                  _/        _/        _/   
   _/        _/_/    _/        _/  _/        _/    _/  _/    _/  _/_/_/    _/  _/_/        _/_/_/      _/_/    
  _/_/_/    _/  _/  _/  _/      _/          _/_/_/_/  _/    _/  _/    _/  _/_/      _/  _/    _/        _/     
 _/        _/    _/_/  _/    _/  _/        _/    _/  _/    _/  _/    _/  _/        _/  _/    _/        _/      
_/_/_/_/  _/      _/  _/  _/      _/      _/    _/    _/_/_/  _/_/_/    _/        _/    _/_/_/        _/       
                                                         _/                                                    
                                                    _/_/                                                       
*/
