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
#include <unistd.h>
#include "Amoeba-Colony.h"
#include "Amoeba-Nucleus.h"
#include "WitchCraft-Colours.h"

struct Colony SizeColony[2];
struct Colony *Nucleii[2];

extern int FIRSTRUN;
extern const char *FILE1;
extern const char *FILE2;

int Nucleii_Initialised=0;
int CACHE_DELETED=1;

int AllowQueen=0;

void AL1_FreeBlob(struct Blob *F){
  char *G=F->Nucleus;
  free(G);
  free(F);
}

void AL1_FreeAmoeba(struct Cytoskelenton *C){
  unsigned long int A,B;
  struct Pseudopod     *FP;
  struct Mitochondrion *FM;
  if(C){
    for(A=0;A<C->Dimension;A++){
      for(B=0;B<C->Pseudopods[A]->Dimension;B++){
	FM=C->Pseudopods[A]->Mitochondria[B];
	free(FM);
      }
      free(C->Pseudopods[A]->Mitochondria);
      FP=C->Pseudopods[A];
      free(FP);
    }
    if(C->Dimension)
      free(C->Pseudopods);
    AL1_FreeBlob(C->Core);
    free(C);
  }
}

int AL1_RepDBEntry(struct Colony *City,struct Cytoskelenton *Data,unsigned long int RecordID){  
  unsigned long int A,B,S=0,Pos,SizeWrite,WriteTo,StartPos,FinishPos;
  SizeWrite=(sizeof(struct Cytoskelenton)+SHMSPACING)+Data->Size+SHMSPACING+Data->Dimension*(sizeof(struct Pseudopod)+SHMSPACING);
  for(A=0;A<Data->Dimension;A++)
    S+=Data->Pseudopods[A]->Dimension;
  SizeWrite+=S*(sizeof(struct Mitochondrion)+SHMSPACING);
  A=(unsigned long int)AL1_GetStartHeader(City,RecordID);
  B=(unsigned long int)AL1_GetFinishHeader(City,RecordID);
  if((B-A)<SizeWrite){
    if((City->LastPos+SizeWrite)>=((unsigned long int)(City->Size))){
      AL1_Defrag(City);
      if((City->LastPos+SizeWrite)>=((unsigned long int)(City->Size))){
	puts("Amoeba (ADBE): Database full, terminating!");
	return 0;
      }
    }  
    WriteTo=City->LastPos+FRAGMENTATION;
    City->LastPos=(WriteTo+SizeWrite);
  }
  else{
    WriteTo=A;
  }
  memcpy(City->Shared+WriteTo,Data,sizeof(struct Cytoskelenton));
  Pos=(WriteTo+sizeof(struct Cytoskelenton)+SHMSPACING);
  memcpy(City->Shared+Pos,Data->Core->Nucleus,Data->Size);
  Pos+=(Data->Size+SHMSPACING);
  for(A=0;A<Data->Dimension;A++){
    memcpy(City->Shared+Pos,Data->Pseudopods[A],sizeof(struct Pseudopod));
    Pos+=(sizeof(struct Pseudopod)+SHMSPACING);
    for(B=0;B<Data->Pseudopods[A]->Dimension;B++){
      memcpy(City->Shared+Pos,Data->Pseudopods[A]->Mitochondria[B],sizeof(struct Mitochondrion));
      Pos+=(sizeof(struct Mitochondrion)+SHMSPACING);
    }
  }
  StartPos=WriteTo;
  FinishPos=WriteTo+SizeWrite;
  A=City->LastPos;
  memcpy(City->Shared+((unsigned long int)City->StartPos +(RecordID*sizeof(RecordID))),&StartPos ,sizeof(StartPos));
  memcpy(City->Shared+((unsigned long int)City->FinishPos+(RecordID*sizeof(RecordID))),&FinishPos,sizeof(FinishPos));
  memcpy(City->Shared+70,&A,sizeof(unsigned long int));
  return RecordID;
}

