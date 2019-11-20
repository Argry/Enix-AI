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

#ifndef ENiXSUBPERM
#define ENiXSUBPERM

#include "ENiX_Subnet.h"
#include "ENiX_Settings.h"
#include "ENiX_IPC.h"

class Permute:public Subnet{
 public:
  int LearnDep();
  Permute();
  Permute(string Prefix,int Deps,int Mul,const Sequence& Answers,const vector<int> DepList,const int NeuralType);
  Permute(string Prefix,int Mul,bool Value);
  ~Permute(){};
  vector<int> DependencyList;
  Sequence Compress(Sequence &Seq);
  void Print(int SubnetNum);
  void PrintBefore(int SubnetNum);
  int Learn() final;
  int FindSubnet() final;
  virtual bool SubnetPasses()=0;
 protected:
  Sequence Compressed;
  string FormatPrefix;
 private:
  int CalcNeuralSize(int Dependencies);
  int RecursionPerm(int Pos);
  bool FindSubnetThread(struct ThreadData *TD);
  static void *SubnetThreadProxyFn(void *TD);
  static void *PollControlProxyFn(void *Container);
  void PollControlDataThread(struct ThreadData *TD,IPC *IPCComms);
  Sequence Input;
  void InitialBuffer2Thread();
  struct ThreadData InitialiseTD(unsigned long int ThreadId,unsigned long int *FoundBy);
};

struct ThreadData{
  int *Found;
  bool *Stop;
  bool *TerminatePoll;
  unsigned long int *Complexity;
  unsigned long int ThreadId;
  unsigned long int *FoundBy;
  Permute *ClassInstance;
};

struct PollThreadContainer{
  struct ThreadData *TD;
  IPC *IPCData;
};

#endif

/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
 */
