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

/*
 should be set to 8
 10 for testing only
*/

#define MAX_ORD 8

int main(int argc,char *argv[]){
  int *Template,*R;
  
  float *D,*F;
  Template=ENiXFillVecInt(CHAOS_DIAMETER,ENiXDefVecInt(MAX_ORD));
  R=ENiXChaosInit(MAX_ORD);

  PolynomialSize=4;
  RecurrenceSize=2;

  while(R[0]){
    ENiXChaosGen(Template,R);
    /*
    D=ENiXCpyVecFlt(R);
    */
    D=ENiXConvertI2F(R);
    D[1]=PolynomialSize;
    D[2]=RecurrenceSize;
    if(R[0]){
      ENiXPrintHybrid("\nIdentities of ",":\n",D);
      F=ENiXHybridCal(MAX_ORD,1,D);
      ENiXPrintFltList("(",", ",")\n",F);
      ENiXHybrid(F);
      free(F);
      free(D);
    }
  }
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