int AL1_AddDBEntry(struct Colony *City,struct Cytoskelenton *Data){
  unsigned long int A,B,S=0,Pos,SizeWrite,WriteTo,Location=0;
  SizeWrite=(sizeof(struct Cytoskelenton)+SHMSPACING)+Data->Size+SHMSPACING+Data->Dimension*(sizeof(struct Pseudopod)+SHMSPACING);
  for(A=0;A<Data->Dimension;A++)
    S+=Data->Pseudopods[A]->Dimension;
  SizeWrite+=S*(sizeof(struct Mitochondrion)+SHMSPACING);
  if((City->LastPos+SizeWrite)>=((unsigned long int)(City->Size))){
    AL1_Defrag(City);
    if((City->LastPos+SizeWrite)>=((unsigned long int)(City->Size))){
      puts("Amoeba (ADBE): Database full, terminating!");
      return 0;
    }
  }  
  Location=City->Population;
  WriteTo=City->LastPos+FRAGMENTATION;
  City->LastPos=(WriteTo+SizeWrite);
  memcpy(City->Shared+WriteTo,Data,sizeof(struct Cytoskelenton));
  Pos=(WriteTo+sizeof(struct Cytoskelenton)+SHMSPACING);
  memcpy(City->Shared+Pos,Data->Core->Nucleus,Data->Size);
  Pos+=(Data->Size+SHMSPACING);
  for(A=0;A<Data->Dimension;A++){
    memcpy(City->Shared+Pos,Data->Pseudopods[A],sizeof(struct Pseudopod));
    Pos+=(sizeof(struct Pseudopod)+SHMSPACING);
    for(B=0;B<Data->Pseudopods[A]->Dimension;B++){
      memcpy(City->Shared+Pos,Data->Pseudopods[A]->Mitochondria[B],sizeof(struct Mitochondrion));
      Pos+=(sizeof(struct Mitochondrion)+SHMSPACING);
    }
  }
  AL1_UpdateHeader(City,City->Population,WriteTo,Pos);
  return Location;
}

void AL1_DefragAllocTable(struct Colony *City){
  unsigned long int _,Position,Gaps=0,*Fragments;
  struct Blob *Name=NULL;
  
  if(CACHE_DELETED){
    for(_=1;_<City->Population;_++){
      if(!strcmp((Name=AL1_GetRecName(City,_))->Nucleus,"->BLANK<-"))
	Gaps++;
      AL1_FreeBlob(Name);
    }
    Fragments=(unsigned long int *)malloc(Gaps*sizeof(unsigned long int));
    Position=0;
    for(_=1;_<City->Population;_++){
      if(!strcmp((Name=AL1_GetRecName(City,_))->Nucleus,"->BLANK<-"))
	Fragments[Position++]=_;
      AL1_FreeBlob(Name);
    }
    Position=City->Population-1; /* Because last element of array doesn't exist */
    
    for(_=0;_<Gaps;_++){
      while((!strcmp((Name=AL1_GetRecName(City,Position))->Nucleus,"->BLANK<-"))&&(Position>0)){
	Position--;
	AL1_FreeBlob(Name);
      }
      /* 
	 Position contains the last real record
	 Fragments[_] contains the first gap
      */
      if(Position<Fragments[_]){
	City->Population=Fragments[_];
	break;
      }
      else{
	AL1_Relocate(City,Fragments[_],Position);
	Position--;
      }
    }
    City->LastPos=AL1_GetFinishHeader(City,City->Population-1);
    AL1_WriteHeader(City);
    free(Fragments);
    CACHE_DELETED=0;
  } 
}

