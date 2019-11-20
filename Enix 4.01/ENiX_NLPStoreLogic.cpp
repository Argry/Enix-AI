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
                                        


Programmed By M.S.Braithwaite 2018.

*/

#include <algorithm>
#include <strings.h>
#include "ENiX_Settings.h"
#include "ENiX_NLPStoreLogic.h"
#include "ENiX_List.h"

int NLPStoreLogic::NLP_L_NOTHING=0;
int NLPStoreLogic::NLP_L_DELETE=1;
int NLPStoreLogic::NLP_L_ADD_NEW=2;
int NLPStoreLogic::NLP_L_REPLACE=4;
int NLPStoreLogic::NLP_L_COMBINE=8;
int NLPStoreLogic::NLP_L_LOG_NEW=16;
int NLPStoreLogic::NLP_L_LOG_OLD=32;
int NLPStoreLogic::NLP_L_OVERLAP=64;
int NLPStoreLogic::NLP_L_OVL_ONE=128;
int NLPStoreLogic::NLP_L_NOVLNEW=256;
int NLPStoreLogic::NLP_L_NOVLOLD=512;

size_t NLPStoreLogic::NumOverlapping=0;

/*
  STATES[Overlap][Existing][Adding]
  0=AND,1=OR,2=NOR,3=XOR

  Adding new data is equivalent of ORing it to the old. 
 */



// AND Logic:
int NLPStoreLogic::AND_STATES[3][4][4]={
  {
    // Overlap::None
    {NLP_L_ADD_NEW,  NLP_L_ADD_NEW,  NLP_L_ADD_NEW,  NLP_L_ADD_NEW},
    {NLP_L_ADD_NEW,  NLP_L_ADD_NEW,  NLP_L_ADD_NEW,  NLP_L_ADD_NEW},
    {NLP_L_ADD_NEW,  NLP_L_ADD_NEW,  NLP_L_ADD_NEW,  NLP_L_ADD_NEW},
    {NLP_L_ADD_NEW,  NLP_L_ADD_NEW,  NLP_L_ADD_NEW,  NLP_L_ADD_NEW},
  },
  {
    // Overlap::Partial

    // AND:
    {NLP_L_COMBINE,                NLP_L_NOTHING,  NLP_L_DELETE,   NLP_L_NOVLNEW},

    // OR:
    {NLP_L_COMBINE|NLP_L_LOG_NEW,  NLP_L_OVERLAP,  NLP_L_DELETE,   NLP_L_OVERLAP|NLP_L_LOG_OLD},

    // NOR:
    {NLP_L_DELETE,                 NLP_L_DELETE,   NLP_L_COMBINE,  NLP_L_NOVLNEW},

    // XOR:
    {NLP_L_OVL_ONE|NLP_L_LOG_NEW,  NLP_L_NOTHING,  NLP_L_NOVLOLD,  NLP_L_ADD_NEW},
  },
  {
    // todo: recheck:

    // Overlap::Full (all the existing items exist in the new)

    // AND:
    {NLP_L_NOTHING,  NLP_L_NOTHING,  NLP_L_DELETE,   NLP_L_OVL_ONE},

    // OR:
    {NLP_L_REPLACE,  NLP_L_NOTHING,  NLP_L_DELETE,   NLP_L_OVL_ONE},

    // NOR:
    {NLP_L_DELETE,   NLP_L_DELETE,   NLP_L_NOTHING,  NLP_L_DELETE},

    // XOR:
    {NLP_L_OVL_ONE,  NLP_L_NOTHING,  NLP_L_DELETE,   NLP_L_NOTHING},
  }
};

