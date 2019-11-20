#include <string.h>
#include "ENiX_Net.h"
#include <sys/types.h>
#include <unistd.h>
#include "ENiX_Packet.h"

string PROTOCOL_PREFIX="ENiX 4 Test Framework";

Packet Application::ExecuteCommand(BinData Input){
  char *Data1=(char *)"ENiX 4 Test Framework";
  char *Data2=(char *)"Message Received";
  vector<BinData> Binaries={};
  vector<BinData> Strings={};
  BinData Protocol((char *)Data1);
  BinData ReturnedData((char *)Data2);
  Strings.push_back(Protocol);
  Strings.push_back(ReturnedData);
  Packet InData(Input);
  InData.Print();
  Packet OutgoingData(Strings,Binaries);  
  return OutgoingData;
}

void Application::Splash(){
  puts("ENiX Test Server, waiting for connections...");
}

void ServerSignal(int SignalData){
  unsigned int LocalPid=getpid();
  printf("\nSignal caught in pid %d... Exiting.\n",LocalPid);
  exit(0);
}

int main(int argc, char *argv[]){
  Networking NewServer(argc,argv,true);
  if(NewServer.ConnectedStatus!=Connected){
    NewServer.DisplayErrorText(NewServer.ConnectedStatus);
    exit(0);
  }
  NewServer.RunServer();
  return 0; 
}
