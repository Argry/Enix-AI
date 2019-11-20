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


#include "Amoeba-Test.h"

extern struct Colony *Nucleii[2];
extern struct Colony Middens[2];

void Amoeba_Con(){
  int _=0;
  for(_=0;_<2;_++){
    AL1_CreateColony(&Middens[_]);
    AL1_AttachColony(&Middens[_]);
    AL1_ReadHeader(&Middens[_]);
  }
}

void Amoeba_Dis(){
  int _=0;
  for(_=0;_<2;_++)
    AL1_DetachColony(&Middens[_]);
}

void CLI(){
  int SeqN=0;
  char In=0,Seq[76];
  memset(Seq,0,76);
  Amoeba_Con();
  AL1_ReadHeader(Nucleii[0]);
  AL1_ReadHeader(Nucleii[1]);
  while(In!='q'){
    if((In!='\r')&&(In!='\n')){
      puts("\033[0;10m-[\033[0;31mProvisioning\033[0;10m]-----------------");
      puts(" [+] = add a new record");
      puts(" [W] = partfill with WMS records");
      puts(" [p] = partfill with Amoeba recs");
      puts(" [f] = fill database");
      puts("-[\033[0;32mInspection\033[0;10m]-------------------");
      puts(" [a] = debug all (Amoeba)");
      puts(" [w] = debug all (WMS)");
      puts(" [h] = debug header here");
      puts(" [H] = debug header within the daemon");
      puts(" [S] = summary of colony data");
      puts("-[\033[0;35mTest Runs\033[0;10m]--------------------");
      puts(" [d] = delete + add run");
      puts(" [m] = modify");
      puts(" [s] = search run");
      puts(" [C] = custom coded function");
      puts("-[\033[0;37mNetwork\033[0;10m]----------------------");
      puts(" [T] = transmission client (requires server)");
      puts(" [R] = receiver server (requires client)");
      puts(" [1] = TX Operation Client");
      puts(" [2] = Search Client");
      puts(" [3] = Provisioning Client");
      puts(" [4] = Force server to regenerate :CONTENTS:");
      puts("-[\033[0;36mDedicated\033[0;10m]--------------------");
      puts(" [c] = cache records");
      puts(" [D] = defrag alloc table");
      puts(" [F] = defrag all");
      puts(" [q] = exit");
      printf("Test Code Seq: [%s]\n",Seq);
      AL1_LockColony(Nucleii[0]);
      AL1_ReadHeader(Nucleii[0]);
      AL1_UnlockColony(Nucleii[0]);
      AL1_LockColony(Nucleii[1]);
      AL1_ReadHeader(Nucleii[1]);
      AL1_UnlockColony(Nucleii[1]);
      printf("Database population: %ld,%ld\n",Nucleii[0]->Population,Nucleii[1]->Population);
      printf("Lost Position: %ld,%ld\n",Nucleii[0]->LastPos,Nucleii[1]->LastPos);
      printf(">");
    }  
    In=getchar();    
    if((In!='\r')&&(In!='\n')){
      SeqN=SeqN%40;
      if(!SeqN)
	Seq[40]=0;
      Seq[SeqN]=In;
      SeqN++;
      Seq[SeqN]='.';
    }
    if(In=='0')
      DEBUG_RXServer();
    if(In=='1')
      DEBUG_TXServer();
    if(In=='2')
      DEBUG_RXClient();
    if(In=='3')
      DEBUG_TXClient();
    if(In=='4')
      DEBUG_ClientRefresh();
    if(In=='H')
      AL3_SrvDisplay();
    if(In=='T')
      DEBUG_NetClient(1000);
    if(In=='C')
      DEBUG_Function();
    if(In=='R')
      DEBUG_NetServer();
    if(In=='+')
      DEBUG_AddRec(Nucleii[0]);
    if(In=='p')
      DEBUG_PartFill(Nucleii[0],1000);
    if(In=='c')
      AL2_CacheFreqUsed(Nucleii[0]);
    if(In=='f')
      DEBUG_Fill(Nucleii[0]);
    if(In=='F')
      DEBUG_DefragAll(Nucleii[0]);
    if(In=='h'){
      DEBUG_Header(Nucleii[0]);
      DEBUG_Header(Nucleii[1]);
    }
    if(In=='m')
      DEBUG_Modify(Nucleii[0],"Wibble-000900",100000);
    if(In=='d')
      DEBUG_Delete(Nucleii[0],"Wibble-000900",100000);
    if(In=='D')
      DEBUG_DAT(Nucleii[0]);
    if(In=='a'){
      puts("#[ COLONY 1 ]###################################################################\n");
      DEBUG_ALL(Nucleii[0]);
      puts("#[ COLONY 2 ]###################################################################\n");
      DEBUG_ALL(Nucleii[1]);
    }
    if(In=='W'){
      DEBUG_WMS2ABA(Nucleii[0],1000);
    }
    if(In=='w'){
      puts("-[SECTOR 1]--------------------");
      DEBUG_ShowWMS(Nucleii[0]);
      puts("-[SECTOR 2]--------------------");
      DEBUG_ShowWMS(Nucleii[1]);
    }
    if(In=='s')
      DEBUG_Search(Nucleii[0],"Wibble-000900",100000);
    if(In=='S'){
      DEBUG_ColonySummary(Nucleii[0]);
      DEBUG_ColonySummary(Nucleii[1]);
    }
  }
  Amoeba_Dis();
}

int main(){  
  ServerID=0;
  CLI();
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
