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
                                        


Programmed By M.S.Braithwaite 2017.

*/


#include<iostream>
#include <sys/types.h>
#include <unistd.h>

#include "ENiX_NLPTest.h"
#include "ENiX_Net.h"
#include "ENiX_Packet.h"
#include "ENiX_WMSAPI.h"

using namespace std;

string PROTOCOL_PREFIX="NLP Client 1.0";

Packet Application::ExecuteCommand(BinData Inp){}

void ServerSignal(int SignalData){
  unsigned int LocalPid=getpid();
  printf("\nSignal caught in pid %d... Exiting.\n",LocalPid);
  exit(0);
}

void Application::Splash(){
  puts("ENiX Test Client, connecting to server...");
}

void NLPFE::Execute(Chocky *NLPInstance,string InputStr){
  vector<string> Words=NLPInstance->DivideInput(InputStr);
  NLPInstance->PrintList(Words);
  NLPInstance->LearnWords(Words);
  NLPInstance->DebugLearnt();
  NLPInstance->Interpreter(Words);
  NLPInstance->Write2Amoeba(NLPInstance->CachedContents);
}

string NLPFE::CombineParameters(size_t argc,char **argv){
  size_t _=0;
  string Combined={};
  for(;_<argc;_++){
    Combined+=string(argv[_]);
    if(_<argc-1)
      Combined+=' ';
  }
  return Combined;
}

int main(int Argc, char *Argv[]){    
  int PortNum=AMOEBAPORTNUM;
  string HostName=AMOEBAHOSTNAME;
  Networking NLPComs(HostName,PortNum,false);
  BinData Input=NLPComs.Receive();
  Packet ProtocolData(Input);
  puts("Received protocol data:");
  ProtocolData.Print();

  Chocky NLPNew;
  NLPNew.AmoebaConnection=&NLPComs;
  NLPFE Start;

  string InputStr={};

  if(Argc-1){
    InputStr=Start.CombineParameters(Argc-1,Argv+1);
    Start.Execute(&NLPNew,InputStr);
  }
  else{
    while(true){
      cout<<"Please enter sentence:\n";
      getline(cin,InputStr);
      cout<<InputStr<<endl;
      Start.Execute(&NLPNew,InputStr);
    }
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
