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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "WMS_API.h"
#include "WMS_Blob.h"
#include "ENiX_STRING.h"
#include "ENiX_LIST.h"
#include "ENiX_WMS.h"
#include "WitchCraft-Net.h"
#include "WitchCraft-Colours.h"

extern struct DataArea *Nucleii[2];

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
  WMS_UnlockDB(Nucleii[0]);
  WMS_UnlockDB(Nucleii[1]);
  WMS_Save_To(Nucleii[0],(char *)FILE1);
  WMS_Save_To(Nucleii[1],(char *)FILE2);
  for(_=0;_<2;_++){
    WMS_SHM_Detach(Nucleii[_]);
    WMS_SHM_Delete(Nucleii[_]);
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
  char *Msg=strdup(" Spawned Queen 0\n");
  for(_=0;_<2;_++){
    Nucleii[_]=WMS_SHM_Create(_);
    if(Nucleii[_]&&Nucleii[_]->Queen){
      Msg[strlen(Msg)-2]=48+_;
      WMS_Log(Msg,0);
    }
    WMS_SHM_Attach(Nucleii[_]);
    if(_==0)
      WMS_Read_From(Nucleii[_],(char *)FILE1,_);
    else
      WMS_Read_From(Nucleii[_],(char *)FILE2,_);
    Nucleii[_]=Nucleii[_];
  }
  if(Nucleii[0]->Queen){
    WMS_Interpret(Nucleii);
  }
  else{
    WMS_Log("Queen already running... aborting",0);
  }
  free(Msg);
}

/*! \brief Enter Amoeba Queen mode. 
 * - The Amoeba worker connects to a database established by the queen and operates on it.
 */
void WMS_Worker(){
  puts("Spawned Amoeba Worker.");
  /* needs to be removed!! */
  WMS_UnlockDB(Nucleii[0]);
  WMS_UnlockDB(Nucleii[1]);
}

/*! \brief Check that the Amoeba daemon is running.
 *
 */
int WMS_CheckService(){
  int Socket=0;
  struct Blob *TX=NULL;
  Socket=OpenClientSocket(SERVERIP);
  if(Socket!=-1){
    TX=WMS_Str2Blob("PNG");
    NetTransmit(TX,Socket);
    NetReceive(Socket);
    close(Socket);
    return 1;
  }
  return 0;
}

/*! \brief Logs messages to stdout.
 *
 */
void WMS_Log(char *Message,int Priority){
  char *Time=Gettime();
  printf("%s:%s",Time,Message);
  free(Time);
}

/*! \brief Request that the server erases the database.
 *
 */
void WMS_Purge(){
  int Socket=0;
  struct Blob *TX=NULL;
  Socket=OpenClientSocket(SERVERIP);
  TX=WMS_Str2Blob("PUR");
  NetTransmit(TX,Socket);
  NetReceive(Socket);
  close(Socket);
}

/*! \brief Request that the server saves the database to disk.
 *
 */
void WMS_SaveFile(char *Filename){
  int Socket=0;
  char *Data=StrCat("SAV ",Filename);
  struct Blob *TX=NULL;
  Socket=OpenClientSocket(SERVERIP);
  TX=WMS_Str2Blob(Data);
  NetTransmit(TX,Socket);
  NetReceive(Socket);
  close(Socket);
}

/*! \brief Request that the server reads the database from disk.
 *
 */
void WMS_ReadFile(char *Filename){
  int Socket=0;
  char *Data=StrCat("LOA ",Filename);
  struct Blob *TX=NULL;
  Socket=OpenClientSocket(SERVERIP);
  TX=WMS_Str2Blob(Data);
  NetTransmit(TX,Socket);
  NetReceive(Socket);
  close(Socket);
}

/*! \brief Internal write to file.
 *
 */
void WMS_WriteFile(struct Blob *Filename){
  char *File1=(char *)malloc((Filename->Size+3)*sizeof(char));
  char *File2=(char *)malloc((Filename->Size+3)*sizeof(char));
  memcpy(File1,Filename->Nucleus+4,Filename->Size-4);
  memcpy(File2,Filename->Nucleus+4,Filename->Size-4);
  strcpy(File1+Filename->Size-5,"1.ENiX");
  strcpy(File2+Filename->Size-5,"2.ENiX"); 
  printf("%s,%s\n",File1,File2);

  WMS_Save_To(Nucleii[0],File1);
  WMS_Save_To(Nucleii[1],File2);
  
  free(File1);
  free(File2);
}

/*! \brief Internal read from file.
 *
 */
void WMS_OpenFile(struct Blob *Filename){
  char *File1=(char *)malloc((Filename->Size+3)*sizeof(char));
  char *File2=(char *)malloc((Filename->Size+3)*sizeof(char));
  memcpy(File1,Filename->Nucleus+4,Filename->Size-4);
  memcpy(File2,Filename->Nucleus+4,Filename->Size-4);
  strcpy(File1+Filename->Size-5,"1.ENiX");
  strcpy(File2+Filename->Size-5,"2.ENiX"); 
  printf(" %s %s\n",File1,File2);

  Nucleii[0]=WMS_Read_From(Nucleii[0],File1,0);
  Nucleii[1]=WMS_Read_From(Nucleii[1],File2,1);

  free(File1);
  free(File2);    
}

