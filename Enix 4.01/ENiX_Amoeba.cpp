/*! \file */

/*

This file is part of ENiX4.

ENiX4 is free software you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

ENiX4 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ENiX4.  If not, see <http://www.gnu.org/licenses/>.

  __    _______   ___ _  __    __ ____  
 / /   / ____/ | / (_) |/ /   / // /\ \ 
/ /   / __/ /  |/ / /|   /   / // /_ \ \
\ \  / /___/ /|  / //   |   /__  __/ / /
 \_\/_____/_/ |_/_//_/|_|     /_/   /_/ 
                                        


Programmed By M.S.Braithwaite 2014.

*/

#include <string.h>
#include <iostream>
#include <unistd.h>
#include <vector>
#include "ENiX_Amoeba.h"

SHMConfig Amoeba::MemConfig;

void Amoeba::Statistics(){
  Statistics("");
}

unsigned long int Amoeba::FirstFreeStartPos(){
  for(size_t _=0;_<MaxRecordNum;_++)
    if(*(unsigned long int *)(DataArea+MemConfig.DBOFFSET+(2*UsableAllocSpace)+(_*sizeof(unsigned long int)))==-1)
      return _;
  return -1;
}

void Amoeba::Statistics(string Prefix){

  double RecordsUsed=100*(double)RecordNum/(double)MaxRecordNum;
  double DefragAllocUsed=100*(double)LastDefragAccel/(double)MaxRecordNum;
  double AllocWriteUsed=100*(double)LastAllocWrittenPos/(double)MaxRecordNum;
  double DataUsed=100*(double)(LastDataWrittenPos-MemConfig.DBALLOCSIZE)/(double)(MemConfig.DBSIZE-MemConfig.DBALLOCSIZE);
  
  putchar('\n');
  printf("%sTotal records used % 3.3f %%\n",Prefix.c_str(),RecordsUsed);
  printf("%sDefrag alloc used  % 3.3f %%\n",Prefix.c_str(),DefragAllocUsed);
  printf("%sWrite alloc used   % 3.3f %%\n",Prefix.c_str(),AllocWriteUsed);
  printf("%sData area used     % 3.3f %%\n",Prefix.c_str(),DataUsed);

  unsigned long int UsedMemory=0;
  unsigned long int FragmentedSize=0;

  unsigned long int _=0,CurrentPosInSHM=0,NextPosInSHM=0,LookupRef=0,FirstWritePos=0,LastWritePos=0,SizeInSHM=0,DefragAllocLastPos=0,BlankSpace=-1,CachedAllocAccelLast=0;
  CurrentPosInSHM=MemConfig.DBALLOCSIZE;
  CachedAllocAccelLast=LastDefragAccel;
  for(_=0;_<CachedAllocAccelLast;_++){
    LookupRef=*(unsigned long int *)(DataArea+MemConfig.DBOFFSET+(4*UsableAllocSpace)+(_*sizeof(unsigned long int)));
    if(LookupRef!=-1){
      FirstWritePos=*(unsigned long int *)(DataArea+MemConfig.DBOFFSET+(2*UsableAllocSpace)+(LookupRef*sizeof(unsigned long int)));
      LastWritePos=*(unsigned long int *)(DataArea+MemConfig.DBOFFSET+(3*UsableAllocSpace)+(LookupRef*sizeof(unsigned long int)));
      SizeInSHM=LastWritePos-FirstWritePos;
      NextPosInSHM=CurrentPosInSHM+SizeInSHM;
      UsedMemory+=SizeInSHM;
      if(FirstWritePos!=CurrentPosInSHM)
	FragmentedSize+=(FirstWritePos-CurrentPosInSHM);
      DefragAllocLastPos++;
      CurrentPosInSHM=NextPosInSHM;
    }
  }

  double PercUsed=100*(double)UsedMemory/(double)(MemConfig.DBSIZE-MemConfig.DBALLOCSIZE);
  double PercFrag=100*(double)FragmentedSize/(double)(MemConfig.DBSIZE-MemConfig.DBALLOCSIZE);

  printf("%sUsage actual       % 3.3f %%\n",Prefix.c_str(),PercUsed);
  printf("%sFragmentation      % 3.3f %%\n",Prefix.c_str(),PercFrag);
  putchar('\n');
}