// OR Logic:
int NLPStoreLogic::OR_STATES[3][4][4]={
  {
    // Overlap::None
    {NLP_L_ADD_NEW,  NLP_L_ADD_NEW,  NLP_L_ADD_NEW,  NLP_L_ADD_NEW},
    {NLP_L_ADD_NEW,  NLP_L_ADD_NEW,  NLP_L_ADD_NEW,  NLP_L_ADD_NEW},
    {NLP_L_ADD_NEW,  NLP_L_ADD_NEW,  NLP_L_ADD_NEW,  NLP_L_ADD_NEW},
    {NLP_L_ADD_NEW,  NLP_L_ADD_NEW,  NLP_L_ADD_NEW,  NLP_L_ADD_NEW},
  },
  {
    // Overlap::Partial

    // AND:
    {NLP_L_ADD_NEW,                             NLP_L_NOVLNEW,  NLP_L_ADD_NEW,                             NLP_L_ADD_NEW},

    // OR:
    {NLP_L_DELETE|NLP_L_ADD_NEW|NLP_L_NOVLOLD,  NLP_L_COMBINE,  NLP_L_DELETE|NLP_L_NOVLOLD|NLP_L_NOVLNEW,  NLP_L_ADD_NEW},

    // NOR:
    {NLP_L_ADD_NEW,                             NLP_L_ADD_NEW,  NLP_L_ADD_NEW,                             NLP_L_DELETE|NLP_L_NOVLNEW},

    // XOR:
    {NLP_L_ADD_NEW,                             NLP_L_ADD_NEW,  NLP_L_DELETE|NLP_L_NOVLOLD,                NLP_L_ADD_NEW},
  },
  {
    // todo: recheck:

    // Overlap::Full (all the existing items exist in the new)

    // AND:
    {NLP_L_NOTHING,  NLP_L_REPLACE,  NLP_L_ADD_NEW,  NLP_L_ADD_NEW},

    // OR:
    {NLP_L_NOTHING,  NLP_L_NOTHING,  NLP_L_ADD_NEW,  NLP_L_NOTHING},

    // NOR:
    {NLP_L_ADD_NEW,  NLP_L_ADD_NEW,  NLP_L_NOTHING,  NLP_L_ADD_NEW},

    // XOR:
    {NLP_L_REPLACE,  NLP_L_NOTHING,  NLP_L_ADD_NEW,  NLP_L_NOTHING},
  }
};

string NLPStoreLogic::GetLogicName(NLPStoreLogic::SLogic Input){
  if(Input==NLPStoreLogic::SLogic::AND){
    return (string)"AND";
  }
  if(Input==NLPStoreLogic::SLogic::OR){
    return (string)"OR";
  }
  if(Input==NLPStoreLogic::SLogic::NOR){
    return (string)"NOR";
  }
  if(Input==NLPStoreLogic::SLogic::XOR){
    return (string)"XOR";
  }
  return (string)"OR";
}

/* Get the type of overlap between the Original phrase and the new phrase */
NLPStoreLogic::Overlap NLPStoreLogic::GetOverlap(vector<string> Original,vector<string> New){
  bool Overlap=false;
  size_t OriginalSize=Original.size();
  size_t NewSize=New.size();
  NLPStoreLogic::NumOverlapping=0;
  vector<string> OverlappedData=List::SetTheoryIntersect(Original,New);
  NLPStoreLogic::NumOverlapping=OverlappedData.size();
  bool AllOld=(NLPStoreLogic::NumOverlapping==OriginalSize);
  bool AllNew=(NLPStoreLogic::NumOverlapping==NewSize);
  Overlap=!!(NLPStoreLogic::NumOverlapping);
  if(!Overlap){
    return NLPStoreLogic::Overlap::None;
  }
  if(Overlap&&AllOld&&AllNew){
    return NLPStoreLogic::Overlap::Full;
  }
  if(Overlap&&AllOld&&(!AllNew)){
    return NLPStoreLogic::Overlap::AllOld;
  }
  if(Overlap&&(!AllOld)&&AllNew){
    return NLPStoreLogic::Overlap::AllNew;
  }
  if(Overlap&&(!AllOld)&&(!AllNew)){
    return NLPStoreLogic::Overlap::Partial;
  }  
  return NLPStoreLogic::Overlap::None;
  /*
  NLPStoreLogic::NumOverlapping=0;
  vector<string> OverlappedData=List::SetTheoryIntersect(Original,New);
  NLPStoreLogic::NumOverlapping=OverlappedData.size();
  Overlap=!!(NLPStoreLogic::NumOverlapping);
  if((NLPStoreLogic::NumOverlapping!=Original.size())||(NLPStoreLogic::NumOverlapping!=New.size())){
    SomeNonOverlap=true;
  }
  if(!Overlap){
    return NLPStoreLogic::Overlap::None;
  }
  if(Overlap&&(!SomeNonOverlap)){
    return NLPStoreLogic::Overlap::Full;
  }
  if(Overlap&&SomeNonOverlap){
    return NLPStoreLogic::Overlap::Partial;
  }  
  return NLPStoreLogic::Overlap::None;
  */
}

