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

#ifndef ENiXMEMORY
#define ENiXMEMORY

#include "ENiX_Net.h"
#include "ENiX_List.h"
#include "ENiX_Binary.h"
#include "ENiX_WMSAPI.h"
#include "ENiX_Sentence.h"
#include "ENiX_NLPStoreLogic.h"
#include "ENiX_NNBridge.h"

class ConditionalParagraphData{
 public:
  vector<vector<string>> Causes;
  vector<vector<string>> Effects;
  NLPStoreLogic::SLogic CauseLogic;
  NLPStoreLogic::SLogic EffectLogic;

  vector<string> CSCauseSyms;
  vector<string> CSEffectSyms;
  
  vector<Sentence> ProcessedCauses;
  vector<Sentence> ProcessedEffects;
  
  string Mood;
  void Debug();
};

class Memory:public NNBridge,public List,public Sentence{
 public:

  Memory();
  ~Memory();

  enum CauseOrEffect{
    Cause,
    Effect
  };

  enum PhraseType{
    IsSubject,
    IsPredicate
  };

  struct SentenceLogic{

    vector<string> Verbs;

    vector<string> NewSubj;
    vector<string> OldSubj;

    vector<string> NewPredNouns;
    vector<string> OldPredNouns;
    
    NLPStoreLogic::SLogic SubjOldLogic;
    NLPStoreLogic::SLogic SubjNewLogic;
    NLPStoreLogic::SLogic PNOldLogic;
    NLPStoreLogic::SLogic PNNewLogic;
  
    int SubjectAction;
    int PNAction;
  };
  
  void DebugSentenceLogic(SentenceLogic &Logic);
  vector<string> GetSentenceSymbols();
  //  string GetCSLocationList(Sentence &Input,vector<string> &SentenceConditions);
  vector<string> GetCSLocationList(Sentence &Input,vector<string> &SentenceConditions);
  void SetupMode();
  void LearnWords(vector<string> Input);

  // Conditionals
  ConditionalParagraphData SplitConditional(vector<string> ConditionalSentence);
  bool AskYNConditional(string &Cause,string &Effect);
  vector<string> AskWHQConditional(string &Cause,string &Effect);
  void StoreConditional(string &Cause,string &Effect);
  
  // Communication with Amoeba
  Packet AmoebaRun(string Command);
  void Write2Amoeba(WMS_Rec &ToWrite);
  void Write2Amoeba(vector<WMS_Rec> &ToWrite);
  void DelAmoebaRec(string ToDelete);
  long int FindOrCreate(string RecName,bool Access=true);
  string GetStrResp(Packet Resp);

  void DebugLearnt();
  static void DebugLearnt(vector<WMS_Rec> &ToWrite);

  vector<BinData> CreateNewHeader(string RecordName);
  vector<BinData> UpdateHeader(vector<BinData> OldHeader);

  WMS_Rec GetFromCache(string RecName);
  size_t FindInCache(string RecName);
  void RepInCache(WMS_Rec &InputRec);
  void RemoveFromCache(string RecName);

  WMS_Rec DelDBPair(WMS_Rec InputRec,size_t Location);
  WMS_Rec DelDBPair(WMS_Rec InputRec,string Attr);
  WMS_Rec DelDBPair(WMS_Rec InputRec,BinData Attr);
  WMS_Rec RepHeader(WMS_Rec InputRec,BinData Value,int Location);
  WMS_Rec RepHeader(WMS_Rec InputRec,string Value,int Location);
  string ExtractString(WMS_Rec InputRec,size_t Sector,size_t Position);

  size_t FindDBAttr(WMS_Rec InputRec,BinData Attr);
  size_t FindDBAttr(WMS_Rec InputRec,string Attr);

  bool IsDBPair(WMS_Rec InputRec,BinData Attr,BinData Value);
  bool IsDBPair(WMS_Rec InputRec,string Attr,string Value);