Amoeba::Amoeba(SHMConfig DBConfig){
  MemConfig=DBConfig;
  char Filename[20];
  sprintf(Filename,"Backup_%d.bak",MemConfig.DBSHMKEY);
  printf("Starting Amoeba...                  \r");
  fflush(stdout);
  //  if(Create(AMOEBADBSIZE,SHMKey))
  if(Create(MemConfig.DBSIZE,MemConfig.DBSHMKEY))
    Queen=true;
  else
    Queen=false;
  Attach(MemConfig.DBSIZE,MemConfig.DBSHMKEY);
  //Attach(MemConfig.DBSIZE,MemConfigSHMKey);
  
  UsableAllocSpace=((MemConfig.DBALLOCSIZE-MemConfig.DBOFFSET)/5);

  /* Check if our database exists on disk */
  if(access(Filename,F_OK)!=-1){
    printf("Attempting to restore from: %s...      \r",Filename);
    fflush(stdout);
    if(!Disk2SHM(Filename))
      puts("ERROR: reading from file.");
    GetDBInfo();
  }
  else{
    printf("%s does not exist, creating...         \r",Filename);
    fflush(stdout);
    CreateEmptyDB();
    PutDBInfo();
    InitFreeList();
  }

  Unlock();
}

Amoeba::~Amoeba(){
  char Filename[20];
  sprintf(Filename,"Backup_%d.bak",Key);
  printf("\r      \n <> [Session:%d] Shutting down Amoeba...\n",getpid());
  if(!SHM2Disk(Filename)){
    printf("ERROR: Cannot write %s to disk.",Filename);
  }
  Detach();
  if(Queen)
    Destroy();
}

unsigned long int Amoeba::GenericSearch(BinData Search,bool (*ComparisonFunc)(unsigned long int A,unsigned long int B)){
  unsigned long int _=0,NewPos=0,StaticPos=0;
  FoundLocation=-1; 
  Found=false;
  vector <SearchThreadData> STData(SEARCHTHREADS); 
  vector <pthread_t> Threads(SEARCHTHREADS);
  for(_=0;_<SEARCHTHREADS;_++){
    STData[_].Found=&Found;
    STData[_].FoundLocation=&FoundLocation;
    STData[_].Offset=_;
    STData[_].SearchData=&Search;
    STData[_].RecordNum=&RecordNum;
    STData[_].UsableAllocSpace=&UsableAllocSpace;
    STData[_].DataArea=DataArea;
    STData[_].ComparisonFunc=ComparisonFunc;
    pthread_create(&Threads[_],NULL,GenericSearchThread,&STData[_]); 
  }
  for(_=0;_<SEARCHTHREADS;_++){
    if(pthread_join(Threads[_],NULL)){
      puts("Error joining thread...");
      return -1;
    }
  }
  if(FoundLocation!=-1){
    StaticPos=*(unsigned long int *)(DataArea+MemConfig.DBOFFSET+FoundLocation*sizeof(unsigned long int));
    NewPos=FoundLocation>>1;
    memmove(DataArea+MemConfig.DBOFFSET+(NewPos+1)*sizeof(unsigned long int),DataArea+MemConfig.DBOFFSET+(NewPos)*sizeof(unsigned long int),(FoundLocation-NewPos)*sizeof(unsigned long int));
    *(unsigned long int *)(DataArea+MemConfig.DBOFFSET+NewPos*sizeof(unsigned long int))=StaticPos;
    return StaticPos;
  }
  return -1;
}

bool Amoeba::EqualTo(unsigned long int A,unsigned long int B){
  if(A==B)
    return true;
  return false;
}

bool Amoeba::GreaterOrEqual(unsigned long int A,unsigned long int B){
  if(A>=B)
    return true;
  return false;
}

unsigned long int Amoeba::Search(BinData Search){
  return GenericSearch(Search,EqualTo);
}

unsigned long int Amoeba::PartSearch(BinData Search){
  return GenericSearch(Search,GreaterOrEqual);
}

