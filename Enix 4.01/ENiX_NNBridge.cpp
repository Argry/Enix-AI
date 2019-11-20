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

#include <string.h>
#include "ENiX_NNBridge.h"

string NNBridge::ExecuteNN(vector<string> NeuralNet,vector<string> Seq){
  string ClearCommand="CLEAR";
  ExtractFromServer(ClearCommand,"",0);
  string LoadCommand="LOAD "+List::List2String(NeuralNet);
  ExtractFromServer(LoadCommand,"",0);
  string Command2Run="EXECUTE "+List::List2String(Seq);
  return ExtractFromServer(Command2Run,"",2);
}

string NNBridge::ShowNN(vector<string> NeuralNet){
  string ClearCommand="CLEAR";
  ExtractFromServer(ClearCommand,"",0);
  string LoadCommand="LOAD "+List::List2String(NeuralNet);
  ExtractFromServer(LoadCommand,"",0);
  string Command2Run="SHOWNEURAL";
  return ExtractFromServer(Command2Run,"",2);
}

string NNBridge::LearnNN(vector<string> Seq){
  string ClearCommand="CLEAR";
  ExtractFromServer(ClearCommand,"",0);
  string Command2Run="LEARN "+List::List2String(Seq);  
  return ExtractFromServer(Command2Run,"",2);  
}

string NNBridge::OddmanNN(vector<string> Seq){
  string ClearCommand="CLEAR";
  ExtractFromServer(ClearCommand,"",0);
  string Command2Run="ODDMAN "+List::List2String(Seq);
  return ExtractFromServer(Command2Run,"",2);
}

string NNBridge::UnderstoodNN(vector<string> Seq){
  string ClearCommand="CLEAR";
  ExtractFromServer(ClearCommand,"",0);
  string Command2Run="UNDERSTOOD "+List::List2String(Seq);
  return ExtractFromServer(Command2Run,"",2);
}

BinData NNBridge::ExtractFromServer(string Command,int ProtocolBinId){
    BinData CommandBin={};
    CommandBin.Change(Command);

    vector<BinData> TXStrings={};
    vector<BinData> TXBinaries={};

    TXStrings.push_back(CommandBin);

    Packet Outgoing(TXStrings,TXBinaries);
    /*
      puts("Sent:");
      Outgoing.Print();
    */
    NeuralConnection->Send(Outgoing.AllData);

    Packet Incoming(NeuralConnection->Receive());
    /*
      puts("Received:");
      Incoming.Print();
    */
    if(Incoming.Binaries.size()>ProtocolBinId)
      return Incoming.Binaries[ProtocolBinId];

    return {};
}

string NNBridge::ExtractFromServer(string Command,string RespPrefix,int ProtocolMsgId){
    BinData CommandBin={};
    CommandBin.Change(Command);

    size_t PrefixLen=RespPrefix.size();
    vector<BinData> TXStrings={};
    vector<BinData> TXBinaries={};
    char *ReceivedData=NULL;
  
    TXStrings.push_back(CommandBin);

    Packet Outgoing(TXStrings,TXBinaries);

    /*
      DEBUGMSG;
      puts("Sent:");
      Outgoing.Print();
    */

    NeuralConnection->Send(Outgoing.AllData);

    Packet Incoming(NeuralConnection->Receive());

    /*
      DEBUGMSG;
      puts("Received:");
      Incoming.Print();
    */

    if(Incoming.Strings.size()>=ProtocolMsgId)
      if((Incoming.Strings[ProtocolMsgId].Size>PrefixLen)&&(!memcmp(Incoming.Strings[ProtocolMsgId].Str,RespPrefix.c_str(),PrefixLen)))
	ReceivedData=(Incoming.Strings[ProtocolMsgId].Str)+PrefixLen;
    
    if(ReceivedData&&(!!strlen(ReceivedData)))
      return string(ReceivedData);

    return "";
}


/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
 */
