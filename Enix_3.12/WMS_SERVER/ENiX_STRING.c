/*! \file */

/*
This file is part of ENiX3.

ENiX3 is free software you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

ENiX3 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ENiX3.  If not, see <http://www.gnu.org/licenses/>.

       oooooooooooo ooooo      ooo  o8o  ooooooo  ooooo   .oooo.          
   .dP `888'     `8 `888b.     `8'  `"'   `8888    d8'  .dP""Y88b  Yb     
 .dP    888          8 `88b.    8  oooo     Y888..8P          ]8P'  `Yb   
dP      888oooo8     8   `88b.  8  `888      `8888'         <88b.     `Yb 
Yb      888    "     8     `88b.8   888     .8PY888.         `88b.    .dP 
 `Yb    888       o  8       `888   888    d8'  `888b   o.   .88P   .dP   
   `Yb o888ooooood8 o8o        `8  o888o o888o  o88888o `8bd88P'   dP     


Programmed By M.S.Braithwaite 2007 - 2011.

*/

#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "ENiX_STRING.h"


/*! \Brief Returns the length of a string, "S".
 *
 */
int StrLen(char *S){
  int _=0;
  if(S)
    while(S[_])
      _++;
  return _;
}

/*! \Brief Returns the first "Len" characters of string "S". 
 *
 */
char *StrFirst(char *S,int Len){
   int Size=0;
   char *R=NULL;
   if(Len>0){
      R=StrCat("",S);
      R[Len]=0;
      return R;
   }
   if(Len<0){
      Size=StrLen(S)+Len;
      R=StrCat("",S);
      R[Size]=0;
      return R;
   }
   return NULL;
}

/*! \Brief Optimised string cat, cats "S2" with length "L2" chars onto
 *         string "S1" with length "L1" chars.
 */
char *SuperStrCat(char *S1,char *S2,int L1,int L2){
  char *R=NULL;
  R=(char *)realloc(S1,L1+L2+1);
  memcpy(R+L1,S2,L2);
  R[L1+L2]=0; 
  return R; 
}

/*! \Brief String cat, appends "S2" onto string "S1".
 *         
 */
char *StrCat(char *S1,char *S2){ 
  int L1=StrLen(S1),L2=StrLen(S2); 
  char *R=NULL; 
  R=(char *)malloc(L1+L2+1);
  memcpy(R,S1,L1);
  memcpy(R+L1,S2,L2); R[L1+L2]=0; 
  return R;
}

/*! \Brief Returns the current time.
 *         
 */
char *Gettime(void){
  char *R=(char *)malloc(20);
  time_t rawtime=0;
  struct tm *timeinfo=NULL;
  time ( &rawtime );
  timeinfo = localtime ( &rawtime ); 
  strftime(R,20,"%H:%M:%S-%Y/%m/%d",timeinfo);
  return R;
}

/*! \Brief Returns the number "c" as a string.
 *         
 */
char *FloatToString(double c){
  /* seems to be a bug here with dealing with rationals */
  int _=0;
  char *R=NULL;
  double B=c;
  if(B<0){
    for(;B<=-10;B/=10)
      _++;
    _+=18;
  }
  else{
    for(;B>=10;B/=10)
      _++;
    _+=17;
  }
  R=(char *)malloc(_);
  snprintf(R,_-1,"%.16f",c);
  R[10]=0;
  return R;
}

/*! \Brief Returns the string "S" in lower case.
 *         
 */
char *ToLower(char *S){
   int _=0;
   while(S[_]){
     S[_]=tolower(S[_]);
     _++;
   }
   return S;
}

/*! \Brief Returns true if "S" appears to be a number.
 *         
 */
int IsNumeric(char *S){
   return (isdigit(S[0])||(S[0]=='-')||(S[0]=='.'));
}

/*! \Brief Strips out the characters "Filter" from string "String".
 *         
 */
char *FilterChars(char *String,char *Filter){
  int _=0,i=0;
  char *Filtered=(char *)malloc((strlen(String)+1)*sizeof(char));
  for(_=0;String[_];_++)
    if(!strchr(Filter,String[_]))
      Filtered[i++]=String[_];
  Filtered[i]=0;
  return Filtered;
}

/*

            88 oooooooooooo ooooo      ooo  o8o  ooooooo  ooooo   .oooo.          
   .dP     .8' `888'     `8 `888b.     `8'  `"'   `8888    d8'  .dP""Y88b  Yb     
 .dP      .8'   888          8 `88b.    8  oooo     Y888..8P          ]8P'  `Yb   
dP       .8'    888oooo8     8   `88b.  8  `888      `8888'         <88b.     `Yb 
Yb      .8'     888    "     8     `88b.8   888     .8PY888.         `88b.    .dP 
 `Yb   .8'      888       o  8       `888   888    d8'  `888b   o.   .88P   .dP   
   `Yb 88      o888ooooood8 o8o        `8  o888o o888o  o88888o `8bd88P'   dP     


 */
