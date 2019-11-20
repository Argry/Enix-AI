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

#include <stdio.h>
#include <string.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <unistd.h>
#include "ENiX_Settings.h"
#include "ENiX_SHM.h"

bool SHM::Disk2SHM(const char *Filename){
  unsigned long int _=0;
  FILE *FileHandle;
  if(FileHandle=fopen(Filename,"r")){
    bool Success=true;
    for(_=0;_<Size;_++){
      DataArea[_]=fgetc(FileHandle);
      if(feof(FileHandle)){
	Success=false;
	break;
      }
    }
    fclose(FileHandle);
    return Success;
  }
  else
    return false;
}

bool SHM::SHM2Disk(const char *Filename){
  unsigned long int _=0;
  FILE *FileHandle;
  if(FileHandle=fopen(Filename,"w+")){
    for(_=0;_<Size;_++)
      fputc(DataArea[_],FileHandle);
    fclose(FileHandle);
    return true;
  }
  else
    return false;
}

SHM::SHM(){
  Size=0;
  Key=0;
  shmid_ds=NULL;
  DataArea=NULL;
}

void SHM::Lock(){
  while(DataArea[0]=='L')
    usleep(SPINTIME);
  DataArea[0]='L';
}

void SHM::Unlock(){
  DataArea[0]='U';
}

bool SHM::WriteData(unsigned long int Offset,BinData Data){
  if(Offset+(Data.Size)>Size){
    return false;
  }
  else{
    memcpy(DataArea+Offset,Data.Str,Data.Size);
    return true;
  }
}

BinData SHM::ReadData(unsigned long int Offset,unsigned long int Size){
  BinData RetrievedData(DataArea+Offset,Size);
  return RetrievedData;
}

bool SHM::Create(unsigned long int SegSize,int SegKey){
  Size=SegSize;
  Key=(key_t)SegKey;
  if((SHMId=shmget(Key,Size,NNACCESS))==-1){
    if((SHMId=shmget(Key,Size,(IPC_CREAT|NNACCESS)))==-1){
      perror("shmget: shmget failed");
      return false;
    }
  }
  return true;
}

bool SHM::Destroy(){
  if (shmctl(SHMId,IPC_RMID,shmid_ds) == -1){
    perror("shmctl: shmctl failed");
    return false;
  }
  return true;
}

bool SHM::Attach(unsigned long int SegSize,int SegKey){
  Size=SegSize;
  Key=(key_t)SegKey;
  if ((DataArea=(char *)shmat(SHMId,NULL,0))==(char *)-1){
    perror("shmat");
    return false;
  }
  return true;
}

bool SHM::Detach(){
  if(shmdt(DataArea)==-1){
    perror("shmdt");
    return false;
  }
  return true;
}

/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
 */
