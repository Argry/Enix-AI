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

#include<iostream>
#include<vector>

#include "ENiX_Deps.h"
#include "ENiX_Net.h"

#ifndef ENiXCORE
#define ENiXCORE

extern ENiX_NN *ThisSessionANN;

struct CommandThreadData{
  string Command;
  Networking *ServerLink;
  Packet Data;
};

class ENiX_NN:public Dependencies{
 public:
  bool NeuralLearnt;
  string NeuralAsStr;
  ENiX_NN(int argc,char **argv);
  ENiX_NN(Sequence Seq);
  ENiX_NN();
  ENiX_NN(unsigned int ClientId,unsigned int NumClients,Sequence Inp);
  bool Learn();
  bool DLearn();
  Sequence Execute(vector<int> Vector);
  Sequence Execute(Sequence Inp);
  bool Neural2Str();
  void Debug();
  void DebugBefore();
  void PrintNN(const ENiX_NN& ANN);
  string NN2String(const ENiX_NN& ANN);
  void LearnTopLevel(unsigned int Id,unsigned int NumClients);
  void DLearnTopLevel(unsigned int Id,unsigned int NumClients);
  vector<string> Deconstruct(unsigned int NumHosts,Sequence Seq);

  void ClearANN();
  string RetrieveANN();

protected:
  void OpenNeuralLinks(size_t DesiredNum);
  void CloseNeuralLinks();
  Networking *CDSLink;
  vector<Networking *> NeuralLinks;
  static void *CommandLinkThread(void *CommandThreadData);
};

#endif

/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
 */
