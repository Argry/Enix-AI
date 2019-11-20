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


#include "Amoeba-Unit-Tests.h"
#include "WitchCraft-Colours.h"

extern struct Colony *Nucleii[2];

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

void DEBUG_TXClient(){
  struct Datablock *DB[2];
  DB[0]=NULL;
  DB[1]=NULL;
  DB[0]=Add_DB_Entry(DB[0],"Test 1");
  DB[0]=Add_DB_Entry(DB[0],"Test 2");
  AL3_Store(&(*DB));
}

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

void DEBUG_RXServer(){
  AL3_Interpret(Nucleii);
}

void DEBUG_TXServer(){
  DEBUG_TXClient();
  DEBUG_RXClient();
}

void DEBUG_Function(){
  struct Datablock *DB=NULL;
  DB=Add_DB_Entry(DB,"Test 1");
  DB=Add_DB_Entry(DB,"Test 2");
  AL2_PutDBContents(DB,Nucleii[0]);
  AL2_GetDBContents(Nucleii[0]);
}

void DEBUG_NetServer(){
  unsigned long int _=0;
  int ClientSocket,Socket=OpenServerSocket();
  struct Blob *RX,*TX=CreateBlob(strlen("Hi"),"Hi");
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

void DEBUG_NetClient(unsigned long int Retries){
  int Socket;
  unsigned long int _;
  struct Blob *RX,*TX=CreateBlob(strlen("hi there! 123456789012345678901234567890"),"hi there! 123456789012345678901234567890");
  Socket=OpenClientSocket(SERVERIP);
  for(_=0;_<Retries;_++){
    NetTransmit(TX,Socket);
    RX=NetReceive(Socket);
    printf("Sent: %ld, Response: [%s]\n",_,RX->Nucleus);
    free(RX);
  }
  close(Socket);
}

void DEBUG_DefragAll(struct Colony *City){
  AL1_Defrag(City);
}

void DEBUG_ColonySummary(struct Colony *City){
  puts("-[Colony Summary]----------");
  printf(" Population:    %ld\n",City->Population);
  printf(" Last Position: %ld\n",City->LastPos);
}

void DEBUG_WMS2ABA(struct Colony *City,unsigned long int Retries){
  unsigned long int _;
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

void DEBUG_ShowWMS(struct Colony *City){
  unsigned long int _;
  struct Datablock *DB;
  for(_=0;_<City->Population;_++){
    DB=Amoeba2WMS(City,_);
    Amoeba_P_DB_Entry(DB,0);
  }
}

void DEBUG_AddRec(struct Colony *City){
  struct Blob *B=CreateBlob(strlen("Martyn-Kate"),"Martyn-Kate");
  NewRec(City,B);
}

void DEBUG_DAT(struct Colony *City){
  AL1_DefragAllocTable(City);
  puts("Finished");
}

void DEBUG_Delete(struct Colony *City,char *Name,unsigned long int Retries){
  unsigned long int Pos,_;
  struct Blob *B;
  for(_=0;_<Retries;_++){
    printf("Retry: %ld\r",_);
    B=CreateBlob(strlen(Name),StrCat("",Name));
    Pos=DEBUG_New(City,B);
    AL1_DelDBEntry(City,Pos);
  }
  puts("");
}

void DEBUG_Modify(struct Colony *City,char *Name,unsigned long int Retries){
  unsigned long int _,Position=0;
  struct Blob *B=CreateBlob(strlen(Name),Name);
  struct Cytoskelenton *C;
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

void DEBUG_Search(struct Colony *City,char *Name,unsigned long int Retries){
  int First=1;
  unsigned long int _,C,Last;
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

void DEBUG_Header(struct Colony *City){
  unsigned long int _,Cache,Start,End;
  struct Blob *Name;
  puts("Amoeba Alloc Table:");
  puts("-------------------");
  puts("[ RECORD ID  -> CACHE POS  ][ SHM RANGE         ][ RECORD NAME  ]");
  for(_=0;_<City->Population;_++){
    Cache=AL1_GetCacheHeader(City,_);
    Start=AL1_GetStartHeader(City,Cache);
    End  =AL1_GetFinishHeader(City,Cache);
    Name =AL1_GetRecName(City,Cache);
    if(Name->Size<20){
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

struct Datablock *DEBUG_MakeWMSRec(){
  struct Datablock *R;
  R=(struct Datablock *)malloc(sizeof(struct Datablock));
  R->DS[0]=Str2List("A B C D E F G H");
  R->DS[1]=Str2List("E F G H");
  R->DS[2]=Str2List("1 2 3 4");
  R->Next=NULL;
  return R;
}

void DEBUG_Mito(struct Mitochondrion *M){
  printf("ARef [%ld], PRef [%d], MRef [%d]\n",M->ARef,M->PRef,M->MRef);
}

void DEBUG_PartFill(struct Colony *City,unsigned long int Total){
  int Continue=1;
  unsigned long int C=0;
  struct Blob *Core;
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

void DEBUG_Fill(struct Colony *City){
  int Continue=1;
  unsigned long int C=0;
  struct Blob *Core;
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

unsigned long int DEBUG_New(struct Colony *City,struct Blob *Name){
  unsigned long int Z,Y,A,B,R;
  struct Cytoskelenton *C;
  struct Pseudopod     *P;
  struct Mitochondrion *M;
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

void DEBUG_Display(struct Cytoskelenton *C){
  unsigned long int A,B;
  struct Pseudopod     *P;
  struct Mitochondrion *M;
  if(C){
    if(C->Core->Size<20)
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

void DEBUG_ALL(struct Colony *City){
  unsigned long int _;
  struct Cytoskelenton *C;
  struct Blob          *N;
  for(_=0;_<City->Population;_++){
    printf("ID        %ld\n",_);
    puts("================================================================================");
    DEBUG_Display(C=AL1_GetDBEntry(City,_));
    N=C->Core;
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
