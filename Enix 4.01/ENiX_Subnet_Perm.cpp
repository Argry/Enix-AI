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
                                        


Programmed By M.S.Braithwaite 2014, 2016.

*/

#include <pthread.h>
#include <unistd.h>
#include "ENiX_Subnet_Perm.h"
#include "ENiX_IPC.h"
#include "ENiX_Settings.h"

void *Permute::PollControlProxyFn(void *Container){
  ((struct PollThreadContainer *)Container)->TD->ClassInstance->PollControlDataThread(((struct PollThreadContainer *)Container)->TD,((struct PollThreadContainer *)Container)->IPCData);
  return NULL;
}

void Permute::PollControlDataThread(struct ThreadData *TD,IPC *IPCComms){
  unsigned int ThisPid=getpid();
  struct SharedNeuralData SND;
  while(1){
    if(*TD->Found||*TD->TerminatePoll)
      break;
    SND=IPCComms->GetFromPid(ThisPid);
    if(SND.Stop==1){
      *TD->Complexity=SND.ComplexityCap;
      *TD->Stop=SND.Stop;
      break;
    }
    usleep(POLLINTERVAL);
  }
  pthread_exit(NULL);
}

int Permute::Learn(){
  if(!Learnt){
    Learnt=LearnDep();
    if(Learnt==1)
      for(int LogicGateNum=0;LogicGateNum<Final.NeuralSize;++LogicGateNum)
	Neural[LogicGateNum]=((Final.Complexity>>(LogicGateNum<<2))&15);
  }
  return Learnt;
}

void Permute::InitialBuffer2Thread(){
  for(unsigned long int ThreadId=0;ThreadId<TOTALTHREADS;ThreadId++)
    SND[ThreadId]=Initial;
}

struct ThreadData Permute::InitialiseTD(unsigned long int ThreadId,unsigned long int *FoundBy){
  struct ThreadData TD;
  TD.Complexity=&AbortAtComplexity;
  TD.Found=&SubnetFound;
  TD.Stop=&AbortNeuralSubnet;
  TD.ThreadId=ThreadId;
  TD.ClassInstance=this;
  TD.FoundBy=FoundBy;
  TD.TerminatePoll=&TerminatePollThread;
  return TD;
}

int Permute::FindSubnet(){
  unsigned long int FoundBy=0,ThreadId=0;
  vector <pthread_t> Threads(TOTALTHREADS+(!DEBUGSERVER));
  vector <ThreadData> TD(TOTALTHREADS+(!DEBUGSERVER));
  AbortAtComplexity=0;
  AbortNeuralSubnet=0;
  SubnetFound=0;
  TerminatePollThread=0;

  /* processor threads */
  for(ThreadId=0;ThreadId<TOTALTHREADS;ThreadId++){
    TD[ThreadId]=InitialiseTD(ThreadId,&FoundBy);
    SND[ThreadId]=Initial;    
    if(pthread_create(&Threads[ThreadId],NULL,SubnetThreadProxyFn,&TD[ThreadId])){
      puts("Error creating thread...");
      return 0;
    }
  }

  if(!DEBUGSERVER){
    /* controller thread */
    TD[ThreadId]=InitialiseTD(ThreadId,&FoundBy);
    SND[ThreadId]=Initial;  
    struct PollThreadContainer PollThreadData;
    PollThreadData.TD=&TD[ThreadId];
    PollThreadData.IPCData=&IPCComms;
    if(pthread_create(&Threads[ThreadId],NULL,PollControlProxyFn,&PollThreadData)){
      puts("Error creating thread...");
      return 0;
    }
  }

  /* wait for all threads to return */
  for(ThreadId=0;ThreadId<TOTALTHREADS+(!DEBUGSERVER);ThreadId++){
    if(pthread_join(Threads[ThreadId],NULL)){
      puts("Error joining thread...");
      return 0;
    }
  }
  if(*TD[0].Found){
    Final=SND[*(TD[0].FoundBy)];
    Learnt=1;
  }
  else
    if(*TD[0].Stop)
      Learnt=2;
  return Learnt;
}

