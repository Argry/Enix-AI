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
#include "ENiX_STRING.h"
#include "ENiX_LIST.h"
#include "ENiX_WMS.h"
#include "Amoeba-Colony.h"
#include "Amoeba-Microtubules.h"
#include "WitchCraft-Net.h"
#include "Amoeba-Unit-Tests.h"
#include "Amoeba-API.h"

extern struct Colony *Nucleii[2];

/*! \brief Check that the Amoeba daemon is running.
 *
 */
int AL3_CheckService(){
  int Socket=0;
  struct Blob *TX=NULL;
  Socket=OpenClientSocket(SERVERIP);
  if(Socket!=-1){
    TX=CreateBlob(3,"PNG");
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
void AL3_Log(char *Message,int Priority){
  char *Time=Gettime();
  printf("%s:%s",Time,Message);
  free(Time);
}

/*! \brief Request that the server erases the database.
 *
 */
void AL3_Purge(){
  int Socket=0;
  struct Blob *TX=NULL;
  Socket=OpenClientSocket(SERVERIP);
  TX=CreateBlob(3,"PUR");
  NetTransmit(TX,Socket);
  NetReceive(Socket);
  close(Socket);
}

/*! \brief Request that the server saves the database to disk.
 *
 */
void AL3_SaveFile(char *Filename){
  int Socket=0;
  char *Data=StrCat("SAV ",Filename);
  struct Blob *TX=NULL;
  Socket=OpenClientSocket(SERVERIP);
  TX=CreateBlob(strlen(Data),Data);
  NetTransmit(TX,Socket);
  NetReceive(Socket);
  close(Socket);
}

/*! \brief Request that the server reads the database from disk.
 *
 */
void AL3_ReadFile(char *Filename){
  int Socket=0;
  char *Data=StrCat("LOA ",Filename);
  struct Blob *TX=NULL;
  Socket=OpenClientSocket(SERVERIP);
  TX=CreateBlob(strlen(Data),Data);
  NetTransmit(TX,Socket);
  NetReceive(Socket);
  close(Socket);
}

/*! \brief Internal write to file.
 *
 */
void AL3_WriteFile(struct Blob *Filename){
  char *File1=(char *)malloc((Filename->Size+3)*sizeof(char));
  char *File2=(char *)malloc((Filename->Size+3)*sizeof(char));
  memcpy(File1,Filename->Nucleus+4,Filename->Size-4);
  memcpy(File2,Filename->Nucleus+4,Filename->Size-4);
  strcpy(File1+Filename->Size-5,"1.ENiX");
  strcpy(File2+Filename->Size-5,"2.ENiX"); 
  printf("%s,%s\n",File1,File2);
  AL1_SaveColony(Nucleii[0],File1);
  AL1_SaveColony(Nucleii[1],File2);
  free(File1);
  free(File2);
}

/*! \brief Internal read from file.
 *
 */
void AL3_OpenFile(struct Blob *Filename){
  char *File1=(char *)malloc((Filename->Size+3)*sizeof(char));
  char *File2=(char *)malloc((Filename->Size+3)*sizeof(char));
  memcpy(File1,Filename->Nucleus+4,Filename->Size-4);
  memcpy(File2,Filename->Nucleus+4,Filename->Size-4);
  strcpy(File1+Filename->Size-5,"1.ENiX");
  strcpy(File2+Filename->Size-5,"2.ENiX"); 
  printf(" %s %s\n",File1,File2);
  AL1_LoadColony(Nucleii[0],File1);
  AL1_LoadColony(Nucleii[1],File2);
  free(File1);
  free(File2);    
}

/*! \brief Runs an Amoeba Server and processes incoming connections.
 *
 */
void AL3_Interpret(struct Colony *Nucleii[2]){
  int ClientSocket=0,Socket=OpenServerSocket(),_=0;
  char *TMP=NULL;
  struct Blob *RX=NULL,*ACK=CreateBlob(strlen("ACK"),StrCat("","ACK"));
  struct Datablock *R=NULL;
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
      AL3_Log(" -- Client adds records\n",3);
      for(_=0;_<2;_++){

	AL1_InitHeader(Nucleii[_]);
	AL1_ReadHeader(Nucleii[_]);

	AL1_ReadHeader(Nucleii[_]);	
	printf("%s: -> S%d Getting data\n",TMP=Gettime(),_);
	free(TMP);
	R=AL3_ReceiveAll(ClientSocket);	
	printf("%s: -- S%d Putting data\n",TMP=Gettime(),_);
	free(TMP);
	AL1_LockColony(Nucleii[_]);
	AL2_PutDBContents(R,Nucleii[_]);
	AL1_UnlockColony(Nucleii[_]);
	FreeWMS(R);
      }
      AL3_Log(" -- Done\n",3);
    }
    if(!strncmp(RX->Nucleus,"GET",RX->Size)){
      AL3_Log(" -- Client gets records\n",3);
      for(_=0;_<2;_++){
	printf("%s: -- S%d Getting data\n",TMP=Gettime(),_);
	free(TMP);
	AL1_ReadHeader(Nucleii[_]);
	AL1_LockColony(Nucleii[_]);
	R=AL2_GetDBContents(Nucleii[_]);
	AL1_UnlockColony(Nucleii[_]);
	ServerID=_;
	printf("%s: <- S%d Sending data\n",TMP=Gettime(),_);
	free(TMP);
	AL3_TransmitAll(R,ClientSocket);
	FreeWMS(R);
      }
      AL3_Log(" -- Done\n",3);
    } 
    if(!strncmp(RX->Nucleus,"RRD",RX->Size)){
      AL3_Log(" -- Contents refresh\n",3);
      AL3_ContentsReg(Nucleii);
      NetTransmit(ACK,ClientSocket);
      AL3_Log(" -- Done\n",3);
    }
    if(!strncmp(RX->Nucleus,"DSP",RX->Size)){
      AL3_Log(" -- Contents display\n",3);
      AL1_ReadHeader(Nucleii[0]);
      AL1_ReadHeader(Nucleii[1]);
      DEBUG_Header(Nucleii[0]);
      DEBUG_Header(Nucleii[1]);    
      NetTransmit(ACK,ClientSocket);
      AL3_Log(" -- Done\n",3);
    }
    if(!strncmp(RX->Nucleus,"PUR",RX->Size)){
      AL3_Log(" -- Purge database\n",3);
      AL1_InitHeader(Nucleii[0]);
      AL1_InitHeader(Nucleii[1]);
      AL1_ReadHeader(Nucleii[0]);
      AL1_ReadHeader(Nucleii[1]);
      NetTransmit(ACK,ClientSocket);
      AL3_Log(" -- Done\n",3);
    }
    if((!strncmp(RX->Nucleus,"SAV",3))&&(RX->Size>4)){
      AL3_Log(" -- Save database as ",3);
      AL3_WriteFile(RX);
      NetTransmit(ACK,ClientSocket);
      AL3_Log(" -- Done\n",3);
    }
    if((!strncmp(RX->Nucleus,"LOA",3))&&(RX->Size>4)){
      AL3_Log(" -- Load database from ",3);
      AL3_OpenFile(RX);      
      NetTransmit(ACK,ClientSocket);
      AL3_Log(" -- Done\n",3);
    }
    if(!strncmp(RX->Nucleus,"PNG",RX->Size)){
      AL3_Log(" -- Ping received.\n",3);
      NetTransmit(ACK,ClientSocket);
      AL3_Log(" -- Done\n",3);
    }
    close(ClientSocket);
  }
  AL1_FreeBlob(ACK);
}

