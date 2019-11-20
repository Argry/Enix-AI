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

#ifndef CDSDAEMON
#define CDSDAEMON

#include <string>
#include <tuple>
#include "ENiX_WMSAPI.h"
#include "ENiX_Packet.h"

using namespace std;

WMSAPI *AmoebaDB;

class CDS{
 public:
  CDS();
  ~CDS();

  enum ServiceStatus{
    Success,
    ServiceAlreadyExists,
    ServiceUnsupported,
    PeerNotFound
  };

  enum PeerAttr{
    ServiceName,
    HostName,
    PortNum,
    OnlineStatus
  };

  void InitialiseServices(void);
  void InitialiseInstructions(void);
  
  typedef Packet (CDS::*ENiXInstructionSet)(vector<string> Commands,Packet Data);
  Packet ExecuteCommand(vector<string> Commands,Packet Data);
  vector<ENiXInstructionSet> InstructionSet;

  bool CmdSetInitialised;

  vector<int>    Parameters;
  vector<string> CommandName;
  vector<string> OnlineHelp;

  Packet InitialisePacket(string Response);

  Packet AddPeer(vector<string> Commands,Packet Data);
  //  Packet BlockPeer(vector<string> Commands,Packet Data);
  Packet Debug(vector<string> Commands,Packet Data);
  Packet DebugPeers(vector<string> Commands,Packet Data);
  Packet DeletePeer(vector<string> Commands,Packet Data);
  Packet GetPeerList(vector<string> Commands,Packet Data);
  Packet GetPeersOnline(vector<string> Commands,Packet Data);
  Packet Help(vector<string> Commands,Packet Data);
  Packet ListSupported(vector<string> Commands,Packet Data);
  Packet PeerOnline(vector<string> Commands,Packet Data);
  Packet PeerOffline(vector<string> Commands,Packet Data);
  Packet Refresh(vector<string> Commands,Packet Data);
  Packet RetrievePeers(vector<string> Commands,Packet Data);
  Packet Purge(vector<string> Commands,Packet Data);

  //  Packet UnblockPeer(vector<string> Commands,Packet Data);

  void   IntLoadPeerData();
  void   IntSavePeerData();
 private:
  vector<string> SupportedServices;

  // Service, Host, PortNum, Online:
  typedef tuple<string,string,int,bool> Peer;
  vector<Peer> Peers;
  ServiceStatus IntCreatePeer(string Service,string HostName,int PortNum,bool Online);
  ServiceStatus IntDeletePeer(string HostName,int PortNum=0);
  ServiceStatus IntModifyPeer(int PeerId,string Service,string HostName,int PortNum,bool Online);
  ServiceStatus IntModifyPeer(string Service,string HostName,bool Online);
  ServiceStatus IntModifyPeer(string HostName,bool Online);
  void   IntDebugServices(string LogPrefix);
  string IntGetPeerVal(Peer Data,const size_t AttrNum);
  string IntPeer2String(Peer Data);
  Peer   IntBinData2Peer(BinData Input);
  void IntScanPeers(string Prefix,vector<Peer *>PeerList);
  bool IntScanPeer(string Prefix,string Service,string HostName,int PortNum);

  void IntDownloadPeers(string Prefix,string Service="");
  void IntDownloadPeer(string HostName,int PortNum,string Service="");

  vector<BinData> IntGetPeerList(string Service="");
  vector<BinData> IntGetPeersOnline(string Service="");

  vector<Peer *> IntGetPeerVec(string Service="");
  vector<Peer *> IntGetPeerVecStatus(int Status,string Service="");
};

class CDS *CDSRef;


#endif


/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
 */
