/*! \file */

#include "WMS_SHM.h"
#include "WMS_Blob.h"
#include "ENiX_LIST.h"
#include "WitchCraft-Colours.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

extern char *YesNo[];

/*! \brief Delete the inposition value in the alloc table move it all along.
 *  - the inpos is for O(n) defragging.
 */
void WMS_DelInpos(struct DataArea *DB,unsigned long int Position){
  unsigned long int _=0,MoveFrom=0;
  for(_=0;_<DB->Population;_++)
    if(*(unsigned long int *)(DB->InPos+_*sizeof(unsigned long int))==Position)
      break;
  for(;_<DB->Population;_++){
    MoveFrom=*(unsigned long int *)(DB->InPos+(_+1)*sizeof(unsigned long int));
    *(unsigned long int *)(DB->InPos+_*sizeof(unsigned long int))=MoveFrom-1;    
  }
}

/*! \brief Move a inposition value to the end of the alloc table.
 *  - the inpos is for O(n) defragging.
 */
void WMS_Mv2LastInpos(struct DataArea *DB,unsigned long int Position){
  unsigned long int _=0,MoveFrom=0,MoveTo=0,FinalEntry=0;
  for(_=0;_<DB->Population;_++)
    if(*(unsigned long int *)(DB->InPos+_*sizeof(unsigned long int))==Position)
      break;
  FinalEntry=*(unsigned long int *)(DB->InPos+_*sizeof(unsigned long int));
  MoveFrom=(_+1)*sizeof(unsigned long int);
  MoveTo=_*sizeof(unsigned long int);
  /* has to be -2 because it follows an addition operation which is actually a replacement
     so the population has to be deducted again later. */
  memmove(DB->InPos+MoveTo,DB->InPos+MoveFrom,(DB->Population-_-2)*sizeof(unsigned long int));  
  *(unsigned long int *)(DB->InPos+(DB->Population-2)*sizeof(unsigned long int))=FinalEntry;
}

/*! \brief Create a SHM memory area.
 *  
 */
struct DataArea *WMS_SHM_Create(unsigned int Sector){
  struct DataArea *DB=(struct DataArea *)malloc(sizeof(struct DataArea));
  DB->LockAlloc=1;
  DB->Sector=Sector;
  DB->Size=SHMSIZE;
  DB->Key=SHMKEY+Sector;
  DB->Queen=0;
  DB->Population=0;
  DB->LastPos=FATSIZE;
  DB->Shared=NULL;
  DB->shmid_ds=NULL;
  if((DB->SHM_ID=shmget(DB->Key,DB->Size,0666))==-1){
    if((DB->SHM_ID=shmget(DB->Key,DB->Size,(IPC_CREAT|0666)))==-1)  
      return NULL;
    else{
      DB->Queen=1;
    }
  }
  else
    puts("Queen already running.");
  DB->LockAlloc=0;
  return DB;
}

/*! \brief Delete a SHM memory area.
 *  
 */
unsigned int WMS_SHM_Delete(struct DataArea *DB){
  int R=0;
  if(DB){
    
    /* TODO: should only delete if a queen */
    if((R=shmctl(DB->SHM_ID,IPC_RMID,DB->shmid_ds))==-1){
      perror("shmctl: shmctl failed");
      free(DB);
      return 0;
    }
    free(DB);
    return 1;
  }
  return 0;
}

/*! \brief Attach to a SHM memory area.
 *  
 */
unsigned int WMS_SHM_Attach(struct DataArea *DB){
  unsigned long int AllocSize=0;
  if(DB){
    if((DB->Shared=(char *)shmat(DB->SHM_ID,NULL,0))==(char *)-1)
      return 0;
    DB->LockAlloc=1;
    DB->Cached=DB->Shared+FATSTART;
    AllocSize=(FATSIZE-FATSTART)/ALLOCSECTIONS;
    DB->StartPos=DB->Shared+(FATSTART+AllocSize);
    DB->FinishPos=DB->Shared+(FATSTART+(AllocSize<<1));
    DB->Lock=DB->Shared+(FATSTART+3*AllocSize);
    DB->InPos=DB->Shared+(FATSTART+(AllocSize<<2));
    DB->MaxAllocEntries=AllocSize/sizeof(unsigned long int);
    DB->LockAlloc=0;
    return 1;
  }
  return 0;
}