/*! \brief Create a contents table within the Amoeba Database.
 *
 */
void AL3_ContentsReg(struct Colony *Nucleii[2]){
  /* used for debugging only */
  int Sector=0;
  unsigned long int _=0;
  char *Name=NULL,*Position=NULL,*ToFree=NULL,*TMP=NULL;
  struct Blob *Rec=NULL,*Contents=NULL;
  struct Datablock *DB=NULL;
  Contents=CreateBlob(strlen(":CONTENTS:"),StrCat("",":CONTENTS:"));
  ToFree=StrCat("",":CONTENTS:");
  for(Sector=0;Sector<2;Sector++){
    DB=Add_DB_Entry(NULL,ToFree);
    AL1_LockColony(Nucleii[Sector]);
    AL1_ReadHeader(Nucleii[Sector]);
    if((_=AL1_FindDBEntry(Nucleii[Sector],Contents))+1){
      DelRec(Nucleii[Sector],_);
    }
    AL2_CacheFreqUsed(Nucleii[Sector]);
    DB->DS[1]=NULL;
    DB->DS[2]=NULL;
    printf("%s: -- S%d Added %ld records\n",TMP=Gettime(),Sector,Nucleii[Sector]->Population);
    free(TMP);
    for(_=1;_<Nucleii[Sector]->Population;_++){
      Rec=AL1_GetRecName(Nucleii[Sector],_);
      Name=(char *)malloc((Rec->Size+1)*sizeof(char));
      memcpy(Name,Rec->Nucleus,Rec->Size);
      Name[Rec->Size]=0;
      Position=FloatToString((double )_);
      Rep_DB_Pair(DB,Name,Position);
      AL1_FreeBlob(Rec);
    }
    WMS2Amoeba(Nucleii[Sector],DB);
    AL1_UnlockColony(Nucleii[Sector]);
    FreeWMS(DB);
  }
  free(ToFree);
  AL1_FreeBlob(Contents);
}

/*! \brief Force a contents refresh on the server.
 *
 */
void AL3_Reread(){
  int Socket=0;
  struct Blob *TX=NULL;
  Socket=OpenClientSocket(SERVERIP);
  TX=CreateBlob(3,"RRD");
  NetTransmit(TX,Socket);
  NetReceive(Socket);
  close(Socket);
}

/*! \brief Force a contents display on the server.
 *
 */
void AL3_SrvDisplay(){
  int Socket=0;
  struct Blob *TX=NULL;
  Socket=OpenClientSocket(SERVERIP);
  TX=CreateBlob(3,"DSP");
  NetTransmit(TX,Socket);
  NetReceive(Socket);
  close(Socket);
}

