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

#include "ENiX_Settings.h"
#include "ENiX_Sentence.h"

Phrase::Phrase(){
  /*
  CanonicalVerbs=NULL;
  */
}

Phrase::~Phrase(){
  /*
  if(CanonicalVerbs)
    delete CanonicalVerbs;
  */
}

void Sentence::AddPreparedSymbols(vector<string> &S,vector<string> &PV,vector<string> &PN){
  tuple<vector<string>,vector<string>,vector<string>> SentenceData=(tuple<vector<string>,vector<string>,vector<string>>){S,PV,PN};
  PreparedSentenceSymbols.push_back(SentenceData);
}

Sentence *Sentence::GetSentence(){
  return this;
}

bool Sentence::IsSentenceValid(){
  if((SentenceVoiceFound!=Sentence::SentenceVoiceType::Unknown)&&(SentenceMoodFound!=""))
    return true;
  return false;
}

void Sentence::SetSentenceInvalid(){
  string SentenceMoodFound="";
  SentenceVoiceType SentenceVoiceFound=Sentence::SentenceVoiceType::Unknown;
}

void Sentence::PrepareSymbols(){
  bool SeparateS=false;
  bool SeparatePN=false;
  string SubjectLogic;
  string PredicateLogic;
  PreparedSentenceSymbols={};
  SubjectLogic=Subject.LogicType;
  if((SubjectLogic=="OR")||(SubjectLogic=="NOR")){
    SeparateS=true;
  }
  PredicateLogic=PredicateNouns.LogicType;
  if(PredicateLogic=="AND"){
    SeparatePN=true;
  }  
  vector<string> Verb={PredicateVerbs.CSVerb};

  // TODO: the subject needs to be prefixed with the logic!
  // same with the predicate!

  for(size_t SubjNum=0;SubjNum<Subject.CSSubPhrases.size();++SubjNum){
    vector<string> SubjectSymbols={};
    if(SeparateS){
      SubjectSymbols={Subject.CSSubPhrases[SubjNum]};
    }
    else{
      SubjectSymbols=Subject.CSSubPhrases;
      SubjNum=Subject.CSSubPhrases.size();
    }
    SubjectSymbols.insert(SubjectSymbols.begin(),Subject.LogicType);

    for(size_t PredNum=0;PredNum<PredicateNouns.CSSubPhrases.size();++PredNum){
      vector<string> PredicateSymbols={};
      if(SeparatePN){
	PredicateSymbols={PredicateNouns.CSSubPhrases[SubjNum]};
      }
      else{
	PredicateSymbols=PredicateNouns.CSSubPhrases;
	PredNum=Subject.CSSubPhrases.size();
      }
      PredicateSymbols.insert(PredicateSymbols.begin(),PredicateNouns.LogicType);
      tuple<vector<string>,vector<string>,vector<string>> SplitSentence=tuple<vector<string>,vector<string>,vector<string>>{SubjectSymbols,Verb,PredicateSymbols};
      PreparedSentenceSymbols.push_back(SplitSentence);
    }
  }
}

void Phrase::DebugPhrase(){
  printf("Words:                 "); List::PrintList(Words);
  printf("Articles:              "); List::PrintList(Articles);
  
  if(CanonicalVerbs.size()){
    printf("CanonicalVerbs:        "); List::PrintList(CanonicalVerbs[0].Words);
  }
  
  for(size_t _=0;_<SubPhrases.size();++_){
    printf("SubPhrases %d:          ",_); List::PrintList(SubPhrases[_].Words);
  }
  for(size_t _=0;_<CanonicalSubPhrases.size();++_){
    printf("CanonicalSubPhrases %d: ",_); List::PrintList(CanonicalSubPhrases[_].Words);
  }
}

