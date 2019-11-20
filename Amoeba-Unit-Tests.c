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

#include "ENiX_LIST.h"
#include "ENiX_WMS.h"
#include "ENiX_STRING.h"
#include "Amoeba-Colony.h"
#include "Amoeba-API.h"
#include "Amoeba-Microtubules.h"
#include "Amoeba-Unit-Tests.h"
#include "WitchCraft-Colours.h"
#include "WitchCraft-Net.h"

extern struct Colony *Nucleii[2];

/*! \brief Emergency erase the database.
 *
 */
void DEBUG_PURGE(){
  AL3_Purge();
}

/*! \brief Load a new database from file.
 *
 */
void DEBUG_LOADFILE(){
  AL3_ReadFile("Test");
}

/*! \brief Save a new database to file.
 *
 */
void DEBUG_SAVEFILE(){
  AL3_SaveFile("Test");
}

/*! \brief Reload header parameters.
 *
 */
void DEBUG_ClientRefresh(){
  puts("Initialising reread of the alloc table.");
  AL3_Reread();
  AL1_LockColony(Nucleii[0]);
  AL1_ReadHeader(Nucleii[0]);
  AL1_UnlockColony(Nucleii[0]);
  AL1_LockColony(Nucleii[1]);
  AL1_ReadHeader(Nucleii[1]);
  AL1_UnlockColony(Nucleii[1]);
}

/*! \brief Add two records and store them.
 *
 */
void DEBUG_TXClient(){
  struct Datablock *DB[2];
  DB[0]=NULL;
  DB[1]=NULL;
  DB[0]=Add_DB_Entry(DB[0],"Test 1");
  DB[0]=Add_DB_Entry(DB[0],"Test 2");
  AL3_Store(&(*DB));
}

/*! \brief Retrieve print data from the Amoeba database.
 *
 */
void DEBUG_RXClient(){
  struct Datablock *Buffer,*DB[2];
  DB[0]=NULL;
  DB[1]=NULL;
  AL3_Acquire(&(*DB));
  for(Buffer=DB[0];Buffer;Buffer=Buffer->Next){
    Amoeba_P_DB_Entry(Buffer,0);
  }
  for(Buffer=DB[1];Buffer;Buffer=Buffer->Next){
    Amoeba_P_DB_Entry(Buffer,0);
  }
}

/*! \brief Retrieve data from the Amoeba database.
 *
 */
void DEBUG_RXServer(){
  AL3_Interpret(Nucleii);
}

/*! \brief Transmit data to the Amoeba database.
 *
 */
void DEBUG_TXServer(){
  DEBUG_TXClient();
  DEBUG_RXClient();
}

/*! \brief Add two records to the database and retrieve them.
 *
 */
void DEBUG_Function(){
  AL3_CheckService();
}

/*! \brief Run an Amoeba Midden Server.
 *
 */
void DEBUG_NetServer(){
  unsigned long int _=0;
  int ClientSocket=0,Socket=OpenServerSocket();
  struct Blob *RX=NULL,*TX=CreateBlob(strlen("Hi"),"Hi");
  puts("Listening...");
  while(1){
    ClientSocket=AcceptServerCon(Socket);
    RX=NetReceive(ClientSocket);
    printf("Received %ld [%s]\n",_,RX->Nucleus);
    free(RX);
    NetTransmit(TX,ClientSocket);
    close(ClientSocket);
    _++;
  }
  puts("");
}

/*! \brief Connect to an Amoeba Midden Server and send some data.
 *
 */
void DEBUG_NetClient(unsigned long int Retries){
  int Socket=0;
  unsigned long int _=0;
  struct Blob *RX=NULL,*TX=CreateBlob(strlen("hi there! 123456789012345678901234567890"),"hi there! 123456789012345678901234567890");
  Socket=OpenClientSocket(SERVERIP);
  for(_=0;_<Retries;_++){
    NetTransmit(TX,Socket);
    RX=NetReceive(Socket);
    printf("Sent: %ld, Response: [%s]\n",_,RX->Nucleus);
    free(RX);
  }
  close(Socket);
}

/*! \brief Defrag the Amoeba database
 *
 */
void DEBUG_DefragAll(struct Colony *City){
  AL1_Defrag(City);
}

/*! \brief Display database metrics.
 *
 */
void DEBUG_ColonySummary(struct Colony *City){
  puts("-[Colony Summary]----------");
  printf(" Population:    %ld\n",City->Population);
  printf(" Last Position: %ld\n",City->LastPos);
}

/*! \brief Write a bunch of records to the Amoeba database.
 *
 */
