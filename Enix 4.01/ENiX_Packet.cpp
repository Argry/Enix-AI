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
                                        


Programmed By M.S.Braithwaite 2016.

*/

#include <string.h>
#include "ENiX_Packet.h"
#include "ENiX_WMSAPI.h"
#include "ENiX_SHM.h"
#include <arpa/inet.h>
#include <cstdint>

Packet::Packet(){}

Packet::~Packet(){
  
  
}

Packet::Packet(vector<BinData> SendStr,vector<BinData> SendData){
  Strings=SendStr;
  Binaries=SendData;
  GenerateBinary();
}

Packet::Packet(BinData RecData){
  uint32_t NumberStr=0,NumberBinary=0,Pos=0,Offset=0,BinSize=0;
  
  char *DEBUGSTR=RecData.Str;
  size_t DEBUGSZ=RecData.Size;

  /*
    if RecData.size is 0 then the server process may have crashed!
  */
  
  Strings.resize(0);
  Binaries.resize(0);
  
  if(DEBUGSZ){
  
    NumberStr=ntohl(*(uint32_t *)RecData.Str);
    Offset+=sizeof(uint32_t);

    NumberBinary=ntohl(*(uint32_t *)(RecData.Str+Offset));
    Offset+=sizeof(uint32_t);

    for(Pos=0;Pos<NumberStr;Pos++){
      BinSize=ntohl(*(uint32_t *)(RecData.Str+Offset));
      Offset+=sizeof(uint32_t);
      BinData NewData(RecData.Str+Offset,BinSize);
      Strings.push_back(NewData);
      Offset+=BinSize;
    }

    for(Pos=0;Pos<NumberBinary;Pos++){
      BinSize=ntohl(*(uint32_t *)(RecData.Str+Offset));
      Offset+=sizeof(uint32_t);
      BinData NewData(RecData.Str+Offset,BinSize);
      Binaries.push_back(NewData);
      Offset+=BinSize;
    }
  }
  
  AllData=RecData;
}

void Packet::Print(){
  unsigned long int C=0;
  puts("String contents:");
  for(C=0;C<Strings.size();C++){
    printf("%03lu: ",(unsigned long int)C);
    Strings[C].Print();
    putchar('\n');
  }
  puts("Data contents:");
  for(C=0;C<Binaries.size();C++){
    printf("%03lu: ",(unsigned long int)C);
    Binaries[C].PrintHex();
    putchar('\n');
  }
}

void Packet::GenerateBinary(){
  uint32_t MaxSize=0,Pos=0,Offset=0,Size=0;
  char *Data=NULL;
  MaxSize+=(2*(sizeof(uint32_t)));  
  for(Pos=0;Pos<Strings.size();Pos++)
    MaxSize+=(sizeof(uint32_t)+Strings[Pos].Size);
  for(Pos=0;Pos<Binaries.size();Pos++)
    MaxSize+=(sizeof(uint32_t)+Binaries[Pos].Size);
  MaxSize++;

  Data=(char *)malloc(MaxSize);

  Size=htonl((uint32_t)Strings.size());
  memcpy(Data+Offset,&Size,sizeof(uint32_t));
  Offset+=sizeof(uint32_t);

  /*
  DEBUGMSG;
  printf("Strings: [%ld][%ld]\n",Strings.size(),Size);
  */

  Size=htonl((uint32_t)Binaries.size());
  memcpy(Data+Offset,&Size,sizeof(uint32_t));
  Offset+=sizeof(uint32_t);

  /*
  DEBUGMSG;
  printf("Binaries: [%ld][%ld]\n",Binaries.size(),Size);
  */

  for(Pos=0;Pos<Strings.size();Pos++){
    Size=htonl((uint32_t)Strings[Pos].Size);
    memcpy(Data+Offset,&Size,sizeof(uint32_t));
    Offset+=sizeof(uint32_t);
    memcpy(Data+Offset,Strings[Pos].Str,Strings[Pos].Size);
    Offset+=Strings[Pos].Size;
  }

  for(Pos=0;Pos<Binaries.size();Pos++){
    Size=htonl((uint32_t)Binaries[Pos].Size);
    memcpy(Data+Offset,&Size,sizeof(uint32_t));
    Offset+=sizeof(uint32_t);
    memcpy(Data+Offset,Binaries[Pos].Str,Binaries[Pos].Size);
    Offset+=Binaries[Pos].Size;
  }

  *(Data+Offset)=0;
  Offset++;

  BinData NewPacketData(Data,Offset);
  AllData=NewPacketData;
}

/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
*/