vector<Phrase> Phrase::PrepCanonicalVerbs(vector<string> ArticleOrder,vector<string> FullTense,string BaseTense){
  bool FoundVerb=false;
  
  CanonicalVerbs={};
  Phrase CanonicalVerbP={};
  // first thing we do is remove the fulltense from the phrase 
  
  for(size_t WordId=0;WordId<Size();++WordId)
    if(!List::SetTheoryIsElem(FullTense,Words[WordId]))
      CanonicalVerbP.push_back(Words[WordId],Articles[WordId]);

  // if the phrase has no verbs left in it, then we add "is"
  for(size_t WordId=0;WordId<CanonicalVerbP.Size();++WordId){
    if(CanonicalVerbP.Articles[WordId]=="verb"){
      FoundVerb=true;
      break;
    }
  }
  if(!FoundVerb){
    CanonicalVerbP.push_back("is","verb");
  }

  // then we order the phrase
  CanonicalVerbP=CanonicalVerbP.CanonicalExp(ArticleOrder);

  // then we append the tense onto the end
  CanonicalVerbP.push_back(BaseTense,":TENSE");

  CanonicalVerbs.push_back(CanonicalVerbP);

  return CanonicalVerbs;
}

void Sentence::GetBaseTense(){
  if(NameofTense.size()){
    vector<string> BaseTenses={"past","present","future"};
    vector<string> Filtered=List::SetTheoryIntersect(BaseTenses,NameofTense);
    BaseTense=List::List2String(Filtered);
  }
}

Phrase Phrase::CanonicalExp(Phrase &SubPhrase,vector<string> &Articles){
  Phrase Arranged;
  for(size_t _=0;_<Articles.size();++_){
    Phrase WordsMatching;
    for(size_t PhrasePos=0;PhrasePos<SubPhrase.Size();++PhrasePos){
      if(Articles[_]==SubPhrase.Articles[PhrasePos])
	WordsMatching.push_back(SubPhrase.Words[PhrasePos],SubPhrase.Articles[PhrasePos]);
    }
    if(WordsMatching.Size()){
      WordsMatching=Sort(WordsMatching);
      Arranged=Append(Arranged,WordsMatching);
    }
  }
  return Arranged;
}

Phrase Phrase::CanonicalExp(vector<string> &Articles){
  return CanonicalExp(*this,Articles);
}

void Phrase::PrintSubphrases(){
  for(size_t _=0;_<SubPhrases.size();++_)
    List::PrintList(SubPhrases[_].Words);
}

vector<Phrase> Phrase::PrepCanonicalPhrase(vector<string> ArticleOrder){
  vector<Phrase> Canonical;  
  for(size_t _=0;_<SubPhrases.size();++_)
    Canonical.push_back(SubPhrases[_].CanonicalExp(ArticleOrder));
  CanonicalSubPhrases=Canonical;
  return Canonical;
}

vector<Phrase> Phrase::SplitSubject(Phrase OriginalPhrase,vector<string> LogicList){
  bool Separator=true;
  bool PreLogic=false;
  vector<Phrase> Split;
  size_t MinimumSize=OriginalPhrase.Size();
  Phrase CurrentPhrase;
  for(size_t _=0;_<MinimumSize;++_){
    if(List::SetTheoryIsElem(LogicList,OriginalPhrase.Words[_])){
      LogicArticles.push_back(OriginalPhrase.Words[_]);
      if((Separator==false)&&(CurrentPhrase.Size())){
	Split.push_back(CurrentPhrase);
	CurrentPhrase.Words={};
	CurrentPhrase.Articles={};
      }
      Separator=true;
    }
    else{
      CurrentPhrase.push_back(OriginalPhrase.Words[_],OriginalPhrase.Articles[_]);
      Separator=false;
    }
  }
  if((Separator==false)&&(CurrentPhrase.Size())){
    Split.push_back(CurrentPhrase);
    CurrentPhrase.Words={};
    CurrentPhrase.Articles={};
  }

  // LogicType
  LogicType="OR";
  if(List::SetTheoryIsElem(LogicArticles,"and")){
    LogicType="AND";
  }
  if(List::SetTheoryIsElem(LogicArticles,"either")){
    LogicType="XOR";
    PreLogic=true;
  }
  if(List::SetTheoryIsElem(LogicArticles,"neither")){
    LogicType="NOR";
    PreLogic=true;
  }
  if(List::SetTheoryIsElem(LogicArticles,"or")&&(!PreLogic)){
    LogicType="OR";
  }

  SubPhrases=Split;
  return Split;
}

Phrase Phrase::Append(Phrase Base,Phrase Extension){
  Phrase Combined=Base;
  size_t MinimumSize=Extension.Size();
  for(size_t _=0;_<MinimumSize;++_){
    Combined.Words.push_back(Extension.Words[_]);
    Combined.Articles.push_back(Extension.Articles[_]);
  }
  return Combined;
}

