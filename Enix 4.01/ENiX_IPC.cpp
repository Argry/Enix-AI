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
#include <vector>
#include "ENiX_Settings.h"
#include "ENiX_IPC.h"

class IPC IPCComms;

void IPC::DispNeuralDataStruct(struct SharedNeuralData Input){
  printf("Pid: %d,%d,%ld\n",Input.Pid,Input.Stop,(unsigned long int)Input.ComplexityCap);  
}

struct SharedNeuralData IPC::CreateNeuralDataStruct(unsigned int ThisPid,bool Exit,unsigned long int Comp){
  struct SharedNeuralData Output;
  Output.Pid=ThisPid;
  Output.Stop=Exit;
  Output.ComplexityCap=Comp;
  return Output;
}

vector<struct SharedNeuralData> IPC::GetSharedData(){
  unsigned int NumProc=0,C=0;
  unsigned long int Offset=1+sizeof(unsigned int);
  vector<struct SharedNeuralData> Output;
  if(IPCComms.DataArea[1]){
    NumProc=(unsigned int)(*((IPCComms.DataArea)+1));
    Output.resize(NumProc);
    for(C=0;C<NumProc;C++){
      memcpy(&Output[C],(char *)((IPCComms.DataArea)+Offset),sizeof(struct SharedNeuralData));
      Offset+=sizeof(struct SharedNeuralData);
    }
  }
  else
    Output.resize(0);
  return Output;
}

void IPC::PutSharedData(vector<struct SharedNeuralData> Input){
  unsigned int NumProc=0,C=0;
  unsigned long int Offset=1+sizeof(unsigned int);  
  NumProc=Input.size();
  memcpy((IPCComms.DataArea)+1,(char *)&NumProc,sizeof(unsigned int));
  for(C=0;C<NumProc;C++){
    memcpy(IPCComms.DataArea+Offset,(char *)&Input[C],sizeof(struct SharedNeuralData));
    Offset+=sizeof(struct SharedNeuralData);
  }
}

struct SharedNeuralData IPC::GetFromPid(unsigned int Pid){
  unsigned int C=0;
  struct SharedNeuralData Output,Buffer;
  IPCComms.Lock();
  vector<struct SharedNeuralData> PidDataList=GetSharedData();
  Output=CreateNeuralDataStruct(0,0,0);
  for(C=0;C<PidDataList.size();C++){
    Buffer=PidDataList[C];
    if(Buffer.Pid==Pid){
      Output=Buffer;
      break;
    }
  }
  IPCComms.Unlock();
  return Output;
}

void IPC::SetFromPid(unsigned int Pid,unsigned long int StopAtComplexity){
  unsigned int C=0;
  struct SharedNeuralData ReplaceWith;
  IPCComms.Lock();
  vector<struct SharedNeuralData> PidDataList=GetSharedData();
  ReplaceWith=CreateNeuralDataStruct(Pid,1,StopAtComplexity);
  for(C=0;C<PidDataList.size();C++){
    if(PidDataList[C].Pid==Pid){
      PidDataList[C]=ReplaceWith;
      break;
    }
  }
  PutSharedData(PidDataList);
  IPCComms.Unlock();
}

bool IPC::ResetAbort(unsigned int Pid){
  unsigned int C=0;
  struct SharedNeuralData ReplaceWith;
  IPCComms.Lock();
  vector<struct SharedNeuralData> PidDataList=GetSharedData();
  ReplaceWith=CreateNeuralDataStruct(Pid,0,0);
  for(C=0;C<PidDataList.size();C++){
    if(PidDataList[C].Pid==Pid){
      PidDataList[C]=ReplaceWith;
      break;
    }
  }
  PutSharedData(PidDataList);
  IPCComms.Unlock();
}

void IPC::AddPidProc(unsigned int Pid){
  bool Found=false;
  unsigned int C=0;
  struct SharedNeuralData ReplaceWith;
  IPCComms.Lock();
  vector<struct SharedNeuralData> PidDataList=GetSharedData();
  for(C=0;C<PidDataList.size();C++){
    if(PidDataList[C].Pid==Pid){
      Found=true;
      break;
    }
  }
  if(!Found){
    ReplaceWith=CreateNeuralDataStruct(Pid,0,0);
    PidDataList.push_back(ReplaceWith);
  }
  PutSharedData(PidDataList);
  IPCComms.Unlock();
}

void IPC::DelPidProc(unsigned int Pid){
  unsigned int C=0;
  struct SharedNeuralData ReplaceWith;
  IPCComms.Lock();
  vector<struct SharedNeuralData> PidDataList=GetSharedData();
  for(C=0;C<PidDataList.size();C++){
  if(PidDataList[C].Pid==Pid){
      PidDataList.erase(PidDataList.begin()+C);
      break;
    }
  }
  PutSharedData(PidDataList);
  IPCComms.Unlock();
}

void IPC::DisplayChildPids(){
  unsigned int C=0;
  struct SharedNeuralData ReplaceWith;
  IPCComms.Lock();
  vector<struct SharedNeuralData> PidDataList=GetSharedData();
  for(C=0;C<PidDataList.size();C++){
    printf(" oo Debug: ");
    DispNeuralDataStruct(PidDataList[C]);
  }
  IPCComms.Unlock();
}

/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
 */