void *Amoeba::GenericSearchThread(void *SearchTD){
  unsigned long int Pos=0,StaticPos=0,AllocPos=0,DataStartPos=0,DataSize=0;
  char *RecordData=NULL,*DataArea=((struct SearchThreadData *)SearchTD)->DataArea;
  unsigned long int UsableAllocSpace=*(((struct SearchThreadData *)SearchTD)->UsableAllocSpace);
  unsigned long int RecordNum=*(((struct SearchThreadData *)SearchTD)->RecordNum);
  for(Pos=((struct SearchThreadData *)SearchTD)->Offset;Pos<RecordNum;Pos+=SEARCHTHREADS){
    if(*(((struct SearchThreadData *)SearchTD)->Found)==true)
      pthread_exit(NULL);
    StaticPos=*(unsigned long int *)(DataArea+MemConfig.DBOFFSET+Pos*sizeof(unsigned long int));
    AllocPos=*(unsigned long int *)(DataArea+MemConfig.DBOFFSET+UsableAllocSpace+StaticPos*sizeof(unsigned long int));
    DataStartPos=(*(unsigned long int *)(DataArea+MemConfig.DBOFFSET+2*UsableAllocSpace+AllocPos*sizeof(unsigned long int)))+(sizeof(unsigned long int)<<1);
    DataSize=*(unsigned long int *)(DataArea+DataStartPos);
    DataStartPos+=sizeof(unsigned long int);
    if(((struct SearchThreadData *)SearchTD)->ComparisonFunc(DataSize,((SearchThreadData *)SearchTD)->SearchData->Size)==true){
      if(!memcmp(DataArea+DataStartPos,((SearchThreadData *)SearchTD)->SearchData->Str,((SearchThreadData *)SearchTD)->SearchData->Size)){
	*((struct SearchThreadData *)SearchTD)->Found=true;
	*((struct SearchThreadData *)SearchTD)->FoundLocation=Pos;
	pthread_exit(NULL);
      }
    }
  }
  pthread_exit(NULL);
}

void Amoeba::InitFreeList(string Prefix){
  unsigned long int Pos=0,Index=0,Blank=-1;
  //  puts("Initialising free list...\n");
  for(Pos=0;Pos<MaxRecordNum;Pos++){
    memcpy(DataArea+MemConfig.DBOFFSET+(Pos*sizeof(unsigned long int)),&Index,sizeof(unsigned long int));
    memcpy(DataArea+MemConfig.DBOFFSET+(4*UsableAllocSpace)+(Pos*sizeof(unsigned long int)),&Blank,sizeof(unsigned long int));
    memcpy(DataArea+MemConfig.DBOFFSET+(1*UsableAllocSpace)+(Pos*sizeof(unsigned long int)),&Blank,sizeof(unsigned long int));
    memcpy(DataArea+MemConfig.DBOFFSET+(2*UsableAllocSpace)+(Pos*sizeof(unsigned long int)),&Blank,sizeof(unsigned long int));
    memcpy(DataArea+MemConfig.DBOFFSET+(3*UsableAllocSpace)+(Pos*sizeof(unsigned long int)),&Blank,sizeof(unsigned long int));
    Index++;
  }
}
 
void Amoeba::CreateEmptyDB(){
  ClearSHM();
  /* Part of the alloc table to write to excluding metrics and locking. */
  MaxRecordNum=((UsableAllocSpace)/sizeof(unsigned long int));
  RecordNum=0;
  FreeRecordNum=MaxRecordNum;
  LastAllocWrittenPos=0;
  LastDataWrittenPos=MemConfig.DBALLOCSIZE;
  LastDefragAccel=0;
}

void Amoeba::PutDBInfo(){
  /* First character is the lock char */
  unsigned long int Pos=1;
  /* All of this must fit in AMOEBAOFFSET */
  Pos=WriteAllocPos(&MaxRecordNum,sizeof(unsigned long int),Pos);
  Pos=WriteAllocPos(&RecordNum,sizeof(unsigned long int),Pos);
  Pos=WriteAllocPos(&FreeRecordNum,sizeof(unsigned long int),Pos);
  Pos=WriteAllocPos(&LastAllocWrittenPos,sizeof(unsigned long int),Pos);
  Pos=WriteAllocPos(&LastDataWrittenPos,sizeof(unsigned long int),Pos);
  Pos=WriteAllocPos(&LastDefragAccel,sizeof(unsigned long int),Pos);
}

unsigned long int Amoeba::WriteAllocPos(void *DataToWrite,size_t DataSize,unsigned long int Offset){
  memcpy(DataArea+Offset,DataToWrite,DataSize);
  return Offset+DataSize;
}