/* Internal function to change the logic of Logic to the logic provided by ChangeTo */
vector<string> NLPStoreLogic::ChangeLogicTo(vector<string> &Logic,string &ChangeTo){
  vector<string> FinalLogic=Logic;
  if(FinalLogic.size()){
    FinalLogic[0]=ChangeTo;
  }
  return FinalLogic;
}

/* change the logic in the OldLogic to the logic in the New */
vector<string> NLPStoreLogic::ChangeLog2New(vector<string> &OldLogic,vector<string> &NewLogic){
  if(NewLogic.size()){
    return NLPStoreLogic::ChangeLogicTo(OldLogic,NewLogic[0]);
  }
  else{
    return OldLogic;
  }
}

/* change the logic in NewLogic to the logic in the oldlogic */
vector<string> NLPStoreLogic::ChangeLog2Old(vector<string> &OldLogic,vector<string> &NewLogic){
  if(OldLogic.size()){
    return NLPStoreLogic::ChangeLogicTo(NewLogic,OldLogic[0]);
  }
  else{
    return OldLogic;
  }
}

/* return the overlap, keeping the old logic */
vector<string> NLPStoreLogic::GetOverlapData(vector<string> &OldLogic,vector<string> &NewLogic){
  vector<string> FinalLogic;
  if(OldLogic.size()&&NewLogic.size()){
    FinalLogic.push_back(OldLogic[0]);
    for(size_t OldLogicId=1;OldLogicId<OldLogic.size();++OldLogicId){
      for(size_t NewLogicId=1;NewLogicId<NewLogic.size();++NewLogicId){
	if(OldLogic[OldLogicId]==NewLogic[NewLogicId]){
	  FinalLogic.push_back(OldLogic[OldLogicId]);
	  break;
	}
      }
    }
  }
  else{
    return OldLogic;
  }
  return FinalLogic;
}

/* Keep old logic and get the overlap if there is one */
vector<string> NLPStoreLogic::GetOverlapIfOne(vector<string> &OldLogic,vector<string> &NewLogic){
  vector<string> FinalLogic;

  if(OldLogic.size()&&NewLogic.size()){
    FinalLogic.push_back(OldLogic[0]);
    for(size_t OldLogicId=1;OldLogicId<OldLogic.size();++OldLogicId){
      for(size_t NewLogicId=1;NewLogicId<NewLogic.size();++NewLogicId){
	if(OldLogic[OldLogicId]==NewLogic[NewLogicId]){
	  FinalLogic.push_back(OldLogic[OldLogicId]);
	  break;
	}
      }
    }
  }
  
  if(FinalLogic.size()==2){
    return FinalLogic;
  }
  else{
    return {};
  }
}


/* Keep new logic operation and any new non-overlapping elements */
vector<string> NLPStoreLogic::GetNewNonOverlap(vector<string> &OldLogic,vector<string> &NewLogic){
  bool Found=false;
  vector<string> FinalLogic;
  if(OldLogic.size()&&NewLogic.size()){
    FinalLogic.push_back(NewLogic[0]);
    for(size_t NewLogicId=1;NewLogicId<NewLogic.size();++NewLogicId){
      Found=false;   
      for(size_t OldLogicId=1;OldLogicId<OldLogic.size();++OldLogicId){
	if(OldLogic[OldLogicId]==NewLogic[NewLogicId]){
	  Found=true;
	  break;
	}
      }
      if(!Found){
	FinalLogic.push_back(NewLogic[NewLogicId]);
      }
    }
  }
  return FinalLogic;
}

/* Keep old logic operation and any old non-overlapping elements */
vector<string> NLPStoreLogic::GetOldNonOverlap(vector<string> &OldLogic,vector<string> &NewLogic){
  bool Found=false;
  vector<string> FinalLogic;
  if(OldLogic.size()&&NewLogic.size()){
    FinalLogic.push_back(OldLogic[0]);
    for(size_t OldLogicId=1;OldLogicId<OldLogic.size();++OldLogicId){
      Found=false;   
      for(size_t NewLogicId=1;NewLogicId<NewLogic.size();++NewLogicId){
	if(OldLogic[OldLogicId]==NewLogic[NewLogicId]){
	  Found=true;
	  break;
	}
      }
      if(!Found){
	FinalLogic.push_back(OldLogic[OldLogicId]);
      }
    }
  }
  return FinalLogic;
}

