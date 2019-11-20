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

#include "CDS_Daemon.h" // Must be before ENiX_Settings.h
#include "ENiX_Net.h"
#include "ENiX_Binary.h"
#include "ENiX_Seq.h"
#include "ENiX_Settings.h"
#include "ENiX_List.h"
#include "ENiX_Daemon.h"

string PROTOCOL_PREFIX="Connection Distribution Service";

void ServerSignal(int SignalData){
  unsigned int LocalPid=getpid();
  AmoebaDB->~WMSAPI();
  printf("\nSignal caught in pid %d... Exiting.\n",LocalPid);
  exit(0);
}

void CDS::IntDownloadPeer(string HostName,int PortNum,string Service){
  Networking NewServer(HostName,PortNum,false);  
  string CommandToSend="GETPEERSONLINE";
  if(Service!="")
    CommandToSend+=" "+Service;
  Packet Incoming=NewServer.CommandToServer(&NewServer,CommandToSend);
  if(Incoming.Strings.size()>2){
    for(size_t _=2;_<Incoming.Strings.size();++_){
      string PeerDataString(Incoming.Strings[_].Str,Incoming.Strings[_].Size);
      vector<string> SplitPeerData=List::String2List(PeerDataString,false);
      bool OnlineStatus=false;
      if(SplitPeerData[3]=="Yes")
	OnlineStatus=true;
      IntCreatePeer(SplitPeerData[0],SplitPeerData[1],atoi(SplitPeerData[2].c_str()),OnlineStatus);
    }
  }  
}

void CDS::IntDownloadPeers(string Prefix,string Service){
  if(Peers.size()){
    putchar('\n');
    for(size_t PeerId=0;PeerId<Peers.size();++PeerId)
      if((get<PeerAttr::OnlineStatus>(Peers[PeerId])==true)&&(get<PeerAttr::ServiceName>(Peers[PeerId])=="CDS")){
	printf("%sScanning peer: %s:%d...\n",Prefix.c_str(),get<PeerAttr::HostName>(Peers[PeerId]).c_str(),get<PeerAttr::PortNum>(Peers[PeerId]));
	IntDownloadPeer(get<PeerAttr::HostName>(Peers[PeerId]),get<PeerAttr::PortNum>(Peers[PeerId]),Service);
      }
    putchar('\n');
  }
  else
    printf("\n%sNo CDS peers available.\n\n",Prefix.c_str());
}

bool CDS::IntScanPeer(string Prefix,string Service,string HostName,int PortNum){
  Networking NewServer(HostName,PortNum,false);  
  if(NewServer.ConnectedStatus==Connected){
    printf("%s%s,%s,%d is ONLINE\n",Prefix.c_str(),Service.c_str(),HostName.c_str(),PortNum);
    return true;
  }
  printf("%s%s,%s,%d is OFFLINE\n",Prefix.c_str(),Service.c_str(),HostName.c_str(),PortNum);
  return false;
}

void CDS::IntScanPeers(string Prefix,vector<Peer *>PeerList){
  if(Peers.size())
    putchar('\n');
  for(size_t PeerId=0;PeerId<PeerList.size();++PeerId){
    Peer *PRef=PeerList[PeerId];
    if(IntScanPeer(Prefix,get<PeerAttr::ServiceName>(Peers[PeerId]),get<PeerAttr::HostName>(Peers[PeerId]),get<PeerAttr::PortNum>(Peers[PeerId])))
      *PeerList[PeerId]=make_tuple(get<PeerAttr::ServiceName>(Peers[PeerId]),get<PeerAttr::HostName>(Peers[PeerId]),get<PeerAttr::PortNum>(Peers[PeerId]),true);
    else
      *PeerList[PeerId]=make_tuple(get<PeerAttr::ServiceName>(Peers[PeerId]),get<PeerAttr::HostName>(Peers[PeerId]),get<PeerAttr::PortNum>(Peers[PeerId]),false);
  }
  if(Peers.size())
    putchar('\n');
}

