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
                                        


Programmed By M.S.Braithwaite 2017.

*/

#include "ENiX_Memory.h"
#include "ENiX_Semantics.h"
#include "ENiX_Time.h"
#include<string.h>
#include<sstream>

Memory *GlobalMemoryAccess=NULL;

vector<vector<string>> Memory::SeparateParagraph(vector<string> &Data){
  vector<vector<string>> SeparatedSentences;
  vector<string> ThisSentence;
  vector<string> SeparatedData=List::SeparateSymbols(Data);
  for(size_t Pos=0;Pos<SeparatedData.size();++Pos){
    if(List::SetTheoryIsElem(EndPunctuation,SeparatedData[Pos])){
      if(ThisSentence.size()){
	ThisSentence.push_back(SeparatedData[Pos]);
	SeparatedSentences.push_back(ThisSentence);
	ThisSentence={};
	continue;
      }
    }
    ThisSentence.push_back(SeparatedData[Pos]);
  }
  if(ThisSentence.size()){
    SeparatedSentences.push_back(ThisSentence);
  }
  return SeparatedSentences;
}

void ConditionalParagraphData::Debug(){
  puts("Causes:");
  puts("=============");
  List::PrintList(Causes);
  printf("Cause logic: %s\n",NLPStoreLogic::GetLogicName(CauseLogic).c_str());
  puts("Effects:");
  puts("=============");
  List::PrintList(Effects);
  printf("Effect logic: %s\n",NLPStoreLogic::GetLogicName(EffectLogic).c_str());
  printf("Conditional Sentence Mood: %s\n",Mood.c_str());
}

ConditionalParagraphData Memory::SplitConditional(vector<string> ConditionalSentence){
  // TODO: Conditional logic
  // Not convinced by the logic, testcase in test.txt
    
  ConditionalParagraphData StoredData;
  StoredData.Mood=GetCondSentenceMood(ConditionalSentence);
  bool IsCause=true;
  bool IsEffect=false;
  bool WasComma=false;
  bool WasConditional=false;
  vector<vector<string>> CauseSentences;
  vector<vector<string>> EffectSentences;
  vector<string> SentenceSoFar;
  vector<string> ConjunctionList={"and","or","either","nor","not"};
  NLPStoreLogic::SLogic ConditionalLogic=NLPStoreLogic::SLogic::AND;
  bool FoundLogic=false;  

  // if conditional sentence question then remove the question mark
  if((StoredData.Mood=="ynq")||(StoredData.Mood=="whq"))
    for(size_t _=0;_<ConditionalSentence.size();++_)
      if(ConditionalSentence[_]=="?")
	ConditionalSentence.erase(ConditionalSentence.begin()+_);
  
  for(size_t _=0;_<ConditionalSentence.size();++_){
    if(WasComma&&SetTheoryIsElem(ConjunctionList,ConditionalSentence[_])){
      if(ConditionalSentence[_]=="and")
	ConditionalLogic=NLPStoreLogic::SLogic::AND;
      if(ConditionalSentence[_]=="neither")
	ConditionalLogic=NLPStoreLogic::SLogic::NOR;
      if(ConditionalSentence[_]=="either")
	ConditionalLogic=NLPStoreLogic::SLogic::XOR;
      if(ConditionalSentence[_]=="nor")
	ConditionalLogic=NLPStoreLogic::SLogic::NOR;
      if(ConditionalSentence[_]=="or")
	if(!FoundLogic)
	  ConditionalLogic=NLPStoreLogic::SLogic::OR;
      if(ConditionalSentence[_]=="not")
	ConditionalLogic=NLPStoreLogic::SLogic::NOR;
      FoundLogic=true;
      if(IsCause^IsEffect){
	if(SentenceSoFar.size()>1){
	  SentenceSoFar.erase(SentenceSoFar.end()-1);
	  if(IsCause){
	    CauseSentences.push_back(SentenceSoFar);
	    SentenceSoFar={};
	  }
	  else{
	    EffectSentences.push_back(SentenceSoFar);
	    SentenceSoFar={};
	  }
	}
      }
      WasComma=false;
      continue;
    }
    if(ConditionalSentence[_]==",")
      WasComma=true;

    if(WasConditional){
      // if it is a verb and was a conditional the previous - in particular a conjugation of the verb "is" etc, then it needs to be removed.
      // it could also be related to a pronoun or preposition
      for(size_t Pos=_;Pos<ConditionalSentence.size();++Pos){
	string Article=ConditionalSentence[Pos];
	vector<string> WordArticles=GetArticlesFromWord(Article);
	bool CouldBeVerb=List::SetTheoryIsElem(WordArticles,"verb");
	bool CouldBeVerbPhrase=List::SetTheoryIsIntersect(WordArticles,{"verb","pronoun","preposition"});
	if(CouldBeVerbPhrase){
	  // we need some more logic here to determine if the verb is a conjugation of "to be".
	  if(CouldBeVerb){
	    if(List::SetTheoryIsElem(VerbFormArticles,ConditionalSentence[Pos]))
	      ConditionalSentence.erase(ConditionalSentence.begin()+_);
	  }
	  else
	    ConditionalSentence.erase(ConditionalSentence.begin()+_);
	}
	else
	  break;
      }
      WasConditional=false;
    }
    
    if(SetTheoryIsElem(CondCauseTokens,ConditionalSentence[_])){
      if(SentenceSoFar.size()){
	EffectSentences.push_back(SentenceSoFar);
	SentenceSoFar={};
      }
      IsEffect=false;
      if(FoundLogic){
	StoredData.EffectLogic=ConditionalLogic;
      }
      WasConditional=true;
      IsCause=true;
      FoundLogic=false;
      continue;
    }
    if(SetTheoryIsElem(CondEffectTokens,ConditionalSentence[_])){
      if(SentenceSoFar.size()){
	CauseSentences.push_back(SentenceSoFar);
	SentenceSoFar={};
      }
      IsEffect=true;
      if(FoundLogic){
	StoredData.CauseLogic=ConditionalLogic;
      }
      WasConditional=true;
      IsCause=false;
      FoundLogic=false;
      continue;
    }

    SentenceSoFar.push_back(ConditionalSentence[_]);
  }

  if(IsCause^IsEffect){
    if(SentenceSoFar.size()){
      if(IsCause){
	CauseSentences.push_back(SentenceSoFar);
	SentenceSoFar={};
      }
      else{
	EffectSentences.push_back(SentenceSoFar);
	SentenceSoFar={};
      }
    }
  }

  StoredData.Causes=CauseSentences;
  StoredData.Effects=EffectSentences;

  return StoredData;
}

bool Memory::IsCondSentence(vector<string> &Data){
  bool CauseOverlap=List::SetTheoryIsIntersect(Data,CondCauseTokens);
  bool EffectOverlap=List::SetTheoryIsIntersect(Data,CondEffectTokens);
  if(CauseOverlap|EffectOverlap){
    return true;
  }
  else{
    return false;
  }
}

string Memory::GetCondSentenceMood(vector<string> &Data){
  bool IsQuestion=List::SetTheoryIsElem(Data,"?");
  if(IsQuestion&&Data.size()){
    vector<string> WordArticles=GetArticlesFromWord(Data[0]);
    bool StartsWithVerb=List::SetTheoryIsElem(WordArticles,"verb");    
    if(StartsWithVerb)
      return "ynq";
    else
      return "whq";
  }
  return "indicative";
}

void Memory::RemoveFromCache(string RecName){
  bool FoundRec=false;
  vector<WMS_Rec>::iterator Pos;
  BinData PrimaryKey(RecName.c_str(),RecName.size());
  for(Pos=CachedContents.begin();Pos!=CachedContents.end();++Pos)
    if((Pos->Contents[0])[0]==PrimaryKey){
      FoundRec=true;
      break;
    }
  CachedContents.erase(Pos);
}

bool Memory::YesNoQuestion(){
  size_t SubjOverlapSize=0;
  size_t PredOverlapSize=0;

  /*
  DEBUGMSG;
  PredicateVerbs.DebugPhrase();
  */  

  FindOrCreate(PredicateVerbs.CSVerb);    
  WMS_Rec VerbRec=GetFromCache(PredicateVerbs.CSVerb);  

  /*
  DEBUGMSG;
  Subject.DebugPhrase();
  DEBUGMSG;
  PredicateNouns.DebugPhrase();
  */

  if((Subject.CSSubPhrases.size())||(PredicateNouns.CSSubPhrases.size())){
    if(VerbRec.Contents.size()>2){
      for(size_t _=0;(_<VerbRec.Contents[1].size())&&(_<VerbRec.Contents[2].size());++_){
	vector<string> OldSubj=List::String2List((VerbRec.Contents[1])[_].Convert2Str());
	if(OldSubj[0]==":DEREF"){
	  continue;
	}
	vector<string> OldPredNouns=List::String2List((VerbRec.Contents[2])[_].Convert2Str());

	// the logic must be removed here using shiftvector

	vector<string> TmpOldSubj=OldSubj;
	vector<string> TmpOldPredNouns=OldPredNouns;
	
	TmpOldSubj=List::ShiftVector(TmpOldSubj,1);
	TmpOldPredNouns=List::ShiftVector(TmpOldPredNouns,1);

	NLPStoreLogic::Overlap SubjOv=NLPStoreLogic::GetOverlap(TmpOldSubj,Subject.CSSubPhrases);
	SubjOverlapSize=NLPStoreLogic::NumOverlapping;
	NLPStoreLogic::Overlap PredOv=NLPStoreLogic::GetOverlap(TmpOldPredNouns,PredicateNouns.CSSubPhrases);
	PredOverlapSize=NLPStoreLogic::NumOverlapping;	

	// we need to work out that where the new subject and old subject can match
	// if the new predicate and old predicate can also match. If it does, then 
	// the answer is yes. Otherwise, it's no.

	NLPStoreLogic::SLogic SubjOldLogic=NLPStoreLogic::GetLogicCode(OldSubj[0]);
	NLPStoreLogic::SLogic SubjNewLogic=NLPStoreLogic::GetLogicCode(Subject.LogicType);
	NLPStoreLogic::SLogic PredOldLogic=NLPStoreLogic::GetLogicCode(OldPredNouns[0]);
	NLPStoreLogic::SLogic PredNewLogic=NLPStoreLogic::GetLogicCode(PredicateNouns.LogicType);	

	bool SubjFound=NLPStoreLogic::QuestionPhraseFound(SubjOldLogic,SubjNewLogic,SubjOv,SubjOverlapSize);
	bool PredFound=NLPStoreLogic::QuestionPhraseFound(PredOldLogic,PredNewLogic,PredOv,PredOverlapSize);

	/*
	DEBUGMSG;
	printf("Logic codes: %d,%d,%d,%d\n",SubjOldLogic,SubjNewLogic,PredOldLogic,PredNewLogic);
	printf("Overlap Size: %d,%d Overlap Type: %d,%d\n",SubjOverlapSize,PredOverlapSize,SubjOv,PredOv);
	List::PrintList(TmpOldSubj);
	List::PrintList(TmpOldPredNouns);
	
	List::PrintList(Subject.CSSubPhrases);
	List::PrintList(PredicateNouns.CSSubPhrases);
	*/

	if(SubjFound&&PredFound){
	  return true;
	}
      }
    }
  }

  // Neural network processing calls go here:
  
  // so we need symbols for sentences, cs symbols
  
  
  
  //

  return false;
}

