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

#ifndef ENiXSHM
#define ENiXSHM

#include <sys/types.h>
#include "ENiX_Binary.h"

class SHM{
 public:
  SHM();
  bool Create(unsigned long int SegSize,int SegKey);
  bool Destroy();
  bool Attach(unsigned long int SegSize,int SegKey);
  bool Detach();
  bool WriteData(unsigned long int Offset,BinData Data);
  BinData ReadData(unsigned long int Offset,unsigned long int Size);
  void Lock();
  void Unlock();

  bool Disk2SHM(const char *Filename);
  bool SHM2Disk(const char *Filename);

  unsigned long int Size;
  key_t Key;
  /* return value from shmget() */ 
  int SHMId;
  struct shmid_ds *shmid_ds;
  char *DataArea;

};


#endif


/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
 */
