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


#ifndef INC_COLONY_H
#define INC_COLONY_H

#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <pthread.h>
#include "Amoeba-Cytoskelenton.h"

#define MODULENAME "Amoeba-Nucleus"
#define SHMSIZE       10485760
#define FATSIZE       3000000
#define FATSTART      200
#define SHMKEY        575
#define ALLACCESS     666
#define SHMSPACING    0
#define FRAGMENTATION 0
#define SPINTIME      5000
#define MAXTHREADS    2

struct Cytoskelenton;

struct Colony {
  /*
    19 = Lock DB
    20 = Needs defrag
    21 = Quit
  */
  char                *Shared;
  int                 Queen;
  unsigned long int   Size;
  unsigned long int   CacheProb;
  unsigned long int   CacheUnkn;
  unsigned long int   CacheDate;
  char                *CachedTime;
  unsigned long int   Population;
  char                *StartPos;
  char                *FinishPos;
  char                *Cached;
  unsigned long int   LastPos;
  key_t               Key;
  int                 SHMId; /* return value from shmget() */ 
  struct shmid_ds     *shmid_ds;
};

struct SearchThreadData {
  struct Blob *Needle;
  int ThreadID;
  unsigned long int *Location;
  unsigned long int *Cached;
  int *Terminate;
  struct Colony *City;
};

/*************************************************

  Notes: 
  ======

  1) Use AL1_InitHeader() to format the database!


**************************************************/

void                  AL1_FreeBlob(struct Blob *F);

void                  AL1_Relocate(struct Colony *City,unsigned long int Gap,unsigned long int From);
void                  AL1_DefragAllocTable(struct Colony *City);

void                  AL1_Rename(struct Colony *City,struct Blob *NewName,unsigned long int RecordID);

void                  AL1_FreeAmoeba(struct Cytoskelenton *C);
void                  *AL1_SearchThread(void *Data);
void                  AL1_CreateColony(struct Colony *City);
void                  AL1_DeleteColony(struct Colony *City);
void                  AL1_AttachColony(struct Colony *City);
void                  AL1_DetachColony(struct Colony *City);
void                  AL1_LoadColony(  struct Colony *City,const char *Filename);
void                  AL1_SaveColony(  struct Colony *City,const char *Filename);  
void                  AL1_UnlockColony(struct Colony *City);
void                  AL1_LockColony(  struct Colony *City);
void                  AL1_Defrag(struct Colony *City);
int                   AL1_AddDBEntry(struct Colony *City,struct Cytoskelenton *Data);
struct Cytoskelenton  *AL1_GetDBEntry(struct Colony *City,unsigned long int RecordID);
unsigned long int     AL1_FindDBEntry(struct Colony *City,struct Blob *Name);
int                   AL1_DelDBEntry(struct Colony *City,unsigned long int RecordID);
int                   AL1_RepDBEntry(struct Colony *City,struct Cytoskelenton *Data,unsigned long int RecordID);
struct Blob           *AL1_GetRecName(struct Colony *City,unsigned long int RecordID);
void                  *AL1_ExtColony(struct Colony *City,void *Int);
void                  *AL1_IntColony(struct Colony *City,void *Ext);
void                  AL1_InitHeader(struct Colony *City);
void                  AL1_ReadHeader(struct Colony *City);
void                  AL1_WriteHeader(struct Colony *City);
int                   AL1_UpdateHeader(struct Colony *City,unsigned long int ID,unsigned long int Start,unsigned long int Finish);
unsigned long int     AL1_GetStartHeader(struct Colony *City,unsigned long int ID);
unsigned long int     AL1_GetFinishHeader(struct Colony *City,unsigned long int ID);
unsigned long int     AL1_GetCacheHeader(struct Colony *City,unsigned long int ID);
void                  AL1_RecacheHeader(struct Colony *City,unsigned long int ID);
void                  AL1_DelIDHeader(struct Colony *City,unsigned long int ID,unsigned long int Cached);

#endif

/*

          /:`:::.     .        :       ...    .,::::::  :::::::.    :::.          
   ,,    /;` ;;`;;    ;;,.    ;;;   .;;;;;;;. ;;;;''''   ;;;'';;'   ;;`;;    ,,   
 ,[['   n[' ,[[ '[[,  [[[[, ,[[[[, ,[[     \[[,[[cccc    [[[__[[\. ,[[ '[[,  '[[, 
c$     c$" c$$$cc$$$c $$$$$$$$"$$$ $$$,     $$$$$""""    $$""""Y$$c$$$cc$$$c    $c
"8bo, o8"   888   888,888 Y88" 888o"888,_ _,88P888oo,__ _88o,,od8P 888   888,,od8"
  "MPmM"    YMM   ""` MMM  M'  "MMM  "YMMMMMP" """"YUMMM""YUMMMP"  YMM   ""` YM"  

*/
