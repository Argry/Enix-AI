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
                                        


Programmed By M.S.Braithwaite 2017.

*/

#include <string.h>
#include "ENiX_Disk.h"

char *Disk::LoadPipe(FILE *PIPE){
  int Offset=0,_=0;
  char *Buffer=(char *)malloc(10*sizeof(char *)),*Contents=(char *)malloc(10*sizeof(char *));                  
  memset(Buffer,0,10);
  memset(Contents,0,10);
  while(fgets(Buffer,9,PIPE)!=NULL){
    memcpy(Contents+Offset,Buffer,_=strlen(Buffer));
    Contents[Offset+_]=0;
    Offset+=_;
    Contents=(char *)realloc(Contents,(Offset+10)*sizeof(char *));
  }
  if(Contents[0]){
    _=strlen(Contents);
    Contents[_-1]=0;
  }
  return Contents;  
}

BinData Disk::ReadFromDisk(const char *Filename){
  size_t _=0;
  size_t FileSize=0;
  FILE *FileHandle;
  BinData Read;
  if(FileHandle=fopen(Filename,"r")){
    fseek(FileHandle,0,SEEK_END);
    FileSize=ftell(FileHandle);
    fseek(FileHandle,0,SEEK_SET);
    char *Buffer=(char *)malloc(FileSize*sizeof(char));
    bool Success=true;
    for(_=0;_<FileSize;_++){
      Buffer[_]=fgetc(FileHandle);
      if(feof(FileHandle)){
	Success=false;
	break;
      }
    }
    fclose(FileHandle);    
    Read.Change(Buffer,FileSize);
    free(Buffer);
  }
  return Read;
}

BinData Disk::ReadFromDisk(string Filename){
  return Disk::ReadFromDisk(Filename.c_str()); 
}

bool Disk::WriteToDisk(BinData Output,const char *Filename){
  if(Output.Size){
    unsigned long int _=0;
    FILE *FileHandle;
    if(FileHandle=fopen(Filename,"w+")){
      for(_=0;_<Output.Size;_++)
	fputc(Output.Str[_],FileHandle);
      fclose(FileHandle);
      return true;
    }
  }
  return false;
}

bool Disk::WriteToDisk(BinData Output,string Filename){
  return Disk::WriteToDisk(Output,Filename.c_str()); 
}

bool Disk::WriteToDisk(string Output,const char *Filename){
  BinData ConvertedOutput(Output);
  return Disk::WriteToDisk(ConvertedOutput,Filename); 
}

bool Disk::WriteToDisk(string Output,string Filename){
  BinData ConvertedOutput(Output);
  return Disk::WriteToDisk(ConvertedOutput,Filename.c_str()); 
}

/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
*/
