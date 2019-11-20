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

#include<iostream>
#include<sstream>
#include<stdio.h>
#include<algorithm>
#include<numeric>
#include<vector>
#include<string>
#include<math.h>

#include "ENiX_Lean.h"

Sequence<int> ENiX_Subnet_Lean::Execute(vector<int> Vector){
  int Pos=0,LogicGateNum=0,Output=0;
  Sequence Solution(Vector.size());
  //Solution=Solution.Zero(Vector.size());
  for(auto TPI:Vector){
    Output=((TPI>>Final.ExpandedDeps[0])&1);
    if(Final.DependenciesCurrent==1)
      Output=((Neural[0]&(1<<Output))>>Output);
    else
      for(LogicGateNum=0;LogicGateNum<Final.NeuralSize;++LogicGateNum)
	Output=!!(Neural[LogicGateNum]&(1<<(Output+(((TPI>>Final.ExpandedDeps[LogicGateNum+1])&1)<<1))));
    Solution.SetValue(Output*Multiplier,Pos++);
  }
  return Solution;
}

bool ENiX_Subnet_Lean::SubnetPasses(){
  int LogicGateNum=0,Output=0,MinLength=Unsolved.Size();
  struct SubnetData *SN=&SND[*(unsigned long int *)pthread_getspecific(ThreadIdKey)];
  for(int TPI=0;TPI<MinLength;++TPI){
    if(Unsolved.Knowns[TPI]){
      Output=((TPI>>SN->ExpandedDeps[0])&1);
      if(SN->DependenciesCurrent==1)
	Output=((SN->Complexity&(1<<Output))>>Output);
      else
	for(LogicGateNum=0;LogicGateNum<SN->NeuralSize;++LogicGateNum)
	  Output=!!(((SN->Complexity>>(LogicGateNum<<2))&15)&(1<<(Output+(((TPI>>SN->ExpandedDeps[LogicGateNum+1])&1)<<1))));
      if(Output!=Unsolved.Values[TPI])
	return 0;
    }
  }
  return 1;
}

//int Double_Input_Gates(int Gate_ID, int In1, int In2){ return !!(Gate_ID&(1<<(In1+In2+In2))); } /* Courtesy of Mj  */
//int Single_Input_Gates(int Gate_ID, int Input){ return (Gate_ID&(1<<Input))>>Input; }           /* Courtesy of Mj */

void ENiX_Subnet_Lean::PrintSN(){
  int i=0,OutputID=log2(Multiplier),Pos=0;
  string DGateNames[]={":FALSE  ",":NOR    ",":AND-NOT",":NOT-2  ",":NOT-AND",":NOT-1  ",":XOR    ",":NAND   ",":AND    ",":NXOR   ",":WIRE-1 ",":OR-NOT ",":WIRE-2 ",":NOT-OR ",":OR     ",":TRUE   "};
  string SGateNames[]={":FALSE  ",":NOT    ",":WIRE   ",":TRUE   "};
  printf("%s %d: ",FormatPrefix.c_str(),OutputID);

  if(Final.IsFakeDependency){
    printf("%s\n",SGateNames[(Final.FakeValue)*3].c_str());
  }
  else{
    if(Final.NeuralSize>0){
      if(Final.DependenciesCurrent==1)
	printf("%s %d\n",SGateNames[Neural[0]].c_str(),Final.ExpandedDeps[0]);
      else{
	for(i=0;i<Final.NeuralSize;i++)
	  printf(" (");
	for(i=0;i<Final.NeuralSize;i++)
	  if(i==0){
	    printf("%d %s %d) ",Final.ExpandedDeps[Pos],DGateNames[Neural[i]].c_str(),Final.ExpandedDeps[Pos+1]);
	    Pos+=2;
	  }
	  else
	    printf("%s %d) ",DGateNames[Neural[i]].c_str(),Final.ExpandedDeps[Pos++]);
	putchar('\n');
      }
    }
    else
      puts("-\n");
  }
}

string ENiX_Subnet_Lean::SN2String(){
  int i=0,OutputID=log2(Multiplier),Pos=0;
  ostringstream ToConvert;
  string DGateNames[]={":FALSE  ",":NOR    ",":AND-NOT",":NOT-2  ",":NOT-AND",":NOT-1  ",":XOR    ",":NAND   ",":AND    ",":NXOR   ",":WIRE-1 ",":OR-NOT ",":WIRE-2 ",":NOT-OR ",":OR     ",":TRUE   "};
  string SGateNames[]={":FALSE  ",":NOT    ",":WIRE   ",":TRUE   "};
  ToConvert<<OutputID<<": ";
  if(Final.IsFakeDependency){
    //    printf("%s\n",SGateNames[(Final.FakeValue)*3].c_str());
    ToConvert<<SGateNames[(Final.FakeValue)*3]<<"\n";
  }
  else{
    if(Final.NeuralSize>0){
      if(Final.DependenciesCurrent==1)
	ToConvert<<SGateNames[Neural[0]]<<" "<<Final.ExpandedDeps[0]<<"\n";
      else{
	for(i=0;i<Final.NeuralSize;i++)
	  ToConvert<<" (";
	for(i=0;i<Final.NeuralSize;i++)
	  if(i==0){
	    ToConvert<<Final.ExpandedDeps[Pos]<<" "<<DGateNames[Neural[i]]<<" "<<Final.ExpandedDeps[Pos+1]<<") ";
	    Pos+=2;
	  }
	  else
	    ToConvert<<DGateNames[Neural[i]]<<" "<<Final.ExpandedDeps[Pos+1]<<") ";
	ToConvert<<"\n";
      }
    }
    else
      ToConvert<<"-\n";
  }
  return ToConvert.str();
}

/*
string ENiX_Subnet_Lean::SN2String(){
  int i=0,OutputID=log2(Multiplier),Pos=0;
  ostringstream ToConvert;
  string DGateNames[]={":FALSE  ",":NOR    ",":AND-NOT",":NOT-2  ",":NOT-AND",":NOT-1  ",":XOR    ",":NAND   ",":AND    ",":NXOR   ",":WIRE-1 ",":OR-NOT ",":WIRE-2 ",":NOT-OR ",":OR     ",":TRUE   "};
  string SGateNames[]={":FALSE  ",":NOT    ",":WIRE   ",":TRUE   "};
  ToConvert<<OutputID<<": ";
  if(Final.NeuralSize>0){
    if(Final.DependenciesCurrent==1)
      ToConvert<<SGateNames[Neural[0]]<<" "<<Final.ExpandedDeps[0]<<"\n";
    else{
      for(i=0;i<Final.NeuralSize;i++)
	ToConvert<<" (";
      for(i=0;i<Final.NeuralSize;i++)
	if(i==0){
	  ToConvert<<Final.ExpandedDeps[Pos]<<" "<<DGateNames[Neural[i]]<<" "<<Final.ExpandedDeps[Pos+1]<<") ";
	  Pos+=2;
	}
	else
	  ToConvert<<DGateNames[Neural[i]]<<" "<<Final.ExpandedDeps[Pos+1]<<") ";
      ToConvert<<"\n";
    }
  }
  else
    ToConvert<<"-\n";
  return ToConvert.str();
}
*/

/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
 */