vector<CDS::Peer *> CDS::IntGetPeerVec(string Service){
  vector<Peer *> PeerRefs;
  for(size_t PeerId=0;PeerId<Peers.size();++PeerId)
    if((Service=="")||(get<PeerAttr::ServiceName>(Peers[PeerId])==Service))
      PeerRefs.push_back(&(Peers[PeerId]));
  return PeerRefs;
}

vector<CDS::Peer *> CDS::IntGetPeerVecStatus(int Status,string Service){
  vector<Peer *> PeerRefs;
  for(size_t PeerId=0;PeerId<Peers.size();++PeerId)
    if((get<PeerAttr::OnlineStatus>(Peers[PeerId])==Status)&&((Service=="")||(get<PeerAttr::ServiceName>(Peers[PeerId])==Service)))
      PeerRefs.push_back(&(Peers[PeerId]));
  return PeerRefs;
}

string CDS::IntGetPeerVal(Peer Data,const size_t AttrNum){
  string AttrVal="";
  stringstream ToConvert;
  switch(AttrNum){
  case 3: 
    {
      bool Online=get<PeerAttr::OnlineStatus>(Data);
      if(Online)
	return "Yes";
      return "No";
    }
    break;
  case 2:
    {
      ToConvert << get<PeerAttr::PortNum>(Data);
      break;
    }
  case 1:
    {
      ToConvert << get<PeerAttr::HostName>(Data);
      break;
    }
  case 0:
    {
      ToConvert << get<PeerAttr::ServiceName>(Data);
      break;
    }
  default:
    return "";
  }
  AttrVal=ToConvert.str();
  return AttrVal;
}

CDS::ServiceStatus CDS::IntCreatePeer(string Service,string HostName,int PortNum,bool Online){
  bool PeerExists=false;
  bool ServiceExists=false;
  for(size_t ServiceId=0;ServiceId<SupportedServices.size();++ServiceId)
    if(SupportedServices[ServiceId]==Service){
      ServiceExists=true;
      break;
    }
  if(!ServiceExists)
    return ServiceStatus::ServiceUnsupported;
  for(size_t PeerId=0;PeerId<Peers.size();++PeerId)
    if((HostName==get<PeerAttr::HostName>(Peers[PeerId]))&&(((Service==get<PeerAttr::ServiceName>(Peers[PeerId]))||(PortNum==get<PeerAttr::PortNum>(Peers[PeerId]))))){
      PeerExists=true;
      break;
    }
  if(PeerExists)
    return ServiceStatus::ServiceAlreadyExists;
  Peers.push_back(make_tuple(Service,HostName,PortNum,Online));
  return ServiceStatus::Success;
}

CDS::ServiceStatus CDS::IntDeletePeer(string HostName,int PortNum){
  bool PeerDeleted=false;  
  for(size_t PeerId=0;PeerId<Peers.size();++PeerId)
    if((get<PeerAttr::HostName>(Peers[PeerId])==HostName)&&(!PortNum||(get<PeerAttr::PortNum>(Peers[PeerId])==PortNum))){
      PeerDeleted=true;
      Peers.erase(Peers.begin()+PeerId);
      if(PortNum)
	return ServiceStatus::Success;
    }
  if(PeerDeleted)
    return ServiceStatus::Success;
  else
    return ServiceStatus::PeerNotFound;
}

string CDS::IntPeer2String(Peer Data){
  string ServiceData="";
  for(int Index=0;Index<4;++Index){
    ServiceData=ServiceData+IntGetPeerVal(Data,Index);
    if(Index<3)
      ServiceData=ServiceData+" ";
  }
  return ServiceData;
}

CDS::Peer CDS::IntBinData2Peer(BinData Input){
  string InputStr=Input.Convert2Str();
  List ListInstance;
  vector<string> SeparatedInput=ListInstance.String2List(InputStr,false);
  
  string ServiceStr=SeparatedInput[PeerAttr::ServiceName];
  string HostStr=SeparatedInput[PeerAttr::HostName];
  int PortInt=atoi(SeparatedInput[PeerAttr::PortNum].c_str());
  int OnlineInt=SeparatedInput[PeerAttr::OnlineStatus]=="Yes"?1:0;

  return make_tuple(ServiceStr,HostStr,PortInt,OnlineInt);
}

