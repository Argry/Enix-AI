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
#include<sstream>

#include "ENiX_Deps.h"
#include "ENiX_Lean.h"
#include "ENiX_Large.h"
#include "ENiX_CORE.h"
#include "ENiX_Awareness.h"
#include "ENiX_Settings.h"
#include "ENiX_List.h"

ENiX_NN *ThisSessionANN=NULL;

ENiX_NN::ENiX_NN(){
  CDSLink=NULL;
}

vector<string> ENiX_NN::Deconstruct(unsigned int NumHosts,Sequence Seq){
  unsigned int Multiplier=1,C=0;
  vector<string> BrokenFragments={};
  Sequence NewSeq=Seq,Values;
  while(!IsZeroSeq(NewSeq)){
    Values=NewSeq;
    Values&=1;
    for(C=0;C<NumHosts;C++){
      ostringstream ToConvert;
      ToConvert<<"FRAGMENT ID "<<C<<" / "<<NumHosts<<" MUL "<<Multiplier<<" SEQ "<<Values;
      BrokenFragments.push_back(ToConvert.str());
    }
    Multiplier=(Multiplier<<1);
    NewSeq/=2;
  }
  return BrokenFragments;
}

Sequence<int> ENiX_NN::Execute(Sequence<int> Inp){
  Sequence S=Sequence::Zero(Inp.Size());
  for(int i=0;i<NumSubnets;++i)
    if(Subnets[i]){
      if(Subnets[i]->Final.IsFakeDependency){
	S+=((Subnets[i]->Final.FakeValue)*(Subnets[i]->Multiplier));
      }
      else{
	S+=(Subnets[i]->Execute(Inp.Values));
      }
    }
  return S;
}

/*
Sequence<int> ENiX_NN::Execute(Sequence<int> Inp){
  Sequence S=Sequence::Zero(Inp.Size());
  for(int i=0;i<NumSubnets;++i)
    if(Subnets[i])
      S+=(Subnets[i]->Execute(Inp.Values));
  return S;
}
*/

Sequence<int> ENiX_NN::Execute(vector<int> Vector){
  Sequence S=Sequence::Zero(Vector.size());
  for(int i=0;i<NumSubnets;++i)
    if(Subnets[i])
      S+=(Subnets[i]->Execute(Vector));
  return S;
}

bool ENiX_NN::Neural2Str(){
  int C=0,SubnetCount=0;
  bool Failed=false;
  ostringstream ToConvert;
  for(int i=0;i<NumSubnets;++i)
    if(Subnets[i])
      SubnetCount++;
  ToConvert<<SubnetCount<<" ";
  for(int i=0;i<NumSubnets;++i){
    if(Subnets[i]){
      Failed|=!(Subnets[i]->Learnt==1);
      if(Failed)
	break;
      ToConvert<<Subnets[i]->Final.IsFakeDependency<<" ";
      if(Subnets[i]->Final.IsFakeDependency){
	ToConvert<<Subnets[i]->Final.FakeValue<<" ";
	ToConvert<<Subnets[i]->Multiplier<<" ";
      }
      else{	
	ToConvert<<Subnets[i]->Final.NeuralType<<" ";
	ToConvert<<Subnets[i]->DependenciesMin<<" ";
	ToConvert<<Subnets[i]->Final.DependenciesCurrent<<" ";
	ToConvert<<Subnets[i]->Final.DependenciesMax<<" ";
	ToConvert<<Subnets[i]->Multiplier<<" ";
	ToConvert<<Subnets[i]->Final.Complexity<<" ";
	ToConvert<<Subnets[i]->Final.MaxGuesses<<" ";
	ToConvert<<Subnets[i]->Final.NeuralSize<<" ";
	for(C=0;C<Subnets[i]->Final.NeuralSize;C++)
	  ToConvert<<Subnets[i]->Neural[C]<<" ";
	ToConvert<<Subnets[i]->Final.ExpandedDeps.size()<<" ";
	for(C=0;C<Subnets[i]->Final.ExpandedDeps.size();C++)
	  ToConvert<<Subnets[i]->Final.ExpandedDeps[C]<<" ";
      }
    }
  }
  NeuralAsStr=ToConvert.str();
  NeuralLearnt=!Failed;
  return NeuralLearnt;
}

