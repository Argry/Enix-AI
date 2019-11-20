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
#include<stdio.h>
#include<algorithm>
#include<numeric>
#include<vector>
#include<string>
#include<math.h>
#include<sstream>

#include "ENiX_Large.h"
#include "ENiX_Settings.h"

Sequence<int> ENiX_Subnet_Large::Execute(vector<int> Vector){
  int Pos=0,LogicGateNum=0,Output=0,VectorSize=Vector.size();
  int RemainingInputs=0,NeuralPad=0,i=0,RemInBuf=0,In1=0,In2=0;
  Sequence Solution(Vector.size());
  Solution=Solution.Zero(Vector.size());
  Sequence Input(Vector);
  Sequence CompressedData;
  CompressedData=Compress(Input);
  for(int TPI=0;TPI<VectorSize;TPI++){
    Output=((Vector[TPI]>>Final.ExpandedDeps[0])&1);
    if(Final.DependenciesCurrent==1)
      Output=((Neural[0]&(1<<Output))>>Output);
    else{
      NeuralPad=CompressedData.Values[TPI];
      RemainingInputs=Final.DependenciesCurrent;
      LogicGateNum=0;
      while(RemainingInputs>1){
	RemInBuf=0;
	Output=0;
	for(i=0;(i<<1)<RemainingInputs-1;i++){
	  In1=((NeuralPad>>(i<<1))&1);
	  In2=((NeuralPad>>((i<<1)+1))&1);
	  Output|=((!!(Neural[LogicGateNum]&(1<<(In1+(In2<<1)))))<<i);
	  LogicGateNum++;
	  RemInBuf++;
	}
	if(RemainingInputs&1){
	  Output|=(((NeuralPad>>(i<<1))&1)<<i);
	  RemInBuf++;
	}
	RemainingInputs=RemInBuf;
	NeuralPad=Output;
      }
    }
    Solution.SetValue(Output*Multiplier,Pos++);
  }
  return Solution;
}

bool ENiX_Subnet_Large::SubnetPasses(){
  int LogicGateNum=0,Output=0,MinLength=Unsolved.Size();
  int RemainingInputs=0,NeuralPad=0,i=0,RemInBuf=0,In1=0,In2=0,Out=0;
  struct SubnetData *SN=&SND[*(unsigned long int *)pthread_getspecific(ThreadIdKey)];
  for(int TPI=0;TPI<MinLength;++TPI){
    if(Unsolved.Knowns[TPI]){
      Output=((TPI>>SN->ExpandedDeps[0])&1);
      if(SN->DependenciesCurrent==1)
	Output=((SN->Complexity&(1<<Output))>>Output);
      else{
	NeuralPad=Compressed.Values[TPI];
	RemainingInputs=SN->DependenciesCurrent;
	LogicGateNum=0;
	while(RemainingInputs>1){
	  RemInBuf=0;
	  Output=0;
	  for(i=0;(i<<1)<RemainingInputs-1;i++){
	    In1=((NeuralPad>>(i<<1))&1);
	    In2=((NeuralPad>>((i<<1)+1))&1);
	    Output|=((!!(((SN->Complexity>>(LogicGateNum<<2))&15)&(1<<(In1+(In2<<1)))))<<i);
	    LogicGateNum++;
	    RemInBuf++;
	  }
	  if(RemainingInputs&1){
	    Output|=(((NeuralPad>>(i<<1))&1)<<i);
	    RemInBuf++;
	  }
	  RemainingInputs=RemInBuf;
	  NeuralPad=Output;
	}
      }
      if(Output!=Unsolved.Values[TPI])
	return 0;
    }
  }
  return 1;
}

//int Double_Input_Gates(int Gate_ID, int In1, int In2){ return !!(Gate_ID&(1<<(In1+In2+In2))); } /* Courtesy of Mj  */
//int Single_Input_Gates(int Gate_ID, int Input){ return (Gate_ID&(1<<Input))>>Input; }           /* Courtesy of Mj */


void ENiX_Subnet_Large::PrintSN(){
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
	string TMP;
	vector<string> Formula(0),Buffer(0);
	for(i=0;i<Final.DependenciesCurrent;i++)
	  Formula.push_back(to_string(Final.ExpandedDeps[i]));
	while(Formula.size()>1){
	  for(i=0;i<Formula.size()-1;i+=2){
	    TMP=" ("+Formula[i]+' '+DGateNames[Neural[Pos++]]+' '+Formula[i+1]+") ";
	    Buffer.push_back(TMP);
	  }
	  if(Formula.size()!=i)
	    Buffer.push_back(Formula[Formula.size()-1]);
	  Formula=Buffer;
	  Buffer.resize(0);
	}
	puts(Formula[0].c_str());
      }
    }
    else
      puts("-\n");
  }
}

string ENiX_Subnet_Large::SN2String(){
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
      if(Final.DependenciesCurrent==1){
	ToConvert<<SGateNames[Neural[0]]<<" "<<Final.ExpandedDeps[0]<<"\n";
      }
      else{
	string TMP;
	vector<string> Formula(0),Buffer(0);
	for(i=0;i<Final.DependenciesCurrent;i++)
	  Formula.push_back(to_string(Final.ExpandedDeps[i]));
	while(Formula.size()>1){
	  for(i=0;i<Formula.size()-1;i+=2){
	    TMP=" ("+Formula[i]+' '+DGateNames[Neural[Pos++]]+' '+Formula[i+1]+") ";
	    Buffer.push_back(TMP);
	  }
	  if(Formula.size()!=i)
	    Buffer.push_back(Formula[Formula.size()-1]);
	  Formula=Buffer;
	  Buffer.resize(0);
	}
	ToConvert<<Formula[0]<<"\n";
      }
    }
    else
      ToConvert<<"-\n";
  }
  return ToConvert.str();
}

/*
string ENiX_Subnet_Large::SN2String(){
  int i=0,OutputID=log2(Multiplier),Pos=0;
  ostringstream ToConvert;
  string DGateNames[]={":FALSE  ",":NOR    ",":AND-NOT",":NOT-2  ",":NOT-AND",":NOT-1  ",":XOR    ",":NAND   ",":AND    ",":NXOR   ",":WIRE-1 ",":OR-NOT ",":WIRE-2 ",":NOT-OR ",":OR     ",":TRUE   "};
  string SGateNames[]={":FALSE  ",":NOT    ",":WIRE   ",":TRUE   "};
  ToConvert<<OutputID<<": ";
  if(Final.NeuralSize>0){
    if(Final.DependenciesCurrent==1){
      ToConvert<<SGateNames[Neural[0]]<<" "<<Final.ExpandedDeps[0]<<"\n";
    }
    else{
      string TMP;
      vector<string> Formula(0),Buffer(0);
      for(i=0;i<Final.DependenciesCurrent;i++)
	Formula.push_back(to_string(Final.ExpandedDeps[i]));
      while(Formula.size()>1){
	for(i=0;i<Formula.size()-1;i+=2){
	  TMP=" ("+Formula[i]+' '+DGateNames[Neural[Pos++]]+' '+Formula[i+1]+") ";
	  Buffer.push_back(TMP);
	}
	if(Formula.size()!=i)
	  Buffer.push_back(Formula[Formula.size()-1]);
	Formula=Buffer;
	Buffer.resize(0);
      }
      ToConvert<<Formula[0]<<"\n";
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
