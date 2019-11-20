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

#ifndef ENiX_PRINT
#define ENiX_PRINT

#include<stdio.h>
#include "ENiX_Vector.h"

/* Print a formatted list of ints */
void ENiXPrintIntList(char *Prefix,char *Separator,char *Suffix,int *Array);

/* Print a formatted list of floats */
void ENiXPrintFltList(char *Prefix,char *Separator,char *Suffix,float *Array);

/* Print a recurrence relation formula */
void ENiXPrintRec(char *Prefix,char *Suffix,float *Array);

/* Print a polynomial formula */
void ENiXPrintPly(char *Prefix,char *Suffix,int *Array);

/* Print a polynomial formula */
void ENiXPrintPlyFlt(char *Prefix,char *Suffix,float *Array);

/* Print a hybrid formula */
void ENiXPrintHybrid(char *Prefix,char *Suffice,float *Array);

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
