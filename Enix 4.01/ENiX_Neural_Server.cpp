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

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <algorithm>
#include "ENiX_Net.h"
#include "ENiX_Awareness.h"
#include "ENiX_Seq.h"
#include "ENiX_Neural_Server.h"
#include "ENiX_Settings.h"
#include "ENiX_Daemon.h"

string PROTOCOL_PREFIX="Neural Network Processor";

void ServerSignal(int SignalData){
  unsigned int LocalPid=getpid();
  printf("\nSignal caught in pid %d... Exiting.\n",LocalPid);
  if(!DEBUGSERVER){
    if(IsParentProcess)
      IPCComms.Destroy();
    else{
      IPCComms.DelPidProc(LocalPid);
      IPCComms.Detach();
    }
  }
  exit(0);
}

/*
void NeuralServer::ClearANN(){
  if(ThisSessionANN!=NULL){
    delete ThisSessionANN;
    ThisSessionANN=NULL;
  }  
}

string NeuralServer::RetrieveANN(){
  string Response="";
  if(ThisSessionANN->Neural2Str())
    Response+=ThisSessionANN->NeuralAsStr;
  else
    Response="NO NEURAL NETWORK";
  return Response;
}
*/

Packet NeuralServer::InitialisePacket(string Response){
  Packet Answer={};
  Answer.Binaries={};
  Answer.Strings={};
  BinData Empty("");
  Answer.Strings.push_back(Empty);
  BinData AddRecStr(Response);
  Answer.Strings.push_back(AddRecStr);
  return Answer;
}

NeuralServer::NeuralServer(){
  CmdSetInitialised=false;
  InitialiseInstructions();
}

void NeuralServer::InitialiseInstructions(){
  if(!CmdSetInitialised){

    CommandName.push_back("ABORT");
    InstructionSet.push_back(&NeuralServer::Abort);
    Parameters.push_back(0);
    OnlineHelp.push_back("ABORT PID# COMPLEXITY#");

    CommandName.push_back("CLEAR");
    InstructionSet.push_back(&NeuralServer::Clear);
    Parameters.push_back(0);
    OnlineHelp.push_back("CLEAR");

    CommandName.push_back("DECONSTRUCT");
    InstructionSet.push_back(&NeuralServer::Deconstruct);
    Parameters.push_back(0);
    OnlineHelp.push_back("DECONSTRUCT HOSTS 3 SEQ 0 1 2 3 4 5 6 7");

    CommandName.push_back("DLEARN");
    InstructionSet.push_back(&NeuralServer::DLearn);
    Parameters.push_back(0);
    OnlineHelp.push_back("DLEARN 0 1 2 3 4 5 ? 7");
    
    CommandName.push_back("EXECUTE");
    InstructionSet.push_back(&NeuralServer::Execute);
    Parameters.push_back(0);
    OnlineHelp.push_back("EXECUTE 2 6 4");

    CommandName.push_back("FRAGMENT");
    InstructionSet.push_back(&NeuralServer::Fragment);
    Parameters.push_back(0);
    OnlineHelp.push_back("FRAGMENT ID 0 / 3 MUL 1 SEQ 0 0 1 1 1 1 1 1 1");

    CommandName.push_back("HELP");
    InstructionSet.push_back(&NeuralServer::Help);
    Parameters.push_back(0);
    OnlineHelp.push_back("HELP");

    CommandName.push_back("LEARN");
    InstructionSet.push_back(&NeuralServer::Learn);
    Parameters.push_back(0);
    OnlineHelp.push_back("LEARN 0 1 2 3 4 5 ? 7");

    CommandName.push_back("LOAD");
    InstructionSet.push_back(&NeuralServer::Load);
    Parameters.push_back(0);
    //    OnlineHelp.push_back("LOAD 3 0 1 1 8 1 2 4 1 2 8 0 0 0 0 0 0 0 0 0 1 1 8 2 2 4 1 2 8 1 0 0 0 0 0 0 0 0 1 1 8 4 2 4 1 2 8 2 0 0 0 0 0 0 0");
    OnlineHelp.push_back("LOAD 3 0 1 1 1 8 1 2 4 1 2 8 0 0 0 0 0 0 0 0 0 1 1 1 8 2 2 4 1 2 8 1 0 0 0 0 0 0 0 0 1 1 1 8 4 2 4 1 2 8 2 0 0 0 0 0 0 0");

    CommandName.push_back("ODDMAN");
    InstructionSet.push_back(&NeuralServer::ODDMAN);
    Parameters.push_back(0);
    OnlineHelp.push_back("ODDMAN 0 1 2 3 4 5 7 7");

    CommandName.push_back("RETRIEVE");
    InstructionSet.push_back(&NeuralServer::Retrieve);
    Parameters.push_back(0);
    OnlineHelp.push_back("RETRIEVE");

    CommandName.push_back("SHOWNEURAL");
    InstructionSet.push_back(&NeuralServer::ShowNeural);
    Parameters.push_back(0);
    OnlineHelp.push_back("SHOWNEURAL");

    CommandName.push_back("TERMINATE");
    InstructionSet.push_back(&NeuralServer::Terminate);
    Parameters.push_back(0);
    OnlineHelp.push_back("TERMINATE [PID#]");
    
    CommandName.push_back("UNDERSTOOD");
    InstructionSet.push_back(&NeuralServer::Understood);
    Parameters.push_back(0);
    OnlineHelp.push_back("UNDERSTOOD 2 3 5 7 11 13 17 19");

    /*
    CommandName.push_back("SHMTEST1");
    InstructionSet.push_back(&NeuralServer::SHMTest1);
    Parameters.push_back(0);
    OnlineHelp.push_back("SHMTEST1 (DO NOT USE)");
    */

    CmdSetInitialised=true;
  }
}