void CDS::IntDebugServices(string LogPrefix){
  string ServiceData={};
  if(Peers.size()){
    printf("\n%sPeer configuration, Service/Host/Port/Online:\n",LogPrefix.c_str());
    for(size_t _=0;_<Peers.size();_++){
      ServiceData=LogPrefix;
      ServiceData+=IntPeer2String(Peers[_]);
      puts(ServiceData.c_str());
    }
    putchar('\n');
  }
  else
    printf("\n%sNo peers configured.\n\n",LogPrefix.c_str());
}

vector<BinData> CDS::IntGetPeerList(string Service){
  vector<BinData> PeerList={};
  string ServiceData={};
  if(Peers.size()){
    for(size_t _=0;_<Peers.size();_++){
      if((Service=="")||(get<PeerAttr::ServiceName>(Peers[_])==Service)){
	BinData ServiceBin(IntPeer2String(Peers[_]));
	PeerList.push_back(ServiceBin);
      }
    }
  }
  return PeerList;
}

vector<BinData> CDS::IntGetPeersOnline(string Service){
  vector<BinData> PeerList={};
  string ServiceData={};
  if(Peers.size())
    for(size_t _=0;_<Peers.size();_++)
      if((Service=="")||(get<PeerAttr::ServiceName>(Peers[_])==Service))
	if(get<PeerAttr::OnlineStatus>(Peers[_])==true){
	  BinData ServiceBin(IntPeer2String(Peers[_]));
	  PeerList.push_back(ServiceBin);
	}
  return PeerList;
}

CDS::ServiceStatus CDS::IntModifyPeer(int PeerId,string Service,string HostName,int PortNum,bool Online){
  if((PeerId+1)&&(PeerId<Peers.size())){
    Peers[PeerId]=make_tuple(Service,HostName,PortNum,Online);
    return ServiceStatus::Success;
  }
  else
    return ServiceStatus::PeerNotFound;    
}

CDS::ServiceStatus CDS::IntModifyPeer(string Service,string HostName,bool Online){
  bool Successful=false;
  for(size_t PeerId=0;PeerId<Peers.size();++PeerId)
    if((Service==get<PeerAttr::ServiceName>(Peers[PeerId]))&&(HostName==get<PeerAttr::HostName>(Peers[PeerId]))){
      Peers[PeerId]=make_tuple(get<PeerAttr::ServiceName>(Peers[PeerId]),get<PeerAttr::HostName>(Peers[PeerId]),get<PeerAttr::PortNum>(Peers[PeerId]),Online);
      Successful=true;
    }
  if(Successful)
    return ServiceStatus::Success;
  else
    return ServiceStatus::PeerNotFound;
}

CDS::ServiceStatus CDS::IntModifyPeer(string HostName,bool Online){
  bool Successful=false;
  for(size_t PeerId=0;PeerId<Peers.size();++PeerId)
    if(HostName==get<PeerAttr::HostName>(Peers[PeerId])){
      Peers[PeerId]=make_tuple(get<PeerAttr::ServiceName>(Peers[PeerId]),get<PeerAttr::HostName>(Peers[PeerId]),get<PeerAttr::PortNum>(Peers[PeerId]),Online);
      Successful=true;
    }
  if(Successful)
    return ServiceStatus::Success;
  else
    return ServiceStatus::PeerNotFound;
}

void CDS::IntLoadPeerData(){
  BinData PeerListName("PeerList");
  size_t LocationRef=AmoebaDB->Search(PeerListName);  
  Peers={};
  if(LocationRef!=-1){
    WMS_Rec PeerData=AmoebaDB->Amoeba2WMS(AmoebaDB->Bin2Amoeba(AmoebaDB->GetData(LocationRef)));
    if(PeerData.Contents.size()>1){
      for(size_t PeerId=0;(PeerId<MAXPEERS)&&(PeerId<PeerData.Contents[1].size());++PeerId)
	Peers.push_back(IntBinData2Peer((PeerData.Contents[1])[PeerId]));
    }
    else
      Peers={};
  }
}