void DEBUG_WMS2ABA(struct Colony *City,unsigned long int Retries){
  unsigned long int _=0;
  char Name[80];
  struct List_Str *LS=NULL;
  struct Datablock *DB=(struct Datablock *)malloc(sizeof(struct Datablock));
  AL1_LockColony(City);
  AL2_CacheFreqUsed(City);
  LS=Ins_List_Element(LS,"Wibble",0);
  LS=Ins_List_Element(LS,"?",1);
  LS=Ins_List_Element(LS,"?",2);
  LS=Ins_List_Element(LS,"?",3);
  LS=Ins_List_Element(LS,City->CachedTime,4);
  LS=Ins_List_Element(LS,City->CachedTime,5);
  LS=Ins_List_Element(LS,"P:1",6);
  LS=Ins_List_Element(LS,"?",7);
  DB->DS[0]=LS;
  DB->DS[1]=Ins_List_Element(NULL,"A",0);
  DB->DS[2]=Ins_List_Element(NULL,"B",0);
  for(_=0;_<Retries;_++){
    printf("Retry %ld\r",_);
    fflush(stdout);
    sprintf(Name,"Test-%ld",_);
    DB->DS[0]->Str=Name;
    WMS2Amoeba(City,DB);
  }
  AL1_UnlockColony(City);
}

/*! \brief Dump the contents of the database to stdout.
 *
 */
void DEBUG_ShowWMS(struct Colony *City){
  unsigned long int _=0;
  struct Datablock *DB=NULL;
  for(_=0;_<City->Population;_++){
    DB=Amoeba2WMS(City,_);
    Amoeba_P_DB_Entry(DB,0);
  }
}

/*! \brief Make a new amoeba record.
 *
 */
void DEBUG_AddRec(struct Colony *City){
  struct Blob *B=CreateBlob(strlen("Martyn-Kate"),"Martyn-Kate");
  NewRec(City,B);
}

/*! \brief Defrag the allocation table.
 *
 */
void DEBUG_DAT(struct Colony *City){
  AL1_DefragAllocTable(City);
  puts("Finished");
}

/*! Delete some records.
 *
 */
void DEBUG_Delete(struct Colony *City,char *Name,unsigned long int Retries){
  unsigned long int Pos=0,_=0;
  struct Blob *B=NULL;
  for(_=0;_<Retries;_++){
    printf("Retry: %ld\r",_);
    B=CreateBlob(strlen(Name),StrCat("",Name));
    Pos=DEBUG_New(City,B);
    AL1_DelDBEntry(City,Pos);
  }
  puts("");
}

/*! Modify some records.
 *
 */
void DEBUG_Modify(struct Colony *City,char *Name,unsigned long int Retries){
  unsigned long int _=0,Position=0;
  struct Blob *B=CreateBlob(strlen(Name),Name);
  struct Cytoskelenton *C=NULL;
  NewRec(City,B);
  Position=AL1_FindDBEntry(City,B);
  if(Position==-1){
    printf("Cannot find %s\n",Name);
    return ;
  }
  for(_=0;_<Retries;_++){
    printf("Retry: %ld\r",_);
    C=AL1_GetDBEntry(City,Position);
    AL1_RepDBEntry(City,C,Position);
  }
  puts("");
}

/*! Search some records.
 *
 */
void DEBUG_Search(struct Colony *City,char *Name,unsigned long int Retries){
  int First=1;
  unsigned long int _=0,C=0,Last=0;
  struct Blob *B=(struct Blob *)malloc(sizeof(struct Blob));
  for(_=0;_<Retries;_++){
    B->Nucleus=Name;
    B->Size=strlen(Name);
    C=AL1_FindDBEntry(City,B);
    if(First)
      Last=C;
  }
  printf("Position: %ld\n",Last);
  free(B);
}

/*! Display allocation table information.
 *
 */
void DEBUG_Header(struct Colony *City){
  unsigned long int _=0,Cache=0,Start=0,End=0;
  struct Blob *Name=NULL;
  puts("Amoeba Alloc Table:");
  puts("-------------------");
  puts("[ RECORD ID  -> CACHE POS  ][ SHM RANGE         ][ RECORD NAME  ]");
  for(_=0;_<City->Population;_++){
    Cache=AL1_GetCacheHeader(City,_);

    Start=AL1_GetStartHeader(City,Cache);
    End  =AL1_GetFinishHeader(City,Cache);
    Name =AL1_GetRecName(City,Cache);

    /*
    Start=AL1_GetStartHeader(City,_);
    End  =AL1_GetFinishHeader(City,_);
    Name =AL1_GetRecName(City,_);
    */
    /*    if(Name->Size<20){ */
    if(Name->Size<100){
      if(strcmp(Name->Nucleus,"->BLANK<-")){
	printf("[ %010ld -> %010ld ][ %ld - %ld ][ %s ]\n",_,Cache,Start,End,Name->Nucleus);
      }
      else{
	printf("%s[ %010ld -> %010ld ][ %ld - %ld ][ %s ]%s\n",WC_RED,_,Cache,Start,End,Name->Nucleus,WC_RESET);
      }
    }
    else
      printf("%s[ %010ld -> %010ld ][ %ld - %ld ][ ->DATA<- ]%s\n",WC_GREEN,_,Cache,Start,End,WC_RESET);
    AL1_FreeBlob(Name);
  }
  putchar('\n');
}

