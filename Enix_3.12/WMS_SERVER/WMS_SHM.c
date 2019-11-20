#include "WMS_SHM.h"
#include "WMS_Blob.h"
#include "ENiX_LIST.h"
#include "WitchCraft-Colours.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

char *YesNo[]={"No","Yes"};

struct DataArea *WMS_SHM_Create(unsigned int Sector){
  struct DataArea *DB=(struct DataArea *)malloc(sizeof(struct DataArea));
  DB->Sector=Sector;
  DB->Size=SHMSIZE;
  DB->Key=SHMKEY+Sector;
  DB->Queen=0;
  DB->Population=0;
  DB->LastPos=FATSIZE;
  DB->Shared=NULL;
  if((DB->SHM_ID=shmget(DB->Key,DB->Size,0666))==-1){
    if((DB->SHM_ID=shmget(DB->Key,DB->Size,(IPC_CREAT|0666)))==-1)  
      return NULL;
    else{
      DB->Queen=1;
    }
  }
  else
    puts("Queen already running.");
  return DB;
}

unsigned int WMS_SHM_Delete(struct DataArea *DB){
  int R=0;
  if(DB){
    /* TODO: should only delete if a queen */
    if((R=shmctl(DB->SHM_ID,IPC_RMID,DB->shmid_ds))==-1){
      perror("shmctl: shmctl failed");
      return 0;
    }
    return 1;
  }
  return 0;
}

unsigned int WMS_SHM_Attach(struct DataArea *DB){
  if(DB){
    if((DB->Shared=(char *)shmat(DB->SHM_ID,NULL,0))==(char *)-1)
      return 0;
    DB->LockAlloc=1;
    DB->Cached=DB->Shared+FATSTART;
    DB->StartPos=DB->Shared+(FATSTART+(FATSIZE-FATSTART)/ALLOCSECTIONS);
    DB->FinishPos=DB->Shared+(FATSTART+(2*(FATSIZE-FATSTART)/ALLOCSECTIONS));
    DB->Lock=DB->Shared+(FATSTART+(3*(FATSIZE-FATSTART)/ALLOCSECTIONS));
    DB->LockAlloc=0;
    return 1;
  }
  return 0;
}

unsigned int WMS_SHM_Detach(struct DataArea *DB){
  if(DB){
    if(shmdt(DB->Shared)==-1)
      return 0;
    return 1;
  }
  return 0;
}

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

void WMS_ALLOC_DEBUG(struct DataArea *DB){
  char Lock=0,*RecordName=NULL;
  struct Blob *B=NULL;
  unsigned long int _=0,Start=0,Finish=0,Cached=0;
  puts("");
  if(DB)
    printf("\nSector: %d\n",DB->Sector);
  puts("-[Record]---[Start]----[Finish]---[Cached]-[L]-[Name]-");
  if(DB){
    for(_=0;_<DB->Population;_++){
      Start=*(unsigned long int *)(DB->StartPos+_*sizeof(unsigned long int));
      Finish=*(unsigned long int *)(DB->FinishPos+_*sizeof(unsigned long int));
      Cached=*(unsigned long int *)(DB->Cached+_*sizeof(unsigned long int));
      Lock=*(char *)(DB->Cached+_*sizeof(char));
      B=WMS_GetBinary(DB,_);
      RecordName=WMS_GetRecName(B);
      printf("%010ld %010ld %010ld %010ld %d  %s\n",_,Start,Finish,Cached,(int)Lock,RecordName);     
      WMS_FreeBlob(B);
      free(RecordName);
    }
  }
  puts("------------------------------------------------------");
}

unsigned int WMS_SHM_Clear(struct DataArea *DB){
  if(DB){
    // memset(DB->Shared+FATSTART,0,FATSIZE-FATSTART);
    DB->Population=0;
    DB->LockAlloc=0;
    DB->LastPos=FATSIZE;
    return 1;
  }
  return 0;
}

long int WMS_AddBinary(struct DataArea *DB,struct Blob *B){  
  unsigned long int EndPos=0;
  long int Position=-1;
  if(DB&&B){
    Position=DB->Population;
    *(char *)(DB->Lock+DB->Population)=1;
    memcpy(DB->Shared+(DB->LastPos),B->Nucleus,B->Size);
    *(unsigned long int *)(DB->Cached+(DB->Population*sizeof(unsigned long int)))=DB->Population;
    *(unsigned long int *)(DB->StartPos+(DB->Population*sizeof(unsigned long int)))=DB->LastPos;
    EndPos=DB->LastPos+B->Size;
    *(unsigned long int *)(DB->FinishPos+(DB->Population*sizeof(unsigned long int)))=EndPos;
    DB->LastPos+=(B->Size+SHMSPACING);
    *(char *)(DB->Lock+DB->Population)=0;
    DB->Population++;
  }
  return Position;
}

struct Blob *WMS_GetBinary(struct DataArea *DB,unsigned long int Position){
  unsigned long int StartPos=0,FinishPos=0;
  struct Blob *B=NULL;
  if(DB&&(Position<DB->Population)){
    B=(struct Blob *)malloc(sizeof(struct Blob));
    StartPos=*(unsigned long int *)(DB->StartPos+(Position*sizeof(unsigned long int)));
    FinishPos=*(unsigned long int *)(DB->FinishPos+(Position*sizeof(unsigned long int)));
    B->Size=FinishPos-StartPos;
    B->Nucleus=(char *)malloc((B->Size+1)*sizeof(char));
    memcpy(B->Nucleus,DB->Shared+StartPos,B->Size);
    // defensive programming only
    B->Nucleus[B->Size]=0;
    return B;
  }
  return NULL;
}

