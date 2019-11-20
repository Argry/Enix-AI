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


#include <iostream>
#include <string>
#include <sstream>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <signal.h>
#include <arpa/inet.h>
#include <cstdint>

#include "ENiX_Net.h"
#include "ENiX_Packet.h"
#include "ENiX_WMSAPI.h"
#include "ENiX_Seq.h"

extern string PROTOCOL_PREFIX;

int IsParentProcess;

void CrashSignal(int SignalData){
  printf("\n !! [Session:%d] ERROR: Process crashed.\n\n",getpid());
  fflush(stdout);

  kill(getpid(), SIGABRT);
  
  exit(0);
}

Packet Networking::CommandToServer(Networking *Net,string Command){
  Packet Outgoing=Preprocess(Command.c_str());
  // Grab the protocol data and ditch it.
  Net->Send(Outgoing.AllData);
  BinData ForgetTest=Net->Receive();
  Packet ForgetMe(ForgetTest);
  // Now get the real data.
  Net->Send(Outgoing.AllData);
  BinData Test=Net->Receive();
  Packet Incoming(Test);
  return Incoming;
}

Packet Networking::Preprocess(const char *Command){
  string CLIInput=string(Command);
  BinData SendMsg;
  vector<BinData> Binaries={};
  BinData CLIData(Command);
  SendMsg.Change(CLIInput);
  vector<BinData> Strings={};
  Strings.push_back(SendMsg);
  Packet Outgoing(Strings,Binaries);
  return Outgoing;
}

void Networking::PopulateErrors(){
  NetErrorType={"Connection Established","Socket Error","Bind Error","No Host","Cannot Connect","Error Writing To Socket","Error Reading From Socket","Accept Error","Fork Error"};  
}

Networking::Networking(){
  PopulateErrors();
  UseIPC=false;
}

NetworkStatus Networking::Send(BinData Data){
  /*
  unsigned long int DataWrittenSize=0;
  uint32_t NetworkSize=htonl((uint32_t)Data.Size);
  DataWrittenSize=write(Socket,&NetworkSize,sizeof(uint32_t));
  DataWrittenSize=write(Socket,Data.Str,Data.Size);
  if(DataWrittenSize<Data.Size){
    return ErrorWritingToSocket; 
  }
  */

  // 100% network performance improvement:
  unsigned long int DataWrittenSize=0;
  uint32_t NetworkSize=htonl((uint32_t)Data.Size);
  BinData ToSend(Data.Size+4);
  *(uint32_t *)ToSend.Str=NetworkSize;
  memcpy(ToSend.Str+4,Data.Str,Data.Size);
  DataWrittenSize=write(Socket,ToSend.Str,ToSend.Size);  
  if(DataWrittenSize<ToSend.Size){
    return ErrorWritingToSocket; 
  }
}

BinData Networking::Receive(){
  int DataReadSize=0,WriteTo=0;
  uint32_t TotalSize=0;
  BinData Data;
  if(read(Socket,&TotalSize,sizeof(uint32_t))<1)
    return {};
  TotalSize=(uint32_t)ntohl(TotalSize);
  Data.Str=(char *)calloc((TotalSize+1)*sizeof(char),sizeof(char));
  Data.Size=TotalSize;
  do{
    if(256<TotalSize)
      DataReadSize=read(Socket,Data.Str+WriteTo,256);
    else
      DataReadSize=read(Socket,Data.Str+WriteTo,TotalSize);
    WriteTo+=DataReadSize; 
    if(DataReadSize<0)
      break;
  } while(WriteTo<TotalSize);
  if(DataReadSize<0){
    return {};
  }
  return Data;
}

NetworkStatus Networking::TestSend(){
  int DataWrittenSize=0;
  char DataBuffer[256];
  printf("Please enter the message: ");
  bzero(DataBuffer,256);
  fgets(DataBuffer,256,stdin);
  DataWrittenSize=write(Socket,DataBuffer,strlen(DataBuffer));
  if(DataWrittenSize<256){
    return ErrorWritingToSocket;
  }
}

NetworkStatus Networking::TestReceive(){
  int DataReadSize=0;
  char DataBuffer[256];
  bzero(DataBuffer,256);
  DataReadSize=read(Socket,DataBuffer,256);
  if(DataReadSize<256){
    return ErrorReadingFromSocket;
  }
  printf("%s\n",DataBuffer);
}

Networking::Networking(int argc,char **argv,bool ServerFlag,bool IPCOn){
  PopulateErrors();
  if(argc<3){
    fprintf(stderr,"usage %s hostname port\n", argv[0]);
    exit(0);
  }
  string Hostname(argv[1]);
  ConnectedStatus=NetSetup(Hostname,atoi(argv[2]),ServerFlag);
  UseIPC=IPCOn;
}