Phrase Phrase::Filter(Phrase OriginalPhrase,vector<string> Articles){
  size_t MinimumSize=OriginalPhrase.Size();
  Phrase Filtered;
  for(size_t _=0;_<MinimumSize;++_){
    if(List::SetTheoryIsElem(Articles,OriginalPhrase.Articles[_])){
      Filtered.Words.push_back(OriginalPhrase.Words[_]);
      Filtered.Articles.push_back(OriginalPhrase.Articles[_]);
    }
  }
  return Filtered;
}

Phrase Phrase::Filter(Phrase OriginalPhrase,string Article){
  size_t MinimumSize=OriginalPhrase.Size();
  Phrase Filtered;
  for(size_t _=0;_<MinimumSize;++_){
    if(OriginalPhrase.Articles[_]==Article){
      Filtered.Words.push_back(OriginalPhrase.Words[_]);
      Filtered.Articles.push_back(OriginalPhrase.Articles[_]);
    }
  }
  return Filtered;
}

Phrase Phrase::Sort(Phrase OriginalPhrase){
  vector<pair<string,string>> FormattedPhrase=ConvertPhrase(OriginalPhrase);
  std::sort(FormattedPhrase.begin(),FormattedPhrase.end(),Phrase::LessThan);
  return ConvertPhrase(FormattedPhrase);
}

vector<pair<string,string>> Phrase::ConvertPhrase(Phrase OriginalPhrase){
  vector<pair<string,string>> Converted;
  size_t MinimumSize=OriginalPhrase.Size();
  for(size_t _=0;_<MinimumSize;++_)
    Converted.push_back(make_pair(OriginalPhrase.Words[_],OriginalPhrase.Articles[_]));
  return Converted;
}

Phrase Phrase::ConvertPhrase(vector<pair<string,string>> OriginalPhrase){
  Phrase Converted;
  for(size_t _=0;_<OriginalPhrase.size();++_){
    Converted.Words.push_back(OriginalPhrase[_].first);
    Converted.Articles.push_back(OriginalPhrase[_].second);
  }
  return Converted;
}

bool Phrase::LessThan(pair<string,string> A,pair<string,string> B){
  if(A.first<B.first){
    return true;
  }
  return false;
}

void Phrase::push_back(string Word,string Article){
  Words.push_back(Word);
  Articles.push_back(Article);
}

Phrase Phrase::InsPhrase(Phrase Into,Phrase ToInsert,size_t Pos){
  Phrase NewPhrase;
  NewPhrase.Words=List::InsVector(Into.Words,ToInsert.Words,Pos);
  NewPhrase.Articles=List::InsVector(Into.Articles,ToInsert.Articles,Pos);
  return NewPhrase;
}

size_t Phrase::Size(){
  size_t WordListSize=Words.size();
  size_t ArticleListSize=Articles.size();
  return WordListSize>ArticleListSize?ArticleListSize:WordListSize;
}

Phrase Phrase::ExcludeWords(Phrase OriginalPhrase,vector<string> ToExclude){
  Phrase Result;
  for(size_t ElPos=0;ElPos<OriginalPhrase.Size();++ElPos){
    bool Found=false;
    for(size_t ExPos=0;ExPos<ToExclude.size();++ExPos)
      if(OriginalPhrase.Words[ElPos]==ToExclude[ExPos]){
        Found=true;
        break;
      }
    if(!Found)
      Result.push_back(OriginalPhrase.Words[ElPos],OriginalPhrase.Articles[ElPos]);
  }
  return Result;
}

Sentence::Sentence(){
  SentenceVoiceFound=Sentence::Unknown;
}

