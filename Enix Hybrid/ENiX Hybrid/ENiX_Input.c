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

/*
 requires stdlib.h, ENiX_Input.h, ENiX_Input.c
*/

#include "ENiX_Input.h"

int *ENiXGetInt(int Start,int Finish,char *Array[]){
  int C,*Return=NULL;
  Return=ENiXDefVecInt(Finish-Start);
  for(C=Start;C<Finish;C++)
    Return[C-Start+1]=atoi(Array[C]);
  return Return;
}

float *ENiXGetFlt(int Start,int Finish,char *Array[]){
  int C;
  float *Return=NULL;
  Return=ENiXDefVecFlt(Finish-Start);
  for(C=Start;C<Finish;C++)
    Return[C-Start+1]=(float)atof(Array[C]);
  return Return;
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
