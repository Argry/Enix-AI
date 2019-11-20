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
#include<cmath>
#include <sys/timeb.h>

#include "ENiX_Large.h"
#include "ENiX_Lean.h"
#include "ENiX_Deps.h"
#include "ENiX_Settings.h"

void Dependencies::PopulateNeural(vector<string> Input){
  Sequence Seq(Input);
  PopulateNeural(Seq);
}

void Dependencies::PopulateNeural(Sequence Seq){
  unsigned long int Pos=1,SubnetIdx=0,C=0,Deps=0,DepsLimit=0,SeqSize=Seq.Size();
  struct SubnetData SNTemplate;
  if(!SeqSize){
    NumSubnets=0;
    return;
  }
  NumSubnets=Seq.Values[0];
  vector<int> DepList;
  Sequence Tmp;
  Subnets.resize(NumSubnets);
  for(SubnetIdx=0;SubnetIdx<NumSubnets;SubnetIdx++){
    /*
    if(Pos+9>SeqSize){
      NumSubnets=0;
      return;
    }
    */
    
    bool IsFakeDependency=Seq.Values[Pos++];
    
    if(IsFakeDependency){
      if(Seq.Values[Pos]==1)
	Subnets[SubnetIdx]=new ENiX_Subnet_Large(FormatPrefix,Deps,SubnetIdx,Tmp,DepList,1);
      else if(Seq.Values[Pos]==0)
	Subnets[SubnetIdx]=new ENiX_Subnet_Lean(FormatPrefix,Deps,SubnetIdx,Tmp,DepList,0);

      Subnets[SubnetIdx]->Final.IsFakeDependency=IsFakeDependency;
      Subnets[SubnetIdx]->Final.FakeValue=Seq.Values[Pos++];
      Subnets[SubnetIdx]->Multiplier=Seq.Values[Pos++];
     
      Subnets[SubnetIdx]->Final.NeuralType=0;
      Subnets[SubnetIdx]->DependenciesMin=0;
      Subnets[SubnetIdx]->Final.DependenciesCurrent=0;
      Subnets[SubnetIdx]->Final.DependenciesMax=0;
      Subnets[SubnetIdx]->Final.Complexity=0;
      Subnets[SubnetIdx]->Final.MaxGuesses=0;
      Subnets[SubnetIdx]->Final.NeuralSize=1;
      Subnets[SubnetIdx]->Neural.resize(Subnets[SubnetIdx]->Final.NeuralSize);

      Subnets[SubnetIdx]->Learnt=true;
     
    }
    else{
      
      // create new finals which are of type struct SubnetData
      if(Seq.Values[Pos]==1)
	Subnets[SubnetIdx]=new ENiX_Subnet_Large(FormatPrefix,Deps,SubnetIdx,Tmp,DepList,1);
      else if(Seq.Values[Pos]==0)
	Subnets[SubnetIdx]=new ENiX_Subnet_Lean(FormatPrefix,Deps,SubnetIdx,Tmp,DepList,0);

      Subnets[SubnetIdx]->Final.IsFakeDependency=0;
      Subnets[SubnetIdx]->Final.FakeValue=0;
      
      Subnets[SubnetIdx]->Final.NeuralType=Seq.Values[Pos++];
      Subnets[SubnetIdx]->DependenciesMin=Seq.Values[Pos++];
      Subnets[SubnetIdx]->Final.DependenciesCurrent=Seq.Values[Pos++];
      Subnets[SubnetIdx]->Final.DependenciesMax=Seq.Values[Pos++];
      Subnets[SubnetIdx]->Multiplier=Seq.Values[Pos++];
      Subnets[SubnetIdx]->Final.Complexity=Seq.Values[Pos++];
      Subnets[SubnetIdx]->Final.MaxGuesses=Seq.Values[Pos++];
      Subnets[SubnetIdx]->Final.NeuralSize=Seq.Values[Pos++];
      Subnets[SubnetIdx]->Neural.resize(Subnets[SubnetIdx]->Final.NeuralSize);
      if(Pos+Subnets[SubnetIdx]->Final.NeuralSize+1>SeqSize){
	NumSubnets=0;
	return;
      }
      for(C=0;C<Subnets[SubnetIdx]->Final.NeuralSize;C++)
	Subnets[SubnetIdx]->Neural[C]=Seq.Values[Pos++];
      DepsLimit=Seq.Values[Pos++];
      Subnets[SubnetIdx]->Final.ExpandedDeps.resize(DepsLimit);
      if(Pos+DepsLimit>SeqSize){
	NumSubnets=0;
	return;
      }
      for(C=0;C<DepsLimit;C++)
	Subnets[SubnetIdx]->Final.ExpandedDeps[C]=Seq.Values[Pos++];
      Subnets[SubnetIdx]->Learnt=true;
      Subnets[SubnetIdx]->Initial=Subnets[SubnetIdx]->Final;
    }
  }
}