void Amoeba::GetDBInfo(){
  /* First character is the lock char */
  unsigned long int Pos=1;

  /* all of this must fit in AMOEBAOFFSET */
  MaxRecordNum=*(unsigned long int *)(DataArea+Pos);
  Pos+=sizeof(unsigned long int);
  RecordNum=*(unsigned long int *)(DataArea+Pos);
  Pos+=sizeof(unsigned long int);
  FreeRecordNum=*(unsigned long int *)(DataArea+Pos);
  Pos+=sizeof(unsigned long int);
  LastAllocWrittenPos=*(unsigned long int *)(DataArea+Pos);
  Pos+=sizeof(unsigned long int);
  LastDataWrittenPos=*(unsigned long int *)(DataArea+Pos);
  Pos+=sizeof(unsigned long int);
  LastDefragAccel=*(unsigned long int *)(DataArea+Pos);
  Pos+=sizeof(unsigned long int);
}

unsigned long int Amoeba::AddData(BinData Data){
  unsigned long int DataStart=0,DataFinish=0,FirstFreeIndex=0;

  /* take out lock on the database */
  if(FreeRecordNum==0){
    puts("ERROR: Cannot add record, alloc table full.");
    return -1;
  }
  if((LastDataWrittenPos+Data.Size>MemConfig.DBSIZE)||(LastDefragAccel==MaxRecordNum)){
    Defrag();
    if(LastDataWrittenPos+Data.Size>MemConfig.DBSIZE){
      puts("ERROR: Cannot add record, database full.");
      return -1;
    }
  }

  /* Write data to data area */
  DataStart=LastDataWrittenPos;
  memcpy(DataArea+DataStart,Data.Str,Data.Size);
  DataFinish=DataStart+Data.Size;

  /* Get the first free index */
  FirstFreeIndex=*(unsigned long int *)(DataArea+MemConfig.DBOFFSET+(RecordNum*sizeof(unsigned long int)));
  unsigned long int FirstAlloc=FirstFreeStartPos(); // Added

  /* Add LastAllocPosition to the index list at position FirstFreeIndex (pointing to the alloc table) */
  memcpy(DataArea+MemConfig.DBOFFSET+(1*UsableAllocSpace)+(FirstFreeIndex*sizeof(unsigned long int)),&FirstAlloc,sizeof(unsigned long int));
  //  memcpy(DataArea+AMOEBAOFFSET+(1*UsableAllocSpace)+(FirstFreeIndex*sizeof(unsigned long int)),&LastAllocWrittenPos,sizeof(unsigned long int));
  
  /* Write the start position to the start alloc table */
  memcpy(DataArea+MemConfig.DBOFFSET+(2*UsableAllocSpace)+(FirstAlloc*sizeof(unsigned long int)),&DataStart,sizeof(unsigned long int));
  //  memcpy(DataArea+AMOEBAOFFSET+(2*UsableAllocSpace)+(LastAllocWrittenPos*sizeof(unsigned long int)),&DataStart,sizeof(unsigned long int));

  /* Write the end position to the end alloc table */
  memcpy(DataArea+MemConfig.DBOFFSET+(3*UsableAllocSpace)+(FirstAlloc*sizeof(unsigned long int)),&DataFinish,sizeof(unsigned long int));
  // memcpy(DataArea+AMOEBAOFFSET+(3*UsableAllocSpace)+(LastAllocWrittenPos*sizeof(unsigned long int)),&DataFinish,sizeof(unsigned long int));

  /* Add LastAllocPosition to the index list at position FirstFreeIndex (pointing to the alloc table) */
  memcpy(DataArea+MemConfig.DBOFFSET+(4*UsableAllocSpace)+(LastDefragAccel*sizeof(unsigned long int)),&FirstAlloc,sizeof(unsigned long int));
  // memcpy(DataArea+AMOEBAOFFSET+(4*UsableAllocSpace)+(LastDefragAccel*sizeof(unsigned long int)),&LastAllocWrittenPos,sizeof(unsigned long int));

  
  /* Increment the record count */
  RecordNum++;
  FreeRecordNum--;
  LastAllocWrittenPos++;
  LastDataWrittenPos=DataFinish;
  
  LastDefragAccel++;
  PutDBInfo();
  
  /* unlock the database */
  
  return FirstFreeIndex;
}