/*! \brief Runs an Amoeba Server and processes incoming connections.
 *
 */
void WMS_Interpret(struct DataArea *Nucleii[2]){
  int ClientSocket=0,Socket=OpenServerSocket(),_=0,RecordNum=0;
  char *TMP=NULL,*TMP2=NULL;
  struct Blob *RX=NULL,*ACK=WMS_Str2Blob("ACK");
  struct Datablock *R=NULL,*CurrentRec=NULL;
  if(Socket==-1)
    return ;
  /* Connect to the database */
  /* Wait for connections and process them */
  printf("%s: Listening for connections on port: %d...\n",TMP=Gettime(),ENiXPort);
  free(TMP);
  while(1){
    printf("%s: -- Database population: %ld,%ld\n",TMP=Gettime(),Nucleii[0]->Population,Nucleii[1]->Population);
    free(TMP);
    printf("%s: -- Last position: %ld,%ld\n",TMP=Gettime(),Nucleii[0]->LastPos,Nucleii[1]->LastPos);
    free(TMP);
    ClientSocket=AcceptServerCon(Socket);
    puts("");
    RX=NetReceive(ClientSocket);
    if(!strncmp(RX->Nucleus,"ADD",RX->Size)){
      WMS_Log(" -- Client adds records\n",3);
      for(_=0;_<2;_++){
	WMS_SHM_Clear(Nucleii);
	printf("%s: -> S%d Getting data\n",TMP=Gettime(),_);
	free(TMP);
	R=WMS_ReceiveAll(ClientSocket);	
	printf("%s: -- S%d Putting data\n",TMP=Gettime(),_);
	free(TMP);
	WMS_LockDB(Nucleii[_]);
	for(CurrentRec=R;CurrentRec;CurrentRec=CurrentRec->Next){
	  WMS_AddBinary(Nucleii[_],WMS_WMS2Blob(R));
	}
	WMS_UnlockDB(Nucleii[_]);
	FreeWMS(R);
      }
      WMS_Log(" -- Done\n",3);
    }
    if(!strncmp(RX->Nucleus,"GET",RX->Size)){
      WMS_Log(" -- Client gets records\n",3);
      for(_=0;_<2;_++){
	printf("%s: -- S%d Getting data\n",TMP=Gettime(),_);
	free(TMP);
	WMS_LockDB(Nucleii[_]);
	for(RecordNum=0;RecordNum<Nucleii[_]->Population;RecordNum++){
	  CurrentRec=WMS_Blob2WMS(WMS_GetBinary(Nucleii[_],RecordNum));
	  CurrentRec->Next=R;
	  R=CurrentRec;
	}
	WMS_UnlockDB(Nucleii[_]);
	ServerID=_;
	printf("%s: <- S%d Sending data\n",TMP=Gettime(),_);
	free(TMP);
	WMS_TransmitAll(R,ClientSocket);
	FreeWMS(R);
      }
      WMS_Log(" -- Done\n",3);
    } 
    if(!strncmp(RX->Nucleus,"RRD",RX->Size)){
      WMS_Log(" -- Contents refresh\n",3);
      /*
      WMS_ContentsReg(Nucleii);
      */
      NetTransmit(ACK,ClientSocket);
      WMS_Log(" -- Done\n",3);
    }
    if(!strncmp(RX->Nucleus,"DSP",RX->Size)){
      WMS_Log(" -- Contents display\n",3);
      /*
      DEBUG_Header(Nucleii[0]);
      DEBUG_Header(Nucleii[1]);
      */

      WMS_ALLOC_DEBUG(Nucleii[0]);
      WMS_ALLOC_DEBUG(Nucleii[1]);

      NetTransmit(ACK,ClientSocket);
      WMS_Log(" -- Done\n",3);
    }
    if(!strncmp(RX->Nucleus,"PUR",RX->Size)){
      WMS_Log(" -- Purge database\n",3);
      WMS_SHM_Clear(Nucleii);
      NetTransmit(ACK,ClientSocket);
      WMS_Log(" -- Done\n",3);
    }
    if((!strncmp(RX->Nucleus,"SAV",3))&&(RX->Size>4)){
      WMS_Log(" -- Save database as ",3);
      WMS_WriteFile(RX);
      NetTransmit(ACK,ClientSocket);
      WMS_Log(" -- Done\n",3);
    }
    if((!strncmp(RX->Nucleus,"LOA",3))&&(RX->Size>4)){
      WMS_Log(" -- Load database from ",3);
      WMS_OpenFile(RX);      
      NetTransmit(ACK,ClientSocket);
      WMS_Log(" -- Done\n",3);
    }
    if(!strncmp(RX->Nucleus,"PNG",RX->Size)){
      WMS_Log(" -- Ping received.\n",3);
      NetTransmit(ACK,ClientSocket);
      WMS_Log(" -- Done\n",3);
    }
    close(ClientSocket);
  }
  WMS_FreeBlob(ACK);
}