void CDS::IntSavePeerData(){
  BinData PeerListName("PeerList");
  size_t LocationRef=AmoebaDB->Search(PeerListName);
  WMS_Rec PeerRecord;
  vector<BinData> Header;
  Header.push_back(PeerListName);
  PeerRecord.Contents.push_back(Header);
  vector<BinData> Attrs;
  for(size_t PeerId=0;(PeerId<MAXPEERS)&&(PeerId<Peers.size());++PeerId){
    BinData PeerEntry(IntPeer2String(Peers[PeerId]));
    Attrs.push_back(PeerEntry);
  }
  PeerRecord.Contents.push_back(Attrs);
  PeerRecord.Contents.push_back(Attrs);
  if(LocationRef==-1){
    AmoebaDB->AddEmptyAmoebaRec(PeerListName);
    LocationRef=AmoebaDB->Search(PeerListName);
    if(LocationRef==-1)
      return;
  }
  vector<Amoeba_Rec> ToReplace=AmoebaDB->WMS2Amoeba(PeerRecord);
  AmoebaDB->WriteAll2DB(ToReplace);
}

Packet CDS::InitialisePacket(string Response){
  Packet Answer={};
  Answer.Binaries={};
  Answer.Strings={};
  BinData Empty("");
  Answer.Strings.push_back(Empty);
  BinData AddRecStr(Response);
  Answer.Strings.push_back(AddRecStr);
  return Answer;
}

CDS::CDS(){
  CmdSetInitialised=false;
  InitialiseInstructions();
  InitialiseServices();
  Peers={};
}

CDS::~CDS(){}

void CDS::InitialiseServices(void){
  SupportedServices.push_back("CDS");
  SupportedServices.push_back("WitchCraft");
  SupportedServices.push_back("NNP");
  SupportedServices.push_back("Chocky");
  SupportedServices.push_back("Amoeba");
}

void CDS::InitialiseInstructions(){
  if(!CmdSetInitialised){

    CommandName.push_back("ADDPEER");
    InstructionSet.push_back(&CDS::AddPeer);
    Parameters.push_back(3);
    OnlineHelp.push_back("ADDPEER <IPADDRESS> <PORT> <SERVICE>");

    CommandName.push_back("DEBUG");
    InstructionSet.push_back(&CDS::Debug);
    Parameters.push_back(0);
    OnlineHelp.push_back("DEBUG");

    CommandName.push_back("DEBUGPEERS");
    InstructionSet.push_back(&CDS::DebugPeers);
    Parameters.push_back(0);
    OnlineHelp.push_back("DEBUGPEERS");

    CommandName.push_back("DELETEPEER");
    InstructionSet.push_back(&CDS::DeletePeer);
    Parameters.push_back(1);
    OnlineHelp.push_back("DELETEPEER <IPADDRESS> [PORTNUM]");

    CommandName.push_back("GETPEERLIST");
    InstructionSet.push_back(&CDS::GetPeerList);
    Parameters.push_back(0);
    OnlineHelp.push_back("GETPEERLIST [SERVICE]");

    CommandName.push_back("GETPEERSONLINE");
    InstructionSet.push_back(&CDS::GetPeersOnline);
    Parameters.push_back(0);
    OnlineHelp.push_back("GETPEERSONLINE [SERVICE]");

    CommandName.push_back("HELP");
    InstructionSet.push_back(&CDS::Help);
    Parameters.push_back(0);
    OnlineHelp.push_back("HELP");

    CommandName.push_back("LISTSUPPORTED");
    InstructionSet.push_back(&CDS::ListSupported);
    Parameters.push_back(0);
    OnlineHelp.push_back("LISTSUPPORTED");

    CommandName.push_back("PEEROFFLINE");
    InstructionSet.push_back(&CDS::PeerOffline);
    Parameters.push_back(1);
    OnlineHelp.push_back("PEEROFFLINE <IPADDRESS> [SERVICE]");

    CommandName.push_back("PEERONLINE");
    InstructionSet.push_back(&CDS::PeerOnline);
    Parameters.push_back(1);
    OnlineHelp.push_back("PEERONLINE <IPADDRESS> [SERVICE]");

    CommandName.push_back("PURGE");
    InstructionSet.push_back(&CDS::Purge);
    Parameters.push_back(0);
    OnlineHelp.push_back("PURGE");

    CommandName.push_back("REFRESH");
    InstructionSet.push_back(&CDS::Refresh);
    Parameters.push_back(0);
    OnlineHelp.push_back("REFRESH [SERVICE]");

    CommandName.push_back("RETRIEVEPEERS");
    InstructionSet.push_back(&CDS::RetrievePeers);
    Parameters.push_back(0);
    OnlineHelp.push_back("RETRIEVEPEERS [SERVICE]");

    CmdSetInitialised=true;
  }
}

