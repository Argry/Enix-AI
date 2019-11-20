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

#ifndef ENiXNNBridge
#define ENiXNNBridge

#include "ENiX_Net.h"
#include "ENiX_Binary.h"
#include "ENiX_List.h"

class NNBridge{
 public:
  Networking *NeuralConnection;

  // Execute
  // Learn
  // Oddman
  // Understood

  string ExecuteNN(vector<string> NeuralNet,vector<string> Seq);
  string LearnNN(vector<string> Seq);
  string OddmanNN(vector<string> Seq);
  string UnderstoodNN(vector<string> Seq);
  string ShowNN(vector<string> Seq);

 private:
  BinData ExtractFromServer(string Command,int ProtocolBinId);
  string ExtractFromServer(string Command,string RespPrefix,int ProtocolMsgId);
};


#endif


/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
 */
