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

#include "Amoeba_Daemon.h"
#include "ENiX_Net.h"
#include "ENiX_Seq.h"
#include "ENiX_List.h"
#include "ENiX_Settings.h" 
#include "ENiX_Daemon.h"

string PROTOCOL_PREFIX="Amoeba Database 2.0";

void ServerSignal(int SignalData){
  unsigned int LocalPid=getpid();
  AmoebaDB->~WMSAPI();
  printf("\nSignal caught in pid %d... Exiting.\n",LocalPid);
  exit(0);
}

Packet AmoebaDaemon::InitialisePacket(string Response){
  Packet Answer={};
  Answer.Binaries={};
  Answer.Strings={};
  BinData Empty("");
  Answer.Strings.push_back(Empty);
  BinData AddRecStr(Response);
  Answer.Strings.push_back(AddRecStr);
  return Answer;
}

AmoebaDaemon::AmoebaDaemon(){
  CmdSetInitialised=false;
  InitialiseInstructions();
}

void AmoebaDaemon::InitialiseInstructions(){
  if(!CmdSetInitialised){
        
    CommandName.push_back("ADD");
    InstructionSet.push_back(&AmoebaDaemon::Add);
    Parameters.push_back(0);
    OnlineHelp.push_back("ADD");

    CommandName.push_back("ADDFILE");
    InstructionSet.push_back(&AmoebaDaemon::AddFile);
    Parameters.push_back(0);
    OnlineHelp.push_back("ADDFILE");
    
    CommandName.push_back("ADDPAIR");
    InstructionSet.push_back(&AmoebaDaemon::AddPair);
    Parameters.push_back(3);
    OnlineHelp.push_back("DELETEPAIR <Record ID> <Attribute 1> <Value 1>");

    CommandName.push_back("DEBUGALLOC");
    InstructionSet.push_back(&AmoebaDaemon::DebugAlloc);
    Parameters.push_back(0);
    OnlineHelp.push_back("DEBUGALLOC");

    CommandName.push_back("DEBUGREC");
    InstructionSet.push_back(&AmoebaDaemon::DebugRec);
    Parameters.push_back(0);
    OnlineHelp.push_back("DEBUGREC");

    CommandName.push_back("DEFRAG");
    InstructionSet.push_back(&AmoebaDaemon::Defrag);
    Parameters.push_back(0);
    OnlineHelp.push_back("DEFRAG");

    CommandName.push_back("DELETE");
    InstructionSet.push_back(&AmoebaDaemon::Delete);
    Parameters.push_back(1);
    OnlineHelp.push_back("DELETE <Record ID>");

    CommandName.push_back("DELETEFILE");
    InstructionSet.push_back(&AmoebaDaemon::DeleteFile);
    Parameters.push_back(1);
    OnlineHelp.push_back("DELETEFILE <Record ID>");

    CommandName.push_back("DELETEFROMPP");
    InstructionSet.push_back(&AmoebaDaemon::DeleteFromPP);
    Parameters.push_back(1);
    OnlineHelp.push_back("DELETEFROMPP <Record ID>");

    CommandName.push_back("DELETEPAIR");
    InstructionSet.push_back(&AmoebaDaemon::DeletePair);
    Parameters.push_back(2);
    OnlineHelp.push_back("DELETEPAIR <Record ID> <Attribute ID>");

    CommandName.push_back("DELETEPP");
    InstructionSet.push_back(&AmoebaDaemon::DeletePP);
    Parameters.push_back(1);
    OnlineHelp.push_back("DELETEPP <Personality Profile Name>");

    CommandName.push_back("EXIT");
    InstructionSet.push_back(&AmoebaDaemon::Exit);
    Parameters.push_back(0);
    OnlineHelp.push_back("EXIT");

    CommandName.push_back("FIND");
    InstructionSet.push_back(&AmoebaDaemon::Find);
    Parameters.push_back(1);
    OnlineHelp.push_back("FIND <Record Name>");

    CommandName.push_back("GETALL");
    InstructionSet.push_back(&AmoebaDaemon::GetAll);
    Parameters.push_back(0);
    OnlineHelp.push_back("GETALL");

    CommandName.push_back("GETALLPP");
    InstructionSet.push_back(&AmoebaDaemon::GetAllPP);
    Parameters.push_back(0);
    OnlineHelp.push_back("GETALLPP");

    CommandName.push_back("GETDATA");
    InstructionSet.push_back(&AmoebaDaemon::GetData);
    Parameters.push_back(1);
    OnlineHelp.push_back("GETDATA <Record ID>");

    CommandName.push_back("GETFILE");
    InstructionSet.push_back(&AmoebaDaemon::GetFile);
    Parameters.push_back(1);
    OnlineHelp.push_back("GETFILE <Record Name>");

    CommandName.push_back("GETNAME");
    InstructionSet.push_back(&AmoebaDaemon::GetName);
    Parameters.push_back(1);
    OnlineHelp.push_back("GETNAME <Record ID>");

    CommandName.push_back("GETPP");
    InstructionSet.push_back(&AmoebaDaemon::GetPP);
    Parameters.push_back(0);
    OnlineHelp.push_back("GETPP");

    CommandName.push_back("HELP");
    InstructionSet.push_back(&AmoebaDaemon::Help);
    Parameters.push_back(0);
    OnlineHelp.push_back("HELP");

    CommandName.push_back("LISTPP");
    InstructionSet.push_back(&AmoebaDaemon::ListPP);
    Parameters.push_back(0);
    OnlineHelp.push_back("LISTPP");

    CommandName.push_back("LOAD");
    InstructionSet.push_back(&AmoebaDaemon::Load);
    Parameters.push_back(1);
    OnlineHelp.push_back("LOAD <Filename>");

    CommandName.push_back("PURGE");
    InstructionSet.push_back(&AmoebaDaemon::Purge);
    Parameters.push_back(0);
    OnlineHelp.push_back("PURGE");

    CommandName.push_back("REPLACE");
    InstructionSet.push_back(&AmoebaDaemon::Replace);
    Parameters.push_back(1);
    OnlineHelp.push_back("REPLACE <Record Position>");

    CommandName.push_back("SAVE");
    InstructionSet.push_back(&AmoebaDaemon::Save);
    Parameters.push_back(1);
    OnlineHelp.push_back("SAVE <Filename>");

    CommandName.push_back("SETPP");
    InstructionSet.push_back(&AmoebaDaemon::SetPP);
    Parameters.push_back(1);
    OnlineHelp.push_back("SETPP <Personality Profile Name>");

    CommandName.push_back("STATS");
    InstructionSet.push_back(&AmoebaDaemon::Stats);
    Parameters.push_back(0);
    OnlineHelp.push_back("STATS");

    CmdSetInitialised=true;
  }
}