vector<string> Memory::WHQuestion(Memory::PhraseType PType){
  vector<string> ConstructedS;
  size_t SubjOverlapSize=0;
  size_t PredOverlapSize=0;
  FindOrCreate(PredicateVerbs.CSVerb);
  WMS_Rec VerbRec=GetFromCache(PredicateVerbs.CSVerb);
  if((Subject.CSSubPhrases.size())||(PredicateNouns.CSSubPhrases.size())){
    if(VerbRec.Contents.size()>2){
      for(size_t _=0;(_<VerbRec.Contents[1].size())&&(_<VerbRec.Contents[2].size());++_){
	string ThisSubject=(VerbRec.Contents[1])[_].Convert2Str();
	if(ThisSubject==":DEREF"){
	  continue;
	}
	if(PType==Memory::PhraseType::IsSubject){
	  vector<string> OldPredNouns=List::String2List((VerbRec.Contents[2])[_].Convert2Str());
	  vector<string> TmpOldPredNouns=OldPredNouns;
	  TmpOldPredNouns=List::ShiftVector(TmpOldPredNouns,1);
	  NLPStoreLogic::Overlap PredOv=NLPStoreLogic::GetOverlap(TmpOldPredNouns,PredicateNouns.CSSubPhrases);
	  PredOverlapSize=NLPStoreLogic::NumOverlapping;
	  NLPStoreLogic::SLogic PredOldLogic=NLPStoreLogic::GetLogicCode(OldPredNouns[0]);
	  NLPStoreLogic::SLogic PredNewLogic=NLPStoreLogic::GetLogicCode(PredicateNouns.LogicType);
	  bool PredFound=NLPStoreLogic::QuestionPhraseFound(PredOldLogic,PredNewLogic,PredOv,PredOverlapSize);
	  vector<string> ThisWordList=PredicateVerbs.CanonicalVerbs[0].Words;
	  vector<string> ThisArticleList=PredicateVerbs.CanonicalVerbs[0].Articles;
	  vector<string> AdverbList=Phrase::FilterArticles(ThisWordList,ThisArticleList,"adverb");
	  vector<string> VerbList=Phrase::FilterArticles(ThisWordList,ThisArticleList,"verb");
	  string ThisVerb;
	  if(VerbList.size()){
	    ThisVerb=VerbList[VerbList.size()-1];
	  }
	  else{
	    ThisVerb=VerbList[0];
	  }
	  vector<string> VerbTense=CreateTense(AdverbList,ThisVerb,Sentence::Vec2Tense(ThisWordList));
	  string ConstructedSent=List::List2String(CreateSentence(ThisSubject,VerbTense,PredicateNouns.CSSubPhrases[0],Sentence::SentenceVoiceType::Active));
	  ConstructedS.push_back(ConstructedSent);
	}
	if(PType==Memory::PhraseType::IsPredicate){
	  vector<string> OldSubj=List::String2List((VerbRec.Contents[1])[_].Convert2Str());
	  vector<string> TmpOldSubj=OldSubj;
	  TmpOldSubj=List::ShiftVector(TmpOldSubj,1);
	  NLPStoreLogic::Overlap SubjOv=NLPStoreLogic::GetOverlap(TmpOldSubj,Subject.CSSubPhrases);
	  SubjOverlapSize=NLPStoreLogic::NumOverlapping;
	  NLPStoreLogic::SLogic SubjOldLogic=NLPStoreLogic::GetLogicCode(OldSubj[0]);
	  NLPStoreLogic::SLogic SubjNewLogic=NLPStoreLogic::GetLogicCode(Subject.LogicType);
	  bool SubjFound=NLPStoreLogic::QuestionPhraseFound(SubjOldLogic,SubjNewLogic,SubjOv,SubjOverlapSize);
	  string ThisPredicate=(VerbRec.Contents[2])[_].Convert2Str();
	  vector<string> ThisWordList=PredicateVerbs.CanonicalVerbs[0].Words;
	  vector<string> ThisArticleList=PredicateVerbs.CanonicalVerbs[0].Articles;
	  vector<string> AdverbList=Phrase::FilterArticles(ThisWordList,ThisArticleList,"adverb");
	  vector<string> VerbList=Phrase::FilterArticles(ThisWordList,ThisArticleList,"verb");
	  string ThisVerb;
	  if(VerbList.size()){
	    ThisVerb=VerbList[VerbList.size()-1];
	  }
	  else{
	    ThisVerb=VerbList[0];
	  }
	  vector<string> VerbTense=CreateTense(AdverbList,ThisVerb,Sentence::Vec2Tense(ThisWordList));
	  /* This may not be right, it may have logic already */
	  ThisSubject="OR "+Subject.CSSubPhrases[0];
	  string ConstructedSent=List::List2String(CreateSentence(ThisSubject,VerbTense,ThisPredicate,Sentence::SentenceVoiceType::Active));
	  ConstructedS.push_back(ConstructedSent);	  
	}
      }
    }
  }

  // Neural network processing
  
  return ConstructedS;
}


void Memory::DebugSentenceLogic(SentenceLogic &Logic){

  printf("Verbs: ");
  List::PrintList(Logic.Verbs);
  
  printf("OldSubject, Logic %d: ",Logic.SubjOldLogic);
  List::PrintList(Logic.OldSubj);
    
  printf("OldPredNouns, Logic %d: ",Logic.PNOldLogic);
  List::PrintList(Logic.OldPredNouns);

  printf("NewSubject, Logic %d: ",Logic.SubjNewLogic);
  List::PrintList(Logic.NewSubj);
    
  printf("NewPredNouns, Logic %d: ",Logic.PNNewLogic);
  List::PrintList(Logic.NewPredNouns);

  printf("Subject action to take: %d\n",Logic.SubjectAction);  
  printf("Predicate noun action to take: %d\n",Logic.PNAction);
}

void Memory::PrepareSentence(vector<string> &Sentence){
  /* WARNING: do not call these functions out of order! */
  ostringstream ToConvert;
  OriginalSentence=Sentence;

  SetSentenceInvalid();
  SentenceToArticles(OriginalSentence);
  
  GetSentenceMood();
  if(SentenceMoodFound!=""){
    vector<string> ToExclude={StartSentenceToken,EndSentenceToken};
    FoundArticles=List::SetTheoryExclude(FoundArticles,ToExclude);
    GetSentenceVoice();
    GetTense();
    GetBaseTense();
    GetTemplate();
    SeparateSentence();
    if(LogicList.size()){
      if(Subject.Size()){
	Subject.SplitSubject(Subject,LogicList);
      }
      if(PredicateNouns.Size()){
	PredicateNouns.SplitSubject(PredicateNouns,LogicList);
      }
    }
    Subject.PrepCanonicalPhrase(SubjectOrderList);
    PrepNounCSSymbols(Subject);
    PredicateNouns.PrepCanonicalPhrase(SubjectOrderList);
    PrepNounCSSymbols(PredicateNouns);

    // Need to get the full tense
    PredicateVerbs.PrepCanonicalVerbs(VerbOrderList,LongTense,BaseTense);
    PrepVerbCSSymbols(PredicateVerbs);

    // append the tense to the end of the predicate verbs
    if(PredicateVerbs.CanonicalSubPhrases.size()){
      PredicateVerbs.CanonicalSubPhrases[0].push_back(BaseTense,BaseTense);
    }

    Memory::PrepareSymbols();
  }
}

void Memory::SaveSentences(){

  // we need to loop through the sentences
  for(size_t _=0;_<PreparedSentenceSymbols.size();++_){

    SentenceLogic All;

    // separate out into S / PV / PN
    All.NewSubj=get<0>(PreparedSentenceSymbols[_]);
    All.Verbs=get<1>(PreparedSentenceSymbols[_]);
    All.NewPredNouns=get<2>(PreparedSentenceSymbols[_]);
    
    FindOrCreate(All.Verbs[0]);    
    WMS_Rec VerbRec=GetFromCache(All.Verbs[0]);

    if(All.NewSubj.size()&&All.NewPredNouns.size()){

      // loop though value / attribute pairings of the db record. 
      if(VerbRec.Contents.size()>2){
	for(size_t _=0;(_<VerbRec.Contents[1].size())&&(_<VerbRec.Contents[2].size());++_){
	  
	  All.OldSubj=List::String2List((VerbRec.Contents[1])[_].Convert2Str());
	  All.OldPredNouns=List::String2List((VerbRec.Contents[2])[_].Convert2Str());

	  if(All.OldSubj.size()&&All.OldPredNouns.size()){
	    
	    // we check for overlap in both the subject and predicate!
	    NLPStoreLogic::Overlap SubjOv=NLPStoreLogic::GetOverlap(All.OldSubj,All.NewSubj);
	    NLPStoreLogic::Overlap PredOv=NLPStoreLogic::GetOverlap(All.OldPredNouns,All.NewPredNouns);
	    
	    // we need to get the logic from each phrase
	    
	    All.SubjOldLogic=NLPStoreLogic::GetLogicCode(All.OldSubj[0]);
	    All.SubjNewLogic=NLPStoreLogic::GetLogicCode(All.NewSubj[0]);

	    All.PNOldLogic=NLPStoreLogic::GetLogicCode(All.OldPredNouns[0]);
	    All.PNNewLogic=NLPStoreLogic::GetLogicCode(All.NewPredNouns[0]);

	    // we need to read the appropriate action from the tables

	    All.SubjectAction=NLPStoreLogic::StateCalculation(NLPStoreLogic::Type::Subject,SubjOv,All.SubjOldLogic,All.SubjNewLogic);
	    All.PNAction=NLPStoreLogic::StateCalculation(NLPStoreLogic::Type::PredicateNouns,PredOv,All.PNOldLogic,All.PNNewLogic);
	    
	    // we need to calculate the changes to be made to the attributes
	    // by taking apart the bits
	    // Need a function that accepts a WMS_Rec and the Action along with the old and new data.

	    DEBUGMSG;
	    
	    VerbRec=GetRecUpdate(VerbRec,_,All);	    
	  }
	}
      }
      else{
	DEBUGMSG;

	VerbRec=AddDBPair(VerbRec,List::List2String(All.NewSubj),List::List2String(All.NewPredNouns));	
      }
      RepInCache(VerbRec);
    }    
  }
}