Packet NeuralServer::Abort(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("MESSAGE TRANSMITTED TO PROCESSORS");
  Sequence<int> Parameters(Commands);
  if(!DEBUGSERVER)
    IPCComms.SetFromPid(Parameters.Values[1],Parameters.Values[2]);
  return Answer;
}

Packet NeuralServer::Clear(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("STATUS:");
  ThisSessionANN->ClearANN();
  Answer.Strings.resize(3);
  BinData Conversion("DONE");
  Answer.Strings[2]=Conversion;
  return Answer;
}

Packet NeuralServer::Deconstruct(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("FRAGMENT LIST:");
  ThisSessionANN->ClearANN();
  vector<string>::iterator HostPos;
  vector<string>::iterator SeqPos;
  HostPos=find(Commands.begin(),Commands.end(),"HOSTS");
  SeqPos=find(Commands.begin(),Commands.end(),"SEQ");
  if((HostPos==Commands.end())||(SeqPos==Commands.end())){
    BinData Error("INVALID SYNTAX");
    Answer.Strings[1]=Error;
  }
  else{
    unsigned int NumHosts=stoi(*(HostPos+1));
    vector<string> SeqDataStr(Commands.end()-SeqPos-1);
    copy((vector<string>::iterator)SeqPos+1,Commands.end(),SeqDataStr.begin());
    Sequence<int> Seq3(SeqDataStr);
    string SeshId=BinData::GetSessionIdentifier();
    ThisSessionANN=new ENiX_NN();
    ThisSessionANN->FormatPrefix="  i "+SeshId;
    vector<string> BrokenData=ThisSessionANN->Deconstruct(NumHosts,Seq3);
    ThisSessionANN->ClearANN();
    if(BrokenData.size()){
      string InfoPrefix="  i "+SeshId;
    const char *CharInfoPrefix=InfoPrefix.c_str();
      putchar('\n');
      for(size_t FragmentId=0;FragmentId<BrokenData.size();FragmentId++){
	printf("%s%s\n",CharInfoPrefix,BrokenData[FragmentId].c_str());
	BinData Conversion(BrokenData[FragmentId]);
	Answer.Strings.push_back(Conversion);
      }
      putchar('\n');
    }
  }
  return Answer;
}

Packet NeuralServer::Execute(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("SEQUENCE:");
  vector<string> Parameters=Commands;
  Answer.Strings.resize(3);
  if(ThisSessionANN){
    Parameters.erase(Parameters.begin());
    Sequence<int> Seq2(Parameters);
    Sequence<int> Calculated;
    Calculated=ThisSessionANN->Execute(Seq2);
    BinData Resp(Calculated.Convert2Str(Calculated));
    Answer.Strings[2]=Resp;
  }
  else{
    BinData Error("NO NEURAL NETWORK");
    Answer.Strings[2]=Error;
  }
  return Answer;
}