/* Combination of both new and old */
vector<string> NLPStoreLogic::GetCombined(vector<string> &OldLogic,vector<string> &NewLogic){
  vector<string> FinalLogic;
  if(OldLogic.size()&&NewLogic.size()){
    FinalLogic.push_back(OldLogic[0]);
    for(size_t OldLogicId=1;OldLogicId<OldLogic.size();++OldLogicId){
      for(size_t NewLogicId=1;NewLogicId<NewLogic.size();++NewLogicId){
	if(OldLogic[OldLogicId]==NewLogic[NewLogicId]){
	  vector<string>::iterator FoundIter=find(FinalLogic.begin(),FinalLogic.end(),OldLogic[OldLogicId]);
	  if(FoundIter==FinalLogic.end()){
	    FinalLogic.push_back(OldLogic[OldLogicId]);	    
	  }
	  break;
	}
      }
    }
  }
  return FinalLogic;
}

void NLPStoreLogic::DebugOverlap(NLPStoreLogic::Overlap Input){
  if(Input==NLPStoreLogic::Overlap::None){
    puts("No overlap");
    return;
  }
  if(Input==NLPStoreLogic::Overlap::Partial){
    puts("Partial overlap");
    return;
  }
  if(Input==NLPStoreLogic::Overlap::AllNew){
    puts("All new has overlapped");
    return;
  }
  if(Input==NLPStoreLogic::Overlap::AllOld){
    puts("All old has overlapped");
    return;
  }
  if(Input==NLPStoreLogic::Overlap::Full){
    puts("Full overlap");
    return;
  }
}

void NLPStoreLogic::DebugSLogic(NLPStoreLogic::SLogic Input){
  if(Input==NLPStoreLogic::SLogic::AND){
    puts("AND Logic");
    return;
  }
  if(Input==NLPStoreLogic::SLogic::OR){
    puts("OR Logic");
    return;
  }
  if(Input==NLPStoreLogic::SLogic::NOR){
    puts("NOR Logic");
    return;
  }
  if(Input==NLPStoreLogic::SLogic::XOR){
    puts("XOR Logic");
    return;
  }
}

int NLPStoreLogic::StateCalculation(NLPStoreLogic::Type PhraseType,NLPStoreLogic::Overlap OverlapType,NLPStoreLogic::SLogic OldLogic,NLPStoreLogic::SLogic NewLogic){
  if(PhraseType==NLPStoreLogic::Type::Subject){
    return NLPStoreLogic::OR_STATES[(int)OverlapType][(int)OldLogic][(int)NewLogic];
  }
  if(PhraseType==NLPStoreLogic::Type::PredicateNouns){
    return NLPStoreLogic::AND_STATES[(int)OverlapType][(int)OldLogic][(int)NewLogic];
  }
  return 0;
}

NLPStoreLogic::SLogic NLPStoreLogic::GetLogicCode(string Operand){
  if(!strcasecmp(Operand.c_str(),"AND")){
    return NLPStoreLogic::SLogic::AND;
  }
  if(!strcasecmp(Operand.c_str(),"OR")){
    return NLPStoreLogic::SLogic::OR;
  }
  if(!strcasecmp(Operand.c_str(),"NOR")){
    return NLPStoreLogic::SLogic::NOR;
  }
  if(!strcasecmp(Operand.c_str(),"XOR")){
    return NLPStoreLogic::SLogic::XOR;
  }
  return NLPStoreLogic::SLogic::AND;
}