WMS_Rec Memory::GetRecUpdate(WMS_Rec &Rec,int EntryNum,SentenceLogic Input){

  WMS_Rec Adjusted=Rec;
  vector<string> SubjectCauses;
  vector<string> PredicateEffects;

  if(Rec.Contents.size()<3)
    Rec.Contents.resize(3);
  
  if(((Input.SubjectAction&NLPStoreLogic::NLP_L_DELETE)==NLPStoreLogic::NLP_L_DELETE)||((Input.PNAction&NLPStoreLogic::NLP_L_DELETE)==NLPStoreLogic::NLP_L_DELETE)){
    Adjusted=DelDBPair(Rec,EntryNum);
  }

  // we need to create the new subjects

  if((Input.SubjectAction&NLPStoreLogic::NLP_L_ADD_NEW)==NLPStoreLogic::NLP_L_ADD_NEW){
    SubjectCauses.push_back(List::List2String(Input.NewSubj));
  }
  if((Input.SubjectAction&NLPStoreLogic::NLP_L_REPLACE)==NLPStoreLogic::NLP_L_REPLACE){
    BinData Replacement(List::List2String(Input.NewSubj));
    (Adjusted.Contents[1])[EntryNum]=Replacement;
  }
  if((Input.SubjectAction&NLPStoreLogic::NLP_L_COMBINE)==NLPStoreLogic::NLP_L_COMBINE){
    SubjectCauses.push_back(List::List2String(NLPStoreLogic::GetCombined(Input.OldSubj,Input.NewSubj)));
  }
  if((Input.SubjectAction&NLPStoreLogic::NLP_L_OVERLAP)==NLPStoreLogic::NLP_L_OVERLAP){
    SubjectCauses.push_back(List::List2String(NLPStoreLogic::GetOverlapData(Input.OldSubj,Input.NewSubj)));
  }
  if((Input.SubjectAction&NLPStoreLogic::NLP_L_OVL_ONE)==NLPStoreLogic::NLP_L_OVL_ONE){
    SubjectCauses.push_back(List::List2String(NLPStoreLogic::GetOverlapIfOne(Input.OldSubj,Input.NewSubj)));
  }
  if((Input.SubjectAction&NLPStoreLogic::NLP_L_NOVLNEW)==NLPStoreLogic::NLP_L_NOVLNEW){
    SubjectCauses.push_back(List::List2String(NLPStoreLogic::GetNewNonOverlap(Input.OldSubj,Input.NewSubj)));
  }
  if((Input.SubjectAction&NLPStoreLogic::NLP_L_LOG_NEW)==NLPStoreLogic::NLP_L_LOG_NEW){
    SubjectCauses.push_back(List::List2String(NLPStoreLogic::ChangeLog2New(Input.OldSubj,Input.NewSubj)));
  }
  if((Input.SubjectAction&NLPStoreLogic::NLP_L_LOG_OLD)==NLPStoreLogic::NLP_L_LOG_OLD){
    SubjectCauses.push_back(List::List2String(NLPStoreLogic::ChangeLog2Old(Input.OldSubj,Input.NewSubj)));
  }

  
  // we need to create the new predicates

  if((Input.PNAction&NLPStoreLogic::NLP_L_ADD_NEW)==NLPStoreLogic::NLP_L_ADD_NEW){
    PredicateEffects.push_back(List::List2String(Input.NewPredNouns));
  }
  if((Input.PNAction&NLPStoreLogic::NLP_L_REPLACE)==NLPStoreLogic::NLP_L_REPLACE){
    BinData Replacement(List::List2String(Input.NewPredNouns));
    (Adjusted.Contents[2])[EntryNum]=Replacement;
  }
  if((Input.PNAction&NLPStoreLogic::NLP_L_COMBINE)==NLPStoreLogic::NLP_L_COMBINE){
    PredicateEffects.push_back(List::List2String(NLPStoreLogic::GetCombined(Input.OldPredNouns,Input.NewPredNouns)));
  }
  if((Input.PNAction&NLPStoreLogic::NLP_L_OVERLAP)==NLPStoreLogic::NLP_L_OVERLAP){
    PredicateEffects.push_back(List::List2String(NLPStoreLogic::GetOverlapData(Input.OldPredNouns,Input.NewPredNouns)));
  }
  if((Input.PNAction&NLPStoreLogic::NLP_L_OVL_ONE)==NLPStoreLogic::NLP_L_OVL_ONE){
    PredicateEffects.push_back(List::List2String(NLPStoreLogic::GetOverlapIfOne(Input.OldPredNouns,Input.NewPredNouns)));
  }
  if((Input.PNAction&NLPStoreLogic::NLP_L_NOVLNEW)==NLPStoreLogic::NLP_L_NOVLNEW){
    PredicateEffects.push_back(List::List2String(NLPStoreLogic::GetNewNonOverlap(Input.OldPredNouns,Input.NewPredNouns)));
  }
  if((Input.PNAction&NLPStoreLogic::NLP_L_LOG_NEW)==NLPStoreLogic::NLP_L_LOG_NEW){
    PredicateEffects.push_back(List::List2String(NLPStoreLogic::ChangeLog2New(Input.OldPredNouns,Input.NewPredNouns)));
  }
  if((Input.PNAction&NLPStoreLogic::NLP_L_LOG_OLD)==NLPStoreLogic::NLP_L_LOG_OLD){
    PredicateEffects.push_back(List::List2String(NLPStoreLogic::ChangeLog2Old(Input.OldPredNouns,Input.NewPredNouns)));
  }


  // we need to store the combinations of subjects x predicates
  
  for(size_t SubId=0;SubId<SubjectCauses.size();++SubId){
    for(size_t PredId=0;PredId<PredicateEffects.size();++PredId){
      Adjusted=AddDBPair(Adjusted,SubjectCauses[SubId],PredicateEffects[PredId]);
    }
  }

  return Adjusted;
}

void Memory::PrepNounCSSymbols(Phrase &NounPhrase){
  for(size_t _=0;_<NounPhrase.CanonicalSubPhrases.size();++_){
    string CSRefId=CSRef(NounPhrase.CanonicalSubPhrases[_].Words);
    NounPhrase.CSSubPhrases.push_back(CSRefId);
  }
}

void Memory::PrepVerbCSSymbols(Phrase &VerbPhrase){
  if(VerbPhrase.CanonicalVerbs.size()){
    VerbPhrase.CSVerb=CSRef(VerbPhrase.CanonicalVerbs[0].Words);
  }
}

void Memory::LoadVerbForm(string RecordName,map<vector<string>,vector<string>> &VerbForms){
  VerbForms={};
  FindOrCreate(RecordName);
  WMS_Rec Rec=GetFromCache(RecordName);
  if(Rec.Contents.size()>2){
    for(size_t _=0;(_<(Rec.Contents[1]).size())&&(_<(Rec.Contents[2]).size());++_){
      vector<string> Form=List::String2List(((Rec.Contents[2])[_]).Convert2Str());
      vector<string> Name=List::String2List(((Rec.Contents[1])[_]).Convert2Str());
      AVF(VerbForms,Form,Name);
    }
  }
}

void Memory::SaveVerbForm(string RecordName,map<vector<string>,vector<string>> &VerbForms){ 
  FindOrCreate(RecordName);
  WMS_Rec Rec=GetFromCache(RecordName);
  map<vector<string>,vector<string>>::iterator VfIter;
  if(Rec.Contents.size()>1){
    Rec.Contents[1]={};
    Rec.Contents[2]={};
  }
  for(VfIter=VerbForms.begin();VfIter!=VerbForms.end();++VfIter){
    string Name=List::List2String(VfIter->second);
    string Form=List::List2String(VfIter->first);
    Rec=AddDBPair(Rec,Name,Form);
  }
  RepInCache(Rec);
}

void Memory::GetTense(){
  // determine if tense is already known
  vector<string> Empty={};
  if(NameofTense==Empty){
    LongTense={};
    /* if we are in here, then there is only one verb */
    vector<string> ToExclude={StartSentenceToken,EndSentenceToken};
    vector<string> SentenceArticles=List::SetTheoryExclude(FoundArticles,ToExclude);    
    string Tense;
    for(size_t _=0;_<SentenceArticles.size();++_){
      FindOrCreate(OriginalSentence[_]); // this will be unnecessary
      WMS_Rec WordRec=GetFromCache(OriginalSentence[_]);
      string Extracted=ExtractString(WordRec,WMSAPI::Sector::Header,WMSAPI::HeaderSeg::Purpose);
      if(Extracted!="`"){
	NameofTense={Extracted};
	break;
      }
      vector<string> Empty={};
      if(NameofTense==Empty){
	NameofTense={"present"};
      }
    }
  }
}

