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

#ifndef WMS
#define WMS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Memory Data */
int DB_Size;
int RB_Size;
int Newly_Learnt;


/* Memory Configuration */
double           MemoryLimit;                      /* Maximum number of concepts to hold in memory. -1 = Disabled.              */
double           MemoryExp;                        /* Number of days to expire before memory can be considered for removal.     */
double           MemoryProbability;                /* Coefficient to multiply occurance probability to compute relevence value. */
double           MemoryRate;                       /* Ratio of concepts forgotten divided by concepts learnt.                   */
double           MemoryThreshold;                  /* Threshold  probability for consideration for removal.                     */



struct Datablock{
   struct List_Str *DS[3];
   struct Datablock *Next;
};

/* Specifies these memory parameters from memory file, otherwise sets to default. */
struct Datablock *MemoryMode(struct Datablock *Q);
void             PrintHalf(struct Datablock *Q);
void             PrintDatabase(struct Datablock *Q[2]);
void             Print_DB_Entry(struct Datablock *LS, int P);
struct Datablock *Add_DB_Entry(struct Datablock *LS, char *S);
struct Datablock *Del_DB_Entry(struct Datablock *LS, int P);
int              Find_DB_Entry(struct Datablock *LS, char *S);
char             *Get_DB_Entry(struct Datablock *LS, int Type,int Position);
void             Rep_DB_Entry(struct Datablock *LS, int Type,int Position,char *S);
struct Datablock *Goto_DB_Entry(struct Datablock *LS,char *S);
void             Add_DB_Pair(struct Datablock *LS,char *Qual,char *Des);
char             *Lookup_DB_Attr(struct Datablock *DB,char *Qual,char *Des);
void             Print_DB_Duplicate(struct Datablock *LS,char *S);

/* Debug (seems to be cleared up now) */
char             *Bugfix1(char *B);

/* Compatibility */
struct List_Str  *CSRecon(struct List_Str *Word_List,struct List_Str *L);
void             FreeWMS(struct Datablock *DB);

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