Networking::Networking(string Host,int Port,bool ServerFlag,bool IPCOn){
  PopulateErrors();
  ConnectedStatus=NetSetup(Host,Port,ServerFlag);
  UseIPC=IPCOn;
}

void Networking::ConnectionHandler(int IncomingSocket,int ProcessPid){
  BinData Received;
  Packet Executed;
  Received=Receive();
  if((Received.Str==NULL)||(Received.Size==0)){
    if(UseIPC)
      if(!DEBUGSERVER)
	IPCComms.DelPidProc(getpid());
    printf(" <> [Session:%d] Terminating Server Child Process (Client Disconnected)\n",getpid());
    exit(0);
  }
  ThisProcessPid=ProcessPid;
  Executed=ExecuteCommand(Received);
  Send(Executed.AllData);
}

NetworkStatus Networking::RunServer(){
  int ProcessPid=0;
  int ConnectionPid=getpid();
  socklen_t SocketSize;
  string ProtocolData="ENiX4 Neural Network Processor: ";
  ostringstream Convert;
  ProcessPid=ConnectionPid;
  SocketSize=sizeof(cli_addr);
  IsParentProcess=1;
  if(UseIPC)
    if(!DEBUGSERVER)
      IPCComms.Create(NNSHMSIZE,NNSHMKEY);
  if(DEBUGSERVER){
    Socket=accept(ServerSocket,(struct sockaddr *)&cli_addr,&SocketSize);
    if(ServerSocket<0)
      error("ERROR on accept");
    Convert<<ConnectionPid;
    ProtocolData+=Convert.str();
    BinData SessionData(ProtocolData);
    Send(SessionData);
  }
  signal(SIGCHLD,SIG_IGN);
  while(1){
    if(DEBUGSERVER)
      ConnectionHandler(Socket,getpid());
    else{
      Socket=accept(ServerSocket,(struct sockaddr *)&cli_addr,&SocketSize);
      if(ServerSocket<0){
	return AcceptError;
      }
      ProcessPid=fork();
      if(ProcessPid<0){
	return ForkError;
      }
      if(ProcessPid==0){
	close(ServerSocket);
	IsParentProcess=0;
	if(UseIPC)
	  if(!DEBUGSERVER)
	    IPCComms.Attach(NNSHMSIZE,NNSHMKEY);

	ProtocolData=(string)PROTOCOL_PREFIX;

	string Pid=BinData::Int2String(getpid());

	printf(" <> [Session:%d] New %s Worker Spawned.\n",getpid(),PROTOCOL_PREFIX.c_str());
	BinData SessionData(ProtocolData);

	vector<BinData> Binaries={};
	vector<BinData> Strings={};

	Strings.push_back(SessionData);
	Strings.push_back("Pid: "+Pid);


	Packet InitMsg(Strings,Binaries);

	Send(InitMsg.AllData);

	while(1){
	  ConnectionHandler(Socket,ConnectionPid);
	}
	exit(0);
      }
      else 
	close(Socket);
    }
  }
  if(UseIPC)
    if(!DEBUGSERVER)
      IPCComms.Destroy();
  close(ServerSocket);  
}

NetworkStatus Networking::NetSetup(string Host,int Port,bool ServerFlag){
  ServerSocket=0;

  static int Inc;
  if(!Inc)
    Splash();
  ++Inc;

  PortNum=Port;
  if(ServerFlag==true){
    printf(" <> [Session:%d] Listening on %s:%d\n",getpid(),Host.c_str(),Port);
    ServerSocket=socket(AF_INET,SOCK_STREAM,0);
    if(ServerSocket<0){
      return SocketError;
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=INADDR_ANY;
    serv_addr.sin_port=htons(PortNum);
    signal(SIGINT,ServerSignal);
    signal(SIGSEGV,CrashSignal);
    if(bind(ServerSocket,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0){
      return BindError;
    }
    listen(ServerSocket,5);
  }
  else{
    //    printf("Connecting to: %s:%d\n",Host.c_str(),Port);
    Socket=socket(AF_INET, SOCK_STREAM,0);
    if(Socket<0){
      return SocketError;
    }
    Server=gethostbyname(Host.c_str());
    if(Server==NULL){
      return NoHost;
    }
    bzero((char *)&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    bcopy((char *)Server->h_addr,(char *)&serv_addr.sin_addr.s_addr,Server->h_length);
    serv_addr.sin_port=htons(PortNum);
    if (connect(Socket,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0){
      return CannotConnect;
    }
  }
  return Connected;
}

Networking::~Networking(){
  close(Socket);
}

void Networking::DisplayErrorText(NetworkStatus Type){
  int Data=(int) Type;
  perror(NetErrorType[(int)Type].c_str());
}

void Networking::error(const char *msg){
  perror(msg);
  //  exit(0);
}

/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
*/