/*! \brief Detach a SHM memory area.
 *  
 */
unsigned int WMS_SHM_Detach(struct DataArea *DB){
  if(DB){
    if(shmdt(DB->Shared)==-1)
      return 0;
    return 1;
  }
  return 0;
}

/*! \brief Show SHM debug statistics.
 *  
 */
void WMS_SHM_Debug(struct DataArea *DB){
  if(DB){
    printf("Sector: %d, Size: %ld, SHM ID: %d\n",DB->Sector,DB->Size,DB->SHM_ID);
    printf("Queen: %s, Population: %ld\n",YesNo[DB->Queen],DB->Population);
    printf("SharedPos: %p, StartPos: %p, FinishPos: %p, CachedPos: %p\n",DB->Shared,DB->StartPos,DB->FinishPos,DB->Cached);
    printf("LastPos: %ld\n",DB->LastPos);
  }
  else{
    puts("SHM not initialised!");
  }
}

/*! \brief Show the contents of the alloc table.
 *  
 */
void WMS_ALLOC_DEBUG(struct DataArea *DB){
  char Lock=0,*RecordName=NULL;
  struct Blob *B=NULL;
  unsigned long int _=0,Start=0,Finish=0,Cached=0,InPos=0;
  putchar('\n');
  if(DB)
    printf("\nSector: %d\n",DB->Sector);
  puts("-[Record]---[InPos]----[Start]----[Finish]---[Cached]-[L]-[Name]-");
  if(DB){
    WMS_LockDB(DB);
    for(_=0;_<DB->Population;_++){
      Start=*(unsigned long int *)(DB->StartPos+_*sizeof(unsigned long int));
      Finish=*(unsigned long int *)(DB->FinishPos+_*sizeof(unsigned long int));
      Cached=*(unsigned long int *)(DB->Cached+_*sizeof(unsigned long int));
      InPos=*(unsigned long int *)(DB->InPos+_*sizeof(unsigned long int));
      Lock=*(char *)(DB->Lock+_);
      B=WMS_GetBinary(DB,_);
      RecordName=WMS_GetRecName(B);
      printf("%010ld %010ld %010ld %010ld %010ld %d  %s\n",_,InPos,Start,Finish,Cached,(int)Lock,RecordName);     
      WMS_FreeBlob(B);
      free(RecordName);
    }
    WMS_UnlockDB(DB);
  }
  puts("-----------------------------------------------------------------");
}

/*! \brief Dump the contents of the database for inspection.
 *  
 */
void WMS_SHM_Dump(struct DataArea *DB){
  unsigned long int _=0;
  struct Blob *B=NULL;  
  if(DB){
    for(_=0;_<DB->Population;_++){
      B=WMS_GetBinary(DB,_);
      WMS_BlobDEBUG(B);
      WMS_FreeBlob(B);
    }
  }
}

/*! \brief Nuke the database allocation table.
 *  
 */
unsigned int WMS_SHM_Clear(struct DataArea *DB){
  if(DB){
    /* memset(DB->Shared+FATSTART,0,FATSIZE-FATSTART); */
    DB->Population=0;
    DB->LockAlloc=0;
    DB->LastPos=FATSIZE;
    return 1;
  }
  return 0;
}

/*! \brief Add a record to the database.
 *  - Takes out a lock 
 */
long int WMS_AddBinary(struct DataArea *DB,struct Blob *B){  
  unsigned long int EndPos=0;
  long int Position=-1;  
  if(DB->Population>=DB->MaxAllocEntries)
    return -1;
  if(DB&&B){
    if((DB->LastPos+B->Size)>=DB->Size){
      WMS_Defrag(DB);
      if((DB->LastPos+B->Size)>=DB->Size)
	return -2;
    }
    WMS_LockDB(DB);
    Position=DB->Population;
    *(char *)(DB->Lock+DB->Population)=1;
    memcpy(DB->Shared+(DB->LastPos),B->Nucleus,B->Size);
    *(unsigned long int *)(DB->InPos+(DB->Population*sizeof(unsigned long int)))=DB->Population;
    *(unsigned long int *)(DB->Cached+(DB->Population*sizeof(unsigned long int)))=DB->Population;
    *(unsigned long int *)(DB->StartPos+(DB->Population*sizeof(unsigned long int)))=DB->LastPos;
    EndPos=DB->LastPos+B->Size;
    *(unsigned long int *)(DB->FinishPos+(DB->Population*sizeof(unsigned long int)))=EndPos;
    DB->LastPos+=(B->Size+SHMSPACING);
    *(char *)(DB->Lock+DB->Population)=0;
    DB->Population++;
    WMS_UnlockDB(DB);
  }
  return Position;
}

