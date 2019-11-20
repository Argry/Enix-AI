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

#ifndef ENiXNEURALSERVER
#define ENiXNEURALSERVER

class NeuralServer{
 public:
  NeuralServer();

  void InitialiseInstructions(void);
  
  typedef Packet (NeuralServer::*ENiXInstructionSet)(vector<string> Commands,Packet Data);
  Packet ExecuteCommand(vector<string> Commands,Packet Data);

  Packet Abort(vector<string> Commands,Packet Data);
  Packet Clear(vector<string> Commands,Packet Data);
  Packet Deconstruct(vector<string> Commands,Packet Data);
  Packet Execute(vector<string> Commands,Packet Data);
  Packet Fragment(vector<string> Commands,Packet Data);
  Packet Help(vector<string> Commands,Packet Data);
  Packet Learn(vector<string> Commands,Packet Data);
  Packet DLearn(vector<string> Commands,Packet Data);
  Packet Load(vector<string> Commands,Packet Data);
  Packet ODDMAN(vector<string> Commands,Packet Data);
  Packet Retrieve(vector<string> Commands,Packet Data);
  Packet Terminate(vector<string> Commands,Packet Data);
  Packet Understood(vector<string> Commands,Packet Data);
  Packet ShowNeural(vector<string> Commands,Packet Data);
  //  Packet SHMTest1(vector<string> Commands,Packet Data);

  vector<ENiXInstructionSet> InstructionSet;

  bool CmdSetInitialised;

  vector<int>    Parameters;
  vector<string> CommandName;
  vector<string> OnlineHelp;

  static char *InfoId();
  static char *OutgoingId();
  static char *IncomingId();
  
  Packet InitialisePacket(string Response);

  /*
 private:
  void ClearANN();
  string RetrieveANN();  
  */
};

#endif

/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
 */
