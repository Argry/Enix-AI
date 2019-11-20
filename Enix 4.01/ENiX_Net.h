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

#ifndef ENiXNet
#define ENiXNet

#include <netinet/in.h>
#include <iostream>
#include "ENiX_Binary.h"
#include "ENiX_NetInterface.h"

using namespace std;

enum NetworkStatus{
  Connected,
  SocketError,
  BindError,
  NoHost,
  CannotConnect,
  ErrorWritingToSocket,
  ErrorReadingFromSocket,
  AcceptError,
  ForkError
};

enum ServiceType{
  NLPService,
  ANNService,
  AmoebaService,
  CDSService
};


struct ConnectionDetails{
  string Hostname;
  int Port;
};

class Networking: public Application{
public:

  int ServerSocket,Socket;
  int PortNum;
  struct sockaddr_in serv_addr,cli_addr;
  struct hostent *Server;
  Networking();
  Networking(int argc,char **argv,bool ServerFlag,bool IPCOn=false);
  Networking(string Host,int Port,bool ServerFlag,bool IPCOn=false);
  NetworkStatus NetSetup(string Host,int Port,bool ServerFlag);
  NetworkStatus RunServer();
  ~Networking();
  NetworkStatus TestSend();
  NetworkStatus TestReceive();
  NetworkStatus Send(BinData Data);
  BinData Receive();
  void ConnectionHandler(int IncomingSocket,int ProcessPid);

  NetworkStatus ConnectedStatus;

  void PopulateErrors();
  vector<string> NetErrorType;

  void DisplayErrorText(NetworkStatus Type);

  Packet CommandToServer(Networking *Net,string Command);

private:
  void error(const char *msg);
  Packet Preprocess(const char *Command);
  bool UseIPC;

};

#endif

/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
*/