/*! \brief Get records from the server.
 *
 */
struct DB_Wrap *AL3_Acquire(struct Datablock *DB[2]){
  int Socket=0;
  struct Blob *TX=NULL;
  struct DB_Wrap *R=(struct DB_Wrap *)malloc(sizeof(struct DB_Wrap));
  Socket=OpenClientSocket(SERVERIP);
  TX=CreateBlob(3,"GET");
  NetTransmit(TX,Socket);
  DB[0]=AL3_ReceiveAll(Socket);
  DB[1]=AL3_ReceiveAll(Socket);
  close(Socket);
  R->DB[0]=DB[0];
  R->DB[1]=DB[1];
  return R;
}

/*! \brief Send records to the server.
 *
 */
void AL3_Store(struct Datablock *DB[2]){
  int Socket=0;
  struct Blob *TX=NULL;
  Socket=OpenClientSocket(SERVERIP);
  TX=CreateBlob(3,"ADD");
  NetTransmit(TX,Socket);
  ServerID=0;
  AL3_TransmitAll(DB[0],Socket);
  ServerID=1;
  AL3_TransmitAll(DB[1],Socket);
  close(Socket);
}

/*! \brief Send records to the server.
 * - like AL3_Store() but lower level.
 */
void AL3_TransmitAll(struct Datablock *R,int Socket){
  unsigned long int Size=0;
  struct Datablock *Buffer=NULL,*DB=NULL;
  struct Blob *RX=NULL,*TX=NULL;
  char *String=(char *)malloc(30*sizeof(char));
  DB=R;
  Size=0;
  for(Buffer=DB;Buffer;Buffer=Buffer->Next)
    Size++;
  sprintf(String,"SIZE:%ld",Size);

  /*  printf("DEBUG AL3_TransmitAll %s\n",String); */

  TX=CreateBlob(strlen(String),String);
  NetTransmit(TX,Socket);
  RX=NetReceive(Socket);
  AL1_FreeBlob(RX);
  for(Buffer=DB;Buffer;Buffer=Buffer->Next){
    TX=AL3_WMS2Blob(Buffer);
    NetTransmit(TX,Socket);
    AL1_FreeBlob(TX);
  }
  free(String);
}

/*! \brief Receive records from the server.
 * - like AL3_Acquire() but lower level.
 */
struct Datablock  *AL3_ReceiveAll(int Socket){
  struct Datablock *R=NULL,*DB=NULL;
  unsigned long int _=0,Size=0;
  struct Blob *RX=NULL,*ACK=CreateBlob(3,"ACK");
  RX=NetReceive(Socket);
  Size=atol(RX->Nucleus+5);
  NetTransmit(ACK,Socket);
  AL1_FreeBlob(RX);
  for(_=0;_<Size;_++){
    DB=AL3_Blob2WMS(RX=NetReceive(Socket));
    DB->Next=R;
    R=DB;
    AL1_FreeBlob(RX);
  }
  return R;
}

/*! \brief Translate Amoeba records to WMS records.
 * 
 */
struct Datablock *AL3_Blob2WMS(struct Blob *Data){
  int _=0,A=0,LLPos=0;
  unsigned long int Position=0,S[3],Length=0;
  char *String=NULL;
  struct Datablock *DB=(struct Datablock *)malloc(sizeof(struct Datablock));
  DB->Next=NULL;
  for(_=0;_<3;_++){
    DB->DS[_]=NULL;
    S[_]=atol(Data->Nucleus+Position);
    Position+=11;
    LLPos=0;
    for(A=0;A<S[_];A++){
      Length=atol(Data->Nucleus+Position);
      Position+=11;
      String=(char *)malloc((Length+1)*sizeof(char));
      memcpy(String,Data->Nucleus+Position,Length);
      String[Length]=0;
      Position+=Length;
      DB->DS[_]=Ins_List_Element(DB->DS[_],String,LLPos++);
    }
  }
  return DB;
}

/*! \brief Translate WMS records to Amoeba records.
 * 
 */
struct Blob *AL3_WMS2Blob(struct Datablock *DB){
  int _=0;
  unsigned long int Size=0,S[3],Position=0;
  struct List_Str *Buffer=NULL;
  struct Blob *R=(struct Blob *)malloc(sizeof(struct Blob));
  for(_=0;_<3;_++){
    Size+=11;
    S[_]=0;
    for(Buffer=DB->DS[_];Buffer;Buffer=Buffer->Next){
      Size+=11;
      Size+=strlen(Buffer->Str);
      S[_]++;
    }
  }
  R->Size=Size;
  R->Nucleus=(char *)malloc(Size*sizeof(char));
  memset(R->Nucleus,0,Size);
  for(_=0;_<3;_++){
    sprintf(R->Nucleus+Position,"%ld",S[_]);
    Position+=11;
    for(Buffer=DB->DS[_];Buffer;Buffer=Buffer->Next){
      Size=strlen(Buffer->Str);
      sprintf(R->Nucleus+Position,"%ld",Size);
      Position+=11;
      memcpy(R->Nucleus+Position,Buffer->Str,Size);
      Position+=Size;
    }
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