Packet CDS::Purge(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("CONNECTIONS CLEARED.");
  Peers={};
  AmoebaDB->CreateEmptyDB();
  AmoebaDB->InitFreeList();
  AmoebaDB->PutDBInfo();  
  return Answer;
}

Packet CDS::AddPeer(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("PEER ADDED");
  CDS::ServiceStatus Status=IntCreatePeer(Commands[3],Commands[1],atoi(Commands[2].c_str()),false);
  if(Status==CDS::ServiceStatus::ServiceAlreadyExists)
    Answer.Strings[1]="CANNOT ADD PEER: PEER EXISTS";
  else if(Status==CDS::ServiceStatus::ServiceUnsupported)
    Answer.Strings[1]="CANNOT ADD PEER: \'"+Commands[3]+"\' IS UNSUPPORTED ON THIS SERVER";
  return Answer;
}

Packet CDS::Debug(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("DEBUG SENT TO SERVER TERMINAL");
  string InfoPrefix="  i "+BinData::GetSessionIdentifier();
  const char *CharInfoPrefix=InfoPrefix.c_str();
  putchar('\n');
  AmoebaDB->DebugAllocTable(InfoPrefix);
  AmoebaDB->DebugDatabase(InfoPrefix);
  AmoebaDB->ShowAllAmoebas(InfoPrefix);
  AmoebaDB->ShowAllWMS(InfoPrefix);
  putchar('\n');
  return Answer;
}

Packet CDS::DebugPeers(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("DEBUG SENT TO SERVER TERMINAL");
  string InfoPrefix="  i "+BinData::GetSessionIdentifier();
  IntDebugServices(InfoPrefix);
  return Answer;
}

Packet CDS::DeletePeer(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("PEER DELETED");
  CDS::ServiceStatus Result;
  if(Commands.size()>2)
    Result=IntDeletePeer(Commands[1],atoi(Commands[2].c_str()));
  else
    Result=IntDeletePeer(Commands[1]);
  if(Result==CDS::ServiceStatus::PeerNotFound)
    Answer.Strings[1]="CANNOT DELETE PEER: PEER NOT FOUND";
  return Answer;
}

Packet CDS::GetPeerList(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("PEER LIST SENT");
  if(Commands.size()>1)
    Answer.Strings=List::BinDataSetAdd(Answer.Strings,IntGetPeerList(Commands[1]));
  else
    Answer.Strings=List::BinDataSetAdd(Answer.Strings,IntGetPeerList());
  return Answer;
}

Packet CDS::GetPeersOnline(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("PEER LIST SENT");
  if(Commands.size()>1)
    Answer.Strings=List::BinDataSetAdd(Answer.Strings,IntGetPeersOnline(Commands[1]));
  else
    Answer.Strings=List::BinDataSetAdd(Answer.Strings,IntGetPeersOnline());
  return Answer;
}

Packet CDS::Help(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("SENDING HELP");
  for(size_t _=0;_<OnlineHelp.size();_++){
    BinData Text(OnlineHelp[_]);
    Answer.Strings.push_back(Text);
  }
  return Answer;
}

Packet CDS::ListSupported(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("SENDING SUPPORTED SERVICES");
  for(size_t _=0;_<SupportedServices.size();_++){
    BinData Text(SupportedServices[_]);
    Answer.Strings.push_back(Text);
  }
  return Answer;
}