unsigned int WMS_DelBinary(struct DataArea *DB,unsigned long int Position){
  char *CachedPos=NULL;
  unsigned long int _=0,MvStart=0,MvFinish=0,MvLen=0,CachedRec=0,StartRec=0,FinishRec=0;
  if(DB&&(Position<DB->Population)){
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
    MvStart=Position*sizeof(unsigned long int);
    MvLen=MvFinish-MvStart-1;
    memmove(DB->StartPos+MvStart,DB->StartPos+MvStart+sizeof(unsigned long int),MvLen);
    memmove(DB->FinishPos+MvStart,DB->FinishPos+MvStart+sizeof(unsigned long int),MvLen);
    memmove(DB->Lock+Position,DB->Lock+Position+1,MvFinish-Position-1);
    DB->Population--;
    return 1;
  }
  return 0;
}

unsigned long int WMS_RepBinary(struct DataArea *DB,unsigned long int Position,struct Blob *B){
  char *StartPos=NULL,*EndPos=NULL;
  unsigned long int Inserted=0,SizeofRec=0;
  if(DB&&B&&Position<DB->Population){
    StartPos=DB->StartPos+Position*sizeof(unsigned long int);
    EndPos=DB->FinishPos+Position*sizeof(unsigned long int);
    SizeofRec=(*(unsigned long int *)EndPos)-(*(unsigned long int *)StartPos);
    if(B->Size>SizeofRec){
      Inserted=WMS_AddBinary(DB,B);
      *(unsigned long int *)StartPos=*(unsigned long int *)(DB->StartPos+Inserted*sizeof(unsigned long int));
      *(unsigned long int *)EndPos=*(unsigned long int *)(DB->FinishPos+Inserted*sizeof(unsigned long int));
      DB->Population--;      
    }
    else{
      memcpy(DB->Shared+(*(unsigned long int *)StartPos),B->Nucleus,B->Size);
      *(unsigned long int *)EndPos=(*(unsigned long int *)StartPos)+B->Size;
    }
    Inserted=Position;
  }
  else
    Inserted=WMS_AddBinary(DB,B);
  return Inserted;
}

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

void WMS_UnlockRec(struct DataArea *DB,unsigned long int Position){
  char *RecordLock=NULL;
  if(DB&&Position<DB->Population){
    RecordLock=DB->Lock+Position;
    *(char *)RecordLock=0;
  }
}

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

void WMS_UnlockDB(struct DataArea *DB){
  if(DB)
    DB->LockAlloc=0;
}

struct Blob *WMS_WMS2Blob(struct Datablock *WMS){
  unsigned int _=0;
  unsigned long int TotalSize=0,StrLen=0;
  struct Blob *B=NULL;
  struct List_Str *Buffer=NULL;
  if(WMS){
    B=(struct Blob *)malloc(sizeof(struct Blob));
    for(_=0;_<3;_++){
      for(Buffer=WMS->DS[_];Buffer;Buffer=Buffer->Next)
	TotalSize+=strlen(Buffer->Str)+1;
      TotalSize++;
    }
    B->Size=TotalSize;
    B->Nucleus=(char *)malloc((TotalSize+1)*sizeof(char));
    TotalSize=0;
    for(_=0;_<3;_++){
      for(Buffer=WMS->DS[_];Buffer;Buffer=Buffer->Next){
	StrLen=strlen(Buffer->Str);
	memcpy(B->Nucleus+TotalSize,Buffer->Str,StrLen);
	TotalSize+=StrLen+1;
	B->Nucleus[TotalSize]=0;
      }
      B->Nucleus[TotalSize]=1;
      TotalSize++;
    }
    B->Nucleus[TotalSize-1]=2;
    B->Nucleus[TotalSize]=0;
    B->Size=TotalSize+1;
  }
  return B;
}

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
  return DB;
}
 
char *WMS_GetRecName(struct Blob *B){
  char *R=NULL;
  if(B)
    R=strdup(B->Nucleus);
  return R;
}

long int WMS_FindRec(struct DataArea *DB,char *RecordName){
  long int Position=-1;
  unsigned long int _=0;
  char *RetrievedName=NULL;
  struct Blob *RetrievedBlob=NULL;
  if(DB&&RecordName){
    for(_=0;_<DB->Population;_++){
      if((RetrievedBlob=WMS_GetBinary(DB,_))){
	if(RetrievedName=WMS_GetRecName(RetrievedBlob)){
	  if(!strcmp(RetrievedName,RecordName))
	    Position=(long int)_;
	  free(RetrievedName);
	}
	WMS_FreeBlob(RetrievedBlob);
      }
      if(Position!=-1)
	break;
    }
  }
  return Position;
}

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

struct DataArea *WMS_Read_From(struct DataArea *DB,char *FileName,unsigned int Sector){
  unsigned long int Offset=0;
  int _=0;
  FILE *File=NULL;
  struct DataArea *Config=NULL;
  if(DB&&FileName){
    if(FileName){
      if((File=fopen(FileName,"r"))==NULL){
	DB=WMS_SHM_Create(Sector);
      }
      else{
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
  }
  return DB;
}

unsigned int WMS_Defrag(struct DataArea *DB){
  
  
}