/*! Make some WMS records.
 *
 */
struct Datablock *DEBUG_MakeWMSRec(){
  struct Datablock *R=NULL;
  R=(struct Datablock *)malloc(sizeof(struct Datablock));
  R->DS[0]=Str2List("A B C D E F G H");
  R->DS[1]=Str2List("E F G H");
  R->DS[2]=Str2List("1 2 3 4");
  R->Next=NULL;
  return R;
}

/*! Print a mitochondrion data type.
 *
 */
void DEBUG_Mito(struct Mitochondrion *M){
  printf("ARef [%ld], PRef [%d], MRef [%d]\n",M->ARef,M->PRef,M->MRef);
}

/*! Partially fill the Amoeba database.
 *
 */
void DEBUG_PartFill(struct Colony *City,unsigned long int Total){
  int Continue=1;
  unsigned long int C=0;
  struct Blob *Core=NULL;
  puts("Partially filling the database...");
  for(C=0;Continue&&(C<Total);C++){
    Core=(struct Blob *)malloc(sizeof(struct Blob));
    Core->Nucleus=(char *)malloc(80*sizeof(char));
    snprintf(Core->Nucleus,80,"Wibble-%06ld",C);
    Core->Size=strlen(Core->Nucleus);
    Continue=DEBUG_New(City,Core);
  }
  printf("%ld records written.\n",C-1);  
}

/*! Fill the Amoeba database completely.
 *
 */
void DEBUG_Fill(struct Colony *City){
  int Continue=1;
  unsigned long int C=0;
  struct Blob *Core=NULL;
  puts("Filling the database...");
  while(Continue){
    Core=(struct Blob *)malloc(sizeof(struct Blob));
    Core->Nucleus=(char *)malloc(80*sizeof(char));
    snprintf(Core->Nucleus,80,"Wibble-%06ld",C);
    Core->Size=strlen(Core->Nucleus);
    Continue=DEBUG_New(City,Core);
    C++;
    printf("%ld records written.\r",C-1);  
  }
  puts("");
}

/*! Create a new record.
 *
 */
unsigned long int DEBUG_New(struct Colony *City,struct Blob *Name){
  unsigned long int Z=0,Y=0,A=0,B=0,R=0;
  struct Cytoskelenton *C=NULL;
  struct Pseudopod     *P=NULL;
  struct Mitochondrion *M=NULL;
  A=rand()%3+1;
  C=(struct Cytoskelenton *)malloc(sizeof(struct Cytoskelenton));
  C->Core=Name;
  C->Size=Name->Size;
  C->Lock=0; 
  C->Dimension=A;
  C->Pseudopods=(struct Pseudopod **)malloc((A)*sizeof(struct Pseudopod *));
  for(Z=0;Z<A;Z++){
    B=rand()%6;
    P=(struct Pseudopod *)malloc(sizeof(struct Pseudopod));
    P->Dimension=B;
    P->Mitochondria=(struct Mitochondrion **)malloc((B)*sizeof(struct Mitochondrion *));
    C->Pseudopods[Z]=P;
    for(Y=0;Y<B;Y++){
      M=(struct Mitochondrion *)malloc(sizeof(struct Mitochondrion));
      M->ARef=(unsigned long int)(rand()%(City->Population+1));
      M->PRef=(int)(rand()%4);
      M->MRef=(int)(rand()%4);
      C->Pseudopods[Z]->Mitochondria[Y]=M;
    } 
  }
  R=AL1_AddDBEntry(City,C);
  AL1_FreeBlob(Name);
  return R;
}

/*! Display a record.
 *
 */
void DEBUG_Display(struct Cytoskelenton *C){
  unsigned long int A=0,B=0;
  struct Pseudopod     *P=NULL;
  struct Mitochondrion *M=NULL;
  if(C){
    /*if(C->Core->Size<20)*/
    if(C->Core->Size<40)
      printf("Record    [%s]\n",C->Core->Nucleus);
    else
      puts("Record    [->DATA<-]");
    for(A=0;A<C->Dimension;A++){
      P=C->Pseudopods[A];
      for(B=0;B<P->Dimension;B++){
	M=P->Mitochondria[B];
	if(B)
	  printf("       -> ");
	else
	  printf(" % 5ld -> ",A);
	printf("[%ld][%d][%d]\n",M->ARef,M->PRef,M->MRef);
      }
    }
    putchar('\n');
  }
}

/*! Display the contents of the database.
 *
 */
void DEBUG_ALL(struct Colony *City){
  unsigned long int _=0;
  struct Cytoskelenton *C=NULL;
  for(_=0;_<City->Population;_++){
    printf("ID        %ld\n",_);
    puts("================================================================================");
    DEBUG_Display(C=AL1_GetDBEntry(City,_));
    AL1_FreeAmoeba(C);
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
