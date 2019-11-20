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


#include "Amoeba-Nucleus.h"

extern struct Colony *Nucleii[2];
struct Colony Middens[2];
extern int AllowQueen;

int FIRSTRUN=1;
const char *FILE1="./Data1.ENiX";
const char *FILE2="./Data2.ENiX";

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

void Queen(){
  AL1_LoadColony(Nucleii[0],FILE1);
  AL1_LoadColony(Nucleii[1],FILE2);
  puts("Done.");
}

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