/*! \brief Get a binary in a known position from the database.
 *  - Does not take a lock. 
 *  - Called from WMS_FindBinary
 */
struct Blob *WMS_GetBinary(struct DataArea *DB,unsigned long int Position){
  unsigned long int StartPos=0,FinishPos=0;
  struct Blob *B=NULL;
  if(DB&&(Position<DB->Population)){
    /*
      WMS_LockDB(DB);
    */
    B=(struct Blob *)malloc(sizeof(struct Blob));
    StartPos=*(unsigned long int *)(DB->StartPos+(Position*sizeof(unsigned long int)));
    FinishPos=*(unsigned long int *)(DB->FinishPos+(Position*sizeof(unsigned long int)));
    B->Size=FinishPos-StartPos;
    B->Nucleus=(char *)malloc((B->Size+1)*sizeof(char));
    memcpy(B->Nucleus,DB->Shared+StartPos,B->Size);
    B->Nucleus[B->Size]=0;
    /*
      WMS_UnlockDB(DB); 
    */
    return B;
  }
  return NULL;
}

/*! \brief Delete the record in a known position from the database.
 *  - Takes out a lock
 */
unsigned int WMS_DelBinary(struct DataArea *DB,unsigned long int Position){
  char *CachedPos=NULL;
  unsigned long int _=0,MvStart=0,MvFinish=0,MvLen=0,CachedRec=0;
  if(DB&&(Position<DB->Population)){
    WMS_LockDB(DB);
    MvFinish=(FATSIZE-FATSTART)/ALLOCSECTIONS;
    for(_=0;_<DB->Population;_++){
      MvStart=_*sizeof(unsigned long int);
      CachedPos=DB->Cached+MvStart;
      CachedRec=*(unsigned long int *)(CachedPos);
      if(CachedRec==Position){
	memmove(CachedPos,CachedPos+sizeof(unsigned long int),MvFinish-MvStart-1);
	break;
      }
    }
    WMS_DelInpos(DB,_);
    MvStart=Position*sizeof(unsigned long int);
    MvLen=MvFinish-MvStart-1;
    memmove(DB->StartPos+MvStart,DB->StartPos+MvStart+sizeof(unsigned long int),MvLen);
    memmove(DB->FinishPos+MvStart,DB->FinishPos+MvStart+sizeof(unsigned long int),MvLen);
    memmove(DB->Lock+Position,DB->Lock+Position+1,MvFinish-Position-1);
    DB->Population--;
    WMS_UnlockDB(DB);
    return 1;
  }
  return 0;
}

/*! \brief Replaces a record at a position with another record.
 *  - Takes out a lock 
 */
unsigned long int WMS_RepBinary(struct DataArea *DB,unsigned long int Position,struct Blob *B){
  char *StartPos=NULL,*EndPos=NULL;
  long int Inserted=0;
  unsigned long int SizeofRec=0;
  if(DB){
    if(B&&Position<DB->Population){
      WMS_LockDB(DB);
      StartPos=DB->StartPos+Position*sizeof(unsigned long int);
      EndPos=DB->FinishPos+Position*sizeof(unsigned long int);
      SizeofRec=(*(unsigned long int *)EndPos)-(*(unsigned long int *)StartPos);
      if(B->Size>SizeofRec){
	WMS_UnlockDB(DB);
	Inserted=WMS_AddBinary(DB,B);
	WMS_LockDB(DB);
	if(Inserted<0){
	  WMS_UnlockDB(DB);
	  return Inserted;
	}
	WMS_DelInpos(DB,Position);
	WMS_Mv2LastInpos(DB,Position);
	*(unsigned long int *)StartPos=*(unsigned long int *)(DB->StartPos+Inserted*sizeof(unsigned long int));
	*(unsigned long int *)EndPos=*(unsigned long int *)(DB->FinishPos+Inserted*sizeof(unsigned long int));
	DB->Population--;
      }
      else{
	memcpy(DB->Shared+(*(unsigned long int *)StartPos),B->Nucleus,B->Size);
	*(unsigned long int *)EndPos=(*(unsigned long int *)StartPos)+B->Size;
      }
      Inserted=Position;
      WMS_UnlockDB(DB);
    }
    else{
      Inserted=WMS_AddBinary(DB,B);
    }
  }
  return Inserted;
}

