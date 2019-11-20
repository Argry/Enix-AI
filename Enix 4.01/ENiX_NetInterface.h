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


#ifndef ENiXAPPL
#define ENiXAPPL

#include<map>
#include<string>
#include "ENiX_IPC.h"
//#include "ENiX_CORE.h"
#include "ENiX_Packet.h"

/* This data is shared across all children */
static map<unsigned int,class ENiX_NN **> IntTCPInterface;

/* We have to make this global because the callback handler needs to access it. */
extern int IsParentProcess;

void ServerSignal(int SignalData);
void CrashSignal(int SignalData);

class Application{
 public:
  Application(){ 
    TCPInterface=&IntTCPInterface;
  }

  Packet ExecuteCommand(BinData Input);

  void Splash();

  map<unsigned int,class ENiX_NN **> *TCPInterface;
  int ThisProcessPid;

};

#endif

/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
*/