void AL1_Relocate(struct Colony *City,unsigned long int Gap,unsigned long int From){
  unsigned long int _,A,B,C,Start,End;
  struct Blob *Name;
  struct Cytoskelenton *Root,*Leaf;
  struct Pseudopod     *P=NULL;
  struct Mitochondrion *M=NULL;
  Root=AL1_GetDBEntry(City,From);
  if(Root->Dimension){
    P=Root->Pseudopods[0];
    for(_=0;_<P->Dimension;_++){
      A=P->Mitochondria[_]->ARef;
      Leaf=AL1_GetDBEntry(City,A);
      if(Leaf){
	Name=Leaf->Core;
	for(B=0;B<Leaf->Dimension;B++){
	  for(C=0;C<Leaf->Pseudopods[B]->Dimension;C++){
	    M=Leaf->Pseudopods[B]->Mitochondria[C];
	    if(M->ARef==From)
	      Leaf->Pseudopods[B]->Mitochondria[C]->ARef=Gap;
	  }
	}
	AL1_RepDBEntry(City,Leaf,A);
	AL1_FreeBlob(Name);
      }
    }
  }
  City->Population-=2; /* AL1_UpdateHeader increments */
  Start=AL1_GetStartHeader(City,From);
  End=AL1_GetFinishHeader(City,From);
  AL1_UpdateHeader(City,Gap,Start,End);
}

void AL1_Rename(struct Colony *City,struct Blob *NewName,unsigned long int RecordID){
  struct Cytoskelenton *C=NULL;
  if(!RecordID)
    return;
  C=AL1_GetDBEntry(City,RecordID);
  C->Core=NewName;
  AL1_RepDBEntry(City,C,RecordID);
}

struct Blob *AL1_GetRecName(struct Colony *City,unsigned long int RecordID){
  struct Cytoskelenton *C;
  unsigned long int Pos;
  struct Blob *R=(struct Blob *)malloc(sizeof(struct Blob));
  if(RecordID<City->Population){
    C=(struct Cytoskelenton *)malloc(sizeof(struct Cytoskelenton));
    Pos=(unsigned long int)AL1_GetStartHeader(City,RecordID);
    memcpy(C,City->Shared+Pos,sizeof(struct Cytoskelenton));
    Pos=((unsigned long int)Pos+sizeof(struct Cytoskelenton)+SHMSPACING);
    R->Size=C->Size;
    R->Nucleus=(char *)malloc((C->Size+1)*sizeof(char));
    memcpy(R->Nucleus,City->Shared+Pos,(C->Size));
    R->Nucleus[C->Size]=0;
    free(C);
  }
  return R;
}

void AL1_ReadHeader(struct Colony *City){
  unsigned long int Gamma=0;
  unsigned long int A;
  memcpy(&City->Population,City->Shared+60,sizeof(unsigned long int));
  memcpy(&A,City->Shared+70,sizeof(unsigned long int));
  City->LastPos=A;
  Gamma=(FATSIZE-FATSTART)/3;
  City->StartPos=(char *)FATSTART;
  City->FinishPos=(char *)(FATSTART+Gamma);
  City->Cached=(char *)(FATSTART+(Gamma<<1));
  City->CacheProb=0;
  City->CacheUnkn=0;
  City->CacheDate=0;
  City->CachedTime=NULL;
}

void AL1_WriteHeader(struct Colony *City){
  unsigned long int A=SHMSIZE,B=FATSIZE,C=FATSTART,D=City->LastPos;
  snprintf(City->Shared,10,"Amoeba DB");
  memcpy(City->Shared+30,&A,sizeof(unsigned long int));
  memcpy(City->Shared+40,&B,sizeof(unsigned long int));
  memcpy(City->Shared+50,&C,sizeof(unsigned long int));
  memcpy(City->Shared+60,&(City->Population),sizeof(unsigned long int));
  memcpy(City->Shared+70,&D,sizeof(unsigned long int));
}

