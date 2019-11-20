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

#ifndef ENiX_RECURSION
#define ENiX_RECURSION

#include "ENiX_Vector.h"

/* return the value at x=Pos for recurrence relation Eq from Vector input */
float ENiXRecCalVal(int Pos,float *Vector,float *Eq);

/* return the vector computed from recurrence relation Eq completely based on Vector input */
float *ENiXRecCalVecBas(float *Vector,float *Eq);

/* return the vector computed from recurrence relation Eq as independently as possible of Vector */
float *ENiXRecCalVecInd(float *Vector,float *Eq);

/* returns a recurrence formula from observed data, Vector */
float *ENiXRecGen(float *Vector);

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
