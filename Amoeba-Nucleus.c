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

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include "ENiX_LIST.h"
#include "ENiX_WMS.h"
#include "Amoeba-Unit-Tests.h"
#include "Amoeba-Colony.h"
#include "Amoeba-Cytoskelenton.h"
#include "Amoeba-Microtubules.h"
#include "Amoeba-Nucleus.h"
#include "Amoeba-API.h"

extern struct Colony *Nucleii[2];
struct Colony Middens[2];
extern int AllowQueen;

int FIRSTRUN=1;
const char *FILE1="./Data1.ENiX";
const char *FILE2="./Data2.ENiX";

/*! \brief Queens the Amoeba database so that workers may attach.
 * - The Amoeba Queen is responsible for creating the database in SHM and populating it.
 */
void MainHandler(){
  int _=0;
  AllowQueen=1;
  for(_=0;_<2;_++){
    AL1_CreateColony(&Middens[_]);
    AL1_AttachColony(&Middens[_]);
    AL1_ReadHeader(&Middens[_]);
    Nucleii[_]=&Middens[_];
  }
  Queen();
  AL3_Interpret(Nucleii);
}

/*! \brief Enter Amoeba Queen mode. 
 * - The Amoeba Queen is responsible for creating the database in SHM and populating it.
 */
void Queen(){
  AL1_LoadColony(Nucleii[0],FILE1);
  AL1_LoadColony(Nucleii[1],FILE2);
  puts("Done.");
}

/*! \brief Enter Amoeba Queen mode. 
 * - The Amoeba worker connects to a database established by the queen and operates on it.
 */
void Worker(){
  puts("Spawned Amoeba Worker.");
  /* needs to be removed!! */
  AL1_UnlockColony(Nucleii[0]);
  AL1_UnlockColony(Nucleii[1]);
  DEBUG_Display(AL1_GetDBEntry(Nucleii[0],0));
}





/*

          /:`:::.     .        :       ...    .,::::::  :::::::.    :::.          
   ,,    /;` ;;`;;    ;;,.    ;;;   .;;;;;;;. ;;;;''''   ;;;'';;'   ;;`;;    ,,   
 ,[['   n[' ,[[ '[[,  [[[[, ,[[[[, ,[[     \[[,[[cccc    [[[__[[\. ,[[ '[[,  '[[, 
c$     c$" c$$$cc$$$c $$$$$$$$"$$$ $$$,     $$$$$""""    $$""""Y$$c$$$cc$$$c    $c
"8bo, o8"   888   888,888 Y88" 888o"888,_ _,88P888oo,__ _88o,,od8P 888   888,,od8"
  "MPmM"    YMM   ""` MMM  M'  "MMM  "YMMMMMP" """"YUMMM""YUMMMP"  YMM   ""` YM"  

*/