void AL1_InitHeader(struct Colony *City){
  unsigned long int Gamma=0;
  char *Name=StrCat("","->BLANK<-");
  struct Cytoskelenton *C=(struct Cytoskelenton *)malloc(sizeof(struct Cytoskelenton));
  City->Size=SHMSIZE;
  memset(City->Shared,0,City->Size);  
  AL1_LockColony(City);
  City->Population=0;
  Gamma=(FATSIZE-FATSTART)/3;
  City->StartPos=(char *)FATSTART;
  City->FinishPos=(char *)(FATSTART+Gamma);
  City->Cached=(char *)(FATSTART+(Gamma<<1));
  City->LastPos=((unsigned long int)AL1_IntColony(City,City->Shared)+FATSIZE);
  AL1_WriteHeader(City);
  AL1_UnlockColony(City);
  City->CacheProb=0;
  City->CacheUnkn=0;
  City->CacheDate=0;
  City->CachedTime=NULL;
  
  /* Need to create blank record to link deletions back to */
  C->Dimension=0;
  C->Pseudopods=NULL;
  C->Core=(struct Blob *)malloc(sizeof(struct Blob));
  C->Core->Size=C->Size=strlen(Name);
  C->Core->Nucleus=Name;
  C->Size=C->Core->Size;
  AL1_AddDBEntry(City,C);
  AL1_FreeAmoeba(C);
}

int AL1_DelDBEntry(struct Colony *City,unsigned long int RecordID){
  unsigned long int Start,End;
  if((RecordID<City->Population)&&(RecordID)){
    City->Population--;
    Start=AL1_GetStartHeader(City,RecordID);
    End=AL1_GetFinishHeader(City,RecordID);
    memset(City->Shared+Start,0,End-Start);
    Start=AL1_GetStartHeader(City,0);
    End=AL1_GetFinishHeader(City,0);
    AL1_UpdateHeader(City,RecordID,Start,End);
    CACHE_DELETED=1;
    return 1;
  }
  return 0;
}

void *AL1_SearchThread(void *Data){
  unsigned long int _,Pos;
  struct Blob *Name;
  struct SearchThreadData *D=Data;
  for(_=(D->ThreadID+1);(_<D->City->Population)&&(!*D->Terminate);_+=MAXTHREADS){
    Pos=AL1_GetCacheHeader(D->City,_);
    Name=AL1_GetRecName(D->City,Pos);
    if(BlobSame(Name,D->Needle)){
      *D->Cached=_;
      *D->Location=Pos;
      *D->Terminate=1;
    }
    AL1_FreeBlob(Name);
  }
  return NULL;
}

unsigned long int AL1_FindDBEntry(struct Colony *City,struct Blob *Name){
  int                     ThreadID=0,Terminate=0;  
  unsigned long int       R=-1,Location=-1,Cached=-1;
  pthread_t               TID[MAXTHREADS];
  struct SearchThreadData S[MAXTHREADS];
  void                    **Ignored=NULL;
  for(ThreadID=0;ThreadID<MAXTHREADS;ThreadID++){
    S[ThreadID].ThreadID=ThreadID;
    S[ThreadID].Location=&Location;
    S[ThreadID].Terminate=&Terminate;
    S[ThreadID].Cached=&Cached;
    S[ThreadID].City=City;
    S[ThreadID].Needle=Name;
    pthread_create(&(TID[ThreadID]),NULL,AL1_SearchThread,&S[ThreadID]);
  }
  for(ThreadID=0;ThreadID<MAXTHREADS;ThreadID++)
    pthread_join(TID[ThreadID],Ignored);
  R=*S[0].Location;
  if(R!=-1)
    AL1_RecacheHeader(City,*(S[0].Cached));
  return R;
}

void AL1_AttachColony(struct Colony *City){
  if ((City->Shared = (char *)shmat(City->SHMId, NULL,0))== (char *)-1){
    perror("shmat");
    exit(1);
  }
}

void AL1_DetachColony(struct Colony *City){
  if (shmdt(City->Shared)== -1) 
    perror("shmdt");    
}

