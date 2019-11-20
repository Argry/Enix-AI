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


Programmed By M.S.Braithwaite 2007 - 2012.

*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "ENiX_LIST.h"
#include "ENiX_STRING.h"
#include "ENiX_WMS.h"
#include "ENiX_CALC.h"
#include "ENiX_Globals.h"

/*! \brief Scale a vector so that the largest number in the vector is max.
 *
 */
struct List_Str  *ScaleVector(struct List_Str *V,float Max){
  float C=0,M=0,Num=0;
  struct List_Str *Buffer=NULL;
  for(Buffer=V;Buffer;Buffer=Buffer->Next){
    C=atof(Buffer->Str);
    Num=C>0?C:-1*C;
    if(Num>M)
      M=Num;
  }
  Div(V,Str2List(FloatToString(M/Max)));
  return ANSWER;
}

/*! \brief Compute XOR of vector against a number.
 *
 */
struct List_Str  *XOR(struct List_Str *Word_List,struct List_Str *L){
   int _=0;
   struct List_Str *I1=List2Nums(Word_List),*I2=List2Nums(Str2List(L->Str));
   L=L->Next;
   ANSWER=NULL;
   for(;I1;I1=I1->Next){
      if(IsNumeric(I1->Str)&&IsNumeric(I2->Str))
	ANSWER=Ins_List_Element(ANSWER,FloatToString((((atof(I1->Str)!=0)^(atof(I2->Str)!=0)))),_++);
      else
	ANSWER=Ins_List_Element(ANSWER,IsNumeric(I1->Str)?I2->Str:I1->Str,_++);
   }
   return L;
}

/*! \brief Compute AND of vector against a number.
 *
 */
struct List_Str  *AND(struct List_Str *Word_List,struct List_Str *L){
   int _=0;
   struct List_Str *I1=List2Nums(Word_List),*I2=List2Nums(Str2List(L->Str));
   L=L->Next;
   ANSWER=NULL;
   for(;I1;I1=I1->Next){
      if(IsNumeric(I1->Str)&&IsNumeric(I2->Str))
	ANSWER=Ins_List_Element(ANSWER,FloatToString((((atof(I1->Str)!=0)&&(atof(I2->Str)!=0)))),_++);
      else
	ANSWER=Ins_List_Element(ANSWER,(IsNumeric(I1->Str)&&(atof(I1->Str)==0))||(IsNumeric(I2->Str)&&(atof(I2->Str)==0))?FloatToString(0):(IsNumeric(I1->Str)?I2->Str:I1->Str),_++);
   }
   return L;
}

/*! \brief Compute NOR of vector against a number.
 *
 */
struct List_Str  *NOR(struct List_Str *Word_List,struct List_Str *L){
   int _=0;
   struct List_Str *I1=List2Nums(Word_List),*I2=List2Nums(Str2List(L->Str));
   L=L->Next;
   ANSWER=NULL;
   for(;I1;I1=I1->Next){
      if(IsNumeric(I1->Str)&&IsNumeric(I2->Str))
	ANSWER=Ins_List_Element(ANSWER,FloatToString((!((atof(I1->Str)!=0)||(atof(I2->Str)!=0)))),_++);
      else
	ANSWER=Ins_List_Element(ANSWER,(IsNumeric(I1->Str)&&(atof(I1->Str)==0))||(IsNumeric(I2->Str)&&(atof(I2->Str)==0))?FloatToString(0):(IsNumeric(I1->Str)?I2->Str:I1->Str),_++);
   }
   return L;
}

/*! \brief Compute NAND of vector against a number.
 *
 */
struct List_Str  *NAND(struct List_Str *Word_List,struct List_Str *L){
   int _=0;
   struct List_Str *I1=List2Nums(Word_List),*I2=List2Nums(Str2List(L->Str));
   L=L->Next;
   ANSWER=NULL;
   for(;I1;I1=I1->Next){
      if(IsNumeric(I1->Str)&&IsNumeric(I2->Str))
	ANSWER=Ins_List_Element(ANSWER,FloatToString((!((atof(I1->Str)!=0)&&(atof(I2->Str)!=0)))),_++);
      else
	ANSWER=Ins_List_Element(ANSWER,IsNumeric(I1->Str)?I2->Str:((atof(I2->Str)==0&&IsNumeric(I2->Str))?FloatToString(1):I1->Str),_++);
   }
   return L;
}