BinData Amoeba::GetData(unsigned long int Reference){
  unsigned long int AllocTablePos=0,DataStart=0,DataFinish=0;
  if(Reference==-1)
    return {};
  /* This is the data in the 2nd part of the alloc table */
  AllocTablePos=*(unsigned long int *)(DataArea+MemConfig.DBOFFSET+(1*UsableAllocSpace)+(Reference*sizeof(unsigned long int)));
  if(AllocTablePos==-1){
    BinData Retrieved(NULL,0);
    return Retrieved;
  }
  /* Get the start and end positions in the data area */
  DataStart=*(unsigned long int *)(DataArea+MemConfig.DBOFFSET+(2*UsableAllocSpace)+(AllocTablePos*sizeof(unsigned long int)));
  DataFinish=*(unsigned long int *)(DataArea+MemConfig.DBOFFSET+(3*UsableAllocSpace)+(AllocTablePos*sizeof(unsigned long int)));
  BinData Retrieved(DataArea+DataStart,DataFinish-DataStart);
  return Retrieved;
}

/*
  Should take a static reference of the record and return its name
 */
BinData Amoeba::GetName(unsigned long int StaticReference){
  unsigned long int DataSize=0,DataStartPos=0,AllocTablePos=0;
  if(StaticReference==-1)
    return {};
  /* This is the data in the 2nd part of the alloc table */
  AllocTablePos=*(unsigned long int *)(DataArea+MemConfig.DBOFFSET+(1*UsableAllocSpace)+(StaticReference*sizeof(unsigned long int)));
  if(AllocTablePos==-1){
    BinData Retrieved(NULL,0);
    return Retrieved;
  }
  DataStartPos=(*(unsigned long int *)(DataArea+MemConfig.DBOFFSET+2*UsableAllocSpace+AllocTablePos*sizeof(unsigned long int)))+(sizeof(unsigned long int)<<1);
  DataSize=*(unsigned long int *)(DataArea+DataStartPos);
  DataStartPos+=sizeof(unsigned long int);
  BinData AmoebaPrimaryKey(DataArea+DataStartPos,DataSize);
  return AmoebaPrimaryKey;
}

bool Amoeba::Defrag(){
  bool Defragged=false;
  unsigned long int _=0,CurrentPosInSHM=0,NextPosInSHM=0,LookupRef=0,FirstWritePos=0,LastWritePos=0,SizeInSHM=0,DefragAllocLastPos=0,BlankSpace=-1,CachedAllocAccelLast=0;
  CurrentPosInSHM=MemConfig.DBALLOCSIZE;
  /* Run through all defrag accel alloc, dereference and check for gaps */
  CachedAllocAccelLast=LastDefragAccel;
  for(_=0;_<CachedAllocAccelLast;_++){
    LookupRef=*(unsigned long int *)(DataArea+MemConfig.DBOFFSET+(4*UsableAllocSpace)+(_*sizeof(unsigned long int)));

    if(LookupRef!=-1){

      FirstWritePos=*(unsigned long int *)(DataArea+MemConfig.DBOFFSET+(2*UsableAllocSpace)+(LookupRef*sizeof(unsigned long int)));
      LastWritePos=*(unsigned long int *)(DataArea+MemConfig.DBOFFSET+(3*UsableAllocSpace)+(LookupRef*sizeof(unsigned long int)));

      if((FirstWritePos==-1)||(LastWritePos==-1))
	continue;

      SizeInSHM=LastWritePos-FirstWritePos;
      NextPosInSHM=CurrentPosInSHM+SizeInSHM;

      if(FirstWritePos!=CurrentPosInSHM){
	Defragged=true;

	/* Move the record */
	memmove(DataArea+CurrentPosInSHM,DataArea+FirstWritePos,SizeInSHM);

	/* Write the start position to the record start alloc table */
	memcpy(DataArea+MemConfig.DBOFFSET+(2*UsableAllocSpace)+(LookupRef*sizeof(unsigned long int)),&CurrentPosInSHM,sizeof(unsigned long int));

	/* Write the end position to the record end alloc table */
	memcpy(DataArea+MemConfig.DBOFFSET+(3*UsableAllocSpace)+(LookupRef*sizeof(unsigned long int)),&NextPosInSHM,sizeof(unsigned long int));

	if(DefragAllocLastPos!=_){
	  /* move the defrag accelerator position from the current position (_) to the last written (DefragAllocLastPos)*/
	  memcpy(DataArea+MemConfig.DBOFFSET+(4*UsableAllocSpace)+(DefragAllocLastPos*sizeof(unsigned long int))
		 ,DataArea+MemConfig.DBOFFSET+(4*UsableAllocSpace)+(_*sizeof(unsigned long int))
		 ,sizeof(unsigned long int));

	  /* set defrag accelerator position (_) to be -1 */
	  memcpy(DataArea+MemConfig.DBOFFSET+(4*UsableAllocSpace)+(_*sizeof(unsigned long int))
		 ,&BlankSpace
		 ,sizeof(unsigned long int));
	}
      }
      DefragAllocLastPos++;
      CurrentPosInSHM=NextPosInSHM;
      LastDefragAccel=DefragAllocLastPos;      
    }
  }
  LastDataWrittenPos=LastWritePos;
  if(Defragged)
    PutDBInfo();
  return Defragged;
}

