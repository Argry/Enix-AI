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

#include "ENiX_Omega.h"

int ENiXOmegaBounded(float *Vector){
  int C,R=0,Size=Vector[0]+1;
  float *New=ENiXCpyVecFlt(Vector);
  for(C=1;C<Size;C++){
    if(New[C]<0)
      New[C]*=-1;
    if(C>1){
      if(New[C]<New[C-1])
	R++;
      else
	R--;
    }
  }
  if(R>0)
    return 1;
  else
    return 0;
}

int ENiXOmegaCnv2Zro(float *Vector){
  int R;
  float A,B,*D;
  if(ENiXOmegaBounded(D=ENiXDifVecFlt(Vector))){
    A=Vector[(int)Vector[0]];
    B=Vector[0]*D[(int)Vector[0]-1];
    if(B<0)
      B*=-1;
    if(((A+B)>1)&&((A-B)<1))
      R=1;
    else
      R=0;
  }
  else
    R=0;
  return R;
}

int ENiXOmegaTest(float *Vector){
  int D,PurePoly=0,C,Size=Vector[0]+1;
  if(ENiXIsEqVecInt(ENiXConvertF2I(Vector)))
    return 1;
  for(C=4;C<Size;C++){
    D=ENiXOmegaCnv2Zro(ENiXDivVecFlt(ENiXCpySubFlt(C,Vector)));
    if(D)
      PurePoly++;
    else
      PurePoly--;
  }
  if(PurePoly>0)
    return 1;
  else
    return 0;
}

/*
                                                                                                               
    _/_/_/_/  _/      _/  _/  _/      _/      _/    _/            _/                  _/        _/        _/   
   _/        _/_/    _/        _/  _/        _/    _/  _/    _/  _/_/_/    _/  _/_/        _/_/_/      _/_/    
  _/_/_/    _/  _/  _/  _/      _/          _/_/_/_/  _/    _/  _/    _/  _/_/      _/  _/    _/        _/     
 _/        _/    _/_/  _/    _/  _/        _/    _/  _/    _/  _/    _/  _/        _/  _/    _/        _/      
_/_/_/_/  _/      _/  _/  _/      _/      _/    _/    _/_/_/  _/_/_/    _/        _/    _/_/_/        _/       
                                                         _/                                                    
                                                    _/_/                                                       
*/
