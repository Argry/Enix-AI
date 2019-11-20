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

#ifndef ENiXDEPS
#define ENiXDEPS

#include "ENiX_Seq.h"
#include "ENiX_Subnet.h"

class Dependencies:public Sequence<int>{
 public:
  Dependencies(const Sequence<int> &Seq);
  Dependencies();
  ~Dependencies();
  void DependencyCheck(unsigned int Id,unsigned int NumClients);
  int NumSubnets;
  vector<Subnet *> Subnets;
  unsigned long int GetTime();
  unsigned long int GetTotalDeps();
  unsigned long int GetTotalComplexity();
  void PopulateNeural(vector<string> Input);
  void PopulateNeural(Sequence<int> Seq);
  string FormatPrefix;
 protected:
  Sequence<int> Unsolved;
  unsigned long int TimeTaken;
 private:
  int Dependency(const Sequence<int>& Seq,int Interval);
  bool CouldBeDifferent(const Sequence<int> &Seq,size_t Pos1,size_t Pos2);
};

#endif

/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
 */