string Memory::SentenceDebug(vector<string> Sentence){
  ostringstream ToConvert;

  PrepareSentence(Sentence);

  ToConvert<<"====[Word]============[Article]====[Article Options]========"<<endl;
  for(size_t _=0;(_<OriginalSentence.size())&&(_<FoundArticles.size())&&(_<PossibleArticles.size());++_){
    ToConvert<<BinData::FormatString("% 010s % 020s % 020s\n",OriginalSentence[_].c_str(),FoundArticles[_].c_str(),List::List2String(PossibleArticles[_]).c_str());
  }
  ToConvert<<"------------------------------------------------------------"<<endl;

  ToConvert<<BinData::FormatString("%45s: %s\n","Sentence Mood",SentenceMoodFound.c_str());
  
  string SentenceVoiceDetect;
  if(SentenceVoiceFound==Sentence::SentenceVoiceType::Active)
    SentenceVoiceDetect="Active";
  if(SentenceVoiceFound==Sentence::SentenceVoiceType::Passive)
    SentenceVoiceDetect="Passive";
  if(SentenceVoiceFound==Sentence::SentenceVoiceType::Unknown)
    SentenceVoiceDetect="Unknown";
  ToConvert<<BinData::FormatString("%45s: %s\n","Sentence Voice",SentenceVoiceDetect.c_str());
  
  ToConvert<<BinData::FormatString("%45s: %s\n","Sentence Tense",List::List2String(NameofTense).c_str());
  ToConvert<<BinData::FormatString("%45s: %s\n","Sentence Base Tense",BaseTense.c_str());
  ToConvert<<BinData::FormatString("%45s: %s\n","Sentence Template",List::List2String(FoundTemplate).c_str());
  ToConvert<<BinData::FormatString("%45s: %s\n","Subject",List::List2String(Subject.Words).c_str());

  for(size_t _=0;_<Subject.SubPhrases.size();++_)
    ToConvert<<BinData::FormatString("%45s: %s\n","Subject Subphrase",List::List2String(Subject.SubPhrases[_].Words).c_str());

  ToConvert<<BinData::FormatString("%45s: %s\n","Subject Logic",Subject.LogicType.c_str());
  ToConvert<<BinData::FormatString("%45s: %s\n","Predicate Nouns",List::List2String(PredicateNouns.Words).c_str());

  for(size_t _=0;_<PredicateNouns.SubPhrases.size();++_)
    ToConvert<<BinData::FormatString("%45s: %s\n","Predicate Noun Subphrase",List::List2String(PredicateNouns.SubPhrases[_].Words).c_str());

  ToConvert<<BinData::FormatString("%45s: %s\n","Predicate Noun Logic",PredicateNouns.LogicType.c_str());
  ToConvert<<BinData::FormatString("%45s: %s\n","Predicate Verbs",List::List2String(PredicateVerbs.Words).c_str());

  if(PredicateVerbs.CanonicalVerbs.size()){
    ToConvert<<BinData::FormatString("%45s: %s\n","Canonical Verb Phrase",List::List2String(PredicateVerbs.CanonicalVerbs[0].Words).c_str());
    ToConvert<<BinData::FormatString("%45s: %s\n","Canonical Verb Phrase Symbol",PredicateVerbs.CSVerb.c_str());
  }

  for(size_t _=0;_<Subject.SubPhrases.size();++_){
    ToConvert<<BinData::FormatString("%45s: %s\n","Canonical Subject Phrase",List::List2String(Subject.CanonicalSubPhrases[_].Words).c_str());
    ToConvert<<BinData::FormatString("%45s: %s\n","Canonical Subject Phrase Symbol",Subject.CSSubPhrases[_].c_str());
  }

  for(size_t _=0;_<PredicateNouns.SubPhrases.size();++_){
    ToConvert<<BinData::FormatString("%45s: %s\n","Canonical Predicate Noun Phrase",List::List2String(PredicateNouns.CanonicalSubPhrases[_].Words).c_str());
    ToConvert<<BinData::FormatString("%45s: %s\n","Canonical Predicate Phrase Symbol",PredicateNouns.CSSubPhrases[_].c_str());
  }

  for(size_t _=0;_<PreparedSentenceSymbols.size();++_){
    ToConvert<<BinData::FormatString("%45s: %s %s %s\n",BinData::FormatString("Sentence %03d",_).c_str(),List::List2String(get<0>(PreparedSentenceSymbols[_])).c_str(),List::List2String(get<1>(PreparedSentenceSymbols[_])).c_str(),List::List2String(get<2>(PreparedSentenceSymbols[_])).c_str());
  }
  
  ToConvert<<"============================================================"<<endl;
  return ToConvert.str();
}

vector<vector<string>> Memory::SentenceToArticles(vector<string> &Sentence){
  OriginalSentence=Sentence;
  PossibleArticles={};
  PossibleArticles.push_back(List::String2List(StartSentenceToken));
  for(size_t _=0;_<Sentence.size();++_){
    vector<string> WordArticles=GetArticlesFromWord(Sentence[_]);
    PossibleArticles.push_back(WordArticles);
  }
  PossibleArticles.push_back(List::String2List(EndSentenceToken));
  return PossibleArticles;
}

vector<string> Memory::GetArticlesFromWord(string Word){
  FindOrCreate(Word);
  WMS_Rec WordRec=GetFromCache(Word);
  string ArticleString=ExtractString(WordRec,WMSAPI::Sector::Header,WMSAPI::HeaderSeg::Grammar);
  if(ArticleString=="`")
    return WordArticleList;
  return List::String2List(ArticleString);
}

void Memory::LoadTemplate(string GrammarName){
  FindOrCreate(UniqueUN);
  WMS_Rec Rec=GetFromCache(UniqueUN);
  string TemplateData=GetDBValue(Rec,GrammarName);
  AddToTemplates(GrammarName,TemplateData);
}

void Memory::SaveTemplate(string GrammarName){
  FindOrCreate(UniqueUN);
  WMS_Rec Rec=GetFromCache(UniqueUN);
  map<string,vector<vector<string>>>::iterator TemplateIter=SentenceTemplates.find(GrammarName);  
  if(TemplateIter!=SentenceTemplates.end()){
    vector<vector<string>> TemplateData=TemplateIter->second;
    vector<vector<string>>::iterator TemplateDataIter;    
    string ToPrint;
    for(TemplateDataIter=TemplateData.begin();TemplateDataIter!=TemplateData.end();++TemplateDataIter){
      ToPrint=ToPrint+List::List2String(*TemplateDataIter);
      if(TemplateDataIter+1!=TemplateData.end())
	ToPrint=ToPrint+",";
    }
    Rec=RepDBPair(Rec,GrammarName,ToPrint);
    RepInCache(Rec);
  }
}

void Memory::LoadGrammar(string GrammarName){
  FindOrCreate(GrammarName);
  WMS_Rec Rec=GetFromCache(GrammarName);
  map<string,vector<string>> GraphData=FindOrCreateGraph(GrammarName);
  if(Rec.Contents.size()>1){
    for(size_t _=0;(_<Rec.Contents[1].size())&&(_<Rec.Contents[2].size());++_){
      string AttrName=(Rec.Contents[1])[_].Convert2Str();
      vector<string> AttrValues=List::String2List((Rec.Contents[2])[_].Convert2Str());
      GraphData[AttrName]=AttrValues;
    }
  }
  map<string,map<string,vector<string>>>::iterator GraphPos=GrammarGraphs.find(GrammarName);
  if(GraphPos==GrammarGraphs.end())
    GrammarGraphs.insert(make_pair(GrammarName,GraphData));
  else
    GrammarGraphs[GrammarName]=GraphData;
}

void Memory::SaveGrammar(string GrammarName){
  FindOrCreate(GrammarName);
  WMS_Rec Rec=GetFromCache(GrammarName);
  map<string,vector<string>> GraphData=FindOrCreateGraph(GrammarName);
  map<string,vector<string>>::iterator GraphDataPos;
  for(GraphDataPos=GraphData.begin();GraphDataPos!=GraphData.end();++GraphDataPos){
    string AttrName=GraphDataPos->first;
    string AttrValue;
    if(GraphDataPos->second.size())
      AttrValue=List::List2String(GraphDataPos->second);
    else
      AttrValue="`";
    Rec=RepDBPair(Rec,AttrName,AttrValue);
  }
  RepInCache(Rec);  
}

WMS_Rec Memory::Mode2WMS(){
  FindOrCreate(UniqueUN);
  WMS_Rec Rec=GetFromCache(UniqueUN);
  string ConvertedCList=List2String(SetTheoryUnion(AllRecords,RecordList));
  Rec=RepDBPair(Rec,":vocab",ConvertedCList);
  string AnswerCList=List2String(ANSWER);
  Rec=RepDBPair(Rec,":answer",AnswerCList);
  string CSNumStr=BinData::Int2String(CSNum);
  Rec=RepDBPair(Rec,":csnum",CSNumStr);
  string ArticleCList=List2String(ArticleList);
  Rec=RepDBPair(Rec,":articles",ArticleCList);
  string WordArticleCList=List2String(WordArticleList);
  Rec=RepDBPair(Rec,":wordarticles",WordArticleCList);
  string PredSection=List2String(PredicateArticles);
  Rec=RepDBPair(Rec,":predarticles",PredSection);
  string SubjSection=List2String(SubjectArticles);
  Rec=RepDBPair(Rec,":subjarticles",SubjSection);
  string MoodCList=List2String(MoodList);
  Rec=RepDBPair(Rec,":moodlist",MoodCList);
  string LogicCList=List2String(LogicList);
  Rec=RepDBPair(Rec,":logiclist",LogicCList);
  Rec=RepDBPair(Rec,":startsentence",StartSentenceToken);
  Rec=RepDBPair(Rec,":endsentence",EndSentenceToken);
  string EndPunctToken=List2String(EndPunctuation);
  Rec=RepDBPair(Rec,":endpuncttokens",EndPunctToken);
  string CondCauseToken=List2String(CondCauseTokens);
  Rec=RepDBPair(Rec,":condcausetokens",CondCauseToken);
  string CondEffectToken=List2String(CondEffectTokens);
  Rec=RepDBPair(Rec,":condeffecttokens",CondEffectToken);
  string PassiveAgentList=List2String(PassiveAgents);
  Rec=RepDBPair(Rec,":spa",PassiveAgentList);
  string VerbOrderListStr=List2String(VerbOrderList);
  Rec=RepDBPair(Rec,":verborder",VerbOrderListStr);
  string SubjectOrderListStr=List2String(SubjectOrderList);
  Rec=RepDBPair(Rec,":subjectorder",SubjectOrderListStr);
  RepInCache(Rec);
  for(size_t _=0;_<MoodList.size();++_){
    SaveGrammar(MoodList[_]);
    SaveTemplate(MoodList[_]);
  }
  SaveVerbForm(":activevf",ActiveVerbForms);
  SaveVerbForm(":passivevf",PassiveVerbForms);
  return Rec;
}