/*
void Dependencies::PopulateNeural(Sequence Seq){
  unsigned long int Pos=1,SubnetIdx=0,C=0,Deps=0,DepsLimit=0,SeqSize=Seq.Size();
  struct SubnetData SNTemplate;
  if(!SeqSize){
    NumSubnets=0;
    return;
  }
  NumSubnets=Seq.Values[0];
  vector<int> DepList;
  Sequence Tmp;
  Subnets.resize(NumSubnets);
  for(SubnetIdx=0;SubnetIdx<NumSubnets;SubnetIdx++){ 
    if(Pos+9>SeqSize){
      NumSubnets=0;
      return;
    }
    // create new finals which are of type struct SubnetData
    if(Seq.Values[Pos]==1)
      Subnets[SubnetIdx]=new ENiX_Subnet_Large(FormatPrefix,Deps,SubnetIdx,Tmp,DepList,1);
    else if(Seq.Values[Pos]==0)
      Subnets[SubnetIdx]=new ENiX_Subnet_Lean(FormatPrefix,Deps,SubnetIdx,Tmp,DepList,0);
    Subnets[SubnetIdx]->Final.NeuralType=Seq.Values[Pos++];
    Subnets[SubnetIdx]->DependenciesMin=Seq.Values[Pos++];
    Subnets[SubnetIdx]->Final.DependenciesCurrent=Seq.Values[Pos++];
    Subnets[SubnetIdx]->Final.DependenciesMax=Seq.Values[Pos++];
    Subnets[SubnetIdx]->Multiplier=Seq.Values[Pos++];
    Subnets[SubnetIdx]->Final.Complexity=Seq.Values[Pos++];
    Subnets[SubnetIdx]->Final.MaxGuesses=Seq.Values[Pos++];
    Subnets[SubnetIdx]->Final.NeuralSize=Seq.Values[Pos++];
    Subnets[SubnetIdx]->Neural.resize(Subnets[SubnetIdx]->Final.NeuralSize);
    if(Pos+Subnets[SubnetIdx]->Final.NeuralSize+1>SeqSize){
      NumSubnets=0;
      return;
    }
    for(C=0;C<Subnets[SubnetIdx]->Final.NeuralSize;C++)
      Subnets[SubnetIdx]->Neural[C]=Seq.Values[Pos++];
    DepsLimit=Seq.Values[Pos++];
    Subnets[SubnetIdx]->Final.ExpandedDeps.resize(DepsLimit);
    if(Pos+DepsLimit>SeqSize){
      NumSubnets=0;
      return;
    }
    for(C=0;C<DepsLimit;C++)
      Subnets[SubnetIdx]->Final.ExpandedDeps[C]=Seq.Values[Pos++];
    Subnets[SubnetIdx]->Learnt=true;
    Subnets[SubnetIdx]->Initial=Subnets[SubnetIdx]->Final;
  }
}
*/

unsigned long int Dependencies::GetTotalDeps(){
  unsigned long int Deps=0;
  for(auto Sub:Subnets){
    if(Sub)
      Deps+=Sub->Final.DependenciesCurrent;
  }
  return Deps;
}

unsigned long int Dependencies::GetTotalComplexity(){
  unsigned long int TotalComplexity=0;
  for(auto Sub:Subnets){
    if(Sub)
      TotalComplexity+=Sub->Final.Complexity;
  }
  return TotalComplexity;
}

unsigned long int Dependencies::GetTime(){
  timeb tb;
  ftime(&tb);
  unsigned long int MSecs=tb.millitm+(tb.time&0xfffff)*1000;
  return MSecs;
}

Dependencies::Dependencies(const Sequence &Seq){
  Unsolved=Seq;
  FormatPrefix="";
}

Dependencies::Dependencies(){
  FormatPrefix="";
}

bool Dependencies::CouldBeDifferent(const Sequence<int> &Seq,size_t Pos1,size_t Pos2){
  if((Seq.Knowns[Pos1]==0)||(Seq.Knowns[Pos2]==0))
    return true;
  return false;
}

int Dependencies::Dependency(const Sequence& Seq,int Interval){
  int Size=0,Pair=0,Compare=0;
  bool CouldDiffer=false;
  Size=(Unsolved.Knowns.size()<Unsolved.Values.size())?Unsolved.Knowns.size():Unsolved.Values.size();
  for(Pair=0;Pair<Size;Pair+=(Interval<<1))
    for(Compare=0;(Compare<Interval)&&(Compare+Pair+Interval<Size);++Compare){
      if(Seq.Knowns[Compare+Pair]&&Seq.Knowns[Compare+Pair+Interval])
	if(Seq.Values[Compare+Pair]!=Seq.Values[Compare+Pair+Interval])
	  return 1;
      if(CouldBeDifferent(Seq,Compare+Pair,Compare+Pair+Interval))
	CouldDiffer=true;
    }
  if(CouldDiffer)
    return 2;
  return 0;
}