Sentence::SentenceVoiceType Sentence::GetSentenceVoice(){
  SentenceVoiceType VoiceType=SentenceVoiceType::Unknown;
  bool PassiveAgentExists=false;

  // search for a passive agent
  if(List::SetTheoryIntersect(OriginalSentence,PassiveAgents).size())
    PassiveAgentExists=true;

  // extract the verb form without the base verb
  vector<string> Verbs;
  for(size_t _=0;_<OriginalSentence.size();++_)
    if(FoundArticles[_]=="verb")
      Verbs.push_back(OriginalSentence[_]);

  if(Verbs.size())
    Verbs.erase(Verbs.end()-1);
  
  NameofTense={};
  map<vector<string>,vector<string>>::iterator ActiveVerbFormIter=ActiveVerbForms.find(Verbs);
  if(ActiveVerbFormIter!=ActiveVerbForms.end()){
    NameofTense=ActiveVerbFormIter->second;
    LongTense=ActiveVerbFormIter->first;
  }

  // search for the past participle in the known passive verb forms
  map<vector<string>,vector<string>>::iterator PassiveVerbFormIter=PassiveVerbForms.find(Verbs);
  bool PastParticipleFound=false;
  if(PassiveVerbFormIter!=PassiveVerbForms.end()){
    NameofTense=PassiveVerbFormIter->second;
    LongTense=PassiveVerbFormIter->first;
    PastParticipleFound=true;
  }
  
  // if PP and PA then sentence is a PVF
  VoiceType=(PastParticipleFound&&PassiveAgentExists)?Sentence::SentenceVoiceType::Passive:Sentence::SentenceVoiceType::Active;

  SentenceVoiceFound=VoiceType;
  return VoiceType;

}

void Sentence::SeparateSentence(){
  // The sentence data must be populated
  if(OriginalSentence.size()&&FoundTemplate.size()){

    vector<string> SentenceArticles=FoundArticles;

    Subject={};
    Predicate={};
    PredicateVerbs={};
    PredicateNouns={};
    size_t ArticlePosition=0;
    string CurrentStatus="";
    if(SentenceArticles.size()==OriginalSentence.size()){
      int Type=0;
      for(size_t _=0;_<SentenceArticles.size();++_){
	if((SentenceVoiceFound==Sentence::SentenceVoiceType::Passive)&&(List::SetTheoryIsElem(PassiveAgents,OriginalSentence[_]))){}
	else{	  
	  if(List::SetTheoryIsElem(SubjectArticles,SentenceArticles[_])&&(Type!=2)){
	    Type=2;
	    CurrentStatus=FoundTemplate[ArticlePosition];
	    ++ArticlePosition;
	  }
	  else{
	    if(List::SetTheoryIsElem(PredicateArticles,SentenceArticles[_])&&(Type!=1)){
	      Type=1;
	      CurrentStatus=FoundTemplate[ArticlePosition];
	      ++ArticlePosition;
	    }
	  }
	  if(CurrentStatus=="s")
	    Subject.push_back(OriginalSentence[_],SentenceArticles[_]);
	  else if(CurrentStatus=="pv"){
	    Predicate.push_back(OriginalSentence[_],SentenceArticles[_]);
	    PredicateVerbs.push_back(OriginalSentence[_],SentenceArticles[_]);
	  }
	  else if(CurrentStatus=="pn"){
	    Predicate.push_back(OriginalSentence[_],SentenceArticles[_]);
	    PredicateNouns.push_back(OriginalSentence[_],SentenceArticles[_]);
	  }
	}
      }
    }
    if(SentenceVoiceFound==Sentence::SentenceVoiceType::Passive){
      Phrase Temp=PredicateNouns;
      PredicateNouns=Subject;
      Subject=Temp;
      Phrase PrepositionList;
      for(size_t _=0;_<OriginalSentence.size();++_){
	if(SentenceArticles[_]=="preposition")
	  if(!List::SetTheoryIsElem(PassiveAgents,OriginalSentence[_]))
	    PrepositionList.push_back(OriginalSentence[_],SentenceArticles[_]);
      }
      Subject=Phrase::ExcludeWords(Subject,PrepositionList.Words);
      PredicateNouns=Phrase::InsPhrase(PredicateNouns,PrepositionList,0);
    }
  }
}

