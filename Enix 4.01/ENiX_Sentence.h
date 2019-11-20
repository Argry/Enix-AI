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

#ifndef ENiXSENTENCE
#define ENiXSENTENCE

#include <tuple>
#include "ENiX_Mode.h"

class Phrase{
 public:

  Phrase();
  ~Phrase();

  vector<string> Words;
  vector<string> Articles;
  vector<Phrase> SubPhrases;
  vector<Phrase> CanonicalSubPhrases;
  vector<string> CSSubPhrases;
  vector<string> LogicArticles;
  
  vector<Phrase> CanonicalVerbs;
  string CSVerb;
  
  string LogicType;
  
  static vector<string> FilterArticles(vector<string> TheseWords,vector<string> TheseArticles,string Article);

  void push_back(string Word,string Article);
  size_t Size();
  static Phrase InsPhrase(Phrase Into,Phrase ToInsert,size_t Pos);
  static Phrase ExcludeWords(Phrase OriginalPhrase,vector<string> ToExclude);
  
  static Phrase Sort(Phrase OriginalPhrase);
  
  static Phrase Filter(Phrase OriginalPhrase,vector<string> Articles);
  static Phrase Filter(Phrase OriginalPhrase,string Article);
  static Phrase Append(Phrase Base,Phrase Extension);

  void PrintSubphrases();

  vector<Phrase> SplitSubject(Phrase OriginalPhrase,vector<string> LogicList);
  vector<Phrase> PrepCanonicalPhrase(vector<string> ArticleOrder);
  
  vector<Phrase> PrepCanonicalVerbs(vector<string> ArticleOrder,vector<string> FullTense,string BaseTense);

  Phrase CanonicalExp(Phrase &SubPhrase,vector<string> &Articles);
  Phrase CanonicalExp(vector<string> &Articles);
  
  void DebugPhrase();

 private:
  static bool LessThan(pair<string,string> A,pair<string,string> B);
  static vector<pair<string,string>> ConvertPhrase(Phrase OriginalPhrase);
  static Phrase ConvertPhrase(vector<pair<string,string>> OriginalPhrase);
};

class Sentence: public Mode{
 public:

  enum TenseType{
    Past,
    Present,
    Future
  };

  enum SentenceVoiceType{
    Unknown,
    Active,
    Passive
  };

  Sentence();

  Sentence *GetSentence();
  
  string GetSentenceMood();
  bool ClassifySentence(vector<vector<string>> SentenceArticles,vector<string> ArticlesSoFar,vector<string> Sentence,string SentenceMood,string LastArticle);
  vector<string> GetTemplate();
  void SeparateSentence();
  SentenceVoiceType GetSentenceVoice();
  
  vector<string> NameofTense;
  vector<string> LongTense;
  vector<string> OriginalSentence;
  vector<string> FoundArticles;
  vector<string> FoundTemplate;
  vector<vector<string>> PossibleArticles;

  bool IsSentenceValid();
  void SetSentenceInvalid();
  
  string BaseTense;
  
  Phrase Subject;
  Phrase Predicate;
  Phrase PredicateVerbs;
  Phrase PredicateNouns;

  string SentenceMoodFound;
  SentenceVoiceType SentenceVoiceFound;

  static string Tense2Str(Sentence::TenseType ThisTense);
  static Sentence::TenseType Str2Tense(string TenseName);
  static Sentence::TenseType Vec2Tense(vector<string> CanonicalVerbData);

  vector<tuple<vector<string>,vector<string>,vector<string>>> PreparedSentenceSymbols;
  void AddPreparedSymbols(vector<string> &S,vector<string> &PV,vector<string> &PN);
  void PrepareSymbols();
  


 protected:
  void GetBaseTense();

};

#endif

/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
*/
