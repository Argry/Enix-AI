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

#ifndef ENiX_LIST
#define ENiX_LIST

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

struct List_Str{
   char *Str;
   struct List_Str *Next;
};

struct List_Str  *ANSWER;

void             PrintList(struct List_Str *LS);
void             PrintListSpecial(char *Prefix,struct List_Str *LS,char *Separator,char *FinalSep,char *Suffix);
struct List_Str  *Cpy_List(struct List_Str *LS);
struct List_Str  *Ins_List_Element(struct List_Str *LS,char *S,int P);
struct List_Str  *Ins_List_List(struct List_Str *L1,struct List_Str *L2,int P);
struct List_Str  *Del_List_Element(struct List_Str *LS,int P);
char             *Get_List_Element(struct List_Str *LS,int P);
struct List_Str  *Rep_List_Element(struct List_Str *LS,int P,char *Data);
int              Find_List_Element(struct List_Str *LS,char *S);
int              Size_of_List(struct List_Str *LS);
struct List_Str  *LowerCaseList(struct List_Str *LS);
struct List_Str  *SortList(struct List_Str *LS);
struct List_Str  *FormatList(struct List_Str *LS,char *First,char *Punct,char *Conj,char *Final);
struct List_Str  *RemoveDups(struct List_Str *LS);
struct List_Str  *AutoFormat(struct List_Str *LS);
struct List_Str **Add_To_Array(struct List_Str **ARRAY,struct List_Str *ToAdd,int Size);

struct List_Str  *Str2List(char *S);
char             *List2Str(struct List_Str *L);

void             FreeList(struct List_Str *L);

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
