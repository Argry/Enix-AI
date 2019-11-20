/*

This file is part of Amoeba.

Amoeba is free software you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Amoeba is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Amoeba.  If not, see <http://www.gnu.org/licenses/>.

      :::.     .        :       ...    .,::::::  :::::::.    :::.          
   ,, ;;`;;    ;;,.    ;;;   .;;;;;;;. ;;;;''''   ;;;'';;'   ;;`;;    ,,   
 ,[[',[[ '[[,  [[[[, ,[[[[, ,[[     \[[,[[cccc    [[[__[[\. ,[[ '[[,  '[[, 
c$  c$$$cc$$$c $$$$$$$$"$$$ $$$,     $$$$$""""    $$""""Y$$c$$$cc$$$c    $c
"8bo,888   888,888 Y88" 888o"888,_ _,88P888oo,__ _88o,,od8P 888   888,,od8"
  "MPYMM   ""` MMM  M'  "MMM  "YMMMMMP" """"YUMMM""YUMMMP"  YMM   ""` YM"  

Programmed By M.S.Braithwaite 2012.

*/


#ifndef MICROTUBULE
#define MICROTUBULE

#include "Amoeba-Colony.h"
#include "ENiX_STRING.h"
#include "ENiX_LIST.h"
#include "ENiX_WMS.h"
#include "Amoeba-Unit-Tests.h"

struct Colony;
struct Cytoskelenton;
struct Pseudopod;
 struct Mitochondrion;

char *DATE;

/* Top Level Functions */

struct Datablock     *AL2_GetDBContents(struct Colony *City);
void                 AL2_PutDBContents(struct Datablock *In,struct Colony *City);

unsigned long int    NewRec(struct Colony *City,struct Blob *Name);
unsigned long int    DelRec(struct Colony *City,unsigned long int RecordID);
unsigned long int    RepRec(struct Colony *City,struct Datablock *DB,unsigned long int RecordID);

struct Datablock     *Amoeba2WMS(struct Colony *City,unsigned long int RecordID);
unsigned long int    WMS2Amoeba(struct Colony *City,struct Datablock *DB);

unsigned long int    DelPair(struct Colony *City,struct Mitochondrion *M);
unsigned long int    AddPair(struct Colony *City,unsigned long int Position,struct Blob *Qual,struct Blob *Value);
struct Mitochondrion *FindPair(struct Colony *City,unsigned long int Position,struct Blob *Qual,struct Blob *Value);
unsigned long int    RepPair(struct Colony *City,unsigned long int Position,struct Blob *Qual,struct Blob *Value);

unsigned long int    RepEntry(struct Colony *City,struct Mitochondrion *M,struct Blob *Attribute);
unsigned long int    AddEntry(struct Colony *City,struct Mitochondrion *M,struct Blob *Attribute);
struct Mitochondrion *FindEntry(struct Colony *City,unsigned long int Position,struct Blob *Pattern);
struct Blob          *GetEntry(struct Colony *City,struct Mitochondrion *M);
struct Cytoskelenton *DeleteEntry(struct Colony *City,struct Cytoskelenton *C,struct Mitochondrion *M);

void                 Amoeba_P_DB_Entry(struct Datablock *LS,int P);

/* Simple functions */

void                 AL2_CacheFreqUsed(struct Colony *City);

struct List_Str      *AL2_ReportReferences(struct Colony *City,struct Blob *Name);
struct List_Str      *AL2_ReportLinks(struct Colony *City,struct Blob *Name);
struct List_Str      *AL2_ReportContents(struct Colony *City);
struct Mitochondrion *AL2_CreateMito(unsigned long int ARef,unsigned short int PRef,unsigned short int MRef);
struct Mitochondrion *AL2_DuplicMito(struct Mitochondrion *M);
struct Pseudopod     *AL2_Add2Pseudo(struct Pseudopod *P,struct Mitochondrion *M);
struct Pseudopod     *AL2_NullPseudo();
struct Cytoskelenton *AL2_Add2Cyto(struct Cytoskelenton *C,struct Pseudopod *P);
struct Cytoskelenton *AL2_NullCyto(struct Blob *Name);
struct Mitochondrion *AL2_ReturnMito(struct Cytoskelenton *C,struct Mitochondrion *Pos);
struct Mitochondrion *AL2_FindMito(struct Cytoskelenton *C,struct Mitochondrion *Token);
struct Cytoskelenton *AL2_DeleteMito(struct Cytoskelenton *C,struct Mitochondrion *Pos);
unsigned long int    AL2_RepLink(struct Colony *City,struct Cytoskelenton *Source,struct Cytoskelenton *Target,struct Mitochondrion *From,struct Mitochondrion *To);
unsigned long int    AL2_AddLink(struct Colony *City,struct Cytoskelenton *Source,struct Cytoskelenton *Target,struct Mitochondrion *From,struct Mitochondrion *To);
unsigned long int    AL2_DelLink(struct Colony *City,struct Mitochondrion *From);
unsigned long int    AL2_Recognise(struct Colony *City,char *Flag,struct Blob *Name);
struct Pseudopod     *AL2_DuplicPseudo(struct Pseudopod *P);
struct Pseudopod     *AL2_CreateHeader(struct Colony *City,unsigned long int RecordID);
unsigned long int    AL2_CreateBlankR(struct Colony *City,struct Blob *Name);



#endif

/*

          /:`:::.     .        :       ...    .,::::::  :::::::.    :::.          
   ,,    /;` ;;`;;    ;;,.    ;;;   .;;;;;;;. ;;;;''''   ;;;'';;'   ;;`;;    ,,   
 ,[['   n[' ,[[ '[[,  [[[[, ,[[[[, ,[[     \[[,[[cccc    [[[__[[\. ,[[ '[[,  '[[, 
c$     c$" c$$$cc$$$c $$$$$$$$"$$$ $$$,     $$$$$""""    $$""""Y$$c$$$cc$$$c    $c
"8bo, o8"   888   888,888 Y88" 888o"888,_ _,88P888oo,__ _88o,,od8P 888   888,,od8"
  "MPmM"    YMM   ""` MMM  M'  "MMM  "YMMMMMP" """"YUMMM""YUMMMP"  YMM   ""` YM"  

*/
