/*! \file */

/*

This file is part of ENiX3.

ENiX3 is free software you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

ENiX3 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ENiX3.  If not, see <http://www.gnu.org/licenses/>.

       oooooooooooo ooooo      ooo  o8o  ooooooo  ooooo   .oooo.          
   .dP `888'     `8 `888b.     `8'  `"'   `8888    d8'  .dP""Y88b  Yb     
 .dP    888          8 `88b.    8  oooo     Y888..8P          ]8P'  `Yb   
dP      888oooo8     8   `88b.  8  `888      `8888'         <88b.     `Yb 
Yb      888    "     8     `88b.8   888     .8PY888.         `88b.    .dP 
 `Yb    888       o  8       `888   888    d8'  `888b   o.   .88P   .dP   
   `Yb o888ooooood8 o8o        `8  o888o o888o  o88888o `8bd88P'   dP     


Programmed By M.S.Braithwaite 2007 - 2011.

*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include "WitchCraft-Net.h"
#include "WMS_Blob.h"

unsigned long int NAct=0;

int DebugMessages=0;

unsigned long int ServerID; 
struct sockaddr_in ClientAddress;

/*! \brief Accept server connections from a socket.
 *
 */
int AcceptServerCon(int Socket){
  int clntSock=0;
  unsigned int clntLen=0;
  clntLen=sizeof(ClientAddress);
  if((clntSock=accept(Socket,(struct sockaddr *)&ClientAddress,&clntLen))<0){
    if(DebugMessages)
      perror("accept() failed");
    return -1;
  }
  return clntSock;
}

/*! \brief Open a server socket.
 *
 */
int OpenServerSocket(){
  int ServerSocket=0;
  struct sockaddr_in ServerAddress;
  if((ServerSocket=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP))<0){
    if(DebugMessages)
      perror("socket() failed");
    return -1;
  }
  memset(&ServerAddress,0,sizeof(ServerAddress));
  ServerAddress.sin_family=AF_INET;
  ServerAddress.sin_addr.s_addr=htonl(INADDR_ANY);
  ServerAddress.sin_port=htons(ENiXPort);
  if (bind(ServerSocket,(struct sockaddr *)&ServerAddress,sizeof(ServerAddress))<0){
    if(DebugMessages)
      perror("bind() failed");
    return -1;
  }
  if (listen(ServerSocket, MAXPENDING) < 0){
    if(DebugMessages)
      perror("listen() failed");
    return -1;
  }  
  return ServerSocket;
}

/*! \brief Open a client socket.
 *
 */
int OpenClientSocket(char *ServerIP){
  int ClientSocket=0;
  struct sockaddr_in ClientAddress;
  if((ClientSocket=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP))<0){
    if(DebugMessages)
      perror("socket() failed");
    return -1;
  }
  memset(&ClientAddress,0,sizeof(ClientAddress));
  ClientAddress.sin_family=AF_INET;
  ClientAddress.sin_addr.s_addr=inet_addr(ServerIP);
  ClientAddress.sin_port=htons(ENiXPort);
  if(connect(ClientSocket,(struct sockaddr *)&ClientAddress,sizeof(ClientAddress))<0){
    if(DebugMessages)
      perror("connect() failed");
    return -1;
  }
  return ClientSocket;
}

/* wraps nettx */
/*! \brief Network transmit data.
 *
 */
void NetTransmit(struct Blob *Data,int Socket){
  char Initial[33];
  struct Blob *Specs=WMS_Bin2Blob(32,NULL);
  Data->Size++;
  memset(Initial,':',32);
  snprintf(Initial,11,"%010ld",ServerID);
  Initial[10]=':';
  snprintf(Initial+11,11,"%010ld",Data->Size);
  Initial[21]=':';
  Initial[32]=0;
  Specs->Nucleus=Initial;
  NetTX(Specs,Socket);
  NetTX(Data,Socket);
  free(Specs);
}

/* wraps netrx */
/*! \brief Network receive data.
 *
 */
struct Blob *NetReceive(int Socket){
  unsigned long int TransmissionLen=0;
  struct Blob *Data=NULL;
  Data=NetRX(32,Socket);
  TransmissionLen=atol(Data->Nucleus+11);
  WMS_FreeBlob(Data);
  Data=NetRX(TransmissionLen,Socket);
  return Data;
}

/* Replaces transmit */
/*! \brief Network transmit data.
 *
 */
void NetTX(struct Blob *Data,int Socket){
  if (send(Socket,Data->Nucleus,Data->Size,0)!=Data->Size)
    if(DebugMessages)
      perror("send() sent a different number of bytes than expected");  
}

/* Replaces receive */
/*! \brief Network receive data.
 *
 */
struct Blob *NetRX(unsigned long int Size,int Socket){
  unsigned long int Received=0;
  char *Buffer=(char *)malloc((Size+1)*sizeof(char));
  struct Blob *Return=WMS_Bin2Blob(Size,NULL);
  if((Received=recv(Socket,Buffer,Size,0))<=0){
    if(DebugMessages)
      perror("recv() failed or connection closed prematurely");
    return NULL;
  }
  Return->Size=Size;
  Return->Nucleus=Buffer;
  return Return;
}

/*

            88 oooooooooooo ooooo      ooo  o8o  ooooooo  ooooo   .oooo.          
   .dP     .8' `888'     `8 `888b.     `8'  `"'   `8888    d8'  .dP""Y88b  Yb     
 .dP      .8'   888          8 `88b.    8  oooo     Y888..8P          ]8P'  `Yb   
dP       .8'    888oooo8     8   `88b.  8  `888      `8888'         <88b.     `Yb 
Yb      .8'     888    "     8     `88b.8   888     .8PY888.         `88b.    .dP 
 `Yb   .8'      888       o  8       `888   888    d8'  `888b   o.   .88P   .dP   
   `Yb 88      o888ooooood8 o8o        `8  o888o o888o  o88888o `8bd88P'   dP     


 */