bool Amoeba::RepData(BinData Data,unsigned long int Reference){
  unsigned long int AllocRef=0,OriginalStartPos=0,OriginalFinishPos=0,NewStartPos=0,NewFinishPos=0,_=0,OldDefragAccelPos=-1,DeletedDataRef=-1;

  /* Locate data area */
  AllocRef=*(unsigned long int *)(DataArea+MemConfig.DBOFFSET+(1*UsableAllocSpace)+(Reference*sizeof(unsigned long int)));  

  if(AllocRef!=-1){
    OriginalStartPos=*(unsigned long int *)(DataArea+MemConfig.DBOFFSET+(2*UsableAllocSpace)+(AllocRef*sizeof(unsigned long int)));
    OriginalFinishPos=*(unsigned long int *)(DataArea+MemConfig.DBOFFSET+(3*UsableAllocSpace)+(AllocRef*sizeof(unsigned long int)));
  
    if(Data.Size>(OriginalFinishPos-OriginalStartPos)){
      NewStartPos=LastDataWrittenPos;
      NewFinishPos=NewStartPos+Data.Size;

      /* TODO check that the finish position is not later than the size of the SHM area */
      if((LastDataWrittenPos+Data.Size>MemConfig.DBSIZE)||(LastDefragAccel==MaxRecordNum)){
	Defrag();
	if(LastDataWrittenPos+Data.Size>MemConfig.DBSIZE){
	  puts("ERROR: Cannot add record, database full.");
	  return false;
	}
      }

      /* add new object at the end of the data area */
      char *DEBUGSTR=Data.Str;
      size_t DEBUGSZ=Data.Size;
      
      memcpy(DataArea+NewStartPos,Data.Str,Data.Size);
      LastDataWrittenPos+=Data.Size;

      /* 1st / 2nd alloc tables don't change*/
    
      unsigned long int FirstAlloc=FirstFreeStartPos(); // Added
      memcpy(DataArea+MemConfig.DBOFFSET+(1*UsableAllocSpace)+(Reference*sizeof(unsigned long int)),&FirstAlloc,sizeof(unsigned long int)); // Added

      /* Write the start position to the start alloc table */
      memcpy(DataArea+MemConfig.DBOFFSET+(2*UsableAllocSpace)+(FirstAlloc*sizeof(unsigned long int)),&NewStartPos,sizeof(unsigned long int));
      memcpy(DataArea+MemConfig.DBOFFSET+(2*UsableAllocSpace)+(AllocRef*sizeof(unsigned long int)),&DeletedDataRef,sizeof(unsigned long int));
      // memcpy(DataArea+AMOEBAOFFSET+(2*UsableAllocSpace)+(AllocRef*sizeof(unsigned long int)),&NewStartPos,sizeof(unsigned long int));

      /* Write the end position to the end alloc table */
      memcpy(DataArea+MemConfig.DBOFFSET+(3*UsableAllocSpace)+(FirstAlloc*sizeof(unsigned long int)),&NewFinishPos,sizeof(unsigned long int));
      memcpy(DataArea+MemConfig.DBOFFSET+(3*UsableAllocSpace)+(AllocRef*sizeof(unsigned long int)),&DeletedDataRef,sizeof(unsigned long int));
      // memcpy(DataArea+AMOEBAOFFSET+(3*UsableAllocSpace)+(AllocRef*sizeof(unsigned long int)),&NewFinishPos,sizeof(unsigned long int));
    
      LastAllocWrittenPos++;

      /* Need to locate and delete the old defrag accelerator reference (AllocRef), set it to -1 and add a new one pointing to the correct position */
      for(_=0;_<MaxRecordNum;_++){
	//	if(*(unsigned long int*)(DataArea+AMOEBAOFFSET+(4*UsableAllocSpace)+(_*sizeof(unsigned long int)))==AllocRef){
	if(*(unsigned long int*)(DataArea+MemConfig.DBOFFSET+(4*UsableAllocSpace)+(_*sizeof(unsigned long int)))==FirstAlloc){
	  OldDefragAccelPos=_;
	  break;
	}
      }
      memcpy(DataArea+MemConfig.DBOFFSET+(4*UsableAllocSpace)+(OldDefragAccelPos*sizeof(unsigned long int)),&DeletedDataRef,sizeof(unsigned long int));
    
      /* Add AllocRef to the defrag accel at position LastDefragAccel (pointing to the alloc table) */
      memcpy(DataArea+MemConfig.DBOFFSET+(4*UsableAllocSpace)+(LastDefragAccel*sizeof(unsigned long int)),&FirstAlloc,sizeof(unsigned long int));
      // memcpy(DataArea+AMOEBAOFFSET+(4*UsableAllocSpace)+(LastDefragAccel*sizeof(unsigned long int)),&AllocRef,sizeof(unsigned long int));

      LastDefragAccel++;
      PutDBInfo();
    }
    else{
      /* Replace the entry */
      memcpy(DataArea+OriginalStartPos,Data.Str,Data.Size);
      NewFinishPos=OriginalStartPos+Data.Size;
      memcpy(DataArea+MemConfig.DBOFFSET+(3*UsableAllocSpace)+(AllocRef*sizeof(unsigned long int)),&NewFinishPos,sizeof(unsigned long int));
    }
    return true;
  }
  else{
    AddData(Data);
  }
  return false;
}