/*! \brief Compute OR of vector against a number.
 *
 */
struct List_Str  *OR(struct List_Str *Word_List,struct List_Str *L){
   int _=0;
   struct List_Str *I1=List2Nums(Word_List),*I2=List2Nums(Str2List(L->Str));
   L=L->Next;
   ANSWER=NULL;
   for(;I1;I1=I1->Next){
      if(IsNumeric(I1->Str)&&IsNumeric(I2->Str))
	ANSWER=Ins_List_Element(ANSWER,FloatToString(((atof(I1->Str)!=0)||(atof(I2->Str)!=0))),_++);
      else
	ANSWER=Ins_List_Element(ANSWER,IsNumeric(I1->Str)?(!IsNumeric(I2->Str)&&(atof(I1->Str)==1)?FloatToString(1):I2->Str):(atof(I2->Str)==1?FloatToString(1):I1->Str),_++);
   }
   return L;
}

/*! \brief Compute NOT of vector against a number.
 *
 */
struct List_Str *NOT(struct List_Str *Word_List,struct List_Str *L){
   int _=0;
   struct List_Str *I1=List2Nums(Word_List);
   ANSWER=NULL;
   for(;I1;I1=I1->Next){
      if(IsNumeric(I1->Str))
	ANSWER=Ins_List_Element(ANSWER,FloatToString(atof(I1->Str)==0),_++);
      else
	ANSWER=Ins_List_Element(ANSWER,I1->Str,_++);
   }
   return L;
}

/*! \brief Return 1.
 *
 */
struct List_Str *TRUE(struct List_Str *Word_List,struct List_Str *L){
   ANSWER=Ins_List_Element(ANSWER,FloatToString(1),0);
   return L;
}

/*! \brief Return 0.
 *
 */
struct List_Str *FALSE(struct List_Str *Word_List,struct List_Str *L){
   ANSWER=Ins_List_Element(ANSWER,FloatToString(0),0);
   return L;
}

/*! \brief Determine if all numbers are greater.
 *
 */
struct List_Str *Greater(struct List_Str *Word_List,struct List_Str *L){
   int R=1;
   struct List_Str *I1=List2Nums(Word_List),*I2=List2Nums(Str2List(L->Str));
   L=L->Next;
   ANSWER=NULL;
   for(;I1;I1=I1->Next){
      if(IsNumeric(I1->Str)&&IsNumeric(I2->Str))
	if(!(atof(I1->Str)>atof(I2->Str)))
	  R=0;
   }
   ANSWER=Ins_List_Element(ANSWER,FloatToString(R),0);
   return L;   
}

/*! \brief Determine if all numbers are smaller.
 *
 */
struct List_Str  *Smaller(struct List_Str *Word_List,struct List_Str *L){
   int R=1;
   struct List_Str *I1=List2Nums(Word_List),*I2=List2Nums(Str2List(L->Str));
   L=L->Next;
   ANSWER=NULL;
   for(;I1;I1=I1->Next){
      if(IsNumeric(I1->Str)&&IsNumeric(I2->Str))
	if(!(atof(I1->Str)<atof(I2->Str)))
	  R=0;
   }
   ANSWER=Ins_List_Element(ANSWER,FloatToString(R),0);
   return L;   
}

/*! \brief Determine if all numbers are equal.
 *
 */
struct List_Str  *Equivalence(struct List_Str *Word_List,struct List_Str *L){
   int R=1;
   struct List_Str *I1=List2Nums(Word_List),*I2=List2Nums(Str2List(L->Str));
   ANSWER=NULL;
   for(;I1;I1=I1->Next){
      if(IsNumeric(I1->Str)&&IsNumeric(I2->Str))
	if(atof(I1->Str)!=atof(I2->Str))
	  R=0;
   }
   ANSWER=Ins_List_Element(ANSWER,FloatToString(R),0);
   return L->Next;
}

