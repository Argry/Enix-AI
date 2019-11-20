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

#include "ENiX_Vector.h"

int *ENiXDefVecInt(int Size){
  int *R=(int *)malloc((Size+1)*sizeof(int)); R[0]=Size;
  return R;
}

int *ENiXDifVecInt(int *Vector){
  int *Result=ENiXDefVecInt(Vector[0]-1),C,Length=Vector[0];
  for(C=1;C<Length;C++)
    Result[C]=Vector[C+1]-Vector[C];
  return Result;
}

int ENiXIsEqVecInt(int *Vector){
  int C,Length=Vector[0]+1;
  for(C=2;C<Length;C++)
    if(Vector[1]!=Vector[C])
      return 0;
  return 1;
}

int ENiXIsEqVecFlt(float *Vector){
  int C,Length=Vector[0]+1;
  for(C=2;C<Length;C++)
    if(Vector[1]!=Vector[C])
      return 0;
  return 1;
}

int *ENiXFillVecInt(int Num,int *Vector){
  int C,Length=Vector[0]+1;
  for(C=1;C<Length;C++)
    Vector[C]=Num;
  return Vector;
}

int *ENiXSubVecInt(int *From,int *Sub){
  int C,Length,*R;
  if(From[0]>Sub[0])
    Length=From[0];
  else
    Length=Sub[0];
  R=ENiXDefVecInt(Length++);
  for(C=1;C<Length;C++)
    R[C]=From[C]-Sub[C];
  return R;
}

int *ENiXCpyVecInt(int *Src){
  int C,Size=Src[0]+1,*R=ENiXDefVecInt(Src[0]);
  for(C=1;C<Size;C++)
    R[C]=Src[C];
  return R;
}

float *ENiXCpyVecFlt(float *Src){
  int C,Size=Src[0]+1;
  float *R=ENiXDefVecFlt(Src[0]);
  for(C=1;C<Size;C++)
    R[C]=Src[C];
  return R;
}

float *ENiXSubVecFlt(float *From,float *Sub){
  int C,Length;
  float *R;
  if(From[0]>Sub[0])
    Length=From[0];
  else
    Length=Sub[0];
  Length=Sub[0];
  R=ENiXDefVecFlt(Length++);
  for(C=1;C<Length;C++)
    R[C]=From[C]-Sub[C];
  return R;
}

float *ENiXDefVecFlt(int Size){
  float *R=(float *)malloc((Size+1)*sizeof(float));
  R[0]=Size;
  return R;
}
 
float *ENiXDifVecFlt(float *Vector){
  float *Result=ENiXDefVecFlt(Vector[0]-1);
  int C,Length=Vector[0];
  for(C=1;C<Length;C++)
    Result[C]=Vector[C+1]-Vector[C];
  return Result;
}

float *ENiXDivVecFlt(float *Vector){
  float *Result=ENiXDefVecFlt(Vector[0]-1);
  int C,Length=Vector[0];
  for(C=1;C<Length;C++)
    Result[C]=Vector[C+1]/Vector[C];
  return
    Result;
}

int ENiXIsEqFlt(float *Vector){
  int C,Length=Vector[0]+1;
  for(C=2;C<Length;C++)
    if(Vector[1]!=Vector[C])
      return 0;
  return 1;
}

float *ENiXFillVecFlt(float Num,float *Vector){
  int C,Length=Vector[0]+1;
  for(C=1;C<Length;C++)
    Vector[C]=Num;
  return Vector;
}

float *ENiXConvertI2F(int *Vector){
  int C,Size=Vector[0];
  float *R=ENiXDefVecFlt(Size++);
  for(C=1;C<Size;C++)
    R[C]=Vector[C];
  return R;
}

int *ENiXConvertF2I(float *Vector){
  int C,Size=(int)Vector[0],*R=ENiXDefVecInt(Size++);
  for(C=1;C<Size;C++)
    R[C]=(int)Vector[C];
  return R;
}

float *ENiXCpySubFlt(int Size,float *Vector){
  int Max,C;
  float *R=ENiXFillVecFlt(0,ENiXDefVecFlt(Size));
  if(Vector[0]<R[0])
    Max=Vector[0];
  else
    Max=R[0];
  Max++;
  for(C=1;C<Max;C++)
    R[C]=Vector[C];
  return R;
}

int *ENiXCpySecInt(int Start,int Finish,int *Vector){
  int A=1-Start,C,D=Finish-Start,*R=ENiXDefVecInt(D); 
  R[0]=D;
  for(C=Start;C<Finish;C++)
    R[C+A]=Vector[C];
  return R;
}

float *ENiXCpySecFlt(int Start,int Finish,float *Vector){
  int A=1-Start,C,D=Finish-Start;
  float *R=ENiXDefVecFlt(D); 
  R[0]=D;
  for(C=Start;C<Finish;C++)
    R[C+A]=Vector[C];
  return R;
}

float *ENiXNewBlankEq(){
  float *R=ENiXFillVecFlt(0,ENiXDefVecFlt(EQSIZE));
  R[0]=0;
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