void Memory::WMS2Mode(WMS_Rec Rec){
  string VocabData=GetDBValue(Rec,":vocab");
  if(VocabData=="")
    AllRecords={};
  else
    AllRecords=String2List(VocabData,true);
  string AnswerData=GetDBValue(Rec,":answer");
  if(AnswerData=="")
    ANSWER={};
  else
    ANSWER=String2List(AnswerData,true);
  AnswerData=GetDBValue(Rec,":csnum");
  if(AnswerData=="")
    CSNum=1;
  else
    CSNum=stoi(AnswerData);
  string ArticleData=GetDBValue(Rec,":articles");
  if(ArticleData=="")
    ArticleList={};
  else
    ArticleList=String2List(ArticleData,true);
  string WordArticleData=GetDBValue(Rec,":wordarticles");
  if(WordArticleData=="")
    WordArticleList={};
  else
    WordArticleList=String2List(WordArticleData,true);
  string MoodData=GetDBValue(Rec,":moodlist");
  if(MoodData=="")
    MoodList={};
  else
    MoodList=String2List(MoodData,true);
  string LogicData=GetDBValue(Rec,":logiclist");
  if(LogicData=="")
    LogicList={};
  else
    LogicList=String2List(LogicData,true);
  string EndPunctToken=GetDBValue(Rec,":endpuncttokens");
  if(EndPunctToken=="")
    EndPunctuation={};
  else
    EndPunctuation=String2List(EndPunctToken,true);
  string CondEffectToken=GetDBValue(Rec,":condeffecttokens");
  if(CondEffectToken=="")
    CondEffectTokens={};
  else
    CondEffectTokens=String2List(CondEffectToken,true);
  string CondCauseToken=GetDBValue(Rec,":condcausetokens");
  if(CondCauseToken=="")
    CondCauseTokens={};
  else
    CondCauseTokens=String2List(CondCauseToken,true);
  string SubjData=GetDBValue(Rec,":subjarticles");
  if(SubjData=="")
    SubjectArticles={};
  else
    SubjectArticles=String2List(SubjData,true);
  string PredData=GetDBValue(Rec,":predarticles");
  if(PredData=="")
    PredicateArticles={};
  else
    PredicateArticles=String2List(PredData,true);
  string PassiveAgentsData=GetDBValue(Rec,":spa");
  if(PassiveAgentsData=="")
    PassiveAgents={};
  else
    PassiveAgents=String2List(PassiveAgentsData,true);
  string SubjectOrderStr=GetDBValue(Rec,":subjectorder");
  if(SubjectOrderStr=="")
    SubjectOrderList={};
  else
    SubjectOrderList=String2List(SubjectOrderStr,true);
  string VerbOrderStr=GetDBValue(Rec,":verborder");
  if(VerbOrderStr=="")
    VerbOrderList={};
  else
    VerbOrderList=String2List(VerbOrderStr,true);
  StartSentenceToken=GetDBValue(Rec,":startsentence");
  EndSentenceToken=GetDBValue(Rec,":endsentence");
  GrammarGraphs={};
  for(size_t _=0;_<MoodList.size();++_){
    LoadGrammar(MoodList[_]);  
    LoadTemplate(MoodList[_]);
  }

  LoadVerbForm(":activevf",ActiveVerbForms);
  LoadVerbForm(":passivevf",PassiveVerbForms);

}

string Memory::ExtractString(WMS_Rec InputRec,size_t Sector,size_t Position){
  if((InputRec.Contents.size()>Sector)&&((InputRec.Contents[Sector]).size()>Position))
    return (InputRec.Contents[Sector])[Position].Convert2Str();
  return "";
}

void Memory::RepInCache(WMS_Rec &InputRec){

  /*
  DEBUGMSG;
  WMSConversion::PrintWMSRec(InputRec);
  */

  for(size_t _=0;_<CachedContents.size();++_)
    if((CachedContents[_].Contents.size())&&(CachedContents[_].Contents[0].size()))
      if((InputRec.Contents.size())&&(InputRec.Contents[0].size()))
	if((InputRec.Contents[0])[0]==(CachedContents[_].Contents[0])[0]){
	  CachedContents[_]=InputRec;
	  return;
	}
  CachedContents.push_back(InputRec);
}

Memory::Memory(){
  GlobalMemoryAccess=this;
  
  //  UniqueUN=NameId();
  /*
  // This crashes probably because the network isn't setup
  size_t Pos=FindOrCreate(UniqueUN);
  WMS_Rec Rec=GetFromCache(UniqueUN);
  WMS2Mode(Rec);
  */
}

void Memory::SetupMode(){
  // Must be setup after the network!
  size_t Pos=FindOrCreate(UniqueUN);
  WMS_Rec Rec=GetFromCache(UniqueUN);
  WMS2Mode(Rec);
}

Memory::~Memory(){
  Mode2WMS();
}

WMS_Rec Memory::GetFromCache(string RecName){
  for(size_t _=0;_<CachedContents.size();_++)
    if(CachedContents[_].Contents.size())
      if((CachedContents[_].Contents)[0].size()){
	BinData RecordName={};
	RecordName.Change(RecName);
	if(RecordName==((CachedContents[_].Contents)[0])[0])
	  return CachedContents[_];
      }
  return {};
}

size_t Memory::FindInCache(string RecName){
  for(size_t _=0;_<CachedContents.size();_++)
    if(CachedContents[_].Contents.size())
      if((CachedContents[_].Contents)[0].size()){
	BinData RecordName={};
	RecordName.Change(RecName);
	if(RecordName==((CachedContents[_].Contents)[0])[0])
	  return _;
      }
  return -1;
}

vector<BinData> Memory::CreateNewHeader(string RecordName){  
  BinData NewRecordName(RecordName);
  BinData Uninitialised("`");
  Time TimeInstance;
  BinData TimeNow=TimeInstance.FormatTime2Bin();
  BinData Probability("1");
  vector<BinData> NewHeader={};
  NewHeader.push_back(RecordName);
  NewHeader.push_back(Uninitialised);
  NewHeader.push_back(Uninitialised);
  NewHeader.push_back(Uninitialised);
  NewHeader.push_back(TimeNow);
  NewHeader.push_back(TimeNow);
  NewHeader.push_back(Probability);
  NewHeader.push_back(Uninitialised);
  return NewHeader;  
}

vector<BinData> Memory::UpdateHeader(vector<BinData> OldHeader){
  Time TimeInstance;
  BinData TimeNow=TimeInstance.FormatTime2Bin();
  BinData Probability("1");
  OldHeader[5]=TimeNow;
  OldHeader[6]=Probability;
  return OldHeader;
}

size_t Memory::FindDBAttr(WMS_Rec InputRec,BinData Attr){
  for(size_t C=0;(C<InputRec.Contents[1].size())&&(C<InputRec.Contents[2].size());C++)
    if((InputRec.Contents[1])[C]==Attr)
      return C;
  return -1;
}

size_t Memory::FindDBAttr(WMS_Rec InputRec,string Attr){
  BinData AttrBin(Attr);
  return FindDBAttr(InputRec,AttrBin);
}

WMS_Rec Memory::DelDBPair(WMS_Rec InputRec,size_t Location){
  if(Location!=-1)
    if((InputRec.Contents[1].size()>Location)&&(InputRec.Contents[2].size()>Location)){
      (InputRec.Contents[1]).erase((InputRec.Contents[1]).begin()+Location);
      (InputRec.Contents[2]).erase((InputRec.Contents[2]).begin()+Location);
    }  
  return InputRec;
}

WMS_Rec Memory::DelDBPair(WMS_Rec InputRec,string Attr){
  BinData AttrBin(Attr);
  return DelDBPair(InputRec,AttrBin);  
}

WMS_Rec Memory::DelDBPair(WMS_Rec InputRec,BinData Attr){
  size_t Pos=0;
  Pos=FindDBAttr(InputRec,Attr);
  return DelDBPair(InputRec,Pos);
}

WMS_Rec Memory::RepHeader(WMS_Rec InputRec,BinData Value,int Location){
  if(InputRec.Contents.size()&&(InputRec.Contents[0]).size()>Location)
    (InputRec.Contents[0])[Location]=Value;
  return InputRec;
}

WMS_Rec Memory::RepHeader(WMS_Rec InputRec,string Value,int Location){
    BinData ValueBin(Value);
    return RepHeader(InputRec,ValueBin,Location);
}

vector<string> Memory::GetAllDBValues(WMS_Rec InputRec,BinData Attr){
  vector<string> FoundList;
  if(InputRec.Contents.size()>2){
    for(size_t C=0;(C<InputRec.Contents[1].size())&&(C<InputRec.Contents[2].size());C++)
      if((InputRec.Contents[1])[C]==Attr)
	FoundList.push_back((InputRec.Contents[2])[C].Convert2Str());
  }
  return FoundList;
}

vector<string> Memory::GetAllDBValues(WMS_Rec InputRec,string Attr){
  BinData AttrBin(Attr);
  return GetAllDBValues(InputRec,AttrBin);
}

string Memory::GetDBValue(WMS_Rec InputRec,BinData Attr){
  bool Found=false;
  size_t Location=0;
  
  for(size_t C=0;(C<InputRec.Contents[1].size())&&(C<InputRec.Contents[2].size());C++)
    if((InputRec.Contents[1])[C]==Attr){
      Found=true;
      Location=C;
      break;
    }

  if(Found)
    return (InputRec.Contents[2])[Location].Convert2Str();
  else
    return "";
}