void *Permute::SubnetThreadProxyFn(void *TD){
  ((struct ThreadData *)TD)->ClassInstance->FindSubnetThread((struct ThreadData *)TD);
  return NULL;
}

bool Permute::FindSubnetThread(struct ThreadData *TD){
  unsigned long int ThreadNum=TD->ThreadId;
  unsigned long int Spacing=MaxClients*TOTALTHREADS;
  pthread_setspecific(ThreadIdKey,(void *)&ThreadNum);
  struct SubnetData *SN=&SND[ThreadNum];
  for(SN->Complexity=(ClientId*TOTALTHREADS)+TD->ThreadId;SN->Complexity<SN->MaxGuesses;SN->Complexity+=Spacing){
    if((*TD->Stop)&&((*TD->Complexity)<SN->Complexity))
      break;
    if(TD->ClassInstance->SubnetPasses()){
      *TD->Stop=1;
      *TD->Found=1;
      *TD->FoundBy=TD->ThreadId;	
      if((!(*TD->Complexity))||((*TD->Complexity)>SN->Complexity)){
	*TD->Complexity=SN->Complexity;
      }
      *TD->TerminatePoll=1;
      pthread_exit(NULL);
    }
  }
  *TD->TerminatePoll=1;
  pthread_exit(NULL);
}

int Permute::CalcNeuralSize(int Dependencies){
  if(Initial.DependenciesCurrent>1)
    return Initial.DependenciesCurrent-1;
  else
    return 1;
}

Permute::Permute(){
  FormatPrefix="";
  Learnt=0;
}

Permute::Permute(string Prefix,int Deps,int Mul,const Sequence& Answers,const vector<int> DepList,const int NeuralType){
  int NumExamples=Answers.Size();
  FormatPrefix=Prefix;

  if(VERBOSECORE){
    puts("Initialising Subnet Data");
  }
  pthread_key_create(&ThreadIdKey,NULL);
  Initial.NeuralType=NeuralType;

  Initial.IsFakeDependency=false;
  Initial.FakeValue=false;
  
  Initial.DependenciesMax=0;
  Initial.MaxGuesses=0;
  Initial.Complexity=0;
  Initial.ExpandedDeps=DependencyList;

  DependenciesMin=Deps;
  Multiplier=Mul;
  Unsolved=Sequence::NonZero(Answers);
  DependencyList=DepList;
  Learnt=0;
  if(DependenciesMin==1){
    Initial.NeuralSize=1;
    Initial.MaxGuesses=4;
  }
  else{
    if(EXTENDDEPS){
      int i=0;
      for(i=1;i<DependenciesMin;i=(i<<1)){}
      Initial.DependenciesMax=i;
    }
    Initial.NeuralSize=Initial.DependenciesMax-1;
    Initial.MaxGuesses=1<<(4*Initial.NeuralSize);
  }
  Initial.DependenciesCurrent=DependenciesMin;
  Neural.resize(Initial.NeuralSize);
  Input(NumExamples);
  for(int i=0;i<NumExamples;i++)
    Input.SetValue(i,i);
  Initial.ExpandedDeps=DependencyList;
}

Permute::Permute(string Prefix,int Mul,bool Value){
  Initial.IsFakeDependency=true;
  Initial.FakeValue=Value;
  Learnt=1;
  Multiplier=Mul;
  FormatPrefix=Prefix;
}

Sequence<int> Permute::Compress(Sequence<int> &Seq){
  int Size=Seq.Size(),NewValue=0,DepNum=0;
  Sequence OutSeq(Size);
  for(int i=0;i<Size;i++){
    for(NewValue=DepNum=0;DepNum<Initial.DependenciesCurrent;DepNum++)
      NewValue+=(((Seq.Values[i]>>(Initial.ExpandedDeps[DepNum]))&1)<<DepNum);
    OutSeq.SetValue(NewValue,i);
  }
  return OutSeq;
}

