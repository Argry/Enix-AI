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
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "WMS_API.h"
#include "WMS_Blob.h"
#include "ENiX_STRING.h"
#include "ENiX_LIST.h"
#include "ENiX_WMS.h"
#include "ENiX_Globals.h"
#include "WitchCraft-Net.h"
#include "WitchCraft-Colours.h"

/* extern struct DataArea *Nucleii[2]; */

const char *FILE1="./Data1.ENiX";
const char *FILE2="./Data2.ENiX";

/*! \brief Amoeba daemon splash screen.
 *
 */
void WMS_ASCII_Art(){
  puts(WC_GREEN);
  puts("                      @@@  @@@  @@@  @@@@@@@@@@    @@@@@@   ");
  puts("                      @@@  @@@  @@@  @@@@@@@@@@@  @@@@@@@   ");
  puts("                      @@!  @@!  @@!  @@! @@! @@!  !@@       ");
  puts("                      !@!  !@!  !@!  !@! !@! !@!  !@!       ");
  puts("                      @!!  !!@  @!@  @!! !!@ @!@  !!@@!!    ");
  puts("                      !@!  !!!  !@!  !@!   ! !@!   !!@!!!   ");
  puts("                      !!:  !!:  !!:  !!:     !!:       !:!  ");
  puts("                      :!:  :!:  :!:  :!:     :!:      !:!   ");
  puts("                       :::: :: :::   :::     ::   :::: ::   ");
  puts("                        :: :  : :     :      :    :: : :    ");
  puts(WC_RESET);
}

/*! \brief Trap signals and close down the Amoeba database safely.
 *
 */
void WMS_SignalHandler(int Sig){
  int _=0;
  printf("\r  \n");
  printf("Exiting, please wait... ");
  fflush(stdout);
  WMS_UnlockDB(DA[0]);
  WMS_UnlockDB(DA[1]);
  /* Re-enable me!! */

  WMS_Save_To(DA[0],(char *)FILE1);
  WMS_Save_To(DA[1],(char *)FILE2);

  for(_=0;_<2;_++){
    WMS_SHM_Detach(DA[_]);
    WMS_SHM_Delete(DA[_]);
  }
  puts("Saved :)");
  exit(0);
}

/*! \brief Main handler for the Amoeba daemon.
 *
 */
int Daemon(){
  WMS_ASCII_Art();
  puts("WMS Queen (Daemon WMS) Initialising... ");
  fflush(stdout);
  signal(SIGINT,WMS_SignalHandler);
  signal(SIGTERM,WMS_SignalHandler);
  WMS_MainHandler();
  WMS_SignalHandler(0);
  puts("Amoeba Queen (Daemon Amoeba) Terminated Correctly.");
  return 0;
}


/*! \brief Queens the Amoeba database so that workers may attach.
 * - The Amoeba Queen is responsible for creating the database in SHM and populating it.
 */
void WMS_MainHandler(){
  int _=0;
  char Msg[20];
  for(_=0;_<2;_++){
    DA[_]=WMS_SHM_Create(_);
    if(DA[_]&&DA[_]->Queen){
      sprintf(Msg," Spawned Queen %d\n",_);
      WMS_Log(Msg,0);
    }
    WMS_SHM_Attach(DA[_]);
    if(_==0)
      WMS_Read_From(DA[_],(char *)FILE1,_);
    else
      WMS_Read_From(DA[_],(char *)FILE2,_);
  }
  if(DA[0]->Queen){
    WMS_Interpret(DA);
  }
  else{
    WMS_Log("Queen already running... aborting",0);
  }
}

/*! \brief Enter Amoeba Queen mode. 
 * - The Amoeba worker connects to a database established by the queen and operates on it.
 */
void WMS_Worker(){
  puts("Spawned Amoeba Worker.");
  /* needs to be removed!! */
  WMS_UnlockDB(DA[0]);
  WMS_UnlockDB(DA[1]);
}

/*! \brief Check that the Amoeba daemon is running.
 *
 */
int WMS_CheckService(){
  int Socket=0;
  if(ENABLEMIDDEN){
    Socket=OpenClientSocket(SERVERIP);
    if(Socket!=-1){
      SendCmd("PNG",Socket);
      RecvAck(Socket);
      close(Socket);
      return 1;
    }
  }
  return 0;
}

/*! \brief Logs messages to stdout.
 *
 */
void WMS_Log(char *Message,...){
  char *Time=Gettime();
  va_list args;
  va_start(args,Message);
  printf("%s:",Time);
  free(Time);
  vprintf(Message, args);
  fflush(stdout);
  va_end(args);
}

/*! \brief Request that the server erases the database.
 *
 */
void WMS_Purge(){
  int Socket=0;
  Socket=OpenClientSocket(SERVERIP);
  SendCmd("PUR",Socket);
  RecvAck(Socket);
  close(Socket);
}

/*! \brief Request that the server saves the database to disk.
 *
 */
void WMS_SaveFile(char *Filename){
  int Socket=0;
  char *Data=StrCat("SAV ",Filename);
  Socket=OpenClientSocket(SERVERIP);
  SendCmd(Data,Socket);
  RecvAck(Socket);
  close(Socket);
}