vector<string> Sentence::GetTemplate(){
  vector<string> Template;
  map<string,vector<vector<string>>>::iterator TemplatePos=SentenceTemplates.find(SentenceMoodFound);
  if(TemplatePos!=SentenceTemplates.end()){
    vector<vector<string>> TemplateList=TemplatePos->second;
    vector<string> GenericForm;
    int Type=0;
    for(size_t ArticlePos=0;ArticlePos<FoundArticles.size();++ArticlePos){
      if((SentenceVoiceFound==Sentence::SentenceVoiceType::Passive)&&(List::SetTheoryIsElem(PassiveAgents,OriginalSentence[ArticlePos]))){}
      else{	
	if(List::SetTheoryIsElem(PredicateArticles,FoundArticles[ArticlePos])&&(Type!=1)){
	  GenericForm.push_back("v");
	  Type=1;
	  continue;
	}
	if(List::SetTheoryIsElem(SubjectArticles,FoundArticles[ArticlePos])&&(Type!=2)){
	  GenericForm.push_back("s");
	  Type=2;
	}
      }
    }
    for(size_t _=0;_<TemplateList.size();++_){
      bool TempIsValid=true;
      if(GenericForm.size()==TemplateList[_].size()){
	for(size_t TempPos=0;TempPos<GenericForm.size();++TempPos){
	  if((GenericForm[TempPos]=="s")&&((TemplateList[_])[TempPos]=="pv")){
	    TempIsValid=false;
	    continue;
	  }
	  if((GenericForm[TempPos]=="v")&&(((TemplateList[_])[TempPos]=="s")||((TemplateList[_])[TempPos]=="pn")))
	    TempIsValid=false;
	}
      }
      else
	TempIsValid=false;
      if(TempIsValid){
	FoundTemplate=TemplateList[_];
	return TemplateList[_];
      }
    }
  }
  FoundTemplate=Template;
  return Template;
}

string Sentence::GetSentenceMood(){
  vector<vector<string>> ClippedSentenceArticles=PossibleArticles;
  ClippedSentenceArticles.erase(ClippedSentenceArticles.begin());
  PossibleArticles=ClippedSentenceArticles;
  vector<string> InitialArticles;
  for(size_t _=0;_<MoodList.size();++_)
    if(ClassifySentence(ClippedSentenceArticles,InitialArticles,OriginalSentence,MoodList[_],StartSentenceToken)){
      SentenceMoodFound=MoodList[_];
      return SentenceMoodFound;
    }
  SentenceMoodFound="";
  return "";
}

bool Sentence::ClassifySentence(vector<vector<string>> SentenceArticles,vector<string> ArticlesSoFar,vector<string> Sentence,string SentenceMood,string LastArticle){
  ArticlesSoFar.push_back(LastArticle);
  if(SentenceArticles.size()==0){
    FoundArticles=ArticlesSoFar;
    return true;
  }
  vector<vector<string>> ClippedSentenceArticles=SentenceArticles;
  ClippedSentenceArticles.erase(ClippedSentenceArticles.begin());
  vector<string> ClippedSentence=Sentence;
  if(ClippedSentence.size())
    ClippedSentence.erase(ClippedSentence.begin());
  vector<string> ArticlesToScan=List::SetTheoryIntersect(SentenceArticles[0],(GrammarGraphs[SentenceMood])[LastArticle]);
  for(size_t _=0;_<ArticlesToScan.size();++_)
    if(ClassifySentence(ClippedSentenceArticles,ArticlesSoFar,ClippedSentence,SentenceMood,ArticlesToScan[_]))
      return true;
  return false;
}

string Sentence::Tense2Str(Sentence::TenseType ThisTense){
  if(Sentence::TenseType::Past==ThisTense){
    return "past";
  }
  if(Sentence::TenseType::Present==ThisTense){
    return "present";
  }
  if(Sentence::TenseType::Future==ThisTense){
    return "future";
  }
  return "present";
}

Sentence::TenseType Sentence::Str2Tense(string TenseName){
  if(TenseName=="past"){
    return Sentence::TenseType::Past;
  }
  if(TenseName=="present"){
    return Sentence::TenseType::Present;
  }
  if(TenseName=="future"){
    return Sentence::TenseType::Future;
  }
  return Sentence::TenseType::Present;
}

Sentence::TenseType Sentence::Vec2Tense(vector<string> CanonicalVerbData){
  size_t VerbSize=CanonicalVerbData.size();
  if(VerbSize){
    string VerbTense=CanonicalVerbData[VerbSize-1];
    return Sentence::Str2Tense(VerbTense);
  }
  return Sentence::TenseType::Present;
}

vector<string> Phrase::FilterArticles(vector<string> TheseWords,vector<string> TheseArticles,string Article){
  vector<string> TheseFoundArticles;
  for(size_t _=0;(_<TheseWords.size())&&(_<TheseArticles.size());++_)
    if(TheseArticles[_]==Article)
      TheseFoundArticles.push_back(TheseWords[_]);
  return TheseFoundArticles;
}

/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
*/
