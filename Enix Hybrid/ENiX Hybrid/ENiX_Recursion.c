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

#include "ENiX_Recursion.h"

float ENiXRecCalVal(int Pos,float *Vector,float *Eq){
  int C;
  float R=0,Size=Eq[0]+1,VSize=Vector[0]+1;
  if(!Pos)
    return Vector[1];
  else{
    Pos++;
    for(C=1;C<Size;C++){
      if((Pos-C>0)&&(Pos-C<VSize)){
	R+=Eq[C]*Vector[Pos-C];
      }
    }
  }
  return R;
}

float *ENiXRecCalVecBas(float *Vector,float *Eq){
  int C;
  float Size=Vector[0],*R=ENiXFillVecFlt(-255,ENiXDefVecFlt(Size));
  for(C=0;C<Size;C++){
    R[C+1]=ENiXRecCalVal(C,Vector,Eq);
  }
  return R;
}

float *ENiXRecCalVecInd(float *Vector,float *Eq){
  int C;
  float Size=Vector[0],*R=ENiXCpyVecFlt(Vector);
  for(C=0;C<Size;C++)
    R[C+1]=ENiXRecCalVal(C,R,Eq);
  return R;
}

float *ENiXRecGen(float *Vector){
  int C,A;
  float Size=Vector[0]+1,*R,*B,*Free;
  R=ENiXFillVecFlt(0,ENiXDefVecFlt(Vector[0]));
  if(!Vector[1]){
    R[0]=0;
    return R;
  }
  else{
    B=ENiXFillVecFlt(0,Free=ENiXDefVecFlt(Vector[0]-1));
    for(C=2;C<Size;C++){
      R[C-1]=Vector[C];
      for(A=1;A<C;A++){
	B[A]=Vector[C-A];
	if(A<C-1)
	  R[C-1]-=R[A]*B[A]/Vector[1];
      }
      R[C-1]/=B[A-1];
    }
    free(Free);
  }
  return R;
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
