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

#include "ENiX_Print.h"

void ENiXPrintIntList(char *Prefix,char *Separator,char *Suffix,int *Array){
  int C,Size=Array[0]+1;
  printf("%s",Prefix);
  for(C=1;C<Size;C++){
    printf("%d",Array[C]);
    if(C<Array[0])
      printf("%s",Separator);
  }
  printf("%s",Suffix);
}

void ENiXPrintFltList(char *Prefix,char *Separator,char *Suffix,float *Array){
  int C,Size=Array[0]+1;
  printf("%s",Prefix);
  for(C=1;C<Size;C++){ 
    printf("%.20g",Array[C]);
    if(C<Array[0])
      printf("%s",Separator);
  }
  printf("%s",Suffix);
}

void ENiXPrintRec(char *Prefix,char *Suffix,float *Array){
  int C;
  float N,First=1,Length=Array[0]+1;
  printf("%s",Prefix);
  if(Array[0])
    for(C=1;C<Length;C++){
      if(Array[C]){
	if(Array[C]<0){
	  printf("-");
	  N=-1*Array[C];
	}
	else{
	  N=Array[C];
	  if(!First)
	    printf("+");
	}
	if(N!=1)
	  printf("%.20g*",N);
	printf("R[n-%d]",C);
	First=0;
      }
    }
  else 
    printf("No solution");
  printf("%s",Suffix);
}

void ENiXPrintPlyFlt(char *Prefix,char *Suffix,float *Array){
  int C,First=1,Length=Array[0]+1;
  float N;
  printf("%s",Prefix);  
  if((ENiXIsEqVecFlt(Array))&&(Array[1]==0))
    printf("0");
  if(Array[0]){
    for(C=1;C<Length;C++){
      if(Array[C]){
	if(Array[C]<0){
	  printf("-");
	  N=-1*Array[C];
	}
	else{
	  N=Array[C];
	  if(!First)
	    printf("+");
	}
	if(C>1){
	  if(N!=1)
	    printf("%.20g",N);
	  else
	    if(C==1)
	      printf("1");
	}
	else
	  printf("%.20g",N);
	if(C>1)
	  printf("n");
	if(C>2)
	  printf("^%d",C-1);
	First=0;
      }
    }
  }
  else
    printf(" No solution");
  printf("%s",Suffix);
}

void ENiXPrintPly(char *Prefix,char *Suffix,int *Array){
  int C,First=1,N,Length=Array[0]+1;
  printf("%s",Prefix);  
  if((ENiXIsEqVecInt(Array))&&(Array[1]==0))
    printf("0");
  if(Array[0]){
    for(C=1;C<Length;C++){
      if(Array[C]){
	if(Array[C]<0){
	  printf("-");
	  N=-1*Array[C];
	}
	else{
	  N=Array[C];
	  if(!First)
	    printf("+");
	}
	if(C>1){
	  if(N!=1)
	    printf("%d",N);
	  else
	    if(C==1)
	      printf("1");
	}
	else
	  printf("%d",N);
	if(C>1)
	  printf("n");
	if(C>2)
	  printf("^%d",C-1);
	First=0;
      }
    }
  }
  else
    printf(" No solution");
  printf("%s",Suffix);
}

void ENiXPrintHybrid(char *Prefix,char *Suffix,float *Array){
  int Size1=Array[1],Size2=Array[2],PrintP,PrintR;
  float *Ply,*Rec;

  Ply=ENiXCpySecFlt(3,3+Size1,Array);
  Rec=ENiXCpySecFlt(3+Size1,3+Size1+Size2,Array);
  printf("%s",Prefix);
  printf("R[n]=");
  if((Size2)&&(!((ENiXIsEqVecFlt(Rec)&&(Rec[1]==0)))))
    PrintR=1;
  else
    PrintR=0;
  if((Size1)&&(!((ENiXIsEqVecFlt(Ply)&&(Ply[1]==0)))))
    PrintP=1;
  else
    PrintP=0;
  if(PrintR)
    ENiXPrintRec("","",Rec);
  if((PrintR)&&(PrintP))
    printf("+");
  if(PrintP)
    ENiXPrintPlyFlt("","",Ply);
  if((!PrintP)&&(!PrintR))
    printf("0");
  printf("%s",Suffix);

  free(Ply);
  free(Rec);
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