  WMS_Rec AddDBPair(WMS_Rec InputRec,BinData Attr,BinData Value);  
  WMS_Rec AddDBPair(WMS_Rec InputRec,string Attr,string Value);
  WMS_Rec RepDBPair(WMS_Rec InputRec,BinData Attr,BinData Value);
  WMS_Rec RepDBPair(WMS_Rec InputRec,string Attr,string Value);
  string GetDBValue(WMS_Rec InputRec,BinData Attr);
  string GetDBValue(WMS_Rec InputRec,string Attr);

  vector<string> GetDBAttr(WMS_Rec InputRec,BinData Value);
  vector<string> GetDBAttr(WMS_Rec InputRec,string Value);
  
  vector<string> GetAllDBValues(WMS_Rec InputRec,BinData Attr);
  vector<string> GetAllDBValues(WMS_Rec InputRec,string Attr);
  
  vector<WMS_Rec> CachedContents;
  
  Networking *AmoebaConnection;

  WMS_Rec Mode2WMS();
  void WMS2Mode(WMS_Rec Rec);
  void LoadGrammar(string GrammarName);
  void SaveGrammar(string GrammarName);
  void LoadTemplate(string GrammarName);
  void SaveTemplate(string GrammarName);
  void SetTense(string RecName,string Tense);
  
  vector<vector<string>> SentenceToArticles(vector<string> &Sentence);
  vector<string> GetArticlesFromWord(string Word);

  void MultiplexClear(string ConceptName);
  void MultiplexAdd(string ConceptName,vector<string> Cause,vector<string> Effect);
  vector<string> MultiplexGen(string ConceptName);
  int MultiplexEncode(string ConceptName,vector<string> Scenario,CauseOrEffect Switch);
  vector<string> MultiplexDecode(string ConceptName,int Scenario,CauseOrEffect Switch);
  void MultiplexRemove(string ConceptName,vector<string> Cause);
  string DisplayNeuralSymbols(string ConceptName);

  void GetTense();
  vector<string> CreateTense(vector<string> Adverbs,string Verb,Sentence::TenseType VerbTense);
  vector<string> CreateSentence(string CSSubj,vector<string> CreatedVerb,string CSPred,Sentence::SentenceVoiceType VoiceType);

  string SentenceDebug(vector<string> Sentence);

  void SaveVerbForm(string RecordName,map<vector<string>,vector<string>> &VerbForms);
  void LoadVerbForm(string RecordName,map<vector<string>,vector<string>> &VerbForms);

  // Top level sentence API:
  // This will handle everything but sentence processing.
  void PrepareSentence(vector<string> &Sentence);

  void PrepNounCSSymbols(Phrase &NounPhrase);
  void PrepVerbCSSymbols(Phrase &VerbPhrase);

  // NLP storage routines
  void SaveSentences();

  bool YesNoQuestion();
  vector<string> WHQuestion(Memory::PhraseType PType);

  vector<vector<string>> SeparateParagraph(vector<string> &Data);
  bool IsCondSentence(vector<string> &Data);
  string GetCondSentenceMood(vector<string> &Data);
  
  // protected:
  string CSRef(vector<string> CSAddr);
  vector<string> CSDeref(string CSRef);
  bool CSSave(vector<string> CSAddr,string Data);
  bool CSSave(string CSAddr,string Data);
  vector<int> CreateDomain(string ConceptName,CauseOrEffect Switch);
  string CSGet(vector<string> CSAddr);
  
 private:
  BinData ExtractFromServer(string Command,int ProtocolBinId);
  string ExtractFromServer(string Command,string RespPrefix,int ProtocolMsgId);
  vector<int> GenerateCodex(size_t Size);

  WMS_Rec GetRecUpdate(WMS_Rec &Rec,int EntryNum,SentenceLogic Input);

  string GetLatestCS();
  string FindTotalCS(string ContextPrefix,string ContextSuffix);
  vector<string> RecCSDeref(vector<string> ContextSoFar,string FinalCS,string CurrentCSSym,string NextCSSym,vector<string> RecsSoFar);
  bool CSPairExists(string Prefix,string Suffix);
  string CSGet(string CSAddr);
};

extern Memory *GlobalMemoryAccess;


#endif

/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
*/