void AL1_CreateColony(struct Colony *City){
  int *SHMId;
  unsigned long int *Size;
  void *Shared;
  key_t *Key;
  Shared=City->Shared;
  Size=&City->Size;
  SHMId=&City->SHMId;
  Key=&City->Key;
  Nucleii[Nucleii_Initialised]=City;    
  *Key=SHMKEY+(++Nucleii_Initialised);
  *Size=SHMSIZE;
  if ((*SHMId = shmget (*Key, *Size, 0666)) == -1){
    if(AllowQueen){
      if ((*SHMId = shmget (*Key, *Size, (IPC_CREAT | 0666))) == -1) 
	perror("shmget: shmget failed");
      else{
	City->Queen=1;
      }
    }
    else{
      puts("Amoeba daemon does not seem to be running. Aborting.");
      exit(1);
    }
  }
}

void AL1_DeleteColony(struct Colony *City){
  int R;
  City->Queen=1;
  Nucleii_Initialised--;
  if ((R = shmctl(City->SHMId, IPC_RMID, City->shmid_ds)) == -1) 
    perror("shmctl: shmctl failed");
}

void AL1_LoadColony(struct Colony *City,const char *Filename){
  unsigned long int Offset=0;
  int _;
  FILE *File;
  if((File=fopen(Filename,"r"))==NULL){
    AL1_InitHeader(City);
  }
  else{
    while((_=fgetc(File))!=EOF)
      City->Shared[Offset++]=_;
    fclose(File);
    AL1_ReadHeader(City);
    AL1_UnlockColony(City);
  }
}

void AL1_SaveColony(struct Colony *City,const char *Filename){
  unsigned long int _;  
  FILE *File;
  if((File=fopen(Filename,"w"))==NULL){
    printf("Cannot open file: %s.\n",Filename);
  }
  else{
    AL1_LockColony(City);
    for(_=0;_<City->Size;_++)
      fputc(City->Shared[_],File);
    fclose(File);
    AL1_UnlockColony(City);
  }
}

struct Cytoskelenton *AL1_GetDBEntry(struct Colony *City,unsigned long int RecordID){
  unsigned long int A,B,Pos;
  char *Nucleus=NULL;
  struct Cytoskelenton *C =(struct Cytoskelenton *)malloc(sizeof(struct Cytoskelenton));
  struct Pseudopod     *P;
  struct Mitochondrion *M;
  if(City->Population>RecordID){
    Pos=(unsigned long int)AL1_GetStartHeader(City,RecordID);
    memcpy(C,City->Shared+Pos,sizeof(struct Cytoskelenton));
    Pos+=(sizeof(struct Cytoskelenton)+SHMSPACING);
    Nucleus=(char *)malloc((C->Size+1)*sizeof(char));
    memcpy(Nucleus,City->Shared+Pos,C->Size);
    Nucleus[C->Size]=0;
    C->Core=(struct Blob *)malloc(sizeof(struct Blob));
    C->Core->Size=C->Size;
    C->Core->Nucleus=Nucleus;
    Pos+=(C->Size+SHMSPACING);
    C->Pseudopods=(struct Pseudopod **)malloc(C->Dimension*sizeof(struct Pseudopod *));
    for(A=0;A<C->Dimension;A++){
      P=(struct Pseudopod *)malloc(sizeof(struct Pseudopod ));
      memcpy(P,City->Shared+Pos,sizeof(struct Pseudopod));
      Pos=(Pos+sizeof(struct Pseudopod)+SHMSPACING);
      C->Pseudopods[A]=P;
      C->Pseudopods[A]->Mitochondria=(struct Mitochondrion **)malloc(C->Pseudopods[A]->Dimension*sizeof(struct Mitochondrion *));
      for(B=0;B<C->Pseudopods[A]->Dimension;B++){
	M=(struct Mitochondrion *)malloc(sizeof(struct Mitochondrion ));
	memcpy(M,City->Shared+Pos,sizeof(struct Mitochondrion));
	Pos=(Pos+sizeof(struct Mitochondrion)+SHMSPACING);
	C->Pseudopods[A]->Mitochondria[B]=M;
      }
    }
    return C;
  }
  else{
    printf("%sCan't find record: %ld / %ld %s\n",WC_FLASH,RecordID,City->Population,WC_RESET);
  }
  return NULL;
}

