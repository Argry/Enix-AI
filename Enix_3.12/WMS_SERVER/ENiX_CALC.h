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

#ifndef ENiXCALC
#define ENiXCALC

#include "ENiX_BDEFS.h"

struct List_Str  *ScaleVector(struct List_Str *V,float Max);
struct List_Str  *Div(struct List_Str *Word_List,struct List_Str *L);
struct List_Str  *Mul(struct List_Str *Word_List,struct List_Str *L);
struct List_Str  *Add(struct List_Str *Word_List,struct List_Str *L);
struct List_Str  *Sub(struct List_Str *Word_List,struct List_Str *L);
struct List_Str  *Exp(struct List_Str *Word_List,struct List_Str *L);
struct List_Str  *Greater(struct List_Str *Word_List,struct List_Str *L);
struct List_Str  *Smaller(struct List_Str *Word_List,struct List_Str *L);
struct List_Str  *Equivalence(struct List_Str *Word_List,struct List_Str *L);
struct List_Str  *XOR(struct List_Str *Word_List,struct List_Str *L);
struct List_Str  *AND(struct List_Str *Word_List,struct List_Str *L);
struct List_Str  *NOT(struct List_Str *Word_List,struct List_Str *L);
struct List_Str  *NOR(struct List_Str *Word_List,struct List_Str *L);
struct List_Str  *NAND(struct List_Str *Word_List,struct List_Str *L);
struct List_Str  *OR(struct List_Str *Word_List,struct List_Str *L);
struct List_Str  *TRUE(struct List_Str *Word_List,struct List_Str *L);
struct List_Str  *FALSE(struct List_Str *Word_List,struct List_Str *L);

double           ScalarProduct(struct List_Str *A,struct List_Str *B);
struct List_Str  *VectorAdd(struct List_Str *V1,struct List_Str *V2);
struct List_Str  *VectorSub(struct List_Str *V1,struct List_Str *V2);

int              Factorial(int In);

struct List_Str  *T_Transform(struct List_Str *Word_List,struct List_Str *L);


#endif

/*

            88 oooooooooooo ooooo      ooo  o8o  ooooooo  ooooo   .oooo.          
   .dP     .8' `888'     `8 `888b.     `8'  `"'   `8888    d8'  .dP""Y88b  Yb     
 .dP      .8'   888          8 `88b.    8  oooo     Y888..8P          ]8P'  `Yb   
dP       .8'    888oooo8     8   `88b.  8  `888      `8888'         <88b.     `Yb 
Yb      .8'     888    "     8     `88b.8   888     .8PY888.         `88b.    .dP 
 `Yb   .8'      888       o  8       `888   888    d8'  `888b   o.   .88P   .dP   
   `Yb 88      o888ooooood8 o8o        `8  o888o o888o  o88888o `8bd88P'   dP     


 */