/*
bool Dependencies::Dependency(const Sequence& Seq,int Interval){
  int Size=0,Pair=0,Compare=0;
  Size=(Unsolved.Knowns.size()<Unsolved.Values.size())?Unsolved.Knowns.size():Unsolved.Values.size();
  for(Pair=0;Pair<Size;Pair+=(Interval<<1))
    for(Compare=0;(Compare<Interval)&&(Compare+Pair+Interval<Size);++Compare)
      if(Seq.Knowns[Compare+Pair]&&Seq.Knowns[Compare+Pair+Interval])
	if(Seq.Values[Compare+Pair]!=Seq.Values[Compare+Pair+Interval])
	  return 1;
  return 0;
}
*/

Dependencies::~Dependencies(){
  for(auto Subnet:Subnets){
    delete(Subnet);
  }
}

void Dependencies::DependencyCheck(unsigned int Id,unsigned int NumClients){
  int Bit=0,Max=0,Size=Unsolved.Size(),Deps=0,NSubnet=0;
  vector<int> DepList;
  Sequence Tmp;
  if(VERBOSECORE){
    puts("Initialising DependencyCheck...");
  }
  if(Size)
    Max=*max_element(Unsolved.Values.begin(),Unsolved.Values.end());
  if(Max<1)
    Max=1;
  Subnets.resize(log2(Max)+1);
  DepList.resize(log2(Size)+1);

  for(int i=1;i<=Max;i<<=1){
    Tmp=Unsolved;
    Tmp&=i;
    if(IsConstantSeq(Tmp)){
      bool Value=0;
      if(ContainsKnowns(Tmp))
	Value=(bool)GetAnyKnownValue(Tmp);
      if(ENABLETREE)
	Subnets[NSubnet]=new ENiX_Subnet_Large(FormatPrefix,i,Value);
      else
	Subnets[NSubnet]=new ENiX_Subnet_Lean(FormatPrefix,i,Value);
      NSubnet++;
    }
    else{
      Deps=0;
      fill(DepList.begin(),DepList.end(),0);
      bool CouldBeDependency=false;
      int CouldBeStart=0;
      for(Bit=0;(1<<Bit)<=Size;++Bit){
	int DependencyStatus=Dependency(Tmp,1<<Bit);
	if(DependencyStatus==1){
	  DepList[Deps]=Bit;
	  Deps++;
	}
	if((!CouldBeDependency)&&(DependencyStatus==2)){
	  CouldBeDependency=true;
	  CouldBeStart=Bit;
	}
      }
      if(!Deps){
	DepList[Deps]=CouldBeStart;
	Deps++;
      }
      if(Deps||CouldBeDependency){
	if(ENABLETREE)
	  Subnets[NSubnet]=new ENiX_Subnet_Large(FormatPrefix,Deps,i,Tmp,DepList,1);
	else
	  Subnets[NSubnet]=new ENiX_Subnet_Lean(FormatPrefix,Deps,i,Tmp,DepList,0);
	Subnets[NSubnet]->ClientId=Id;
	Subnets[NSubnet]->MaxClients=NumClients;
      }
      else{
	// We should never be here.
	Subnets[NSubnet]=NULL;
      }
      NSubnet++;
    }
  }
  NumSubnets=NSubnet;
}

/*
void Dependencies::DependencyCheck(unsigned int Id,unsigned int NumClients){
  int Bit=0,Max=0,Size=Unsolved.Size(),Deps=0,NSubnet=0;
  vector<int> DepList;
  Sequence Tmp;
  if(VERBOSECORE){
    puts("Initialising DependencyCheck...");
  }
  if(Size)
    Max=*max_element(Unsolved.Values.begin(),Unsolved.Values.end());
  else
    Max=0;
  Subnets.resize(log2(Max)+1);
  DepList.resize(log2(Size)+1);  
  for(int i=1;i<=Max;i<<=1){
    Tmp=Unsolved;
    Tmp&=i;
    Deps=0;
    fill(DepList.begin(),DepList.end(),0);
    for(Bit=0;(1<<Bit)<=Size;++Bit){
      if(Dependency(Tmp,1<<Bit)){
	DepList[Deps]=Bit;
	Deps++;
      }
    }
    if(Deps){
      if(ENABLETREE)
	Subnets[NSubnet]=new ENiX_Subnet_Large(FormatPrefix,Deps,i,Tmp,DepList,1);
      else
	Subnets[NSubnet]=new ENiX_Subnet_Lean(FormatPrefix,Deps,i,Tmp,DepList,0);
      Subnets[NSubnet]->ClientId=Id;
      Subnets[NSubnet]->MaxClients=NumClients;
    }
    else
      Subnets[NSubnet]=NULL;
    NSubnet++;
  }
  NumSubnets=NSubnet;
}
*/

/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
 */