/*! \brief Calculate exponents of all numbers.
 *
 */
struct List_Str *Exp(struct List_Str *Word_List,struct List_Str *L){
   int _=0;
   struct List_Str *I1=List2Nums(Word_List),*I2=List2Nums(Str2List(L->Str));
   L=L->Next;
   ANSWER=NULL;
   for(;I1;I1=I1->Next){
      if(IsNumeric(I1->Str)&&IsNumeric(I2->Str))
	ANSWER=Ins_List_Element(ANSWER,FloatToString(pow(atof(I1->Str),atof(I2->Str))),_++);
      else
	ANSWER=Ins_List_Element(ANSWER,IsNumeric(I1->Str)?I2->Str:I1->Str,_++);
   }
   return L;
}

/*! \brief Divide all numbers by another number.
 *
 */
struct List_Str  *Div(struct List_Str *Word_List,struct List_Str *L){
   int _=0;
   struct List_Str *I1=List2Nums(Word_List),*I2=List2Nums(Str2List(L->Str));
   L=L->Next;
   ANSWER=NULL;
   for(;I1;I1=I1->Next){
      if(IsNumeric(I1->Str)&&IsNumeric(I2->Str))
	ANSWER=Ins_List_Element(ANSWER,FloatToString(atof(I1->Str)/atof(I2->Str)),_++);
      else
	ANSWER=Ins_List_Element(ANSWER,IsNumeric(I1->Str)?I2->Str:I1->Str,_++);
   }
   return L;
}

/*! \brief Multiply all numbers by another number.
 *
 */
struct List_Str  *Mul(struct List_Str *Word_List,struct List_Str *L){
   int _=0;
   struct List_Str *I1=List2Nums(Word_List),*I2=List2Nums(Str2List(L->Str));
   L=L->Next;
   ANSWER=NULL;
   for(;I1;I1=I1->Next){
     if(IsNumeric(I1->Str)&&IsNumeric(I2->Str))
	ANSWER=Ins_List_Element(ANSWER,FloatToString(atof(I1->Str)*atof(I2->Str)),_++);
      else
	ANSWER=Ins_List_Element(ANSWER,IsNumeric(I1->Str)?I2->Str:I1->Str,_++);
   }
   return L;
}

/*! \brief Add a number to all other numbers.
 *
 */
struct List_Str  *Add(struct List_Str *Word_List,struct List_Str *L){
   int _=0;
   struct List_Str *I1=List2Nums(Word_List),*I2=List2Nums(Str2List(L->Str));
   L=L->Next;
   ANSWER=NULL;
   for(;I1;I1=I1->Next){
      if(IsNumeric(I1->Str)&&IsNumeric(I2->Str))
	 ANSWER=Ins_List_Element(ANSWER,FloatToString(atof(I1->Str)+atof(I2->Str)),_++);
      else
	 ANSWER=Ins_List_Element(ANSWER,IsNumeric(I1->Str)?I2->Str:I1->Str,_++);
   }
   return L;
}

/*! \brief Add a number from all other numbers.
 *
 */
struct List_Str  *Sub(struct List_Str *Word_List,struct List_Str *L){
   int _=0;
   struct List_Str *I1=List2Nums(Word_List),*I2=List2Nums(Str2List(L->Str));
   L=L->Next;
   ANSWER=NULL;
   for(;I1;I1=I1->Next)
      ANSWER=Ins_List_Element(ANSWER,FloatToString(atof(I1->Str)-atof(I2->Str)),_++);
   return L;
}

/*! \brief Compute a factorial.
 *
 */
int Factorial(int In){
  int _=1,R=1;
  for(;_<In+1;_++)
    R*=_;
  return R;
}

/*! \brief Calculate a vector subtraction between two vectors.
 *
 */