void *AL1_ExtColony(struct Colony *City,void *Int){
  return (void *)((unsigned long int)Int+(unsigned long int)City->Shared); 
}

void *AL1_IntColony(struct Colony *City,void *Ext){
  return (void *)((unsigned long int)Ext-(unsigned long int)City->Shared); 
}

void AL1_Defrag(struct Colony *City){ /* Bug with deleted records */
  unsigned long int _,Fragmented=0,C,A,B,OldB,LastPos,F,G;
  unsigned long int *SortedArray=NULL,*LastArray=NULL,*FirstArray=NULL;
  AL1_DefragAllocTable(City);
  SortedArray=(unsigned long int *)malloc((City->Population)*sizeof(unsigned long int));
  LastArray=(unsigned long int *)malloc((City->Population)*sizeof(unsigned long int));
  FirstArray=(unsigned long int *)malloc((City->Population)*sizeof(unsigned long int));
  for(C=0;C<City->Population;C++){
    SortedArray[C]=C;
    LastArray[C]=(unsigned long int)AL1_GetFinishHeader(City,C);
    FirstArray[C]=(unsigned long int)AL1_GetStartHeader(City,C);
  }
  for(A=1;A<City->Population-1;A++){
    for(B=1;B<City->Population-1;B++){
      if(LastArray[B]>LastArray[B+1]){
	C=LastArray[B+1];
	LastArray[B+1]=LastArray[B];
	LastArray[B]=C;
	C=FirstArray[B+1];
	FirstArray[B+1]=FirstArray[B];
	FirstArray[B]=C;
	C=SortedArray[B+1];
	SortedArray[B+1]=SortedArray[B];
	SortedArray[B]=C;
      }
    }
  }
  for(C=0;C<City->Population;C++){
    A=FirstArray[C];
    B=LastArray[C];
    if(C)
      Fragmented+=(A-F);
    F=B;
  }
  if(Fragmented){
    OldB=FATSIZE;
    Fragmented=0;
    LastPos=City->LastPos;
    for(C=0;C<City->Population;C++){
      A=FirstArray[C];
      B=LastArray[C];
      Fragmented+=(A-OldB);
      if(Fragmented){
	LastPos=(A-Fragmented);
	memmove(City->Shared+LastPos,City->Shared+A,B-A);
	F=A-Fragmented;
	G=B-Fragmented;
	memcpy(City->Shared+((unsigned long int)City->StartPos +(SortedArray[C]*sizeof(C))),&F,sizeof(A));
	memcpy(City->Shared+((unsigned long int)City->FinishPos+(SortedArray[C]*sizeof(C))),&G,sizeof(B));
      }
      OldB=B;
    }
    LastPos=0;
    for(C=0;C<City->Population;C++)
      if((_=(unsigned long int)AL1_GetFinishHeader(City,C))>LastPos)
	LastPos=_;
    City->LastPos=LastPos;
    memcpy(City->Shared+70,&LastPos,sizeof(unsigned long int));
  }
  free(SortedArray);
  free(LastArray);
  free(FirstArray);
}

int AL1_UpdateHeader(struct Colony *City,unsigned long int ID,unsigned long int Start,unsigned long int Finish){
  unsigned long int A;
  if((FATSIZE-FATSTART)/3>(unsigned long int)City->StartPos+(ID*sizeof(ID))){
    City->Population++;
    A=City->LastPos;
    memcpy(City->Shared+((unsigned long int)City->StartPos +(ID*sizeof(ID))),&Start ,sizeof(Start));
    memcpy(City->Shared+((unsigned long int)City->FinishPos+(ID*sizeof(ID))),&Finish,sizeof(Finish));
    memcpy(City->Shared+((unsigned long int)City->Cached   +(ID*sizeof(ID))),&ID    ,sizeof(ID));
    memcpy(City->Shared+60,&(City->Population),sizeof(unsigned long int));
    memcpy(City->Shared+70,&A,sizeof(unsigned long int));
    return 1;
  }
  else
    puts("UpdateHeader: FATSIZE exhausted!");
  return 0;
}

