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

#include "ENiX_STRING.h"

int StrLen(char *S){
  int _=0;
  while(S[_])
    _++;
  return _;
}

char *SuperStrCat(char *S1,char *S2,int L1,int L2){
  char *R=NULL;
  R=(char *)realloc(S1,L1+L2+1);
  memcpy(R+L1,S2,L2);
  R[L1+L2]=0; 
  return R; 
}

char *StrCat(char *S1,char *S2){ 
  int L1=StrLen(S1),L2=StrLen(S2); 
  char *R=NULL; 
  R=(char *)malloc(L1+L2+1);
  memcpy(R,S1,L1);
  memcpy(R+L1,S2,L2); R[L1+L2]=0; 
  return R;
}

char *Gettime(void){
  char *R=malloc(20);
  time_t rawtime;
  struct tm * timeinfo;
  time ( &rawtime );
  timeinfo = localtime ( &rawtime ); 
  strftime(R,20,"%H:%M:%S-%Y/%m/%d",timeinfo);
  return R;
}

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

char *ToLower(char *S){
   int _=0;
   while(S[_]){
     S[_]=tolower(S[_]);
     _++;
   }
   return S;
}

char *LoadFile(FILE *INPUT){
  int _=0,Stuff;
  char *Contents=NULL;
  fseek(INPUT,0,SEEK_END);
  _=ftell(INPUT);
  rewind(INPUT);
  Contents=(char *)malloc(_+1); 
  Stuff=fread(Contents,_,1,INPUT); 
  Contents[_]=0; 
  return Contents;
}

char *LoadPipe(FILE *PIPE){
   int Offset=0,_;                                                                                             
   char *Buffer=(char *)malloc(10*sizeof(char *)),*Contents=(char *)malloc(10*sizeof(char *));                  
   memset(Buffer,0,10);
   memset(Contents,0,10);
   while(fgets(Buffer,9,PIPE)!=NULL){
      memcpy(Contents+Offset,Buffer,_=StrLen(Buffer));
      Contents[Offset+_]=0;
      Offset+=_;
      Contents=(char *)realloc(Contents,(Offset+10)*sizeof(char *));
   }
   if(Contents[0]){
      _=StrLen(Contents);
      Contents[_-1]=0;
   }
   return Contents;
}


int IsNumeric(char *S){
   return (isdigit(S[0])||(S[0]=='-')||(S[0]=='.'));
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
