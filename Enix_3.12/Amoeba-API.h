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

#include "Amoeba-Cytoskelenton.h"

#define AL3_ADD     1
#define AL3_DELETE  2
#define AL3_REPLACE 3
#define AL3_FORMAT  4

struct DB_Wrap{
  struct Datablock *DB[2];
};

void              AL3_SrvDisplay();
void              AL3_Reread();
void              AL3_ContentsReg(struct Colony *Nucleii[2]);
void              AL3_Log(char *Message,int Priority);

struct DB_Wrap    *AL3_Acquire(struct Datablock *DB[2]);
void              AL3_Store(struct Datablock *DB[2]);

void              AL3_Interpret(struct Colony *Nucleii[2]);
void              AL3_TransmitAll(struct Datablock *R,int Socket);
struct Datablock  *AL3_ReceiveAll(int Socket);

void              AL3_Purge();
void              AL3_SaveFile(char *Filename);
void              AL3_ReadFile(char *Filename);
int               AL3_CheckService();

void              AL3_ReadFile(char *Filename);
void              AL3_WriteFile(struct Blob *Filename);

struct Datablock  *AL3_Blob2WMS(struct Blob *Data);
struct Blob       *AL3_WMS2Blob(struct Datablock *DB);


#endif

/*

          /:`:::.     .        :       ...    .,::::::  :::::::.    :::.          
   ,,    /;` ;;`;;    ;;,.    ;;;   .;;;;;;;. ;;;;''''   ;;;'';;'   ;;`;;    ,,   
 ,[['   n[' ,[[ '[[,  [[[[, ,[[[[, ,[[     \[[,[[cccc    [[[__[[\. ,[[ '[[,  '[[, 
c$     c$" c$$$cc$$$c $$$$$$$$"$$$ $$$,     $$$$$""""    $$""""Y$$c$$$cc$$$c    $c
"8bo, o8"   888   888,888 Y88" 888o"888,_ _,88P888oo,__ _88o,,od8P 888   888,,od8"
  "MPmM"    YMM   ""` MMM  M'  "MMM  "YMMMMMP" """"YUMMM""YUMMMP"  YMM   ""` YM"  

*/