string Memory::GetDBValue(WMS_Rec InputRec,string Attr){
  BinData AttrBin(Attr);
  return GetDBValue(InputRec,AttrBin);
}

vector<string> Memory::GetDBAttr(WMS_Rec InputRec,BinData Value){
  vector<string> Values;
  for(size_t C=0;(C<InputRec.Contents[1].size())&&(C<InputRec.Contents[2].size());C++)
    if((InputRec.Contents[2])[C]==Value)
      Values.push_back((InputRec.Contents[1])[C].Convert2Str());
  return Values;
}

vector<string> Memory::GetDBAttr(WMS_Rec InputRec,string Value){
  BinData ValueBin(Value);
  return GetDBAttr(InputRec,ValueBin);
}

bool Memory::IsDBPair(WMS_Rec InputRec,BinData Attr,BinData Value){
  if(InputRec.Contents.size()<3)
    InputRec.Contents.resize(3);
  for(size_t PairId=0;(PairId<InputRec.Contents[1].size())&&(PairId<InputRec.Contents[2].size());++PairId){
    if(((InputRec.Contents[1])[PairId]==Attr)&&(InputRec.Contents[2])[PairId]==Value){
      return true;
    }
  }
  return false;
}

bool Memory::IsDBPair(WMS_Rec InputRec,string Attr,string Value){
  BinData AttrBin(Attr);
  BinData ValueBin(Value);
  return IsDBPair(InputRec,AttrBin,ValueBin);
}

WMS_Rec Memory::AddDBPair(WMS_Rec InputRec,BinData Attr,BinData Value){
  if(InputRec.Contents.size()<3)
    InputRec.Contents.resize(3);
  if(IsDBPair(InputRec,Attr,Value))
    return InputRec;
  InputRec.Contents[1].push_back(Attr);
  InputRec.Contents[2].push_back(Value);
  return InputRec;
}

WMS_Rec Memory::AddDBPair(WMS_Rec InputRec,string Attr,string Value){
  BinData AttrBin(Attr);
  BinData ValueBin(Value);
  return AddDBPair(InputRec,AttrBin,ValueBin);
}

WMS_Rec Memory::RepDBPair(WMS_Rec InputRec,BinData Attr,BinData Value){
  bool Found=false;
  size_t Location=0;
  if(InputRec.Contents.size()<3)
    InputRec.Contents.resize(3);
  
  for(size_t C=0;(C<InputRec.Contents[1].size())&&(C<InputRec.Contents[2].size());C++){
    if((InputRec.Contents[1])[C]==Attr){
      Found=true;
      Location=C;
      break;
    }
    else{
    }
  }
  if(Found){
    (InputRec.Contents[2])[Location]=Value;
  }
  else{
    InputRec.Contents[1].push_back(Attr);
    InputRec.Contents[2].push_back(Value);
  }
  return InputRec;
}

WMS_Rec Memory::RepDBPair(WMS_Rec InputRec,string Attr,string Value){
  BinData AttrBin(Attr);
  BinData ValueBin(Value);
  return RepDBPair(InputRec,AttrBin,ValueBin);
}


void Memory::DebugLearnt(){
  Memory::DebugLearnt(CachedContents);
}

void Memory::DebugLearnt(vector<WMS_Rec> &ToWrite){
  for(size_t _=0;_<ToWrite.size();_++)
    WMSConversion::PrintWMSRec(ToWrite[_]);
}


Packet Memory::AmoebaRun(string Command){
  vector<BinData> TXStrings={};
  BinData FindCmdBin(Command);
  TXStrings.push_back(FindCmdBin);
  Packet Outgoing(TXStrings,{});
  AmoebaConnection->Send(Outgoing.AllData);
  Packet Retrieved(AmoebaConnection->Receive());  
  return Retrieved;
}

string Memory::GetStrResp(Packet Resp){
  if(Resp.Strings.size()>1)
    return Resp.Strings[1].Convert2Str();    
  return {};
}

void Memory::DelAmoebaRec(string ToDelete){
  string Position=ExtractFromServer("FIND "+ToDelete,"RESULT FOUND @: ",1);
  if((Position!="")&&(Position!="-1"))
    AmoebaRun("DELETE "+Position);
}

void Memory::Write2Amoeba(WMS_Rec &ToWrite){
  WMSConversion Conversion;
  vector<BinData> TXBinaries={};
  Time TimeInstance;
  BinData TimeNow=TimeInstance.FormatTime2Bin();
  size_t Pos=FindOrCreate(UniqueUN);    
  BinData ConvertedRecord=Conversion.WMS2Binary(ToWrite);
  TXBinaries.push_back(ConvertedRecord);
  if(TXBinaries.size()){
    BinData CommandBin("ADD");
    vector<BinData> TXStrings={};
    TXStrings.push_back(CommandBin);
    Packet Outgoing(TXStrings,TXBinaries);
    AmoebaConnection->Send(Outgoing.AllData);
    Packet Incoming(AmoebaConnection->Receive());
  }
}

void Memory::Write2Amoeba(vector<WMS_Rec> &ToWrite){
  size_t InputSize=ToWrite.size(),C=0;
  if(InputSize){
    WMSConversion Conversion;
    vector<BinData> TXBinaries={};
    Time TimeInstance;
    BinData TimeNow=TimeInstance.FormatTime2Bin();
    size_t Pos=FindOrCreate(UniqueUN);
    for(C=0;C<InputSize;C++)
      if(ToWrite[C].Accessed){
	ToWrite[C]=RepHeader(ToWrite[C],"1",6);
	ToWrite[C]=RepHeader(ToWrite[C],TimeNow,5);
	BinData ConvertedRecord=Conversion.WMS2Binary(ToWrite[C]);
	TXBinaries.push_back(ConvertedRecord);
      }
    if(TXBinaries.size()){
      BinData CommandBin("ADD");
      vector<BinData> TXStrings={};
      TXStrings.push_back(CommandBin);
      Packet Outgoing(TXStrings,TXBinaries);
      AmoebaConnection->Send(Outgoing.AllData);
      Packet Incoming(AmoebaConnection->Receive());
    }
  }
}

void Memory::LearnWords(vector<string> Input){
  size_t _=0,Size=Input.size();
  
  RecordList={};
  
  // we need to pull out the name of the records
  
  // DivideBySpaces()
  for(_=0;_<Size;++_)
    if(((Input[_].c_str())[0]!=':')&&(BinData::IsNumber(Input[_])==false))
      FindOrCreate(Input[_]);
}

long int Memory::FindOrCreate(string RecName,bool Access){
  size_t Size=CachedContents.size(),_=0;
  WMSConversion Conversion;
  for(_=0;_<Size;++_)
    if(CachedContents[_].Contents.size())
      if((CachedContents[_].Contents)[0].size()){
	BinData RecordName={};
	RecordName.Change(RecName);
	if(RecordName==((CachedContents[_].Contents)[0])[0]){
	  return _;
	}
      }
  string CommandStr="FIND "+RecName;
  string Response=ExtractFromServer(CommandStr,"RESULT FOUND @: ",1);
  WMS_Rec ConvertedWMS={};
  if(Response!=""){
    long int Position=atol(Response.c_str());    
    if(Position>-1){
      string GetDataCommand="GETDATA "+Response;
      BinData WMSBinary=ExtractFromServer(GetDataCommand,0);
      if(WMSBinary.Size){
	WMS_Rec ConvertedWMS=Conversion.Binary2WMS(WMSBinary);
	if((ConvertedWMS.Contents.size())&&(ConvertedWMS.Contents[0].size()>7)){
	  // this is a fully populated record, rather than an attribute!
	  ConvertedWMS.Accessed=Access;
	  CachedContents.push_back(ConvertedWMS);
	  return Size;
	}
      }
    }
  }
  /* Else we create them and cache them */  
  vector<BinData> Header=CreateNewHeader(RecName);
  vector<BinData> Attr={};
  vector<BinData> Values={};
  ConvertedWMS.Contents.push_back(Header);
  ConvertedWMS.Contents.push_back(Attr);
  ConvertedWMS.Contents.push_back(Values);
  ConvertedWMS.Accessed=Access;
  CachedContents.push_back(ConvertedWMS);

  RecordList=SetTheoryAdd(RecordList,RecName);
  AllRecords=SetTheoryAdd(AllRecords,RecName);

  return Size;
}

BinData Memory::ExtractFromServer(string Command,int ProtocolBinId){
    BinData CommandBin={};
    CommandBin.Change(Command);

    vector<BinData> TXStrings={};
    vector<BinData> TXBinaries={};

    TXStrings.push_back(CommandBin);

    Packet Outgoing(TXStrings,TXBinaries);
    /*
      puts("Sent:");
      Outgoing.Print();
    */
    AmoebaConnection->Send(Outgoing.AllData);

    Packet Incoming(AmoebaConnection->Receive());
    /*
      puts("Received:");
      Incoming.Print();
    */
    if(Incoming.Binaries.size()>ProtocolBinId)
      return Incoming.Binaries[ProtocolBinId];

    return {};
}

string Memory::ExtractFromServer(string Command,string RespPrefix,int ProtocolMsgId){  
    BinData CommandBin={};
    CommandBin.Change(Command);

    size_t PrefixLen=RespPrefix.size();
    vector<BinData> TXStrings={};
    vector<BinData> TXBinaries={};
    char *ReceivedData=NULL;
  
    TXStrings.push_back(CommandBin);

    Packet Outgoing(TXStrings,TXBinaries);

    /*
    puts("Sent:");
    Outgoing.Print();
    */

    AmoebaConnection->Send(Outgoing.AllData);

    Packet Incoming(AmoebaConnection->Receive());

    /*
    puts("Received:");
    Incoming.Print();
    */

    if(Incoming.Strings.size()>=ProtocolMsgId)
      if((Incoming.Strings[ProtocolMsgId].Size>PrefixLen)&&(!memcmp(Incoming.Strings[ProtocolMsgId].Str,RespPrefix.c_str(),PrefixLen)))
	ReceivedData=(Incoming.Strings[ProtocolMsgId].Str)+PrefixLen;
    
    if(ReceivedData&&(!!strlen(ReceivedData)))
      return string(ReceivedData);

    return "";
}

