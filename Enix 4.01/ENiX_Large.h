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

#include "ENiX_Subnet_Perm.h"

#ifndef ENiXLARGE
#define ENiXLARGE
 
class ENiX_Subnet_Large:public Permute{
 public:
  ENiX_Subnet_Large(string Prefix,int Deps,int Mul,const Sequence& Answers, const vector<int> DepList, const int NeuralType):Permute(Prefix,Deps,Mul,Answers,DepList,NeuralType){};
 ENiX_Subnet_Large(string Prefix,int Mul,bool Value):Permute(Prefix,Mul,Value){};
  ENiX_Subnet_Large(){};
  ~ENiX_Subnet_Large(){};
  Sequence Execute(vector<int> Vector) final;
  bool SubnetPasses() final;
  void PrintSN() final;
  string SN2String() final;
};

#endif

/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
 */
