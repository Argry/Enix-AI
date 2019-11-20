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
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <signal.h>
#include <unistd.h>
#include "Amoeba-Colony.h"
#include "Amoeba-Cytoskelenton.h"
#include "Amoeba-Nucleus.h"


#include "Amoeba-Daemon.h"
#include "WitchCraft-Colours.h"

extern struct Colony *Nucleii[2];
extern struct Colony Middens[2];
extern const char *FILE1;
extern const char *FILE2;

/*! \brief Amoeba daemon splash screen.
 *
 */
void Amoeba_ASCII_Art(){
  puts(WC_GREEN);
  puts("          @@@@@@   @@@@@@@@@@    @@@@@@   @@@@@@@@  @@@@@@@    @@@@@@   ");
  puts("         @@@@@@@@  @@@@@@@@@@@  @@@@@@@@  @@@@@@@@  @@@@@@@@  @@@@@@@@  ");
  puts("         @@!  @@@  @@! @@! @@!  @@!  @@@  @@!       @@!  @@@  @@!  @@@  ");
  puts("         !@!  @!@  !@! !@! !@!  !@!  @!@  !@!       !@   @!@  !@!  @!@  ");
  puts("         @!@!@!@!  @!! !!@ @!@  @!@  !@!  @!!!:!    @!@!@!@   @!@!@!@!  ");
  puts("         !!!@!!!!  !@!   ! !@!  !@!  !!!  !!!!!:    !!!@!!!!  !!!@!!!!  ");
  puts("         !!:  !!!  !!:     !!:  !!:  !!!  !!:       !!:  !!!  !!:  !!!  ");
  puts("         :!:  !:!  :!:     :!:  :!:  !:!  :!:       :!:  !:!  :!:  !:!  ");
  puts("         ::   :::  :::     ::   ::::: ::   :: ::::   :: ::::  ::   :::  ");
  puts("         :   : :   :      :     : :  :   : :: ::   :: : ::    :   : :  ");
  puts(WC_RESET);
}

/*! \brief Trap signals and close down the Amoeba database safely.
 *
 */
void SignalHandler(int Sig){
  int _=0;
  printf("\r  \n");
  printf("Exiting, please wait... ");
  fflush(stdout);
  AL1_UnlockColony(Nucleii[0]);
  AL1_UnlockColony(Nucleii[1]);
  AL1_SaveColony(Nucleii[0],FILE1);
  AL1_SaveColony(Nucleii[1],FILE2);
  for(_=0;_<2;_++){
    AL1_DetachColony(&Middens[_]);
    AL1_DeleteColony(&Middens[_]);
  }
  puts("Saved :)");
  exit(0);
}

/*! \brief Main handler for the Amoeba daemon.
 *
 */
int main(){
  Amoeba_ASCII_Art();
  printf("Amoeba Queen (Daemon Amoeba) Initialising... ");
  fflush(stdout);
  signal(SIGINT,SignalHandler);
  signal(SIGTERM,SignalHandler);
  MainHandler();
  SignalHandler(0);
  puts("Amoeba Queen (Daemon Amoeba) Terminated Correctly.");
  return 0;
}

/*

          /:`:::.     .        :       ...    .,::::::  :::::::.    :::.          
   ,,    /;` ;;`;;    ;;,.    ;;;   .;;;;;;;. ;;;;''''   ;;;'';;'   ;;`;;    ,,   
 ,[['   n[' ,[[ '[[,  [[[[, ,[[[[, ,[[     \[[,[[cccc    [[[__[[\. ,[[ '[[,  '[[, 
c$     c$" c$$$cc$$$c $$$$$$$$"$$$ $$$,     $$$$$""""    $$""""Y$$c$$$cc$$$c    $c
"8bo, o8"   888   888,888 Y88" 888o"888,_ _,88P888oo,__ _88o,,od8P 888   888,,od8"
  "MPmM"    YMM   ""` MMM  M'  "MMM  "YMMMMMP" """"YUMMM""YUMMMP"  YMM   ""` YM"  

*/
