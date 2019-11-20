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

#ifndef ENiXMODE
#define ENiXMODE

#include<string>
#include<vector>
#include<map>
#include "ENiX_List.h"

class Mode{
 public:
  Mode();
  size_t CSNum;
  string UniqueUN;
  string StartSentenceToken;
  string EndSentenceToken;
  vector<string> RecordList;
  vector<string> AllRecords;
  vector<string> ANSWER;
  vector<string> ArticleList;
  vector<string> WordArticleList;
  vector<string> MoodList;
  vector<string> LogicList;
  vector<string> CondCauseTokens;
  vector<string> CondEffectTokens;
  vector<string> EndPunctuation;

  vector<string> VerbOrderList;
  vector<string> SubjectOrderList;

  map<string,map<string,vector<string>>> GrammarGraphs;
  map<string,vector<string>> FindOrCreateGraph(string GraphName);
  map<string,vector<vector<string>>> SentenceTemplates;

  vector<string> SubjectArticles;
  vector<string> PredicateArticles;

  vector<string> PassiveAgents;

  string DisplayGraphs();
  void AddToGraph(string GraphName,string Prefix,string Suffix);
  bool AddToTemplates(string SentenceMood,vector<string> Template);
  void AddToTemplates(string SentenceMood,string Templates);

  // Verb forms
  map<vector<string>,vector<string>> ActiveVerbForms;
  map<vector<string>,vector<string>> PassiveVerbForms;
  vector<string> VerbFormArticles;
  
  void AAVF(vector<string> VerbForm,vector<string> Name);
  void APVF(vector<string> VerbForm,vector<string> Name);
  void AVF(map<vector<string>,vector<string>> &AllVerbForms,vector<string> &VerbForm,vector<string> Name);

  // Show verb forms
  string SAVF();
  string SPVF();

};

#endif

/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
*/
