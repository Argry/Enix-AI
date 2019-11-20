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
#include "ENiX_Net.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <stdlib.h>
#include <time.h>
#include "ENiX_Packet.h"
#include "ENiX_WMSAPI.h"
#include "ENiX_Settings.h"
#include "ENiX_Disk.h"
#include "ENiX_Seq.h"

string PROTOCOL_PREFIX="Neural Network Client";

Packet Application::ExecuteCommand(BinData Inp){}

void GenericCLI(string Prompt,Networking Net);
WMS_Rec CreateTestRec(void);
WMS_Rec CreateRandomRec(string RecName);
WMS_Rec CreateTotallyRandom();
void PresentReceivedData(char *Prompt,Packet Source,size_t StartAt=1);
Packet Preprocess(char *Command);
void RequestHelp(Networking &Net);
void RunCommand(string ProtcolName,Networking &Net,string SemiColonSeparated);

void RequestHelp(Networking &Net){
  char *CLIRawInput=NULL,*Prompt=(char *)"  ";
  char *HelpCmd=(char *)"HELP";
  puts("Displaying help commands for this connection:\n");
  Packet HelpPacket=Preprocess(HelpCmd);
  Net.Send(HelpPacket.AllData);
  BinData HelpResp=Net.Receive();
  Packet IncomingHelpPacket(HelpResp);
  if(IncomingHelpPacket.Strings.size()>1){
    for(size_t _=2;_<IncomingHelpPacket.Strings.size();_++){
      string ReplyData(IncomingHelpPacket.Strings[_].Str,IncomingHelpPacket.Strings[_].Size);
      printf("%s%s\n",Prompt,ReplyData.c_str());
    }
  }
  else
    puts("[Response received from server, but with no data.]");
}

Packet Preprocess(char *Command){
  string RecordName="testrecord1";
  string CLIInput=string(Command);
  BinData SendMsg;
  vector<BinData> Binaries={};
  Sequence<int> Seq;
  BinData CLIData(Command);
  vector<string> ToProcess=Seq.Convert2StrVec(CLIData);  
  if(ToProcess[0]=="ADD"){
    //    DemoRec=CreateTotallyRandom();
    WMS_Rec DemoRec=CreateTestRec();
    WMSConversion Conversions;
    BinData RecordData=Conversions.WMS2Binary(DemoRec);
    Binaries.push_back(RecordData);
  }
  if(ToProcess[0]=="ADDFILE"){
    BinData FileData=Disk::ReadFromDisk(ToProcess[1].c_str());
    Binaries.push_back(FileData);
  }
  SendMsg.Change(CLIInput);
  vector<BinData> Strings={};
  Strings.push_back(SendMsg);
  Packet Outgoing(Strings,Binaries);
  return Outgoing;
}

void PresentReceivedData(char *Prompt,Packet Source,size_t StartAt){
  BinData Response;
  if(Source.Strings.size()>1){
    for(size_t _=StartAt;_<Source.Strings.size();_++){
      string ReplyData(Source.Strings[_].Str,Source.Strings[_].Size);
      printf("%s%s\n",Prompt,ReplyData.c_str());
    }
    Response=Source.Strings[1];
  }
  else
    puts("[Response received from remote system, but with no data.]");
  if(Source.Binaries.size()){
    BinData RecTransferMsg("SENDING RECORDS");
    BinData FileTransferMsg("SENDING FILE: ");
    if(RecTransferMsg==Response){
      putchar('\n');
      for(int _=0;_<Source.Binaries.size();_++){
	WMSConversion InitConv;
	InitConv.PrintWMSRec(InitConv.Binary2WMS(Source.Binaries[_]));
      }
    }
    else if(BinData::CompareWithin(Response,FileTransferMsg)){      
      if(Source.Binaries.size()>0)
	Disk::WriteToDisk(Source.Binaries[0],Response.Str+FileTransferMsg.Size);
    }
  }
}

void ServerSignal(int SignalData){
  unsigned int LocalPid=getpid();
  printf("\nSignal caught in pid %d... Exiting.\n",LocalPid);
  exit(0);
}

void Application::Splash(){
  puts("ENiX injector client, connecting to server...");
}

WMS_Rec CreateTestRec(){
  WMS_Rec TestRecord={};
  vector<BinData> TestVec={};
  string TestRecName="TEST1";
  string TestRecD1="Data1";
  string TestRecD2="Data2";
  BinData Test1=TestRecName;
  BinData Test2=TestRecD1;
  BinData Test3=TestRecD2;
  TestVec.push_back(Test1);
  TestVec.push_back(Test2);
  TestVec.push_back(Test3);
  TestRecord.Contents.push_back(TestVec);
  return TestRecord;
}