Packet AmoebaDaemon::ExecuteCommand(vector<string> Commands,Packet Data){
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

Packet AmoebaDaemon::Add(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("DATA STORED");
  string InfoPrefix="  i "+BinData::GetSessionIdentifier();
  const char *CharInfoPrefix=InfoPrefix.c_str();
  if(Data.Binaries.size()){
    printf("%sAdding record(s) to personality profile: \"",CharInfoPrefix);
    AmoebaDB->ActivePP.Print();
    puts("\":");
    for(int _=0;_<Data.Binaries.size();_++){
      WMSConversion InitConv;
      WMS_Rec WMSRec={};
      WMS_Rec Input=InitConv.Binary2WMS(Data.Binaries[_]);
      putchar('\n');
      WMSConversion::PrintWMSRec(InfoPrefix,Input);
      if(AMOEBAPARTITION){
	WMSRec=AmoebaDB->MergeFilteredWMSToDB(Input);
	AmoebaDB->AddWMSToDB(WMSRec);
      }
      else{
	AmoebaDB->AddWMSToDB(Input); // this is an experiment
      }
    }
    putchar('\n');
  }
  return Answer;
}

Packet AmoebaDaemon::AddFile(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("FILE STORED");
  string InfoPrefix="  i "+BinData::GetSessionIdentifier();
  const char *CharInfoPrefix=InfoPrefix.c_str();
  if(Data.Binaries.size()){
    string DataPrefix=InfoPrefix+"Saving file: "+Commands[1];
    printf("%s\n",DataPrefix.c_str());
    vector<BinData> Header;
    vector<BinData> Attrs;
    vector<BinData> Values;
    BinData RecordName(Commands[1]);
    Header.push_back(RecordName);
    BinData Attribute("FILE");
    Attrs.push_back(Attribute);
    Values.push_back(Data.Binaries[0]);
    WMS_Rec Record2Add;
    Record2Add.Contents.push_back(Header);
    Record2Add.Contents.push_back(Attrs);
    Record2Add.Contents.push_back(Values);
    AmoebaDB->AddWMSToDB(Record2Add);
  }
  return Answer;
}

Packet AmoebaDaemon::AddPair(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("PAIR ADDED");
  unsigned long int Location=stoi(Commands[1]);
  BinData Attrib(Commands[2]);
  BinData Value(Commands[3]);
  AmoebaDB->AddWMSPair(Attrib,Value,Location);
  return Answer;
}

Packet AmoebaDaemon::DebugAlloc(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("DEBUG DATA SENT TO SERVER CONSOLE");
  string InfoPrefix="  i "+BinData::GetSessionIdentifier();
  const char *CharInfoPrefix=InfoPrefix.c_str();
  putchar('\n');
  AmoebaDB->DebugAllocTable(InfoPrefix);
  putchar('\n');
  AmoebaDB->DebugDatabase(InfoPrefix);
  putchar('\n');
  return Answer;
}

Packet AmoebaDaemon::DebugRec(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("DEBUG DATA SENT TO SERVER CONSOLE");
  string InfoPrefix="  i "+BinData::GetSessionIdentifier();
  const char *CharInfoPrefix=InfoPrefix.c_str();
  AmoebaDB->ShowAllAmoebas(InfoPrefix);
  return Answer;
}

Packet AmoebaDaemon::Defrag(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("DATABASE DEFRAGMENTED");
  AmoebaDB->Defrag();
  return Answer;
}

Packet AmoebaDaemon::Delete(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("RECORD DELETED");
  int Location=stoi(Commands[1]);
  if(Location==-1){
    BinData Error("ERROR: RECORD NOT FOUND");
    Answer.Strings[1]=Error;
  }
  else{
    AmoebaDB->DelData(Location);
  }
  return Answer;
}

Packet AmoebaDaemon::DeleteFromPP(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("RECORD DELETED");
  int Location=stoi(Commands[1]);
  if(Location==-1){
    BinData Error("ERROR: RECORD NOT FOUND");
    Answer.Strings[1]=Error;
  }
  else
    AmoebaDB->DelFromPP(Location);
  return Answer;
}

Packet AmoebaDaemon::DeleteFile(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("FILE DELETED");
  BinData FileName(Commands[1]);
  unsigned long int Pos=AmoebaDB->Search(FileName);
  Amoeba_Rec Record;
  bool Error=false;
  if(Pos==-1)
    Error=true;
  else{
    Record=AmoebaDB->Bin2Amoeba(AmoebaDB->GetData(Pos));
    if(Record.RefSeg.size()<2){
      DEBUGMSG;
      Error=true;
    }
    else{
      if((Record.RefSeg[3]).size()){
	unsigned long int FilePos=(Record.RefSeg[3])[0];
	AmoebaDB->DeleteAmoeba(FilePos);
	AmoebaDB->DeleteAmoeba(Pos);
      }
      else{
	DEBUGMSG;
	Error=true;
      }
    }
  }
  if(Error)
    Answer.Strings[1]="COULD NOT DELETE FILE: "+Commands[1];    
  return Answer;
}

Packet AmoebaDaemon::DeletePair(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("PAIR DELETED");
  int Location=stoi(Commands[1]);
  int AttribID=stoi(Commands[2]);
  AmoebaDB->DelWMSPair(Location,AttribID);
  return Answer;
}

Packet AmoebaDaemon::DeletePP(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("DELETED PERSONALITY PROFILE: "+Commands[1]);
  string InfoPrefix="  i "+BinData::GetSessionIdentifier();
  const char *CharInfoPrefix=InfoPrefix.c_str();
  AmoebaDB->DeletePP(Commands[1]);
  return Answer;
}

Packet AmoebaDaemon::Exit(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("EXITING");
  ServerSignal(15);
  return Answer;
}

Packet AmoebaDaemon::Find(vector<string> Commands,Packet Data){
  BinData Needle(Commands[1].c_str(),Commands[1].size());
  unsigned long int FoundPos=AmoebaDB->Search(Needle);
  string LocationStr="";
  if(FoundPos==-1)
    LocationStr=(char *)"-1";
  else
    LocationStr=BinData::Int2String((int)(AmoebaDB->Search(Needle)));
  Packet Answer=InitialisePacket("RESULT FOUND @: "+LocationStr);
  return Answer;
}

Packet AmoebaDaemon::GetAll(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("SENDING RECORDS");
  string InfoPrefix="  i "+BinData::GetSessionIdentifier();
  const char *CharInfoPrefix=InfoPrefix.c_str();
  for(unsigned long int C=0;C<AmoebaDB->RecordNum;C++){
    int LocationIndex=AmoebaDB->GetIndexFromNumber(C);
    WMSConversion InitConv;
    BinData OutgoingRecord=InitConv.WMS2Binary(AmoebaDB->Amoeba2WMS(AmoebaDB->Bin2Amoeba(AmoebaDB->GetData(LocationIndex))));
    Answer.Binaries.push_back(OutgoingRecord);
  }
  return Answer;
}

Packet AmoebaDaemon::GetAllPP(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("SENDING RECORDS");
  vector<unsigned long int> RecordPositions=AmoebaDB->GetAllPPRec();
  for(size_t _=0;_<RecordPositions.size();_++){
    int LocationIndex=RecordPositions[_];
    WMSConversion InitConv;
    Amoeba_Rec AmoebaRec=AmoebaDB->Bin2Amoeba(AmoebaDB->GetData(LocationIndex));
    WMS_Rec Before=AmoebaDB->Amoeba2WMS(AmoebaRec);
    WMS_Rec After=AmoebaDB->InFilter(Before);
    BinData OutgoingRecord=InitConv.WMS2Binary(After);
    Answer.Binaries.push_back(OutgoingRecord);
  }
  return Answer;
}

Packet AmoebaDaemon::GetData(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("SENDING RECORDS");
  string InfoPrefix="  i "+BinData::GetSessionIdentifier();
  int Location=stoi(Commands[1]);
  WMSConversion InitConv;
  WMS_Rec DebugData=AmoebaDB->Amoeba2WMS(AmoebaDB->Bin2Amoeba(AmoebaDB->GetData(Location)));
  putchar('\n');
  WMSConversion::PrintWMSRec(InfoPrefix,DebugData);
  putchar('\n');
  BinData OutgoingRecord=InitConv.WMS2Binary(DebugData);
  Answer.Binaries.push_back(OutgoingRecord);
  return Answer;
}

Packet AmoebaDaemon::GetFile(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("SENDING FILE: "+Commands[1]);
  string InfoPrefix="  i "+BinData::GetSessionIdentifier();
  const char *CharInfoPrefix=InfoPrefix.c_str();
  BinData Needle(Commands[1]);
  unsigned long int Pos=AmoebaDB->Search(Needle);
  if(Pos==-1){
    printf("%sERROR: CANNOT FIND: %s.\n",CharInfoPrefix,Commands[1].c_str());
    Answer.Strings[1]="ERROR: CANNOT FIND: "+Commands[1];
  }
  else{
    Amoeba_Rec FileData=AmoebaDB->Bin2Amoeba(AmoebaDB->GetData(Pos));
    BinData FileContents;
    unsigned long int FileDataPos=(FileData.RefSeg[3])[0];
    if(FileDataPos!=-1){
      FileContents=AmoebaDB->GetName(FileDataPos);
      Answer.Binaries.push_back(FileContents);
    }
  }
  return Answer;
}

Packet AmoebaDaemon::GetName(vector<string> Commands,Packet Data){
  int Location=stoi(Commands[1]);
  Packet Answer=InitialisePacket("RECORD NAME: "+string(AmoebaDB->GetName(Location).Str));
  return Answer;
}

Packet AmoebaDaemon::GetPP(vector<string> Commands,Packet Data){
  string PersonalityProfile(AmoebaDB->ActivePP.Str,AmoebaDB->ActivePP.Size);
  Packet Answer=InitialisePacket("PERSONALITY PROFILE: "+PersonalityProfile);
  return Answer;
}

Packet AmoebaDaemon::Help(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("SENDING HELP");
  for(size_t _=0;_<OnlineHelp.size();_++){
    BinData Text(OnlineHelp[_]);
    Answer.Strings.push_back(Text);
  }
  return Answer;
}

Packet AmoebaDaemon::ListPP(vector<string> Commands,Packet Data){
  vector<string> PPList=AmoebaDB->GetPPList();
  class List NewList;
  string PPListStr=NewList.List2String(PPList);
  Packet Answer=InitialisePacket("LISTING PERSONALITY PROFILES: "+PPListStr);
  return Answer;
}

Packet AmoebaDaemon::Load(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("LOADED: "+Commands[1]);
  string InfoPrefix="  i "+BinData::GetSessionIdentifier();
  const char *CharInfoPrefix=InfoPrefix.c_str();
  if(!AmoebaDB->Disk2SHM(Commands[1].c_str())){
    printf("%sERROR: CANNOT READ FROM %s ON DISK.\n",CharInfoPrefix,Commands[1].c_str());
    Answer.Strings[1]="ERROR: CANNOT LOAD: "+Commands[1];
  }
  AmoebaDB->GetDBInfo();
  return Answer;
}

Packet AmoebaDaemon::Purge(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("EMERGENCY DATABASE ERASE");
  string InfoPrefix="  i "+BinData::GetSessionIdentifier();
  const char *CharInfoPrefix=InfoPrefix.c_str();
  printf("%s",CharInfoPrefix);
  AmoebaDB->CreateEmptyDB();
  printf("%s",CharInfoPrefix);
  AmoebaDB->InitFreeList();
  AmoebaDB->PutDBInfo();
  BinData PersonalityProfile("unknown");
  AmoebaDB->SetPP(PersonalityProfile);
  return Answer;
}

Packet AmoebaDaemon::Replace(vector<string> Commands,Packet Data){
  // Note: this hasn't been tested.
  Packet Answer=InitialisePacket("REPLACING RECORD: "+Commands[1]);
  string InfoPrefix="  i "+BinData::GetSessionIdentifier();
  const char *CharInfoPrefix=InfoPrefix.c_str();
  if(Data.Binaries.size()){
    printf("%sReplacing record at %s, to personality profile: \"",CharInfoPrefix,Commands[1].c_str());
    int Location=stoi(Commands[1]);
    if(Location!=-1){
      AmoebaDB->DelFromPP(Location);
    }
    WMSConversion InitConv;
    WMS_Rec WMSRec={};
    WMS_Rec Input=InitConv.Binary2WMS(Data.Binaries[0]);
    putchar('\n');
    WMSConversion::PrintWMSRec(InfoPrefix,Input);
  }
  return Answer;
}

Packet AmoebaDaemon::Save(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("SAVED: "+Commands[1]);
  string InfoPrefix="  i "+BinData::GetSessionIdentifier();
  const char *CharInfoPrefix=InfoPrefix.c_str();
  if(!AmoebaDB->SHM2Disk(Commands[1].c_str())){
    printf("%sERROR: CANNOT WRITE %s TO DISK.\n",CharInfoPrefix,Commands[1].c_str());
    Answer.Strings[1]="ERROR: CANNOT WRITE SAVE TO DISK: "+Commands[1];
  }
  return Answer;
}

Packet AmoebaDaemon::SetPP(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("SETTING PERSONALITY PROFILE AS: "+string(Commands[1]));
  BinData PersonalityProfile(Commands[1]);
  AmoebaDB->SetPP(PersonalityProfile);
  return Answer;
}

Packet AmoebaDaemon::Stats(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("STATS SENT TO SERVER CONSOLE");

  string InfoPrefix="  i "+BinData::GetSessionIdentifier();
  const char *CharInfoPrefix=InfoPrefix.c_str();

  AmoebaDB->Statistics(CharInfoPrefix);

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
    AmoebaDaemon ThisCommand;
    AmoebaDB->Lock();
    AmoebaDB->GetDBInfo();
    Outgoing=ThisCommand.ExecuteCommand(ToProcess,Incoming);
    AmoebaDB->PutDBInfo();
    AmoebaDB->Unlock();
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
  puts("    _                         _             ____  ");
  puts("   / \\   _ __ ___   ___   ___| |__   __ _  |___ \\ ");
  puts("  / _ \\ | '_ ` _ \\ / _ \\ / _ \\ '_ \\ / _` |   __) |");
  puts(" / ___ \\| | | | | | (_) |  __/ |_) | (_| |  / __/ ");
  puts("/_/   \\_\\_| |_| |_|\\___/ \\___|_.__/ \\__,_| |_____|");
  putchar('\n');
  puts("------------[Amoeba 2 Database Module]------------");
}

int main(int argc,char *argv[]){
  Daemon IfConfigured;
  {
    SHMConfig ConfigurationData(AMOEBABACKUP,AMOEBAALLOCSIZE,AMOEBAOFFSET,AMOEBASHMKEY,AMOEBADBSIZE);
    WMSAPI AmoebaActual(ConfigurationData);
    AmoebaDB=&AmoebaActual;
    string HostName=AMOEBAHOSTNAME;
    int PortNum=AMOEBAPORTNUM;
    if(argc==3){
      HostName=string(argv[1]);
      PortNum=atoi(argv[2]);
    }
    Networking NewServer(HostName,PortNum,true);
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