void Memory::MultiplexClear(string ConceptName){
  // lets cache the ConceptName
  FindOrCreate(ConceptName);
  WMS_Rec ConceptRec=GetFromCache(ConceptName);

  // Remove neural and cause / effect symbols
  ConceptRec=DelDBPair(ConceptRec,":NEURALNET");
  ConceptRec=DelDBPair(ConceptRec,":CAUSELIST");
  ConceptRec=DelDBPair(ConceptRec,":EFFECTLIST");

  // Remove the cause / effect pairings
  if(ConceptRec.Contents.size()>1){
    for(size_t AttrPos=0;AttrPos<(ConceptRec.Contents[1]).size();++AttrPos){
      vector<string> AttrNames=BinList2ListStr(ConceptRec.Contents[1]);
      if(List::SetTheoryIsElem(List::String2List(AttrNames[AttrPos]),":CAUSE")){
	ConceptRec=DelDBPair(ConceptRec,AttrPos);
	--AttrPos;
	continue;
      }
    }
  }
  RepInCache(ConceptRec);  
}

void Memory::MultiplexAdd(string ConceptName,vector<string> Cause,vector<string> Effect){
  // lets cache the ConceptName
  FindOrCreate(ConceptName);
  WMS_Rec ConceptRec=GetFromCache(ConceptName);
  Cause=List::SortList(Cause);
  Effect=List::SortList(Effect);

  // We get the list of causes, :CAUSELIST
  vector<string> CauseList=List::String2List(GetDBValue(ConceptRec,":CAUSELIST"));
  
  // We get the list of effects, :EFFECTLIST
  vector<string> EffectList=List::String2List(GetDBValue(ConceptRec,":EFFECTLIST"));
  
  // Need to combine the cause and effect list
  CauseList=List::SortList(List::SetTheoryUnion(CauseList,Cause));
  EffectList=List::SortList(List::SetTheoryUnion(EffectList,Effect));
  
  // We update the cause and effect lists
  ConceptRec=RepDBPair(ConceptRec,":CAUSELIST",List::List2String(CauseList));
  ConceptRec=RepDBPair(ConceptRec,":EFFECTLIST",List::List2String(EffectList));
  
  // Then we add the cause effect pairing
  Cause.insert(Cause.begin(),":CAUSE");
  Effect.insert(Effect.begin(),":EFFECT");
  ConceptRec=RepDBPair(ConceptRec,List::List2String(Cause),List::List2String(Effect));
  
  RepInCache(ConceptRec);  
}

vector<int> Memory::GenerateCodex(size_t Size){
  vector<int> Codex(Size);
  int Ref=1;
  generate(Codex.begin(),Codex.end(),[&Ref]{int OldRef=Ref; Ref=(Ref<<1); return OldRef;});
  return Codex;
}

vector<string> Memory::MultiplexGen(string ConceptName){
  FindOrCreate(ConceptName);
  WMS_Rec ConceptRec=GetFromCache(ConceptName);
  vector<string> CauseSymbols=List::String2List(GetDBValue(ConceptRec,":CAUSELIST"));
  vector<string> EffectSymbols=List::String2List(GetDBValue(ConceptRec,":EFFECTLIST"));
  vector<int> Codex=GenerateCodex(CauseSymbols.size()>EffectSymbols.size()?CauseSymbols.size():EffectSymbols.size());
  vector<string> MultiplexedSeq(1<<CauseSymbols.size());
  fill(MultiplexedSeq.begin(),MultiplexedSeq.end(),"?");
  if(ConceptRec.Contents.size()>2){
    vector<string> AttrNames=BinList2ListStr(ConceptRec.Contents[1]);
    vector<string> AttrValues=BinList2ListStr(ConceptRec.Contents[2]);
    for(size_t AttrPos=0;AttrPos<(ConceptRec.Contents[1]).size();++AttrPos){
      vector<string> Cause=List::String2List(AttrNames[AttrPos]);
      vector<string> Effect=List::String2List(AttrValues[AttrPos]);
      if(List::SetTheoryIsElem(Cause,":CAUSE")){
	Cause.erase(Cause.begin());   // Remove :CAUSE marker token
	Effect.erase(Effect.begin()); // Remove :EFFECT marker token
	int EncodedEffect=0;
	int TPI=0;
	for(size_t _=0;_<Cause.size();++_){
	  int Pos=FindFirst(CauseSymbols,Cause[_]);
	  if(Pos!=-1)
	    TPI+=Codex[Pos];
	}
	for(size_t _=0;_<Effect.size();++_){
	  int Pos=FindFirst(EffectSymbols,Effect[_]);
	  if(Pos!=-1)
	    EncodedEffect+=Codex[Pos];
	}
	MultiplexedSeq[TPI]=BinData::Int2String(EncodedEffect);
      }
    }
  }
  return MultiplexedSeq;
}

int Memory::MultiplexEncode(string ConceptName,vector<string> Scenario,CauseOrEffect Switch){
  FindOrCreate(ConceptName);
  WMS_Rec ConceptRec=GetFromCache(ConceptName);
  vector<string> ScenarioSymbols=(Switch==CauseOrEffect::Cause)?List::String2List(GetDBValue(ConceptRec,":CAUSELIST")):List::String2List(GetDBValue(ConceptRec,":EFFECTLIST"));
  vector<int> Codex=GenerateCodex(ScenarioSymbols.size());
  int Encoded=0;
  for(size_t _=0;_<Scenario.size();++_){
    int Pos=FindFirst(ScenarioSymbols,Scenario[_]);
    if(Pos!=-1)
      Encoded+=Codex[Pos];
  }
  return Encoded;
}

vector<string> Memory::MultiplexDecode(string ConceptName,int Scenario,CauseOrEffect Switch){
  FindOrCreate(ConceptName);
  WMS_Rec ConceptRec=GetFromCache(ConceptName);
  vector<string> ScenarioSymbols=(Switch==CauseOrEffect::Cause)?List::String2List(GetDBValue(ConceptRec,":CAUSELIST")):List::String2List(GetDBValue(ConceptRec,":EFFECTLIST"));
  vector<int> Codex=GenerateCodex(ScenarioSymbols.size());
  vector<string> Decoded;
  for(size_t _=0;Scenario>>_;++_){
    int Pos=FindFirst(Codex,(Scenario&(1<<_)));
    if(Pos!=-1)
      Decoded.push_back(ScenarioSymbols[_]);
  }
  return Decoded;
}

void Memory::MultiplexRemove(string ConceptName,vector<string> Cause){
  // lets cache the ConceptName
  FindOrCreate(ConceptName);
  WMS_Rec ConceptRec=GetFromCache(ConceptName);

  // Prepare the cause string
  Cause=List::SortList(Cause);
  Cause.insert(Cause.begin(),":CAUSE");
  string CauseStr=List::List2String(Cause);

  // Remove the cause / effect pairings
  if(ConceptRec.Contents.size()>1){
    for(size_t AttrPos=0;AttrPos<(ConceptRec.Contents[1]).size();++AttrPos){
      vector<string> AttrNames=BinList2ListStr(ConceptRec.Contents[1]);
      if(AttrNames[AttrPos]==CauseStr){
	ConceptRec=DelDBPair(ConceptRec,AttrPos);
	--AttrPos;
	continue;
      }
    }
  }
  
  RepInCache(ConceptRec);  
}

vector<int> Memory::CreateDomain(string ConceptName,CauseOrEffect Switch){
  FindOrCreate(ConceptName);
  WMS_Rec ConceptRec=GetFromCache(ConceptName);
  vector<string> ScenarioSymbols=(Switch==CauseOrEffect::Cause)?List::String2List(GetDBValue(ConceptRec,":CAUSELIST")):List::String2List(GetDBValue(ConceptRec,":EFFECTLIST"));
  vector<int> Domain(1<<ScenarioSymbols.size());
  int Ref=0;
  generate(Domain.begin(),Domain.end(),[&Ref]{ int OldRef=Ref; ++Ref; return OldRef;});
  return Domain;
}

string Memory::DisplayNeuralSymbols(string ConceptName){
  FindOrCreate(ConceptName);
  WMS_Rec ConceptRec=GetFromCache(ConceptName);  
  vector<string> CauseSymbols=List::String2List(GetDBValue(ConceptRec,":CAUSELIST"));
  vector<string> EffectSymbols=List::String2List(GetDBValue(ConceptRec,":EFFECTLIST"));
  ostringstream ToConvert;
  if(CauseSymbols.size()){
    ToConvert<<"-[Causes]--------------------\n";
    for(int _=0;_<CauseSymbols.size();_++)
      ToConvert<<_<<" "<<CauseSymbols[_]<<"\n";
  }
  if(EffectSymbols.size()){
    ToConvert<<"-[Effects]-------------------\n";
    for(int _=0;_<EffectSymbols.size();_++)
      ToConvert<<_<<" "<<EffectSymbols[_]<<"\n";
  }
  return ToConvert.str();
}

void Memory::SetTense(string RecName,string Tense){
  FindOrCreate(RecName);
  WMS_Rec Rec=GetFromCache(RecName);
  Rec=RepHeader(Rec,Tense,2);
  RepInCache(Rec);  
}

vector<string> Memory::CreateTense(vector<string> Adverbs,string Verb,Sentence::TenseType VerbTense){
  vector<string> CompleteTense;
  if(VerbTense==Sentence::TenseType::Past){
    CompleteTense={"was"};
  }
  if(VerbTense==Sentence::TenseType::Present){
    CompleteTense={"is"};
  }
  if(VerbTense==Sentence::TenseType::Future){
    CompleteTense={"will","be"};
  }
  vector<string> SortedAdverbs=List::SortList(Adverbs);
  CompleteTense=List::VecAdd(CompleteTense,SortedAdverbs);
  FindOrCreate(Verb);
  WMS_Rec VerbRec=GetFromCache(Verb);
  string BaseTenseVerb=GetDBValue(VerbRec,":baseverb");
  if(BaseTenseVerb=="")
    BaseTenseVerb=Verb;
  CompleteTense.push_back(BaseTenseVerb);  
  return CompleteTense;
}