Packet NeuralServer::Fragment(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("NEURAL NET:");
  ThisSessionANN->ClearANN();
  vector<string>::iterator IdPos;
  vector<string>::iterator OutofPos;
  vector<string>::iterator MulPos;
  vector<string>::iterator SeqPos;
  IdPos=find(Commands.begin(),Commands.end(),"ID");
  OutofPos=find(Commands.begin(),Commands.end(),"/");
  MulPos=find(Commands.begin(),Commands.end(),"MUL");
  SeqPos=find(Commands.begin(),Commands.end(),"SEQ");
  Answer.Strings.resize(3);
  if((IdPos==Commands.end())||(OutofPos==Commands.end())||(MulPos==Commands.end())||(SeqPos==Commands.end())){
    BinData Error("INVALID SYNTAX");
    Answer.Strings[2]=Error;
  }
  else{
    putchar('\n');
    unsigned int Id=stoi(*(IdPos+1)),Outof=stoi(*(OutofPos+1)),Multiplier=stoi(*(MulPos+1));
    vector<string> SeqDataStr(Commands.end()-SeqPos-1);
    copy((vector<string>::iterator)SeqPos+1,Commands.end(),SeqDataStr.begin());
    Sequence<int> Seq3(SeqDataStr);
    ThisSessionANN=new ENiX_NN(Seq3);
    ThisSessionANN->FormatPrefix="  i "+BinData::GetSessionIdentifier();
    ThisSessionANN->LearnTopLevel(Id,Outof);
    BinData Resp(ThisSessionANN->RetrieveANN());
    Answer.Strings[2]=Resp;
  }
  return Answer;
}

Packet NeuralServer::Help(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("HELP:");
  for(size_t _=0;_<OnlineHelp.size();_++){
    BinData Text(OnlineHelp[_]);
    Answer.Strings.push_back(Text);
  }
  return Answer;
}

Packet NeuralServer::Learn(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("NEURAL NET:");
  ThisSessionANN->ClearANN();
  putchar('\n');
  vector<string> Parameters=Commands;
  Parameters.erase(Parameters.begin());
  Sequence<int> Seq2(Parameters);
  ThisSessionANN=new ENiX_NN(Seq2);
  ThisSessionANN->FormatPrefix="  i "+BinData::GetSessionIdentifier();
  ThisSessionANN->LearnTopLevel(0,1);
  BinData Resp(ThisSessionANN->RetrieveANN());
  Answer.Strings.resize(3);
  Answer.Strings[2]=Resp;
  return Answer;
}

Packet NeuralServer::DLearn(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("NEURAL NET:");
  ThisSessionANN->ClearANN();
  putchar('\n');
  vector<string> Parameters=Commands;
  Parameters.erase(Parameters.begin());
  Sequence<int> Seq2(Parameters);
  ThisSessionANN=new ENiX_NN(Seq2);
  ThisSessionANN->FormatPrefix="  i "+BinData::GetSessionIdentifier();
  ThisSessionANN->DLearnTopLevel(0,1);
  BinData Resp(ThisSessionANN->RetrieveANN());
  Answer.Strings.resize(3);
  Answer.Strings[2]=Resp;
  return Answer;
}

Packet NeuralServer::Load(vector<string> Commands,Packet Data){
  ThisSessionANN->ClearANN();
  vector<string> Parameters=Commands;
  ThisSessionANN=new ENiX_NN();
  ThisSessionANN->FormatPrefix="  i "+BinData::GetSessionIdentifier();
  Parameters.erase(Parameters.begin());
  Sequence<int> Seq2(Parameters);
  ThisSessionANN->PopulateNeural(Seq2);
  if(ThisSessionANN->NumSubnets==0)
    return InitialisePacket("FAILED TO LOAD NEURAL DATA");
  putchar('\n');
  ThisSessionANN->Debug();
  return InitialisePacket("NEURAL DATA LOADED");
}

Packet NeuralServer::ODDMAN(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("TPI:");
  ThisSessionANN->ClearANN();
  vector<string> Parameters=Commands;
  Parameters.erase(Parameters.begin());
  Sequence<int> Seq2(Parameters);
  Awareness Oddman(Seq2);
  BinData Resp(Seq2.Convert2Str(Oddman.OddmanOut()));
  Answer.Strings.resize(3);
  Answer.Strings[2]=Resp;
  return Answer;
}

Packet NeuralServer::Retrieve(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("NEURAL NET:");
  Answer.Strings.resize(3);
  if(ThisSessionANN!=NULL){
    BinData Resp(ThisSessionANN->RetrieveANN());
    Answer.Strings[2]=Resp;
  }
  else{
    BinData Error("NO NEURAL NETWORK");
    Answer.Strings[2]=Error;
  }
  return Answer;
}

Packet NeuralServer::ShowNeural(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("NEURAL DIAGRAM:");
  vector<string> Parameters=Commands;
  Answer.Strings.resize(3);
  if(ThisSessionANN){
    BinData Resp(ThisSessionANN->NN2String(*ThisSessionANN));
    Answer.Strings[2]=Resp;
  }
  else{
    BinData Error("NO NEURAL NETWORK");
    Answer.Strings[2]=Error;
  }
  return Answer;
}