bool Amoeba::DelData(unsigned long int Reference){
  unsigned long int AllocRef=0,Blank=-1,_=0,SearchLocation=0;
  
  /* Locate data area */
  AllocRef=*(unsigned long int *)(DataArea+MemConfig.DBOFFSET+(1*UsableAllocSpace)+(Reference*sizeof(unsigned long int)));
  
  /* Overwrite the alloc table entries for start and finish */
  memcpy(DataArea+MemConfig.DBOFFSET+(2*UsableAllocSpace)+(AllocRef*sizeof(unsigned long int)),&Blank,sizeof(unsigned long int));
  memcpy(DataArea+MemConfig.DBOFFSET+(3*UsableAllocSpace)+(AllocRef*sizeof(unsigned long int)),&Blank,sizeof(unsigned long int));

  /* Find the index that points to AllocRef swap it with the last free entry in the search index */
  for(_=0;_<RecordNum;_++){
    if(*(unsigned long int *)(DataArea+MemConfig.DBOFFSET+(_*sizeof(unsigned long int)))==Reference){
      SearchLocation=_;
      break;
    }
  }
  memcpy(DataArea+MemConfig.DBOFFSET+(SearchLocation*sizeof(unsigned long int)),DataArea+MemConfig.DBOFFSET+((RecordNum-1)*sizeof(unsigned long int)),sizeof(unsigned long int));
  memcpy(DataArea+MemConfig.DBOFFSET+((RecordNum-1)*sizeof(unsigned long int)),&Reference,sizeof(unsigned long int));

  /* Deallocate from the ptr list */
  memcpy(DataArea+MemConfig.DBOFFSET+(1*UsableAllocSpace)+(Reference*sizeof(unsigned long int)),&Blank,sizeof(unsigned long int));
  
  /* Decrement the record count */
  RecordNum--;
  FreeRecordNum++;
  PutDBInfo();
}

