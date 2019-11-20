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

#ifndef AMOEBADAEMON
#define AMOEBADAEMON

#include<vector>
#include "ENiX_WMSAPI.h"
#include "ENiX_Binary.h"
#include "ENiX_Packet.h"

WMSAPI *AmoebaDB;

class AmoebaDaemon{
 public:
  AmoebaDaemon();

  void InitialiseInstructions(void);
  
  typedef Packet (AmoebaDaemon::*ENiXInstructionSet)(vector<string> Commands,Packet Data);
  Packet ExecuteCommand(vector<string> Commands,Packet Data);

  Packet Add(vector<string> Commands,Packet Data);
  Packet AddFile(vector<string> Commands,Packet Data);
  Packet AddPair(vector<string> Commands,Packet Data);
  Packet DebugAlloc(vector<string> Commands,Packet Data);
  Packet DebugRec(vector<string> Commands,Packet Data);
  Packet Defrag(vector<string> Commands,Packet Data);
  Packet Delete(vector<string> Commands,Packet Data);
  Packet DeletePP(vector<string> Commands,Packet Data);
  Packet DeleteFile(vector<string> Commands,Packet Data);
  Packet DeletePair(vector<string> Commands,Packet Data);
  Packet DeleteFromPP(vector<string> Commands,Packet Data);
  Packet Exit(vector<string> Commands,Packet Data);
  Packet Find(vector<string> Commands,Packet Data);
  Packet GetAll(vector<string> Commands,Packet Data);
  Packet GetAllPP(vector<string> Commands,Packet Data);
  Packet GetData(vector<string> Commands,Packet Data);
  Packet GetFile(vector<string> Commands,Packet Data);
  Packet GetName(vector<string> Commands,Packet Data);
  Packet GetPP(vector<string> Commands,Packet Data);
  Packet Help(vector<string> Commands,Packet Data);
  Packet ListPP(vector<string> Commands,Packet Data);
  Packet Load(vector<string> Commands,Packet Data);
  Packet Purge(vector<string> Commands,Packet Data);
  Packet Replace(vector<string> Commands,Packet Data);
  Packet Save(vector<string> Commands,Packet Data);
  Packet Stats(vector<string> Commands,Packet Data);
  Packet SetPP(vector<string> Commands,Packet Data);

  vector<ENiXInstructionSet> InstructionSet;

  bool CmdSetInitialised;

  vector<int>    Parameters;
  vector<string> CommandName;
  vector<string> OnlineHelp;

  Packet InitialisePacket(string Response);
};



#endif


/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
 */