Packet NeuralServer::Terminate(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("TERMINATED");
  vector<string> Parameters=Commands;
  Parameters.erase(Parameters.begin());
  Sequence<int> Seq2(Parameters);
  if(Seq2.Size()>0)
    kill(Seq2.Values[0],SIGTERM);
  else
    exit(0);
  return Answer;
}

Packet NeuralServer::Understood(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("BOOLEAN:");
  ThisSessionANN->ClearANN();
  vector<string> Parameters=Commands;
  Parameters.erase(Parameters.begin());
  Sequence<int> Seq2(Parameters);
  Awareness Oddman(Seq2);
  Oddman.OddmanOut();
  string UnderstoodStatus="";
  if(Oddman.Understood)
    UnderstoodStatus="YES";
  else
    UnderstoodStatus="NO";
  BinData Resp(UnderstoodStatus);
  Answer.Strings.resize(3);
  Answer.Strings[2]=Resp;
  return Answer;
}

/*
Packet NeuralServer::SHMTest1(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("CLEARED");
  if(!DEBUGSERVER){
    vector <struct SharedNeuralData> TestData,NewData;
    TestData.resize(2);
    TestData[0]=IPCComms.CreateNeuralDataStruct(42,0,12);
    TestData[1]=IPCComms.CreateNeuralDataStruct(43,1,13);
    IPCComms.DispNeuralDataStruct(TestData[0]);
    IPCComms.DispNeuralDataStruct(TestData[1]);
    IPCComms.PutSharedData(TestData);
    NewData=IPCComms.GetSharedData();
    IPCComms.DispNeuralDataStruct(NewData[0]);
    IPCComms.DispNeuralDataStruct(NewData[1]);
    BinData Resp("INFORMATION TRANSMITTED TO SERVER CONSOLE");
    Answer.Strings[1]=Resp;
  }
  else{
    BinData Resp("NOT AVAILABLE IN DEBUGSERVER MODE");
    Answer.Strings[1]=Resp;
  }
  return Answer;
}
*/

Packet NeuralServer::ExecuteCommand(vector<string> Commands,Packet Data){
  bool Found=false;
  size_t CommandIndex=-1;
  BinData Protocol(PROTOCOL_PREFIX);
  if(Commands.size()){
    for(size_t _=0;_<CommandName.size();_++){
      BinData Converted(Commands[0]);
      if(Converted==CommandName[_]){
	Found=true;
	CommandIndex=_;
	break;
      }
    }
    if(Found&&(CommandIndex!=-1)){
      if(Commands.size()>Parameters[CommandIndex]){
	Packet Retrieved=(this->*InstructionSet[CommandIndex])(Commands,Data);
	Retrieved.Strings[0]=Protocol;
	return Retrieved;
      }
      else{
	Packet Answer=InitialisePacket("ERROR: WRONG COMMAND OPTIONS");
	Answer.Strings[0]=Protocol;
	return Answer;
      }
    }
  }
  Packet Answer=InitialisePacket("ERROR: UNRECOGNISED COMMAND: "+Commands[0]);
  Answer.Strings[0]=Protocol;
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
    Sequence<int> Seq1={};
    vector<string> ToProcess=Seq1.Convert2StrVec(Incoming.Strings[0]);
    if(!DEBUGSERVER)
      IPCComms.AddPidProc(ThisProcessPid);
    NeuralServer ThisCommand;
    Outgoing=ThisCommand.ExecuteCommand(ToProcess,Incoming);
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

void Application::Splash(){
  puts("");
  puts("     _/_/_/_/  _/      _/  _/  _/      _/      _/  _/");
  puts("    _/        _/_/    _/        _/  _/        _/  _/ ");
  puts("   _/_/_/    _/  _/  _/  _/      _/          _/_/_/_/");
  puts("  _/        _/    _/_/  _/    _/  _/            _/   ");
  puts(" _/_/_/_/  _/      _/  _/  _/      _/          _/    ");
  puts("");
  puts("----------[Neural Network Processor Module]----------");
}

int main(int argc, char *argv[]){
  Daemon IfConfigured;
  {
    string HostName=NNPHOSTNAME;
    int PortNum=NNPPORTNUM;
    if(argc==3){
      HostName=string(argv[1]);
      PortNum=atoi(argv[2]);
    }
    ThisSessionANN=NULL;
    Networking NewServer(HostName,PortNum,true,true);
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