Packet CDS::PeerOnline(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("PEER ONLINE");
  CDS::ServiceStatus Result;
  if(Commands.size()>2)
    Result=IntModifyPeer(Commands[2],Commands[1],true);
  else
    Result=IntModifyPeer(Commands[1],true);
  if(Result==CDS::ServiceStatus::PeerNotFound)
    Answer.Strings[1]="CANNOT SET PEER ONLINE: PEER NOT FOUND";
  return Answer;
}

Packet CDS::PeerOffline(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("PEER OFFLINE");
  CDS::ServiceStatus Result;
  if(Commands.size()>2)
    Result=IntModifyPeer(Commands[2],Commands[1],false);
  else
    Result=IntModifyPeer(Commands[1],false);
  if(Result==CDS::ServiceStatus::PeerNotFound)
    Answer.Strings[1]="CANNOT SET PEER OFFLINE: PEER NOT FOUND";
  return Answer;
}

Packet CDS::Refresh(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("PEERS SCANNED");
  string InfoPrefix="  i "+BinData::GetSessionIdentifier();
  IntScanPeers(InfoPrefix,IntGetPeerVec());
  
  return Answer;
}

Packet CDS::RetrievePeers(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("PEERS ADDED");
  string InfoPrefix="  i "+BinData::GetSessionIdentifier();
  if(Commands.size()>1)
    IntDownloadPeers(InfoPrefix,Commands[1]);  
  else
    IntDownloadPeers(InfoPrefix);
  return Answer;
}

