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

#include<stdio.h>
#include<stdlib.h>

#include "ENiX_Vector.h"
#include "ENiX_Input.h"
#include "ENiX_Print.h"
#include "ENiX_Maths.h"
#include "ENiX_Poly.h"
#include "ENiX_Hybrid.h"
#include "ENiX_Chaos.h"
#include "ENiX_Omega.h"
#include "ENiX_Recursion.h"

int main(int argc,char *argv[]){
  float *R;
  if(argc<3){
    printf("Syntax: %s <j> <k> <P={C1,...,Cj}> <R[C1,...,Ck]> <R[0]> <MaxValues>\n",argv[0]);
    printf("eg    : %s 3  2  1 1 2  1 1  2  20",argv[0]);
    puts("      : will generate the first 20 terms starting at 2 of:");
    puts("      : R[n]=1+x+2x^2+R[n-1]+R[n-2]");
    return 1;
  }
  ENiXPrintHybrid("","\n",R=ENiXGetFlt(1,argc,argv));
  ENiXPrintFltList("",",","\n",ENiXHybridCal(atoi(argv[argc-1]),atoi(argv[argc-2]),R));
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