void Permute::Print(int SubnetNum){
  int i=0;
  const char *CharPrefixFmt=FormatPrefix.c_str();
  printf("%s-[DebugSubnet Final %08d]-----\n",CharPrefixFmt,SubnetNum);
  if(Initial.IsFakeDependency){
    printf("%s Output multiplier: %d, Output value: %d\n",CharPrefixFmt,Multiplier,Initial.FakeValue);
  }
  else{
    printf("%s NeuralSize: %d, Dependencies: %d/%d/%d, Multiplier: %d\n",CharPrefixFmt,Final.NeuralSize,DependenciesMin,Final.DependenciesCurrent,Final.DependenciesMax,Multiplier);
    printf("%s Complexity: %d/%d, Learnt: %d, Type: %d\n",CharPrefixFmt,Final.Complexity,Final.MaxGuesses,Learnt,Final.NeuralType);
    printf("%s Neural: ",CharPrefixFmt);
    for(i=0;i<Final.NeuralSize;i++)
      printf("[%d]",Neural[i]);
    putchar('\n');
    printf("%s Dependencies Calculated: ",CharPrefixFmt);
    for(auto C:DependencyList)
      cout<<C<<' ';
    putchar('\n');
    printf("%s Dependencies Extended:   ",CharPrefixFmt);
    for(i=0;i<Final.DependenciesCurrent;i++)
      printf("%d ",Final.ExpandedDeps[i]);
    putchar('\n');
    printf("%s Inputs:                  ",CharPrefixFmt);
    cout<<Compressed<<'\n';
    printf("%s Outputs:                 ",CharPrefixFmt);
    cout<<Unsolved<<"\n\n";
  }
}

void Permute::PrintBefore(int SubnetNum){
  int i=0;
  const char *CharPrefixFmt=FormatPrefix.c_str();
  printf("%s-[DebugSubnet Initial %08d]-----\n",CharPrefixFmt,SubnetNum);
  if(Initial.IsFakeDependency){
    printf("%s Output multiplier: %d, Output value: %d\n",CharPrefixFmt,Multiplier,Initial.FakeValue);
  }
  else{
    printf("%s NeuralSize: %d, Dependencies: %d/%d/%d, Multiplier: %d\n",CharPrefixFmt,Initial.NeuralSize,DependenciesMin,Initial.DependenciesCurrent,Initial.DependenciesMax,Multiplier);
    printf("%s Complexity: %d/%d, Learnt: %d\n",CharPrefixFmt,Initial.Complexity,Initial.MaxGuesses,Learnt);
    printf("%s Neural: ",CharPrefixFmt);
    for(i=0;i<Initial.NeuralSize;i++)
      printf("[%d]",Neural[i]);
    putchar('\n');
    printf("%s Dependencies Calculated: ",CharPrefixFmt);
    for(auto C:DependencyList)
      cout<<C<<' ';
    putchar('\n');
    printf("%s Dependencies Extended:   ",CharPrefixFmt);
    for(i=0;i<Initial.DependenciesCurrent;i++)
      printf("%d ",Initial.ExpandedDeps[i]);
    putchar('\n');
    printf("%s Inputs:                  ",CharPrefixFmt);
    cout<<Compressed<<'\n';
    printf("%s Outputs:                 ",CharPrefixFmt);
    cout<<Unsolved<<"\n\n";    
  }
}