Packet CDS::ExecuteCommand(vector<string> Commands,Packet Data){
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
    AmoebaDB->Lock();
    AmoebaDB->GetDBInfo();

    // TODO: load peerdata from the Database
    CDSRef->IntLoadPeerData();

    Outgoing=CDSRef->ExecuteCommand(ToProcess,Incoming);

    // TODO: save peerdata to the Database
    CDSRef->IntSavePeerData();
    
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
  /* echo "ENiX4 Connection Distribution Service" | cowsay -f eyes */
  /*
  printf(" [0;1;35;95m_[0;1;31;91m__[0;1;33;93m__[0;1;32;92m__[0;1;36;96m__[0;1;34;94m__[0;1;35;95m__[0;1;31;91m__[0;1;33;93m__[0;1;32;92m__[0;1;36;96m__[0;1;34;94m__[0;1;35;95m__[0;1;31;91m__[0;1;33;93m__[0;1;32;92m__[0;1;36;96m__[0;1;34;94m__[0;1;35;95m__[0;1;31;91m__[0m        \n");
  printf("[0;1;35;95m<[0m [0;1;31;91mEN[0;1;33;93miX[0;1;32;92m4[0m [0;1;36;96mCo[0;1;34;94mnn[0;1;35;95mec[0;1;31;91mti[0;1;33;93mon[0m [0;1;32;92mD[0;1;36;96mis[0;1;34;94mtr[0;1;35;95mib[0;1;31;91mut[0;1;33;93mio[0;1;32;92mn[0m [0;1;36;96mSe[0;1;34;94mrv[0;1;35;95mic[0;1;31;91me[0m [0;1;33;93m>[0m\n");
  printf(" [0;1;35;95m-[0;1;31;91m--[0;1;33;93m--[0;1;32;92m--[0;1;36;96m--[0;1;34;94m--[0;1;35;95m--[0;1;31;91m--[0;1;33;93m--[0;1;32;92m--[0;1;36;96m--[0;1;34;94m--[0;1;35;95m--[0;1;31;91m--[0;1;33;93m--[0;1;32;92m--[0;1;36;96m--[0;1;34;94m--[0;1;35;95m--[0;1;31;91m--[0m \n");
  printf("    [0;1;33;93m\\[0m\n");
  printf("     [0;1;33;93m\\[0m\n");
*/
  puts("                                                    ");
  printf("                                   [0;1;34;94m.[0;1;35;95m::[0;1;31;91m!![0;1;33;93m!![0;1;32;92m!![0;1;36;96m!:[0;1;34;94m.[0m\n");
  printf("  [0;1;31;91m.![0;1;33;93m!![0;1;32;92m!![0;1;36;96m:.[0m                        [0;1;34;94m.:[0;1;35;95m!![0;1;31;91m!![0;1;33;93m!![0;1;32;92m!![0;1;36;96m!![0;1;34;94m!![0m\n");
  printf("  [0;1;31;91m~~[0;1;33;93m~~[0;1;32;92m!![0;1;36;96m!![0;1;34;94m!![0;1;35;95m.[0m                 [0;1;32;92m.:[0;1;36;96m!![0;1;34;94m!![0;1;35;95m!![0;1;31;91m!![0;1;33;93m!U[0;1;32;92mWW[0;1;36;96mW$[0;1;34;94m$$[0m \n");
  printf("      [0;1;32;92m:$[0;1;36;96m$N[0;1;34;94mWX[0;1;35;95m!![0;1;31;91m:[0m           [0;1;31;91m.:[0;1;33;93m!![0;1;32;92m!![0;1;36;96m!![0;1;34;94mXU[0;1;35;95mWW[0;1;31;91m$$[0;1;33;93m$$[0;1;32;92m$$[0;1;36;96m$$[0;1;34;94m$P[0m \n");
  printf("      [0;1;32;92m$$[0;1;36;96m$$[0;1;34;94m$#[0;1;35;95m#W[0;1;31;91mX![0;1;33;93m:[0m      [0;1;34;94m.[0;1;35;95m<![0;1;31;91m!![0;1;33;93m!U[0;1;32;92mW$[0;1;36;96m$$[0;1;34;94m$\"[0m  [0;1;31;91m$$[0;1;33;93m$$[0;1;32;92m$$[0;1;36;96m$$[0;1;34;94m#[0m \n");
  printf("      [0;1;32;92m$$[0;1;36;96m$$[0;1;34;94m$[0m  [0;1;35;95m$[0;1;31;91m$$[0;1;33;93mUX[0m   [0;1;36;96m:[0;1;34;94m!![0;1;35;95mUW[0;1;31;91m$$[0;1;33;93m$$[0;1;32;92m$$[0;1;36;96m$$[0;1;34;94m$[0m   [0;1;31;91m4$[0;1;33;93m$$[0;1;32;92m$$[0;1;36;96m*[0m \n");
  printf("      [0;1;32;92m^$[0;1;36;96m$$[0;1;34;94mB[0m  [0;1;35;95m$[0;1;31;91m$$[0;1;33;93m$\\[0m     [0;1;34;94m$[0;1;35;95m$$[0;1;31;91m$$[0;1;33;93m$$[0;1;32;92m$$[0;1;36;96m$$[0;1;34;94m$[0m   [0;1;31;91md$[0;1;33;93m$R[0;1;32;92m\"[0m \n");
  printf("        [0;1;36;96m\"*[0;1;34;94m$b[0;1;35;95md$[0;1;31;91m$$[0;1;33;93m$[0m      [0;1;34;94m'[0;1;35;95m*$[0;1;31;91m$$[0;1;33;93m$$[0;1;32;92m$$[0;1;36;96m$$[0;1;34;94m$$[0;1;35;95mo+[0;1;31;91m#\"[0m \n");
  printf("             [0;1;35;95m\"[0;1;31;91m\"\"[0;1;33;93m\"[0m          [0;1;31;91m\"[0;1;33;93m\"\"[0;1;32;92m\"\"[0;1;36;96m\"\"[0m \n\n");
  puts("---------[Connection Distribution Service]--------");
}

int main(int argc,char *argv[]){
  Daemon IfConfigured;
  {
    SHMConfig ConfigurationData(CDSBACKUP,CDSALLOCSIZE,CDSOFFSET,CDSSHMKEY,CDSDBSIZE);
    string HostName=CDSHOSTNAME;
    int PortNum=CDSPORTNUM;
    if(argc==3){
      HostName=string(argv[1]);
      PortNum=atoi(argv[2]);
    }
    WMSAPI AmoebaActual(ConfigurationData);
    AmoebaDB=&AmoebaActual;
    CDS CDSActual;
    CDSRef=&CDSActual;
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
