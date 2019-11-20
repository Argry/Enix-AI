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

#include "ENiX_Seq.h"
#include "ENiX_Settings.h"

#ifndef ENiXSUBNET
#define ENiXSUBNET

struct SubnetData{
  int NeuralType;
  bool IsFakeDependency;
  bool FakeValue;
  int DependenciesCurrent;
  int DependenciesMax;
  int MaxGuesses;
  int Complexity;
  int NeuralSize;
  vector<int> ExpandedDeps;
};

class Subnet:public Sequence<int>{
 public:
  pthread_key_t ThreadIdKey;
  struct SubnetData SND[TOTALTHREADS];
  struct SubnetData Final;
  struct SubnetData Initial;

  int DependenciesMin;  
  int Learnt;
  vector<int> Neural;
  int Multiplier;
  Sequence<int> Unsolved;

  unsigned int ClientId;
  unsigned int MaxClients;

  virtual int Learn()=0;
  virtual int FindSubnet()=0;
  virtual ~Subnet(){};
  virtual Sequence<int> Execute(vector<int> Vector)=0;
  virtual void Print(int SubnetNum)=0;
  virtual void PrintBefore(int SubnetNum)=0;
  virtual void PrintSN()=0;
  virtual string SN2String()=0;

  /* API access for network distribution */
  unsigned long int AbortAtComplexity;
  bool AbortNeuralSubnet;
  int SubnetFound;
  bool TerminatePollThread;
};

#endif

/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
 */