int Permute::LearnDep(){
  int Returned=0;
  if(EXTENDDEPS==0){
    if(VERBOSECORE)
      puts("Not extending dependencies...");
    Initial.NeuralSize=CalcNeuralSize(Initial.DependenciesCurrent);
    Compressed=Compress(Input);
    return FindSubnet();
  }
  if(EXTENDDEPS==1){
    if(VERBOSECORE)
      printf("Extending dependencies between %d and %d...\n",DependenciesMin,Initial.DependenciesMax);
    if(DependenciesMin==Initial.DependenciesMax){ 
      Initial.NeuralSize=CalcNeuralSize(Initial.DependenciesCurrent);
      Compressed=Compress(Input);
      return FindSubnet();
    }
    for(Initial.DependenciesCurrent=DependenciesMin+1;Initial.DependenciesCurrent<=Initial.DependenciesMax;Initial.DependenciesCurrent++){
      if(VERBOSECORE)
	printf("Extending %d dependencies to %d...\n",DependenciesMin,Initial.DependenciesCurrent);
      if((Returned=RecursionPerm(DependenciesMin)))
	return Returned;
      else{
	Initial.NeuralSize=CalcNeuralSize(Initial.DependenciesCurrent);
	Compressed=Compress(Input);
	return FindSubnet();
      }
    }
  }
  if(EXTENDDEPS==2){
    if(VERBOSECORE)
      printf("Extending dependencies between 0 and %d...\n",Initial.DependenciesMax);
    if(DependenciesMin==Initial.DependenciesMax){
      Initial.NeuralSize=CalcNeuralSize(Initial.DependenciesCurrent);
      Compressed=Compress(Input);
      return FindSubnet();
    }
    for(Initial.DependenciesCurrent=DependenciesMin;Initial.DependenciesCurrent<=Initial.DependenciesMax;Initial.DependenciesCurrent++){
      if(VERBOSECORE)
	printf("Extending dependencies to %d...\n",Initial.DependenciesCurrent);
      Initial.ExpandedDeps=DependencyList;
      Initial.NeuralSize=CalcNeuralSize(Initial.DependenciesCurrent);
      if((Returned=RecursionPerm(0)))
	return Returned;
    }
  }
  if(EXTENDDEPS==3){
    Initial.DependenciesMax=DEPMAX;
    Initial.ExpandedDeps.resize(DEPMAX);
    Initial.NeuralSize=DEPMAX-1;
    Neural.resize(DEPMAX-1);
    if(VERBOSECORE)
      printf("Extending dependencies between 0 and %d...\n",DEPMAX);
    if(DependenciesMin==DEPMAX){
      Initial.NeuralSize=CalcNeuralSize(Initial.DependenciesCurrent);
      Compressed=Compress(Input);
      return FindSubnet();
    }
    for(Initial.DependenciesCurrent=DependenciesMin;Initial.DependenciesCurrent<=DEPMAX;Initial.DependenciesCurrent++){
      if(VERBOSECORE)
	printf("Extending dependencies to %d...\n",Initial.DependenciesCurrent);
      for(int i=0;i<DependencyList.size();i++)
	Initial.ExpandedDeps[i]=DependencyList[i];
      Initial.NeuralSize=CalcNeuralSize(Initial.DependenciesCurrent);
      if((Returned=RecursionPerm(0)))
	return Returned;
    }
  }
  return 0;
}

int Permute::RecursionPerm(int Pos){
  bool Found=0;
  int Returned=0;
  unsigned int i=0,C=0;
  if(Pos>=Initial.DependenciesCurrent){
    for(i=0;i<DependenciesMin;i++){
      Found=0;
      for(C=0;C<Initial.DependenciesCurrent;C++)
	if(Initial.ExpandedDeps[C]==DependencyList[i]){
	  Found=1;
	  break;
	}
      if(!Found)
	return 0;
    }
    if((Initial.DependenciesCurrent==0)||(Initial.DependenciesCurrent==1))
      Initial.MaxGuesses=4;
    else
      Initial.MaxGuesses=(1<<((Initial.DependenciesCurrent-1)<<2));
    Compressed=Compress(Input);
    return FindSubnet();
  }
  else{
    for(int i=0;i<DependenciesMin;i++){
      Initial.ExpandedDeps[Pos]=DependencyList[i];
      if((Returned=RecursionPerm(Pos+1)))
	return Returned;
    }
  }
  return 0;
}

/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
 */
