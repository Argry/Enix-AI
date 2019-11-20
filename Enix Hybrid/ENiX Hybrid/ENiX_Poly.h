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

#ifndef ENiX_POLY
#define ENiX_POLY

#include <stdlib.h>
#include <stdio.h>
#include "ENiX_Maths.h"
#include "ENiX_Vector.h"


/* returns a y ordinate of polynomial Vector at x ordinate Pos */
int    ENiXPolyCalVal(int Pos, int *Vector);

/* returns a vector from results computed from polynomial Vector */
int   *ENiXPolyCalVec(int Size,int *Vector);

/* returns a y ordinate of polynomial Vector at x ordinate Pos */
float  ENiXPolyCalValFlt(int Pos, float *Vector);

/* returns a vector from results computed from polynomial Vector */
float *ENiXPolyCalVecFlt(int Size,float *Vector);

/* return a polynomial regressed around Vector input */
int   *ENiXPolyGen(int *Vector);

/* floating point version of above */
float *ENiXPolyGenFlt(float *Vector);

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
