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


#ifndef UNITTESTS
#define UNITTESTS

#include "Amoeba-Cytoskelenton.h"
#include "ENiX_BDEFS.h"

struct Colony;

void              DEBUG_PURGE();
void              DEBUG_LOADFILE();
void              DEBUG_SAVEFILE();

void              DEBUG_RXServer();
void              DEBUG_TXServer();

void              DEBUG_TXClient();
void              DEBUG_RXClient();
void              DEBUG_ClientRefresh();

void              DEBUG_Function();

void              DEBUG_NetServer();
void              DEBUG_NetClient(unsigned long Retries);

void              DEBUG_DefragAll(struct Colony *City);
void              DEBUG_ColonySummary(struct Colony *City);
void              DEBUG_WMS2ABA(struct Colony *City,unsigned long int Retries);
void              DEBUG_ShowWMS(struct Colony *City);

void              DEBUG_AddRec(struct Colony *City);

void              DEBUG_Delete(struct Colony *City,char *Name,unsigned long int Retries);
void              DEBUG_DAT(struct Colony *City);
void              DEBUG_Header(struct Colony *City);
void              DEBUG_ALL(struct Colony *City);
unsigned long int DEBUG_New(struct Colony *City,struct Blob *Name);
void              DEBUG_Fill(struct Colony *City);
void              DEBUG_PartFill(struct Colony *City,unsigned long int Total);
void              DEBUG_Search(struct Colony *City,char *Name,unsigned long int Retries);
void              DEBUG_Modify(struct Colony *City,char *Name,unsigned long int Retries);

struct Datablock  *DEBUG_MakeWMSRec();
void              DEBUG_Display(struct Cytoskelenton *C);
void              DEBUG_Mito(struct Mitochondrion *M);

#endif

/*

          /:`:::.     .        :       ...    .,::::::  :::::::.    :::.          
   ,,    /;` ;;`;;    ;;,.    ;;;   .;;;;;;;. ;;;;''''   ;;;'';;'   ;;`;;    ,,   
 ,[['   n[' ,[[ '[[,  [[[[, ,[[[[, ,[[     \[[,[[cccc    [[[__[[\. ,[[ '[[,  '[[, 
c$     c$" c$$$cc$$$c $$$$$$$$"$$$ $$$,     $$$$$""""    $$""""Y$$c$$$cc$$$c    $c
"8bo, o8"   888   888,888 Y88" 888o"888,_ _,88P888oo,__ _88o,,od8P 888   888,,od8"
  "MPmM"    YMM   ""` MMM  M'  "MMM  "YMMMMMP" """"YUMMM""YUMMMP"  YMM   ""` YM"  

*/
