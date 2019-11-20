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

#include "ENiX_Poly.h"

int ENiXPolyCalVal(int Pos, int *Vector){
  int C,R=0,Length=Vector[0]+1;
  for(C=1;C<Length;C++)
    R+=Vector[C]*ENiXPower(Pos,C-1);
  return R;
}

float ENiXPolyCalValFlt(int Pos, float *Vector){
  int C,Length=Vector[0]+1;
  float R=0;
  for(C=1;C<Length;C++)
    R+=Vector[C]*ENiXPower(Pos,C-1);
  return R;
}

int *ENiXPolyCalVec(int Size,int *Vector){
  int C,*R=ENiXDefVecInt(Size);
  Size++;
  for(C=1;C<Size;C++)
    R[C]=ENiXPolyCalVal(C-1,Vector);
  return R;
}

float *ENiXPolyCalVecFlt(int Size,float *Vector){
  int C;
  float *R=ENiXDefVecFlt(Size);
  Size++;
  for(C=1;C<Size;C++)
    R[C]=ENiXPolyCalValFlt(C-1,Vector);
  return R;
}

int *ENiXPolyGen(int *Vector){
  int T,Size=Vector[0]+1,D;
  int *B1=Vector,*B2=NULL,*F,*R=ENiXFillVecInt(0,ENiXDefVecInt(Vector[0]));
  int *Old=NULL;
  if((ENiXIsEqVecInt(Vector))&&(!Vector[1]))
    return R;
  while(Size>0){
    if(ENiXIsEqVecInt(B1)){
      if(B1[1]){
	T=Vector[0]+1-Size;
	D=ENiXFactorial(T);
	R[T+1]=B1[1]/D;
	if(((ENiXIsEqVecInt(R))&&(R[1]==0))||(D>B1[1])){
	  if((Vector!=B1))
	    free(B1);
	  R[0]=0;
	  return R;
	}
	if((Vector!=B1))
	  free(B1);
	B1=ENiXSubVecInt(Vector,F=ENiXPolyCalVec(Vector[0],R));
	Size=Vector[0]+1;
	free(F);
      }
      else{
	free(B1);
	return R;
      }
    }
    else{
      B2=ENiXDifVecInt(Old=B1);
      if((Vector!=Old))
	free(Old);
      B1=B2;
      Size--;
    }
  }
  R[0]=0;
  free(B1);
  return R;
}

float *ENiXPolyGenFlt(float *Vector){
  int T,Size=Vector[0]+1,D;
  float *B1=Vector,*B2=NULL,*F;
  float *R=ENiXFillVecFlt(0,ENiXDefVecFlt(Vector[0]));
  float *Old=NULL;
  if((ENiXIsEqVecFlt(Vector))&&(!Vector[1]))
    return R;
  while(Size>0){
    if(ENiXIsEqVecFlt(B1)){
      if(B1[1]){
	T=Vector[0]+1-Size;
	D=ENiXFactorial(T);
	R[T+1]=B1[1]/D;
	if(((ENiXIsEqVecFlt(R))&&(R[1]==0))||(Size<3)){
	  if((Vector!=B1))
	    free(B1);
	  R[0]=0;
	  return R;
	}
	if((Vector!=B1))
	  free(B1);
	B1=ENiXSubVecFlt(Vector,F=ENiXPolyCalVecFlt(Vector[0],R));
	Size=Vector[0]+1;
	free(F);
      }
      else{
	free(B1);
	return R;
      }
    }
    else{
      B2=ENiXDifVecFlt(Old=B1);
      if((Vector!=Old))
	free(Old);
      B1=B2;
      Size--;
    }
  }
  R[0]=0;
  free(B1);
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
