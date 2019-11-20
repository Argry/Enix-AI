/*! \file */

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

#ifndef AMOEBA_API
#define AMOEBA_API

#define WMS_ADD     1
#define WMS_DELETE  2
#define WMS_REPLACE 3
#define WMS_FORMAT  4

#include "WMS_SHM.h"

struct DB_Wrap{
  struct Datablock *DB[2];
};

struct DataArea *Nucleii[2];

void              WMS_MainHandler();
void              WMS_Worker();
int               Daemon();

void              WMS_SrvDisplay();
void              WMS_Reread();
/*
void              WMS_ContentsReg(struct DataArea *Nucleii[2]);
*/
void              WMS_Log(char *Message,int Priority);

struct DB_Wrap    *WMS_Acquire(struct Datablock *DB[2]);
void              WMS_Store(struct Datablock *DB[2]);

void              WMS_Interpret(struct DataArea *Nucleii[2]);
void              WMS_TransmitAll(struct Datablock *R,int Socket);
struct Datablock  *WMS_ReceiveAll(int Socket);

void              WMS_Purge();
void              WMS_SaveFile(char *Filename);
void              WMS_ReadFile(char *Filename);
int               WMS_CheckService();

void              WMS_ReadFile(char *Filename);
void              WMS_WriteFile(struct Blob *Filename);

struct Datablock  *WMS_Blob2WMS(struct Blob *Data);
struct Blob       *WMS_WMS2Blob(struct Datablock *DB);


#endif

/*

          /:`:::.     .        :       ...    .,::::::  :::::::.    :::.          
   ,,    /;` ;;`;;    ;;,.    ;;;   .;;;;;;;. ;;;;''''   ;;;'';;'   ;;`;;    ,,   
 ,[['   n[' ,[[ '[[,  [[[[, ,[[[[, ,[[     \[[,[[cccc    [[[__[[\. ,[[ '[[,  '[[, 
c$     c$" c$$$cc$$$c $$$$$$$$"$$$ $$$,     $$$$$""""    $$""""Y$$c$$$cc$$$c    $c
"8bo, o8"   888   888,888 Y88" 888o"888,_ _,88P888oo,__ _88o,,od8P 888   888,,od8"
  "MPmM"    YMM   ""` MMM  M'  "MMM  "YMMMMMP" """"YUMMM""YUMMMP"  YMM   ""` YM"  

*/