vector<string> Memory::CreateSentence(string CSSubj,vector<string> CreatedVerb,string CSPred,Sentence::SentenceVoiceType VoiceType){
  vector<string> CompleteSentence;
  vector<string> SubjectComponents=String2List(CSSubj);  
  vector<string> PredComponents=String2List(CSPred);
  if(SubjectComponents.size()||PredComponents.size()){
    string SubjLogic=SubjectComponents[0];
    string PredLogic=PredComponents[0];    
    for(size_t _=1;_<SubjectComponents.size();++_){
      string CSSubjItem=SubjectComponents[_];
      vector<string> SubjDeref=CSDeref(CSSubjItem);
      if(_>1){
	if(_<SubjectComponents.size()-2)
	  CompleteSentence.push_back(",");
	else{
	  if(SubjectComponents.size()>1)
	    CompleteSentence.push_back(SubjLogic);
	}
      }
      CompleteSentence=List::VecAdd(CompleteSentence,SubjDeref);
    }
    CompleteSentence=List::VecAdd(CompleteSentence,CreatedVerb);
    for(size_t _=0;_<PredComponents.size();++_){
      string CSPredItem=PredComponents[_];
      vector<string> PredDeref=CSDeref(CSPredItem);
      if(_>1){
	if(_<PredComponents.size()-2)
	  CompleteSentence.push_back(",");
	else{
	  if(PredComponents.size()>1)
	    CompleteSentence.push_back(PredLogic);
	}
      }
      CompleteSentence=List::VecAdd(CompleteSentence,PredDeref);
    }
  }
  return CompleteSentence;
}

vector<string> Memory::GetSentenceSymbols(){

  /*
    Phrase Subject;
    Phrase Predicate;
    Phrase PredicateVerbs;
    Phrase PredicateNouns;

    .CSSubPhrases
  */  

  Subject.DebugPhrase();
  PredicateVerbs.DebugPhrase();
  PredicateNouns.DebugPhrase();
  
  vector<string> Data;
  // verb

  Data.push_back(PredicateVerbs.CSVerb);
  
  // subject components

  // subject logic

  Data=VecAdd(Data,Subject.CSSubPhrases);
  Data.push_back(Subject.LogicType);
    
  
  // predicate noun components

  // predicate logic
  Data=VecAdd(Data,PredicateNouns.CSSubPhrases);
  Data.push_back(PredicateNouns.LogicType);

  List::PrintList(Data);
  
  return Data;
}

vector<string> Memory::GetCSLocationList(Sentence &Input,vector<string> &SentenceConditions){
  vector<string> AddressContext;
  AddressContext.push_back(Input.PredicateVerbs.CSVerb);
  
  vector<string> SubjectContext;
  SubjectContext.push_back(Input.Subject.LogicType);  
  vector<string> CSSymbols=SortList(Input.Subject.CSSubPhrases); 
  SubjectContext=List::VecAdd(SubjectContext,CSSymbols);
  string SubjectRef=CSRef(SubjectContext);
  
  vector<string> PredicateContext;
  PredicateContext.push_back(Input.PredicateNouns.LogicType);
  CSSymbols=SortList(Input.PredicateNouns.CSSubPhrases); 
  PredicateContext=List::VecAdd(PredicateContext,CSSymbols);
  string PredicateRef=CSRef(PredicateContext);
  
  AddressContext.push_back(SubjectRef);
  AddressContext.push_back(PredicateRef);

  string CauseOrEffect=CSRef(AddressContext);
  
  SentenceConditions.push_back(CauseOrEffect);
  return SentenceConditions;
}

bool Memory::AskYNConditional(string &Cause,string &Effect){
  
  DEBUGMSG;
  
  string DBRecName=":ConditionalData";
  FindOrCreate(DBRecName);
  WMS_Rec Rec=GetFromCache(DBRecName);

  
  DEBUGMSG;
  printf("AskYNC Cause: [%s],Effect: [%s]\n",Cause.c_str(),Effect.c_str());
  WMSConversion::PrintWMSRec(Rec);
  
  
  vector<string> Causes=CSDeref(Cause);
  vector<string> Effects=CSDeref(Effect);
  
  if((!Causes.size())||(!Effects.size())){
    DEBUGMSG;
    printf("ERROR: context addresses of cause / effect are in invalid format [%s][%s], should start with a logic operand.\n",Cause.c_str(),Effect.c_str());
    return false;
  }
  
  NLPStoreLogic::SLogic CauseLogic=NLPStoreLogic::GetLogicCode(Causes[0]);
  NLPStoreLogic::SLogic EffectLogic=NLPStoreLogic::GetLogicCode(Effects[0]);  
  Causes.erase(Causes.begin());
  Effects.erase(Effects.begin());

  // should we go straight to neural computation ? 
  
  if(Rec.Contents.size()>2){

    vector<size_t> ToMultiplex={};
    
    for(size_t Pos=0;(Pos<Rec.Contents[2].size())&&(Pos<Rec.Contents[1].size());++Pos){

      string RecCause=(Rec.Contents[1])[Pos].Convert2Str();
      string RecEffect=(Rec.Contents[2])[Pos].Convert2Str();
      
      bool RelevantCause=Semantics::ConditionalOverlap(CauseLogic,Causes,RecCause);
      bool RelevantEffect=Semantics::ConditionalOverlap(EffectLogic,Effects,RecEffect);

      if(RelevantCause){
	DEBUGMSG;
	printf("Cause [%zu] is relevant.\n",Pos);
      }
      if(RelevantEffect){
	DEBUGMSG;
	printf("Effect [%zu] is relevant.\n",Pos);
      }

      if(RelevantCause&&RelevantEffect)
	ToMultiplex.push_back(Pos);
    }

    // collect the data.
    vector<string> CauseSymbols;
    vector<string> EffectSymbols;
    for(size_t Pos=0;Pos<ToMultiplex.size();++Pos){
      vector<string> RecCauses=CSDeref((Rec.Contents[1])[Pos].Convert2Str());
      vector<string> RecEffects=CSDeref((Rec.Contents[2])[Pos].Convert2Str());
      if((RecCauses.size()>1)&&(RecEffects.size()>1)){
	RecCauses.erase(RecCauses.begin());
	RecEffects.erase(RecEffects.begin());
	if(RecCauses.size())
	  CauseSymbols=List::SetTheoryUnion(CauseSymbols,RecCauses);
	if(RecEffects.size())
	  EffectSymbols=List::SetTheoryUnion(EffectSymbols,RecEffects);
      }
    }
    CauseSymbols=List::SortList(CauseSymbols);
    EffectSymbols=List::SortList(EffectSymbols);
    
    // multiplex the data.
    size_t Max=CauseSymbols.size()>EffectSymbols.size()?CauseSymbols.size():EffectSymbols.size();
    int DoubleMe=1;
    vector<int> MultiplexIndices;
    for(size_t Pos=0;Pos<Max+1;++Pos){
      MultiplexIndices.push_back(DoubleMe);
      DoubleMe=DoubleMe<<1;
    }

    vector<string> Multiplexed;
    for(size_t Pos=0;Pos<(1<<(CauseSymbols.size()));++Pos)
      Multiplexed.push_back("?");
    
    for(size_t Pos=0;Pos<ToMultiplex.size();++Pos){
      vector<string> RecCauses=CSDeref((Rec.Contents[1])[Pos].Convert2Str());
      vector<string> RecEffects=CSDeref((Rec.Contents[2])[Pos].Convert2Str());
      if((RecCauses.size()>1)&&(RecEffects.size()>1)){
	RecCauses.erase(RecCauses.begin());
	RecEffects.erase(RecEffects.begin());

	int TPI=0;
	int Value=0;
	
	for(size_t CausePos=0;CausePos<RecCauses.size();++CausePos){
	  TPI|=List::FindFirst(CauseSymbols,RecCauses[CausePos]);
	}
	for(size_t EffectPos=0;EffectPos<RecEffects.size();++EffectPos){
	  Value|=List::FindFirst(EffectSymbols,RecEffects[EffectPos]);
	}
	Multiplexed[TPI]=BinData::Int2String(Value);
      }
    }

    DEBUGMSG;
    List::PrintList(Multiplexed);
    
    // learn the sequence (Look at Memory::AmoebaRun and adapt it)
    string LearntResp=LearnNN(Multiplexed);
    DEBUGMSG;
    printf("%s\n",LearntResp.c_str());
    
   

    
    // using the original question, determine if that pairing occurs in the completed sequence

    // if it does then return true

    // else if it does not then return false
    
  }
  return false;
}

vector<string> Memory::AskWHQConditional(string &Cause,string &Effect){

  DEBUGMSG;
  printf("Cause: [%s], Effect: [%s]\n",Cause.c_str(),Effect.c_str());

  
  string DBRecName=":ConditionalData";
  FindOrCreate(DBRecName);
  WMS_Rec Rec=GetFromCache(DBRecName);


  
  return {};
  
  /*
  WMS_Rec Rec=GetFromCache(DBRecName);

  DEBUGMSG;
  WMSConversion::PrintWMSRec(Rec);

  // Are we storing the data in a larger CS unit?
  
  string AnswerText;
  if(Cause.size()){
    
    // calculate the effect
    AnswerText=GetDBValue(Rec,Cause);

    DEBUGMSG;
    printf("Answer: [%s]\n",AnswerText.c_str());
    
    if(AnswerText.size()){
      DEBUGMSG;
      vector<string> TmpAnswer=List::String2List(AnswerText);
      return TmpAnswer;
    }
    else{
      // if no answer then multiplex
      
    }
  }
  else{
    // calculate the cause
    vector<string> AnswerList=GetDBAttr(Rec,Effect);
    if(AnswerList.size()){
      DEBUGMSG;
      return AnswerList;
    }
    else{
      // if no answer then multiplex
      
    }
  }
  return {};
  */
}

void Memory::StoreConditional(string &Cause,string &Effect){
  string DBRecName=":ConditionalData";
  FindOrCreate(DBRecName);
  WMS_Rec Rec=GetFromCache(DBRecName);
  Rec=AddDBPair(Rec,Cause,Effect);  
  RepInCache(Rec);
}

/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
*/