/*! \brief Lock a specific record in the database
 * 
 */
void WMS_LockRec(struct DataArea *DB,unsigned long int Position){
  char *RecordLock=NULL;
  if(DB&&Position<DB->Population){
    RecordLock=DB->Lock+Position;
    if(*(char *)RecordLock){
      printf("%sWaiting for lock to release...%s",WC_FLASH,WC_RESET);
      fflush(stdout);
      while(*(char *)RecordLock)
	usleep(SPINTIME);
      puts(" ok");
    }
    *(char *)RecordLock=1;
  }
}

/*! \brief Unlock a specific record in the database
 * 
 */
void WMS_UnlockRec(struct DataArea *DB,unsigned long int Position){
  char *RecordLock=NULL;
  if(DB&&Position<DB->Population){
    RecordLock=DB->Lock+Position;
    *(char *)RecordLock=0;
  }
}

/*! \brief Lock the whole database for modification or reads.
 * 
 */
void WMS_LockDB(struct DataArea *DB){
  if(DB){
    if(DB->LockAlloc){
      printf("%sWaiting for lock to release...%s",WC_FLASH,WC_RESET);
      fflush(stdout);
      while(DB->LockAlloc)
	usleep(SPINTIME);
      puts(" ok");
    }
    DB->LockAlloc=1;
  }
}

/*! \brief Unlock the whole database for modification or reads.
 * 
 */
void WMS_UnlockDB(struct DataArea *DB){
  if(DB)
    DB->LockAlloc=0;
}

/*! \brief Convert a WMS record until a blob data structure.
 * 
 */
struct Blob *WMS_WMS2Blob(struct Datablock *WMS){
  unsigned int _=0;
  unsigned long int TotalSize=0,Position=0,StrLen=0;
  struct Blob *B=NULL;
  struct List_Str *Buffer=NULL;
  if(WMS){
    B=(struct Blob *)malloc(sizeof(struct Blob));
    for(_=0;_<3;_++){
      for(Buffer=WMS->DS[_];Buffer;Buffer=Buffer->Next)
	TotalSize+=strlen(Buffer->Str)+1;
      TotalSize++;
    }
    TotalSize+=2;
    B->Nucleus=(char *)malloc(TotalSize*sizeof(char));
    memset(B->Nucleus,'*',TotalSize);
    for(_=0;_<3;_++){
      for(Buffer=WMS->DS[_];Buffer;Buffer=Buffer->Next){
	StrLen=strlen(Buffer->Str);
	memcpy(B->Nucleus+Position,Buffer->Str,StrLen);
	Position+=StrLen+1;
	B->Nucleus[Position-1]=0;
      }
      B->Nucleus[Position]=1;
      Position++;
    }
    B->Nucleus[Position]=2;
    Position++;
    B->Nucleus[Position]=0;
    B->Size=Position;
  }
  else{
    puts("ERROR: WMS_WMS2Blob - This should never happen!");
    exit(0);
  }
  return B;
}

/*! \brief Convert a blob data structure into a WMS record.
 * 
 */
struct Datablock *WMS_Blob2WMS(struct Blob *B){
  unsigned int Sector=0;
  unsigned long int _=0,LastEnd=0;
  struct Datablock *DB=NULL;
  if(B){
    DB=(struct Datablock *)malloc(sizeof(struct Datablock));
    DB->DS[0]=NULL;
    DB->DS[1]=NULL;
    DB->DS[2]=NULL;
    DB->Next=NULL;
    for(_=0;_<B->Size;_++){
      if(B->Nucleus[_]==0){
	DB->DS[Sector]=Append_List_Element(DB->DS[Sector],strdup(B->Nucleus+LastEnd));
	LastEnd=_+1;
      }
      else if(B->Nucleus[_]==1){
	Sector++;
	LastEnd=_+1;   
      }
      else if(B->Nucleus[_]==2){
	break;
      }
    }  
  }
  else{
    puts("ERROR: WMS_Blob2WMS - This should never happen!");
    exit(0);
  }
  return DB;
}
 