unsigned long int AL1_GetStartHeader(struct Colony *City,unsigned long int ID){
  unsigned long int R;
  memcpy(&R,(City->Shared+((unsigned long int)City->StartPos+(ID*sizeof(ID)))),sizeof(unsigned long int));
  return R;
}

unsigned long int AL1_GetFinishHeader(struct Colony *City,unsigned long int ID){
  unsigned long int R;
  memcpy(&R,(City->Shared+((unsigned long int)City->FinishPos+(ID*sizeof(ID)))),sizeof(unsigned long int));
  return R;
}

unsigned long int AL1_GetCacheHeader(struct Colony *City,unsigned long int ID){
  unsigned long int R;
  memcpy(&R,(City->Shared+((unsigned long int)City->Cached+(ID*sizeof(ID)))),sizeof(unsigned long int));
  return R;
}

void AL1_RecacheHeader(struct Colony *City,unsigned long int ID){
  unsigned long int NewPos=(unsigned long int)City->Cached+((ID>>1)*sizeof(ID)),Value;
  if(NewPos&&ID>1){
    Value=AL1_GetCacheHeader(City,ID);
    memmove(City->Shared+NewPos+sizeof(ID),City->Shared+NewPos,(ID-(ID>>1))*sizeof(ID));
    memcpy(City->Shared+NewPos,&Value,sizeof(ID));
  }
}

void AL1_DelIDHeader(struct Colony *City,unsigned long int ID,unsigned long int Cached){
  unsigned long int MaxPos=((FATSIZE-FATSTART)/3),A,B,C;
  A=(unsigned long int)City->StartPos+ID*sizeof(ID);
  memmove(City->Shared+A,City->Shared+(A+sizeof(ID)),MaxPos-ID*sizeof(ID));
  B=(unsigned long int)City->FinishPos+ID*sizeof(ID);
  memmove(City->Shared+B,City->Shared+(B+sizeof(ID)),MaxPos-ID*sizeof(ID));
  C=(unsigned long int)City->Cached+Cached*sizeof(Cached);
  memmove(City->Shared+C,City->Shared+(C+sizeof(ID)),MaxPos-Cached*sizeof(Cached));
  City->Population--;
  memcpy(City->Shared+60,&(City->Population),sizeof(unsigned long int));
}

void AL1_UnlockColony(struct Colony *City){
  City->Shared[19]='N';
}

void AL1_LockColony(  struct Colony *City){
  char *Lock;
  Lock=&City->Shared[19];
  if(*Lock=='Y'){
    printf("%sWaiting for lock to release...%s",WC_FLASH,WC_RESET);
    fflush(stdout);
    while(*Lock=='Y'){
      usleep(SPINTIME);
    }
    puts(" ok");
  }  
  *Lock='Y';  
}

/*

          /:`:::.     .        :       ...    .,::::::  :::::::.    :::.          
   ,,    /;` ;;`;;    ;;,.    ;;;   .;;;;;;;. ;;;;''''   ;;;'';;'   ;;`;;    ,,   
 ,[['   n[' ,[[ '[[,  [[[[, ,[[[[, ,[[     \[[,[[cccc    [[[__[[\. ,[[ '[[,  '[[, 
c$     c$" c$$$cc$$$c $$$$$$$$"$$$ $$$,     $$$$$""""    $$""""Y$$c$$$cc$$$c    $c
"8bo, o8"   888   888,888 Y88" 888o"888,_ _,88P888oo,__ _88o,,od8P 888   888,,od8"
  "MPmM"    YMM   ""` MMM  M'  "MMM  "YMMMMMP" """"YUMMM""YUMMMP"  YMM   ""` YM"  

*/