/*! \brief Request that the server reads the database from disk.
 *
 */
void WMS_ReadFile(char *Filename){
  int Socket=0;
  char *Data=StrCat("LOA ",Filename);
  Socket=OpenClientSocket(SERVERIP);
  SendCmd(Data,Socket);
  RecvAck(Socket);
  close(Socket);
}

/*! \brief Internal write to file.
 *
 */
void WMS_WriteFile(char *Filename){
  int FileNameLen=strlen(Filename);
  char *File1=(char *)malloc((FileNameLen+3)*sizeof(char));
  char *File2=(char *)malloc((FileNameLen+3)*sizeof(char));
  memcpy(File1,Filename+4,FileNameLen-4);
  memcpy(File2,Filename+4,FileNameLen-4);
  strcpy(File1+FileNameLen-4,"1.ENiX");
  strcpy(File2+FileNameLen-4,"2.ENiX"); 
  printf("%s,%s\n",File1,File2);

  WMS_Save_To(DA[0],File1);
  WMS_Save_To(DA[1],File2);
  
  free(File1);
  free(File2);
}

/*! \brief Internal read from file.
 *
 */
void WMS_OpenFile(char *Filename){
  int FileNameLen=strlen(Filename);
  char *File1=(char *)malloc((FileNameLen+3)*sizeof(char));
  char *File2=(char *)malloc((FileNameLen+3)*sizeof(char));
  memcpy(File1,Filename+4,FileNameLen-4);
  memcpy(File2,Filename+4,FileNameLen-4);
  strcpy(File1+FileNameLen-4,"1.ENiX");
  strcpy(File2+FileNameLen-4,"2.ENiX"); 
  printf(" %s %s\n",File1,File2);

  DA[0]=WMS_Read_From(DA[0],File1,0);
  DA[1]=WMS_Read_From(DA[1],File2,1);

  free(File1);
  free(File2);    
}

/*! \brief Runs an Amoeba Server and processes incoming connections.
 *
 */
void WMS_Interpret(struct DataArea *DA[2]){
  int ClientSocket=0,Socket=OpenServerSocket(),_=0;
  char *Command=NULL;

  if(Socket==-1)
    return ;
  /* Connect to the database */
  /* Wait for connections and process them */
  WMS_Log(" Listening for connections on port: %d...\n",ENiXPort);
  while(1){


    ClientSocket=AcceptServerCon(Socket);
    
    Command=RecvCmd(ClientSocket);

    if(!strncmp(Command,"ADD",3)){
      WMS_Log(" -> Client adds records\r",3);
      for(_=0;_<2;_++){
	WMS_SHM_Clear(DA[_]);
	WMS_ServGetDB(DA[_],ClientSocket);
      }
    }
    if(!strncmp(Command,"GET",3)){
      WMS_Log(" <- Client gets records\r",3);
      for(_=0;_<2;_++){
	WMS_ServSendDB(DA[_],ClientSocket);
      }
    } 
    if(!strncmp(Command,"RRD",3)){
      WMS_Log(" -- Contents refresh\n",3);
      SendAck(ClientSocket);
    }
    if(!strncmp(Command,"DSP",3)){
      WMS_Log(" -- Contents display\n",3);
      WMS_ALLOC_DEBUG(DA[0]);
      WMS_ALLOC_DEBUG(DA[1]);
      SendAck(ClientSocket);
    }
    if(!strncmp(Command,"PUR",3)){
      WMS_Log(" -- Purge database       \n",3);
      WMS_SHM_Clear(DA[0]);
      WMS_SHM_Clear(DA[1]);
      SendAck(ClientSocket);
    }
    if((!strncmp(Command,"SAV",3))&&(strlen(Command)>4)){
      WMS_Log(" -- Save database as ",3);
      WMS_WriteFile(Command);
      WMS_Log(" -- [S0:%ld,%ld][S1:%ld,%ld]\n",DA[0]->Population,DA[0]->LastPos,DA[1]->Population,DA[1]->LastPos);
      SendAck(ClientSocket);
    }
    if((!strncmp(Command,"LOA",3))&&(strlen(Command)>4)){
      WMS_Log(" -- Load database from ",3);
      WMS_OpenFile(Command);
      WMS_Log(" -- [S0:%ld,%ld][S1:%ld,%ld]\n",DA[0]->Population,DA[0]->LastPos,DA[1]->Population,DA[1]->LastPos);
      SendAck(ClientSocket);
    }
    if(!strncmp(Command,"PNG",3)){
      SendAck(ClientSocket);
    }
    close(ClientSocket);
    free(Command);
  }
}

/*! \brief Force a contents refresh on the server.
 *
 */
void WMS_Reread(){
  int Socket=0;
  Socket=OpenClientSocket(SERVERIP);
  SendCmd("RRD",Socket);
  RecvAck(Socket);
  close(Socket);
}

/*! \brief Force a contents display on the server.
 *
 */
void WMS_SrvDisplay(){
  int Socket=0;
  Socket=OpenClientSocket(SERVERIP);
  SendCmd("DSP",Socket);
  RecvAck(Socket);
  close(Socket);
}

