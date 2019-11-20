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

#ifndef WITCHCRAFT_NET_H
#define WITCHCRAFT_NET_H

#include <arpa/inet.h>
#include "ENiX_BDEFS.h"

#define MAXPENDING 5
#define RCVBUFSIZE 32
#define ENiXPort   22576
#define SERVERIP   "127.0.0.1"

#define CLIENT 0
#define SERVER 1

/* This is also used to represent the operation type */
extern unsigned long int ServerID; 

extern struct sockaddr_in ClientAddress;

struct Connection{
  int ServerFlag;
  char *ServerIP;
  unsigned long int ServerID;
  int Port;
  int Socket;
  int ClientSocket;
  struct sockaddr_in ConnectionDetails;
  struct Blob *Sent;
  struct Blob *Received;
};

int          OpenServerSocket();
int          OpenClientSocket(char *ServerIP);

int          AcceptServerCon(int Socket);

void         NetTransmit(struct Blob *Data,int Socket);
struct Blob  *NetReceive(int Socket);

void         NetTX(struct Blob *Data,int Socket);
struct Blob  *NetRX(unsigned long int Size,int Socket);

void         AutoTX(char *BinaryData,unsigned long int Size,int Socket);
char         *AutoRX(char *Buffer,unsigned long int Size,int Socket);


void         SendAck(int Socket);
void         RecvAck(int Socket);

void         SendCmd(char *Command,int Socket);
char         *RecvCmd(int Socket);

/*

Usage Pseudocode:
=================

Server:
-------
OpenServerSocket();
loop{
        AcceptServerCon();
	NetReceive();
	NetTransmit();
	close();
}

Client:
-------
loop{
        OpenClientSocket();
	NetTransmit();
	NetReceive();
	close();
}
*/


#endif

/*

            88 oooooooooooo ooooo      ooo  o8o  ooooooo  ooooo   .oooo.          
   .dP     .8' `888'     `8 `888b.     `8'  `"'   `8888    d8'  .dP""Y88b  Yb     
 .dP      .8'   888          8 `88b.    8  oooo     Y888..8P          ]8P'  `Yb   
dP       .8'    888oooo8     8   `88b.  8  `888      `8888'         <88b.     `Yb 
Yb      .8'     888    "     8     `88b.8   888     .8PY888.         `88b.    .dP 
 `Yb   .8'      888       o  8       `888   888    d8'  `888b   o.   .88P   .dP   
   `Yb 88      o888ooooood8 o8o        `8  o888o o888o  o88888o `8bd88P'   dP     


 */