bool NLPStoreLogic::QuestionPhraseFound(NLPStoreLogic::SLogic OldLogic,NLPStoreLogic::SLogic NewLogic,NLPStoreLogic::Overlap OverlapType,size_t SizeofOverlap){
  bool IsFound=true;
  
  /*
  DEBUGMSG;
  printf("OldLogic: %d, NewLogic: %d, Overlap: %d, SizeofOverlap: %d\n",OldLogic,NewLogic,OverlapType,SizeofOverlap);
  */

  if((OldLogic==NLPStoreLogic::SLogic::AND)&&(NewLogic==NLPStoreLogic::SLogic::AND)&&((OverlapType==NLPStoreLogic::Overlap::Full)||(OverlapType==NLPStoreLogic::Overlap::AllNew))){
    return true;
  }
  if((OldLogic==NLPStoreLogic::SLogic::AND)&&(NewLogic==NLPStoreLogic::SLogic::OR)&&(OverlapType!=NLPStoreLogic::Overlap::None)){
    return true;
  }
  if((OldLogic==NLPStoreLogic::SLogic::AND)&&(NewLogic==NLPStoreLogic::SLogic::NOR)){
    return false;
  }
  if((OldLogic==NLPStoreLogic::SLogic::AND)&&(NewLogic==NLPStoreLogic::SLogic::XOR)&&(SizeofOverlap==1)&&((OverlapType==NLPStoreLogic::Overlap::Full)||(OverlapType==NLPStoreLogic::Overlap::AllOld))){
    return true;
  }
  if((OldLogic==NLPStoreLogic::SLogic::OR)&&(NewLogic==NLPStoreLogic::SLogic::AND)&&(SizeofOverlap==1)&&((OverlapType==NLPStoreLogic::Overlap::AllNew)||(OverlapType==NLPStoreLogic::Overlap::Full))){
    return true;
  }
  if((OldLogic==NLPStoreLogic::SLogic::OR)&&(NewLogic==NLPStoreLogic::SLogic::OR)&&((OverlapType==NLPStoreLogic::Overlap::Full)||(OverlapType==NLPStoreLogic::Overlap::AllOld))){
    return true;
  }
  if((OldLogic==NLPStoreLogic::SLogic::OR)&&(NewLogic==NLPStoreLogic::SLogic::NOR)&&(OverlapType!=NLPStoreLogic::Overlap::Full)&&(OverlapType!=NLPStoreLogic::Overlap::AllOld)){
    // If there are more ORs than there are NORs. 
    return true;
  }
  //  if((OldLogic==NLPStoreLogic::SLogic::OR)&&(NewLogic==NLPStoreLogic::SLogic::XOR)&&(SizeofOverlap==1)&&(OverlapType==NLPStoreLogic::Overlap::AllNew)){

  if((OldLogic==NLPStoreLogic::SLogic::OR)&&(NewLogic==NLPStoreLogic::SLogic::XOR)&&(SizeofOverlap==1)&&(OverlapType==NLPStoreLogic::Overlap::AllOld)){
    return true;
  }
  if((OldLogic==NLPStoreLogic::SLogic::NOR)&&(NewLogic==NLPStoreLogic::SLogic::AND)){
    return false;
  }
  if((OldLogic==NLPStoreLogic::SLogic::NOR)&&(NewLogic==NLPStoreLogic::SLogic::OR)){
    return false;
  }
  if((OldLogic==NLPStoreLogic::SLogic::NOR)&&(NewLogic==NLPStoreLogic::SLogic::NOR)&&((OverlapType==NLPStoreLogic::Overlap::Full)||(OverlapType==NLPStoreLogic::Overlap::AllNew))){
    // if there are more or equal old nors
    return true;
  }
  if((OldLogic==NLPStoreLogic::SLogic::NOR)&&(NewLogic==NLPStoreLogic::SLogic::XOR)){
    return false;
  }
  if((OldLogic==NLPStoreLogic::SLogic::XOR)&&(NewLogic==NLPStoreLogic::SLogic::AND)&&(SizeofOverlap==1)){
    return true;
  }
  if((OldLogic==NLPStoreLogic::SLogic::XOR)&&(NewLogic==NLPStoreLogic::SLogic::OR)&&((OverlapType==NLPStoreLogic::Overlap::Full)||(OverlapType==NLPStoreLogic::Overlap::AllOld))){
    // if there are more or equal new ors than xors
    return true;
  }
  if((OldLogic==NLPStoreLogic::SLogic::XOR)&&(NewLogic==NLPStoreLogic::SLogic::NOR)){
    return false;
  }
  if((OldLogic==NLPStoreLogic::SLogic::XOR)&&(NewLogic==NLPStoreLogic::SLogic::XOR)&&(OverlapType==NLPStoreLogic::Overlap::Full)){
    return true;
  }
  return false;
}


/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
*/