/*! \brief Get records from the server.
 *
 */
struct DB_Wrap *WMS_Acquire(struct Datablock *DB[2]){
  int Socket=0;
  struct DB_Wrap *R=(struct DB_Wrap *)malloc(sizeof(struct DB_Wrap));
  Socket=OpenClientSocket(SERVERIP);
  SendCmd("GET",Socket);
  R->DB[0]=WMS_ReceiveAll(Socket);
  R->DB[1]=WMS_ReceiveAll(Socket);
  close(Socket);
  return R;
}

/*! \brief Send records to the server.
 *
 */
void WMS_Store(struct Datablock *DB[2]){
  int Socket=0;
  Socket=OpenClientSocket(SERVERIP);
  SendCmd("ADD",Socket);
  WMS_TransmitAll(DB[0],Socket);
  WMS_TransmitAll(DB[1],Socket);
  close(Socket);
}

/*! \brief Client sends records to the server.
 * - like WMS_Store() but lower level.
 */
void WMS_TransmitAll(struct Datablock *R,int Socket){
  unsigned long int Size=0;
  struct Datablock *Buffer=NULL,*DB=NULL;
  struct Blob *TX=NULL;
  char *String=(char *)malloc(30*sizeof(char));
  DB=R;
  for(Buffer=DB;Buffer;Buffer=Buffer->Next)
    Size++;
  sprintf(String,"SIZE:%ld",Size);
  TX=WMS_Str2Blob(String);
  NetTransmit(TX,Socket);
  RecvAck(Socket);
  for(Buffer=DB;Buffer;Buffer=Buffer->Next){
    TX=WMS_WMS2Blob(Buffer);
    NetTransmit(TX,Socket);
    WMS_FreeBlob(TX);
  }
  free(String);
}

/*! \brief Gets records and puts them directly into SHM from blob format.
 * 
 */
void WMS_ServGetDB(struct DataArea *DB,int Socket){
  /* contains memory leaks */  
  unsigned long int _=0,Size=0;
  char *String=NULL;
  struct Blob *RX=NULL;
  RX=NetReceive(Socket);  
  String=(char *)malloc((RX->Size+1)*sizeof(char));
  memcpy(String,RX->Nucleus,RX->Size);
  String[RX->Size]=0;
  Size=atol(String+5);
  WMS_FreeBlob(RX);
  SendAck(Socket);
  for(_=0;_<Size;_++){
    RX=NetReceive(Socket);
    WMS_AddBinary(DB,RX);
    WMS_FreeBlob(RX);
  }
}


/*! \brief Client receives records from the server.
 * - like WMS_Acquire() but lower level.
 */
struct Datablock *WMS_ReceiveAll(int Socket){
  struct Datablock *R=NULL,*DB=NULL;
  unsigned long int _=0,Size=0;
  char *String=NULL;
  struct Blob *RX=NULL;
  RX=NetReceive(Socket);
  String=(char *)malloc((RX->Size+1)*sizeof(char));
  memcpy(String,RX->Nucleus,RX->Size);
  String[RX->Size]=0;
  Size=atol(String+5);
  SendAck(Socket);
  for(_=0;_<Size;_++){
    RX=NetReceive(Socket);
    DB=WMS_Blob2WMS(RX);
    if(!DB){
      printf("Cannot find record %ld of %ld\n",_,Size);
      exit(0);
    }
    else{
      DB->Next=R;
      R=DB;
    }
  }
  return R;
}

/*! \brief Send records direct from the server in blob format.
 * 
 */
void WMS_ServSendDB(struct DataArea *DB,int Socket){
  /* contains memory leaks */
  unsigned long int _=0;
  struct Blob *TX=NULL;
  char *String=(char *)malloc(30*sizeof(char));
  sprintf(String,"SIZE:%ld",DB->Population);
  TX=WMS_Str2Blob(String);
  NetTransmit(TX,Socket);
  WMS_FreeBlob(TX);
  free(String);
  RecvAck(Socket);
  for(_=0;_<DB->Population;_++){
    TX=WMS_GetBinary(DB,_);
    NetTransmit(TX,Socket);
    WMS_FreeBlob(TX);
  }
}



/*

          /:`:::.     .        :       ...    .,::::::  :::::::.    :::.          
   ,,    /;` ;;`;;    ;;,.    ;;;   .;;;;;;;. ;;;;''''   ;;;'';;'   ;;`;;    ,,   
 ,[['   n[' ,[[ '[[,  [[[[, ,[[[[, ,[[     \[[,[[cccc    [[[__[[\. ,[[ '[[,  '[[, 
c$     c$" c$$$cc$$$c $$$$$$$$"$$$ $$$,     $$$$$""""    $$""""Y$$c$$$cc$$$c    $c
"8bo, o8"   888   888,888 Y88" 888o"888,_ _,88P888oo,__ _88o,,od8P 888   888,,od8"
  "MPmM"    YMM   ""` MMM  M'  "MMM  "YMMMMMP" """"YUMMM""YUMMMP"  YMM   ""` YM"  

*/