WMS_Rec CreateRandomRec(string RecName){
  int AttributeNum=rand()%10;
  WMS_Rec TestRecord={};
  vector<BinData> Header={};
  vector<BinData> Values={};
  vector<BinData> Attributes={};
  BinData Name=RecName;
  Header.push_back(Name);
  TestRecord.Contents.push_back(Header);
  for(int _=0;_<AttributeNum;_++){
    int Index=rand()%10;
    string IndexCache=BinData::Int2String(Index);
    string Value="Value"+IndexCache;
    string Attribute="Attrib"+IndexCache;
    BinData BinValue=Value;
    BinData BinAttr=Attribute;
    Attributes.push_back(BinAttr);
    Values.push_back(BinValue);
  }
  TestRecord.Contents.push_back(Attributes);
  TestRecord.Contents.push_back(Values);
  return TestRecord;
}

WMS_Rec CreateTotallyRandom(){
  int RecordIndex=rand()%10;
  string RecName="recordname"+BinData::Int2String(RecordIndex);
  int AttributeNum=rand()%10;
  WMS_Rec TestRecord={};
  TestRecord.Contents={};
  vector<BinData> Header={};
  vector<BinData> Values={};
  vector<BinData> Attributes={};
  BinData Name=RecName;
  Header.push_back(Name);
  TestRecord.Contents.push_back(Header);
  for(int _=0;_<AttributeNum;_++){
    int Index=rand()%100;
    string IndexCache=BinData::Int2String(Index);
    string Value="Value"+IndexCache;
    string Attribute="Attrib"+IndexCache;
    BinData BinValue=Value;
    BinData BinAttr=Attribute;
    Attributes.push_back(BinAttr);
    Values.push_back(BinValue);
  }
  TestRecord.Contents.push_back(Attributes);
  TestRecord.Contents.push_back(Values);
  return TestRecord;
}

void GenericCLI(string PromptStr,Networking Net){
  string NewPrompt="ENiX4:"+PromptStr+"> ";
  char *CLIRawInput=NULL;
  char *Prompt=const_cast<char *>(NewPrompt.c_str());
  char *UserPrompt=(char *)"User> ";
  puts("\nTip: Use the pid from above to abort processes.");
  while(1){
    putchar('\n');
    CLIRawInput=readline(UserPrompt);
    if(CLIRawInput){
      add_history(CLIRawInput);
      Packet Outgoing=Preprocess(CLIRawInput);
      Net.Send(Outgoing.AllData);
      BinData Test=Net.Receive();
      Packet Incoming(Test);
      PresentReceivedData(Prompt,Incoming);
    }
  }
}

void RunCommand(string ProtcolName,Networking &Net,string SemiColonSeparated){
  vector<string> Commands=List::String2List(SemiColonSeparated,';');
  for(size_t _=0;_<Commands.size();++_){
    char *Data=const_cast<char *>(Commands[_].c_str());
    printf(" <- %s\n",Data);
    Packet Outgoing=Preprocess(Data);
    Net.Send(Outgoing.AllData);
    BinData Test=Net.Receive();
    Packet Incoming(Test);
    PresentReceivedData((char *)" -> ",Incoming);
  }
}

int main(int argc, char *argv[]){  
  if(argc<3){
    puts("Invalid parameters");
    puts("Usage:");
    printf("%s <hostname> <port> [Commands]\n\nAborting.",argv[0]);
    puts("\nIf [Commands] do not exist, then the interface will go into interactive mode.");
    puts("\nOtherwise, use semicolons to separate commands.");
    return -1;
  }

  Networking Test(argc,argv,false);
  if(Test.ConnectedStatus!=Connected){
    Test.DisplayErrorText(Test.ConnectedStatus);
    exit(0);
  }
  
  BinData Input=Test.Receive();
  Packet ProtocolData(Input);
  putchar('\n');
  PresentReceivedData((char *)"Probed: ",ProtocolData,0);
  putchar('\n');
  
  if(ProtocolData.Strings.size()==0){
    puts("Unrecognised protocol");
    return -1;
  }
  srand (time(NULL));
  
  RequestHelp(Test);
  
  string ProtocolName(ProtocolData.Strings[0].Str,ProtocolData.Strings[0].Size);
  
  if(argc<4){
    GenericCLI(ProtocolName,Test);
  }
  else{
    string SemiColonSeparated(argv[3],strlen(argv[3]));
    RunCommand(ProtocolName,Test,SemiColonSeparated);
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
