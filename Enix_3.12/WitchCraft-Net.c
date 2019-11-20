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
#include <errno.h>
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
  struct Blob *Specs=WMS_Bin2Blob(32,NULL);
  memset(Specs->Nucleus,':',32);
  snprintf(Specs->Nucleus,12,"%010ld:",ServerID);
  snprintf(Specs->Nucleus+11,12,"%010ld:",Data->Size);
  Specs->Nucleus[31]=0;
  NetTX(Specs,Socket);
  NetTX(Data,Socket);
  WMS_FreeBlob(Specs);
}

/* wraps netrx */
/*! \brief Network receive data.
 *
 */
struct Blob *NetReceive(int Socket){
  unsigned long int TransmissionLen=0;
  struct Blob *Data=NULL;
  Data=NetRX(32,Socket);
  if(Data){
    TransmissionLen=atol(Data->Nucleus+11);
    WMS_FreeBlob(Data);
    Data=NetRX(TransmissionLen,Socket);
  }
  return Data;
}

/* Replaces transmit */
/*! \brief Network transmit data.
 *
 */
void NetTX(struct Blob *Data,int Socket){
  AutoTX(Data->Nucleus,Data->Size,Socket);
}

/* Replaces receive */
/*! \brief Network receive data.
 *
 */
struct Blob *NetRX(unsigned long int Size,int Socket){
  struct Blob *Return=WMS_Bin2Blob(Size+1,NULL);
  AutoRX(Return->Nucleus,Return->Size-1,Socket);
  return Return;
}

/*! \brief Send ACK signal.
 *
 */
void SendAck(int Socket){  
  AutoTX("ACK",3,Socket);
}

/*! \brief Wait for ACK before continuing.
 *
 */
void RecvAck(int Socket){
  char Data[3];
  AutoRX(Data,3,Socket);
}

/*! \brief Send a 31 byte command.
 *
 */
void SendCmd(char *Command,int Socket){
  char ToSend[32];
  memset(ToSend,0,32);
  strcpy(ToSend,Command);
  AutoTX(ToSend,31,Socket);
}

/*! \brief Receive a 32 byte command with null termination.
 *
 */
char *RecvCmd(int Socket){
  char *Returned=(char *)malloc(32*sizeof(char));
  memset(Returned,0,32);  
  AutoRX(Returned,31,Socket);
  return Returned;
}

/*! \brief Handle all the transmission logic and packet fragmentation etc.
 *
 */
void AutoTX(char *BinaryData,unsigned long int Size,int Socket){
  unsigned long int SentSoFar=0;
  long int SentThisTime=0;
  if(BinaryData){
    while(SentSoFar<Size){
      SentThisTime=send(Socket,BinaryData+SentSoFar,Size-SentSoFar,0);
      if(SentThisTime<=0){
	perror("NetTX() sent a different number of bytes than expected"); 
	printf("NetTX() tried to send %ld bytes but failed\n",Size);
	exit(0);
      }
      else{
	/*
	if(SentThisTime!=Size)
	  puts("Warning: Transmission fragmentation detected - continuing...");
	*/
	SentSoFar+=SentThisTime;
      }
    }
  }
}

/*! \brief Handle all the receipt logic and packet fragmentation etc.
 *
 */
char *AutoRX(char *Buffer,unsigned long int Size,int Socket){
  unsigned long int RecvSoFar=0;
  long int RecvThisTime=0;
  while(RecvSoFar<Size){
    RecvThisTime=recv(Socket,Buffer+RecvSoFar,Size-RecvSoFar,0);
    if(RecvThisTime<=0){
      perror("NetRX() received a different number of bytes than expected"); 
      printf("NetRX() tried to receive %ld bytes but failed\n",Size);
      return NULL;
    }
    else{
      /*
      if(RecvThisTime!=Size)
	puts("Warning: Received packet fragmentation detected - continuing...");
      */
      RecvSoFar+=RecvThisTime;
    }
  }
  Buffer[Size]=0;
  return Buffer;  
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
