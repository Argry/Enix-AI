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

#ifndef WMS
#define WMS

#include "ENiX_BDEFS.h"

/* Memory Data */
extern int DB_Size;
extern int RB_Size;
extern int Newly_Learnt;


/* Memory Configuration */
extern double MemoryLimit;                      /* Maximum number of concepts to hold in memory. -1 = Disabled.              */
extern double MemoryExp;                        /* Number of days to expire before memory can be considered for removal.     */
extern double MemoryProbability;                /* Coefficient to multiply occurance probability to compute relevence value. */
extern double MemoryRate;                       /* Ratio of concepts forgotten divided by concepts learnt.                   */
extern double MemoryThreshold;                  /* Threshold  probability for consideration for removal.                     */

/* Specifies these memory parameters from memory file, otherwise sets to default. */
void             Load2RAM();
void             Save2Persistent(char *BackupName);
struct Datablock *MemoryMode(struct Datablock *Q);
struct Datablock *Add_DB_Entry(struct Datablock *LS, char *S);
struct Datablock *Del_DB_Entry(struct Datablock *LS, int P);
int              Find_DB_Entry(struct Datablock *LS, char *S);
char             *Get_DB_Entry(struct Datablock *LS, int Type,int Position);
void             Rep_DB_Entry(struct Datablock *LS, int Type,int Position,char *S);
struct Datablock *Goto_DB_Entry(struct Datablock *LS,char *S);
int              Rep_DB_Pair(struct Datablock *LS,char *Qual,char *Des); /* returns 1 if replaced */
int              Add_DB_Pair(struct Datablock *LS,char *Qual,char *Des); /* returns 1 if already exists */
int              Is_DB_Pair(struct Datablock *LS,char *Qual,char *Des); /* returns 1 if already exists */
int              Del_DB_Pair(struct Datablock *LS,char *Qual,char *Des); /* returns 1 if already exists */

char             *Lookup_DB_Attr(struct Datablock *DB,char *Qual,char *Des);
void             Print_DB_Duplicate(struct Datablock *LS,char *S);

char             *GetValue(char *Attribute);
struct List_Str  *Purge(struct List_Str *Word_List,struct List_Str *L);

/* Debug (seems to be cleared up now) */
char             *Bugfix1(char *B);

/* Compatibility */
void             FreeWMS(struct Datablock *DB);

struct Datablock *GetLang();
struct List_Str  *AddWRef(struct List_Str *Word_List,struct List_Str *L);
int              Detect_Sentiency();
struct List_Str  *Unknown_Words(struct List_Str *Word_List);
int              IsEnabled(char *Concept,char *Qualifier);
struct List_Str  *AllRels(struct List_Str *Word_List,struct List_Str *L);


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