/*
bool ENiX_NN::Neural2Str(){
  int C=0,SubnetCount=0;
  bool Failed=false;
  ostringstream ToConvert;
  for(int i=0;i<NumSubnets;++i)
    if(Subnets[i])
      SubnetCount++;
  ToConvert<<SubnetCount<<" ";
  for(int i=0;i<NumSubnets;++i){
    if(Subnets[i]){      
      Failed|=!(Subnets[i]->Learnt==1);
      if(Failed)
	break;
      ToConvert<<Subnets[i]->Final.NeuralType<<" ";
      ToConvert<<Subnets[i]->DependenciesMin<<" ";
      ToConvert<<Subnets[i]->Final.DependenciesCurrent<<" ";
      ToConvert<<Subnets[i]->Final.DependenciesMax<<" ";
      ToConvert<<Subnets[i]->Multiplier<<" ";
      ToConvert<<Subnets[i]->Final.Complexity<<" ";
      ToConvert<<Subnets[i]->Final.MaxGuesses<<" ";
      ToConvert<<Subnets[i]->Final.NeuralSize<<" ";
      for(C=0;C<Subnets[i]->Final.NeuralSize;C++)
	ToConvert<<Subnets[i]->Neural[C]<<" ";
      ToConvert<<Subnets[i]->Final.ExpandedDeps.size()<<" ";
      for(C=0;C<Subnets[i]->Final.ExpandedDeps.size();C++)
	ToConvert<<Subnets[i]->Final.ExpandedDeps[C]<<" ";
    }
  }
  NeuralAsStr=ToConvert.str();
  NeuralLearnt=!Failed;
  return NeuralLearnt;
}
*/

void ENiX_NN::PrintNN(const ENiX_NN& ANN){
  printf("%s-[Neural Network Summary]----\n",FormatPrefix.c_str());
  if(ANN.NumSubnets){
    for(int i=0;i<ANN.NumSubnets;++i)
      if(ANN.Subnets[i])
	ANN.Subnets[i]->PrintSN();
  }
  else{
    printf("%s No neural network.",FormatPrefix.c_str());
  }
  putchar('\n');
}

string ENiX_NN::NN2String(const ENiX_NN& ANN){
  string NeuralData="-[Neural Network Summary]----\n";
  if(ANN.NumSubnets){
    for(int i=0;i<ANN.NumSubnets;++i)
      if(ANN.Subnets[i])
	NeuralData=NeuralData+ANN.Subnets[i]->SN2String();
  }
  else{
    NeuralData=NeuralData+" No neural network.";
  }
  NeuralData+="\n";
  return NeuralData;
}

void ENiX_NN::Debug(){
  for(int i=0;i<NumSubnets;++i)
    if(Subnets[i])
      Subnets[i]->Print(i);
}

void ENiX_NN::DebugBefore(){
  for(int i=0;i<NumSubnets;++i)
    if(Subnets[i])
      Subnets[i]->PrintBefore(i);
}

bool ENiX_NN::DLearn(){
  
  return false;
}

bool ENiX_NN::Learn(){
  bool Learnt=1;
  unsigned long int StartTime=0;
  StartTime=GetTime();
  for(int i=0;i<NumSubnets;++i)
    if(Subnets[i]){
      if(Subnets[i]->Initial.IsFakeDependency==false){
	Learnt&=(Subnets[i]->Learn()==1);
      }
      else{
	Subnets[i]->Final=Subnets[i]->Initial;
      }
    }
  if(VERBOSECORE){
    if(!(Learnt==1))
      puts("Failed to learn problem.");
    else
      puts("Problem learnt.");
    TimeTaken=GetTime()-StartTime;
    printf("Time taken: %ld ms\n\n",(unsigned long int)TimeTaken);
  }
  return Learnt;
}

/*
bool ENiX_NN::Learn(){
  bool Learnt=1;
  unsigned long int StartTime=0;
  StartTime=GetTime();
  for(int i=0;i<NumSubnets;++i)
    if(Subnets[i]){
      Learnt&=(Subnets[i]->Learn()==1);
    }
  if(VERBOSECORE){
    if(!(Learnt==1))
      puts("Failed to learn problem.");
    else
      puts("Problem learnt.");
    TimeTaken=GetTime()-StartTime;
    printf("Time taken: %ld ms\n\n",(unsigned long int)TimeTaken);
  }
  return Learnt;
}
*/

ENiX_NN::ENiX_NN(int argc,char **argv){
  Sequence ConvertInput(argc,argv);
  Unsolved=ConvertInput;
  CDSLink=NULL;
}

ENiX_NN::ENiX_NN(Sequence Seq){
  Unsolved=Seq;
  CDSLink=NULL;
}

ENiX_NN::ENiX_NN(unsigned int ClientId,unsigned int NumClients,Sequence Seq){
  Unsolved=Seq;
  CDSLink=NULL;
}

void ENiX_NN::LearnTopLevel(unsigned int Id,unsigned int NumClients){
  DependencyCheck(Id,NumClients);
  Learn();
  Debug();
  PrintNN(*this);  
}

void *ENiX_NN::CommandLinkThread(void *Command){
  CommandThreadData *CmdTD=(CommandThreadData *)Command;
  CmdTD->Data=CmdTD->ServerLink->CommandToServer(CmdTD->ServerLink,CmdTD->Command);
  return NULL;
}

