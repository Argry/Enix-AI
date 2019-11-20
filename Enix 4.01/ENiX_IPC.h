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

#ifndef ENiXIPC
#define ENiXIPC

#include <sys/types.h>
#include <vector>
#include "ENiX_SHM.h"
//#include "ENiX_Binary.h"

struct SharedNeuralData{
  unsigned int Pid;
  bool Stop;
  unsigned long int ComplexityCap;
};

class IPC:public SHM{
 public:
  /* API access for network distribution */
  struct SharedNeuralData GetFromPid(unsigned int Pid);
  void SetFromPid(unsigned int Pid,unsigned long int StopAtComplexity);
  void AddPidProc(unsigned int Pid);
  void DelPidProc(unsigned int Pid);
  void DisplayChildPids();
  bool ResetAbort(unsigned int Pid);

  vector<struct SharedNeuralData> GetSharedData();
  void PutSharedData(vector<struct SharedNeuralData>);
  struct SharedNeuralData CreateNeuralDataStruct(unsigned int ThisPid,bool Exit,unsigned long int Comp);
  void DispNeuralDataStruct(struct SharedNeuralData Input);
};

/* We have to make this global because the callback handler needs to access it. */
extern IPC IPCComms;

#endif


/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
 */