/*! \brief Force a contents refresh on the server.
 *
 */
void WMS_Reread(){
  int Socket=0;
  struct Blob *TX=NULL;
  Socket=OpenClientSocket(SERVERIP);
  TX=WMS_Str2Blob("RRD");
  NetTransmit(TX,Socket);
  NetReceive(Socket);
  close(Socket);
}

/*! \brief Force a contents display on the server.
 *
 */
void WMS_SrvDisplay(){
  int Socket=0;
  struct Blob *TX=NULL;
  Socket=OpenClientSocket(SERVERIP);
  TX=WMS_Str2Blob("DSP");
  NetTransmit(TX,Socket);
  NetReceive(Socket);
  close(Socket);
}

/*! \brief Get records from the server.
 *
 */
struct DB_Wrap *WMS_Acquire(struct Datablock *DB[2]){
  int Socket=0;
  struct Blob *TX=NULL;
  struct DB_Wrap *R=(struct DB_Wrap *)malloc(sizeof(struct DB_Wrap));
  Socket=OpenClientSocket(SERVERIP);
  TX=WMS_Str2Blob("GET");
  NetTransmit(TX,Socket);
  DB[0]=WMS_ReceiveAll(Socket);
  DB[1]=WMS_ReceiveAll(Socket);
  close(Socket);
  R->DB[0]=DB[0];
  R->DB[1]=DB[1];
  return R;
}

/*! \brief Send records to the server.
 *
 */
void WMS_Store(struct Datablock *DB[2]){
  int Socket=0;
  struct Blob *TX=NULL;
  Socket=OpenClientSocket(SERVERIP);
  TX=WMS_Str2Blob("ADD");
  NetTransmit(TX,Socket);
  ServerID=0;
  WMS_TransmitAll(DB[0],Socket);
  ServerID=1;
  WMS_TransmitAll(DB[1],Socket);
  close(Socket);
}

/*! \brief Send records to the server.
 * - like WMS_Store() but lower level.
 */
void WMS_TransmitAll(struct Datablock *R,int Socket){
  unsigned long int Size=0;
  struct Datablock *Buffer=NULL,*DB=NULL;
  struct Blob *RX=NULL,*TX=NULL;
  char *String=(char *)malloc(30*sizeof(char));
  DB=R;
  Size=0;
  for(Buffer=DB;Buffer;Buffer=Buffer->Next)
    Size++;
  sprintf(String,"SIZE:%ld",Size);

  //  printf("DEBUG WMS_TransmitAll %s\n",String);

  //  TX=CreateBlob(strlen(String),String);
  TX=WMS_Str2Blob(String);
  NetTransmit(TX,Socket);
  RX=NetReceive(Socket);
  WMS_FreeBlob(RX);
  for(Buffer=DB;Buffer;Buffer=Buffer->Next){
    TX=WMS_WMS2Blob(Buffer);
    NetTransmit(TX,Socket);
    WMS_FreeBlob(TX);
  }
  free(String);
}

/*! \brief Receive records from the server.
 * - like WMS_Acquire() but lower level.
 */
struct Datablock  *WMS_ReceiveAll(int Socket){
  struct Datablock *R=NULL,*DB=NULL;
  unsigned long int _=0,Size=0;
  struct Blob *RX=NULL,*ACK=WMS_Str2Blob("ACK");
  RX=NetReceive(Socket);
  Size=atol(RX->Nucleus+5);
  NetTransmit(ACK,Socket);
  WMS_FreeBlob(RX);
  for(_=0;_<Size;_++){
    DB=WMS_Blob2WMS(RX=NetReceive(Socket));
    DB->Next=R;
    R=DB;
    WMS_FreeBlob(RX);
  }
  return R;
}

/*

          /:`:::.     .        :       ...    .,::::::  :::::::.    :::.          
   ,,    /;` ;;`;;    ;;,.    ;;;   .;;;;;;;. ;;;;''''   ;;;'';;'   ;;`;;    ,,   
 ,[['   n[' ,[[ '[[,  [[[[, ,[[[[, ,[[     \[[,[[cccc    [[[__[[\. ,[[ '[[,  '[[, 
c$     c$" c$$$cc$$$c $$$$$$$$"$$$ $$$,     $$$$$""""    $$""""Y$$c$$$cc$$$c    $c
"8bo, o8"   888   888,888 Y88" 888o"888,_ _,88P888oo,__ _88o,,od8P 888   888,,od8"
  "MPmM"    YMM   ""` MMM  M'  "MMM  "YMMMMMP" """"YUMMM""YUMMMP"  YMM   ""` YM"  

*/