/*! \brief Get the name of the database record from the retrieved record.
 * 
 */
char *WMS_GetRecName(struct Blob *B){
  char *R=NULL;
  if(B)
    R=strdup(B->Nucleus);
  return R;
}

/*! \brief Search for a record given its name.
 * 
 */
long int WMS_FindRec(struct DataArea *DB,char *RecordName){
  long int Position=-1;
  unsigned long int _=0;
  char *RetrievedName=NULL;
  struct Blob *RetrievedBlob=NULL;
  if(DB&&RecordName){
    WMS_LockDB(DB);
    for(_=0;_<DB->Population;_++){
      if((RetrievedBlob=WMS_GetBinary(DB,_))){
	if((RetrievedName=WMS_GetRecName(RetrievedBlob))){
	  if(!strcmp(RetrievedName,RecordName))
	    Position=(long int)_;
	  free(RetrievedName);
	}
	WMS_FreeBlob(RetrievedBlob);
      }
      if(Position!=-1)
	break;
    }
    WMS_UnlockDB(DB);
  }
  return Position;
}

/*! \brief Write the database to disk.
 * 
 */
unsigned int WMS_Save_To(struct DataArea *DB,char *FileName){
  unsigned long int _=0;
  FILE *File=NULL;  
  if(DB&&FileName){
    memcpy(DB->Shared,DB,sizeof(struct DataArea));
    if((File=fopen(FileName,"w"))==NULL){
      printf("Cannot open file: %s.\n",FileName);
      return 0;
    }
    else{
      for(_=0;_<DB->Size;_++)
	fputc(DB->Shared[_],File);
      fclose(File);
    }
    return 1;
  }
  return 0;
}

/*! \brief Load the database from disk.
 * 
 */
struct DataArea *WMS_Read_From(struct DataArea *DB,char *FileName,unsigned int Sector){
  unsigned long int Offset=0;
  int _=0;
  FILE *File=NULL;
  struct DataArea *Config=NULL;
  if(DB&&FileName&&DB->Queen){
    if((File=fopen(FileName,"r"))!=NULL){
      Config=(struct DataArea *)malloc(sizeof(struct DataArea));
      while((_=fgetc(File))!=EOF)
	DB->Shared[Offset++]=_;
      fclose(File);
      memcpy(Config,DB->Shared,sizeof(struct DataArea));
      DB->Population=Config->Population;
      DB->LastPos=Config->LastPos;
      free(Config);
    }
  }
  return DB;
}

/*! \brief Defrag the database.
 * 
 */
unsigned int WMS_Defrag(struct DataArea *DB){
  unsigned long int _=0,LastPos=FATSIZE,Inpos=0,OldStartPos=0,OldFinishPos=0,RecSize=0;
  unsigned long int NewStartPos=0,NewFinishPos=0;
  if(DB){
    WMS_LockDB(DB);
    for(_=0;_<DB->Population;_++){
      Inpos=*(unsigned long int *)(DB->InPos+_*sizeof(unsigned long int));
      OldStartPos=*(unsigned long int *)(DB->StartPos+Inpos*sizeof(unsigned long int));
      OldFinishPos=*(unsigned long int *)(DB->FinishPos+Inpos*sizeof(unsigned long int));
      RecSize=OldFinishPos-OldStartPos;
      if(OldStartPos!=LastPos){
	NewStartPos=LastPos;
	NewFinishPos=NewStartPos+RecSize;
	*(unsigned long int *)(DB->StartPos+Inpos*sizeof(unsigned long int))=NewStartPos;
	*(unsigned long int *)(DB->FinishPos+Inpos*sizeof(unsigned long int))=NewFinishPos;
	memmove(DB->Shared+OldStartPos,DB->Shared+NewStartPos,RecSize);
	LastPos=NewFinishPos;
      }
      else
	LastPos=OldFinishPos;
    }
    DB->LastPos=LastPos;
    WMS_UnlockDB(DB);
  }
  return 1;
}