void ENiX_NN::DLearnTopLevel(unsigned int Id,unsigned int NumClients){
  DependencyCheck(Id,NumClients);
  vector<string> Deconstructed=Deconstruct(NumClients,Unsolved);  
  OpenNeuralLinks(Deconstructed.size());
  string SeshId=BinData::GetSessionIdentifier();
  if(NeuralLinks.size()){
    vector<pthread_t> DistNN(Deconstructed.size());
    vector<CommandThreadData> CmdData(Deconstructed.size());
    vector<int> FinalSequence={(int)(Deconstructed.size())};
    for(size_t _=0;_<Deconstructed.size();++_){
      printf(" <> [%s][Frag:%003d]: ",SeshId.c_str(),_);
      puts(Deconstructed[_].c_str());
      CmdData[_].Command=Deconstructed[_];
      CmdData[_].ServerLink=NeuralLinks[_];      
      pthread_create(&DistNN[_],NULL,CommandLinkThread,&CmdData[_]);
    }
    for(size_t _=0;_<DistNN.size();++_){
      if(pthread_join(DistNN[_],NULL)){
	printf("  E Error joining thread [%d]\n",_);
      }
      //      CmdData[_].Data.Print();

      /* the data is stored in CmdData[_].Data.Strings[2].
	 the first number needs to removed, then appended to the neural network.
	 The neural network must start with with the DeconstructedSize */
      
      if(CmdData[_].Data.Strings.size()>1){
	vector<string> SubnetData=List::String2List(CmdData[_].Data.Strings[2].Convert2Str(),false);
	vector<int> SubnetInts=List::List2Int(SubnetData);
	if(SubnetInts.size()){
	  SubnetInts.erase(SubnetInts.begin());
	  if(SubnetInts[0])
	    SubnetInts[2]=1<<_;
	  else
	    SubnetInts[5]=1<<_;

	  string SubnetData=List::List2String(SubnetInts);
	  printf(" >< [%s][Frag:%003d]: %s\n",SeshId.c_str(),_,SubnetData.c_str());
	  
	  FinalSequence=List::VecAdd(FinalSequence,SubnetInts);
	}
      }
    }

    /* The neural network must be saved in the usual place */
    ThisSessionANN->ClearANN();
    ThisSessionANN=new ENiX_NN();

    string FinalNeural=List::List2String(FinalSequence);
    printf(" -- [%s]: Final: %s\n",SeshId.c_str(),FinalNeural.c_str());
    
    ThisSessionANN->PopulateNeural(FinalSequence);
    CloseNeuralLinks();
  }
  else{
    puts("  * No neural links found, running locally...\n");
    Learn();
    Debug();
    PrintNN(*this);  
  }
}

void ENiX_NN::CloseNeuralLinks(){
  for(size_t _=0;_<NeuralLinks.size();++_)
    delete NeuralLinks[_];
  NeuralLinks={};
}

void ENiX_NN::OpenNeuralLinks(size_t DesiredNum){
  if(!CDSLink){
    CDSLink=new Networking("localhost",577,false);     
    string HostName;
    int PortNum=0;
    if(CDSLink && CDSLink->ConnectedStatus==NetworkStatus::Connected){
      Packet PeerList=CDSLink->CommandToServer(CDSLink,"GETPEERSONLINE NNP");      
      if(PeerList.Strings.size()>2){
	for(size_t PeerId=0;(PeerId<DesiredNum);++PeerId){
	  int DistributeTo=(PeerId%(PeerList.Strings.size()-2));
	  vector<string> ConnectionParameters=List::String2List(PeerList.Strings[DistributeTo+2].Convert2Str(),false);
	  if(ConnectionParameters.size()>2){
	    HostName=ConnectionParameters[1];
	    PortNum=atoi(ConnectionParameters[2].c_str());
	    printf("  i Establishing connection to: %s:%d\n",HostName.c_str(),PortNum);
	    Networking *RefNeuralLink=new Networking(HostName,PortNum,false);
	    NeuralLinks.push_back(RefNeuralLink);
	    if(RefNeuralLink&&(RefNeuralLink->ConnectedStatus==NetworkStatus::Connected)){
	      // receive the protocol data and dump it.
	      Packet Incoming(RefNeuralLink->Receive());
	    }
	  }
	}	
	putchar('\n');
      }
    }
  }
}

void ENiX_NN::ClearANN(){
  if(ThisSessionANN!=NULL){
    delete ThisSessionANN;
    ThisSessionANN=NULL;
  }  
}

string ENiX_NN::RetrieveANN(){
  string Response="";
  if(ThisSessionANN->Neural2Str())
    Response+=ThisSessionANN->NeuralAsStr;
  else
    Response="NO NEURAL NETWORK";
  return Response;
}



/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
 */