struct List_Str *VectorSub(struct List_Str *V1,struct List_Str *V2){
   int _=0;
   struct List_Str *R=NULL,*Buffer1=NULL,*Buffer2=NULL;
   if(Size_of_List(V1)==Size_of_List(V2)){
      Buffer2=V2;
      for(Buffer1=V1;Buffer1;Buffer1=Buffer1->Next){
	 R=Ins_List_Element(R,FloatToString(atof(Buffer1->Str)-atof(Buffer2->Str)),_++);	 
	 Buffer2=Buffer2->Next;
      }
   }
   else
     if(ThinkOutLoud)
       puts("VectorSub requires dimensionally compatable vectors!");
   return R;
}

/*! \brief Calculate a vector addition between two vectors.
 *
 */
struct List_Str *VectorAdd(struct List_Str *V1,struct List_Str *V2){
   int _=0;
   struct List_Str *R=NULL,*Buffer1=NULL,*Buffer2=NULL;
   if(Size_of_List(V1)==Size_of_List(V2)){
      Buffer2=V2;
      for(Buffer1=V1;Buffer1;Buffer1=Buffer1->Next){
	 R=Ins_List_Element(R,FloatToString(atof(Buffer1->Str)+atof(Buffer2->Str)),_++);	 
	 Buffer2=Buffer2->Next;
      }
   }
   else
     if(ThinkOutLoud)
       puts("VectorAdd requires dimensionally compatable vectors!");
   return R;
}

/*! \brief Calculate the scalar produce between two vectors.
 *
 */
double ScalarProduct(struct List_Str *A,struct List_Str *B){
   struct List_Str *Buffer1=NULL,*Buffer2=NULL;
   double Result=0,C=0,D=0,MagA=0,MagB=0,Deno=0;
   if(Size_of_List(A)==Size_of_List(B)){
      Buffer2=B;
      for(Buffer1=A;Buffer1;Buffer1=Buffer1->Next){
	 C=atof(Buffer1->Str);
	 D=atof(Buffer2->Str);
	 Deno+=(C*D);
	 MagA+=(C*C);
	 MagB+=(D*D);
	 Buffer2=Buffer2->Next;
      }
   }
   else{
      if(ThinkOutLoud)
	puts("Scalar Products need dimensionally compatible vectors!");
      return 0;
   }
   Result=(Deno/(sqrt(MagA)*sqrt(MagB)));
   return acos(Result);
}

/*! \brief Perform a matrix t-transform on the binary expansion of a number sequence. Recombine the new number sequence from binary afterwards.
 *
 */
struct List_Str *T_Transform(struct List_Str *Word_List,struct List_Str *L){
   int _=0,Cache=0,Current=0,Position=0,Max=0,Length=0,*BeforeMatrix=NULL,Pos=0;
   struct List_Str *I1=List2Nums(Word_List),*Buffer=NULL;
   Length=Size_of_List(I1);
   ANSWER=NULL;
   for(Buffer=I1;Buffer;Buffer=Buffer->Next)
     if((Current=atoi(Buffer->Str))>_)
       _=Current;
   for(Max=0;_>>Max;Max++){}
   BeforeMatrix=((int *)malloc((_=(Max*Length))*sizeof(int *)));
   memset(BeforeMatrix,0,_);
   /*
     memset(BeforeMatrix=((int *)malloc((_=(Max*Length))*sizeof(int *))),0,_);
   */
   Current=Cache=0;
   for(Buffer=I1;Buffer;Buffer=Buffer->Next){
      Position=0;
      for(_=atoi(Buffer->Str);Position<Max;Position++)
	 BeforeMatrix[Cache+Position]=((_>>Position)&1);
      Cache=Max*(++Current);
   }
   for(Position=0;Position<Max;Position++){
      _=1;
      for(Current=Cache=0;Current<Length;Current++){
	 Cache+=BeforeMatrix[(Max*Current)+Position]*_;
	 _<<=1;
      }
      ANSWER=Ins_List_Element(ANSWER,FloatToString(Cache),Pos++);
   }
   return L;
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
