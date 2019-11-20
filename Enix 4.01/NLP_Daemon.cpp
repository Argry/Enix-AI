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

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sstream>

#include "NLP_Daemon.h" // Must be before ENiX_Settings.h
#include "ENiX_Binary.h"
#include "ENiX_Seq.h"
#include "ENiX_Settings.h"
#include "ENiX_List.h"
#include "ENiX_Chocky.h"
#include "ENiX_Daemon.h"

string PROTOCOL_PREFIX="Natural Language Processing";

void ServerSignal(int SignalData){
  unsigned int LocalPid=getpid();

  DEBUGMSG;

  NLPRef->~NLP();
  printf("\nSignal caught in pid %d... Exiting.\n",LocalPid);
  exit(0);
}

NLP::NLP(){
  CDSLink=NULL;
  AmoebaLink=NULL;
}

Packet NLP::Vector2Packet(vector<string> Input){
  Packet Answer={};
  Answer.Binaries={};
  Answer.Strings={};
  BinData Empty("");
  Answer.Strings.push_back(Empty);
  for(size_t _=0;_<Input.size();++_){
    BinData AddRecStr(Input[_]);
    Answer.Strings.push_back(AddRecStr);
  }
  return Answer;  
}

Packet NLP::InitialisePacket(string Response){
  Packet Answer={};
  Answer.Binaries={};
  Answer.Strings={};
  BinData Empty("");
  Answer.Strings.push_back(Empty);
  BinData AddRecStr(Response);
  Answer.Strings.push_back(AddRecStr);
  return Answer;
}

NLP::~NLP(){
  DEBUGMSG;
  if(NLPInstance){
    delete NLPInstance;
    NLPInstance=NULL;
  }
  if(CDSLink){
    delete CDSLink;
    CDSLink=NULL;
  }
  if(AmoebaLink){
    delete AmoebaLink;
    AmoebaLink=NULL;
  }
}

void NLP::SetupConnections(){
  if(!CDSLink){
    CDSLink=new Networking("localhost",577,false);     
    string HostName;
    int PortNum=0;
    if(CDSLink && CDSLink->ConnectedStatus==NetworkStatus::Connected){
      Packet PeerList=CDSLink->CommandToServer(CDSLink,"GETPEERSONLINE Amoeba");
      if(PeerList.Strings.size()>2){
	vector<string> ConnectionParameters=List::String2List(PeerList.Strings[2].Convert2Str(),false);
	if(ConnectionParameters.size()>2){
	  HostName=ConnectionParameters[1];
	  PortNum=atoi(ConnectionParameters[2].c_str());
	  AmoebaLink=new Networking(HostName,PortNum,false);
	  if(AmoebaLink && AmoebaLink->ConnectedStatus==NetworkStatus::Connected)
	    // receive the protocol data and dump it.
	    Packet Incoming(AmoebaLink->Receive());
	}
      }
      PeerList=CDSLink->CommandToServer(CDSLink,"GETPEERSONLINE NNP");
      if(PeerList.Strings.size()>2){
	vector<string> ConnectionParameters=List::String2List(PeerList.Strings[2].Convert2Str(),false);
	if(ConnectionParameters.size()>2){
	  HostName=ConnectionParameters[1];
	  PortNum=atoi(ConnectionParameters[2].c_str());
	  NeuralNetLink=new Networking(HostName,PortNum,false);
	  if(NeuralNetLink && NeuralNetLink->ConnectedStatus==NetworkStatus::Connected)
	    // receive the protocol data and dump it.
	    Packet Incoming(NeuralNetLink->Receive());
	}
      }
    }
  }
}

Packet NLP::InterpretData(string ToParse){
  Packet Answer=InitialisePacket("ACK");
  SetupConnections();
  if(AmoebaLink){
    if(!NLPInstance){
      NLPInstance=new Chocky(AmoebaLink,NeuralNetLink,ToParse);
      NLPInstance->SetupMode();
    }
    
    //    vector<string> Words=List::String2List(ToParse);
    vector<string> Input={ToParse};
    vector<string> Words=List::PrepareInput(Input);
    
    // NLPInstance->LearnWords(Words);
    NLPInstance->LearnSelected(Words);
    vector<string> ToReturn=NLPInstance->Interpreter(Words);
    NLPInstance->Mode2WMS();
    NLPInstance->Write2Amoeba(NLPInstance->CachedContents);
    Answer=Vector2Packet(ToReturn);
  }
  else{
    // We shouldn't really be here unless we cannot find a service!
  }
  return Answer;
}