void Amoeba::DebugAllocTable(string Prefix){
  unsigned long int _=0,AllocTablePos=0,DataStart=0,DataFinish=0;
  const char *CharPrefix=Prefix.c_str();
  printf("%s=[Amoeba Alloc Table Debug]================================\n",CharPrefix);
  for(_=0;_<RecordNum;_++){
    /* This is the data in the 2nd part of the alloc table */
    AllocTablePos=*(unsigned long int *)(DataArea+MemConfig.DBOFFSET+(1*UsableAllocSpace)+(_*sizeof(unsigned long int)));
    if(AllocTablePos!=(unsigned long int)-1){
      /* Get the start and end positions in the data area */
      DataStart=*(unsigned long int *)(DataArea+MemConfig.DBOFFSET+(2*UsableAllocSpace)+(AllocTablePos*sizeof(unsigned long int)));
      DataFinish=*(unsigned long int *)(DataArea+MemConfig.DBOFFSET+(3*UsableAllocSpace)+(AllocTablePos*sizeof(unsigned long int)));
    }
    else{
      DataStart=0;
      DataFinish=0;
    }
    printf("%s[%09ld]: [%09ld] [%09ld - %09ld][",CharPrefix,(unsigned long int)_,(unsigned long int)AllocTablePos,(unsigned long int)DataStart,(unsigned long int)DataFinish);
    BinData AttrName=GetName(_);
    AttrName.PrintIfASCII();
    printf("]\n");
  }
  putchar('\n');
  DebugAllocGeneral(Prefix);
}

void Amoeba::DebugAllocGeneral(string Prefix){
  unsigned long int _=0,Alloc[5],Index=0;
  const char *CharPrefix=Prefix.c_str();
  printf("%s-[Amoeba Alloc General Debug]------------------------------\n",CharPrefix);
  printf("%s[0:SE/FRE] [1:F-ORDE] [2:START....3:FINISH] [4:F-ACCE]\n",CharPrefix);
  for(_=0;_<RecordNum+3;_++){
    Alloc[0]=*(unsigned long int *)(DataArea+MemConfig.DBOFFSET+(_*sizeof(unsigned long int)));
    Alloc[1]=*(unsigned long int *)(DataArea+MemConfig.DBOFFSET+(1*UsableAllocSpace)+(_*sizeof(unsigned long int)));
    Alloc[2]=*(unsigned long int *)(DataArea+MemConfig.DBOFFSET+(2*UsableAllocSpace)+(_*sizeof(unsigned long int)));
    Alloc[3]=*(unsigned long int *)(DataArea+MemConfig.DBOFFSET+(3*UsableAllocSpace)+(_*sizeof(unsigned long int)));
    Alloc[4]=*(unsigned long int *)(DataArea+MemConfig.DBOFFSET+(4*UsableAllocSpace)+(_*sizeof(unsigned long int)));
    printf("%s",CharPrefix);
    for(Index=0;Index<5;Index++){
      if(Alloc[Index]==-1)
	printf("BLANK      ");
      else
	printf("%09lu  ",(unsigned long int)Alloc[Index]);
    }
    putchar('\n');
  }
  printf("%s===========================================================\n",CharPrefix);
}

void Amoeba::DebugDatabase(string Prefix){
  const char *CharPrefix=Prefix.c_str();
  printf("%s=[Amoeba Metrics]==========================================\n",CharPrefix);
  printf("%sSize of alloc table / database: %lu/%lu bytes\n",CharPrefix,(unsigned long int)MemConfig.DBALLOCSIZE,(unsigned long int)MemConfig.DBSIZE);
  printf("%sNumber of records: %ld/%ld, %ld free\n",CharPrefix,RecordNum,MaxRecordNum,FreeRecordNum);
  printf("%sLast written position: %ld (alloc), %ld (data), %ld (accel)\n",CharPrefix,LastAllocWrittenPos,LastDataWrittenPos,LastDefragAccel);
  printf("%sSHM Key: %d, Usable alloc space: %ld\n",CharPrefix,Key,UsableAllocSpace);
  printf("%s===========================================================\n",CharPrefix);
}

void Amoeba::DebugAllocTable(){
  DebugAllocTable("");
}

void Amoeba::DebugAllocGeneral(){
  DebugAllocGeneral("");
}

void Amoeba::DebugDatabase(){
  DebugDatabase("");
}

void Amoeba::ClearSHM(){
  printf("Clearing database...        \r");
  memset(DataArea,0,Size);
}

unsigned long int Amoeba::GetIndexFromNumber(unsigned long int RecordNum){
  return *(unsigned long int *)(DataArea+MemConfig.DBOFFSET+(RecordNum*sizeof(unsigned long int)));
}

/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
 */