Packet Application::ExecuteCommand(BinData Input){
  Packet Incoming(Input);
  Packet Outgoing;
  BinData OutgoingRecord={};
  string SessionIdentifier=BinData::GetSessionIdentifier();
  if(Incoming.Strings.size()){
    printf(" -> %s",SessionIdentifier.c_str());
    Incoming.Strings[0].Print();
    putchar('\n');
    string ToExecute((Incoming.Strings[0]).Str,(Incoming.Strings[0]).Size);
    Outgoing=NLPRef->InterpretData(ToExecute);
    Outgoing.GenerateBinary();
  }
  if((Outgoing.Strings.size()<2))
    printf(" <- %sNONE\n",SessionIdentifier.c_str());
  else{
    string Output(Outgoing.Strings[1].Str,Outgoing.Strings[1].Size);
    printf(" <- %s%s\n",SessionIdentifier.c_str(),Output.c_str());
  }
  return Outgoing;
}

void Application::Splash(void){
  /* echo "ENiX4 NLP" | toilet --metal -t */

  printf("\n [0;1;34;94mmmmmmm[0m [0;34mmm[0m   [0;34mm[0m   [0;34m\"[0m    [0;34mm[0m    [0;37mm[0m    [0;37mmm[0m         [0;1;30;90mmm[0m   [0;1;30;90mm[0m [0;1;30;90mm[0m      [0;1;34;94mmmmmm[0m \n");
  printf(" [0;34m#[0m      [0;34m#\"m[0m  [0;34m#[0m [0;34mm[0;37mmm[0m     [0;37m#[0m  [0;37m#[0m    [0;37mm[0;1;30;90m\"#[0m         [0;1;30;90m#\"m[0m  [0;1;34;94m#[0m [0;1;34;94m#[0m      [0;1;34;94m#[0m   [0;1;34;94m\"#[0m\n");
  printf(" [0;34m#mmmmm[0m [0;37m#[0m [0;37m#m[0m [0;37m#[0m   [0;37m#[0m      [0;1;30;90m##[0m    [0;1;30;90m#\"[0m [0;1;30;90m#[0m         [0;1;34;94m#[0m [0;1;34;94m#m[0m [0;1;34;94m#[0m [0;1;34;94m#[0m      [0;34m#mmm#\"[0m\n");
  printf(" [0;37m#[0m      [0;37m#[0m  [0;37m#[0m [0;37m#[0m   [0;1;30;90m#[0m     [0;1;30;90mm\"\"m[0m  [0;1;30;90m#mm[0;1;34;94mm#m[0m        [0;1;34;94m#[0m  [0;1;34;94m#[0m [0;34m#[0m [0;34m#[0m      [0;34m#[0m     \n");
  printf(" [0;37m#mmmmm[0m [0;1;30;90m#[0m   [0;1;30;90m##[0m [0;1;30;90mmm#mm[0m  [0;1;30;90mm\"[0m  [0;1;34;94m\"m[0m     [0;1;34;94m#[0m         [0;34m#[0m   [0;34m##[0m [0;34m#mmmmm[0m [0;37m#[0m     \n\n");
  puts("-----------------[Natural Language Processor]--------------------");  
}

int main(int argc,char *argv[]){
  Daemon IfConfigured;
  {
    string HostName=CHOCKYHOSTNAME;
    int PortNum=CHOCKYPORTNUM;
    if(argc==3){
      HostName=string(argv[1]);
      PortNum=atoi(argv[2]);
    }

    NLP NLPActual;
    NLPActual.NLPInstance=NULL;

    NLPRef=&NLPActual;
    Networking NewServer(HostName,PortNum,true);
    NetworkLib=&NewServer;
    if(NewServer.ConnectedStatus!=Connected){
      NewServer.DisplayErrorText(NewServer.ConnectedStatus);
      exit(0);
    }
    NewServer.RunServer();
  }
  return 0;
}


/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
 */
