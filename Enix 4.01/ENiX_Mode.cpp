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

#include <sstream>
#include "ENiX_Mode.h"
#include "ENiX_Settings.h"

void Mode::AVF(map<vector<string>,vector<string>> &AllVerbForms,vector<string> &VerbForm,vector<string> Name){
  map<vector<string>,vector<string>>::iterator VfIter=AllVerbForms.find(VerbForm);
  VerbFormArticles=List::SetTheoryUnion(VerbFormArticles,VerbForm);
  if(VfIter==AllVerbForms.end())
    AllVerbForms.insert(make_pair(VerbForm,Name));
  else
    AllVerbForms[VerbForm]=Name;
}

string Mode::SAVF(){
  ostringstream ToConvert;
  ToConvert<<"Active Verb Forms:"<<endl;
  map<vector<string>,vector<string>>::iterator AvfIter;
  for(AvfIter=ActiveVerbForms.begin();AvfIter!=ActiveVerbForms.end();++AvfIter){
    ToConvert<<BinData::FormatString("  % 020s -> %s\n",List::List2String(AvfIter->first).c_str(),List::List2String(AvfIter->second).c_str());
  }
  return ToConvert.str();
}

string Mode::SPVF(){
  ostringstream ToConvert;
  ToConvert<<"Passive Verb Forms:"<<endl;
  map<vector<string>,vector<string>>::iterator PvfIter;
  for(PvfIter=PassiveVerbForms.begin();PvfIter!=PassiveVerbForms.end();++PvfIter){
    ToConvert<<BinData::FormatString("  % 020s -> %s\n",List::List2String(PvfIter->first).c_str(),List::List2String(PvfIter->second).c_str());
  }
  return ToConvert.str();
}

void Mode::AAVF(vector<string> VerbForm,vector<string> Name){
  AVF(ActiveVerbForms,VerbForm,Name);
}

void Mode::APVF(vector<string> VerbForm,vector<string> Name){
  AVF(PassiveVerbForms,VerbForm,Name);
}

map<string,vector<string>> Mode::FindOrCreateGraph(string GraphName){
  map<string,map<string,vector<string>>>::const_iterator GraphSection=GrammarGraphs.find(GraphName);
  if(GraphSection==GrammarGraphs.end()){
    map<string,vector<string>> GraphSection;
    for(size_t _=0;_<ArticleList.size();++_)
      GraphSection.insert(make_pair<string,vector<string>>((string)ArticleList[_],vector<string>{}));
    return GraphSection;
  }
  else
    return GraphSection->second;
}

string Mode::DisplayGraphs(){
  map<string,map<string,vector<string>>>::iterator GraphList=GrammarGraphs.begin();
  ostringstream ToConvert;  
  for(GraphList=GrammarGraphs.begin();GraphList!=GrammarGraphs.end();++GraphList){
    ToConvert<<BinData::FormatString("%s\n",GraphList->first.c_str());
    map<string,vector<string>> GraphSection=GraphList->second;
    map<string,vector<string>>::iterator GraphData;    
    for(GraphData=GraphSection.begin();GraphData!=GraphSection.end();++GraphData){
      const char *First=GraphData->first.c_str();
      string TmpStr=List::List2String(GraphData->second);
      const char *Second=TmpStr.c_str();
      ToConvert<<BinData::FormatString("  % 020s -> %s\n",First,Second);
    }
  }
  return ToConvert.str();
}

void Mode::AddToTemplates(string SentenceMood,string Templates){
  vector<string> TemplateList=List::String2List(Templates,',');
  for(size_t _=0;_<TemplateList.size();++_){
    vector<string> Template=List::String2List(TemplateList[_]);
    AddToTemplates(SentenceMood,Template);
  }
}

bool Mode::AddToTemplates(string SentenceMood,vector<string> Template){
  vector<string> CoverSet={"s","pv","pn"};
  if(List::SetTheoryIsSubset(CoverSet,Template)){
    map<string,vector<vector<string>>>::iterator Pos=SentenceTemplates.find(SentenceMood);
    vector<vector<string>> VecofVec;    
    if(Pos==SentenceTemplates.end()){
      VecofVec.push_back(Template);
      SentenceTemplates.insert(make_pair(SentenceMood,VecofVec));
    }
    else{
      vector<vector<string>> RetrievedData=SentenceTemplates[SentenceMood];
      vector<vector<string>>::iterator TemplateDataIter;
      bool Found=false;
      for(TemplateDataIter=RetrievedData.begin();TemplateDataIter!=RetrievedData.end();++TemplateDataIter)
	if((*TemplateDataIter)==Template)
	  Found=true;
      if(!Found)
	RetrievedData.push_back(Template);
      SentenceTemplates[SentenceMood]=RetrievedData;
    }
  }
  else{
    return false;
  }
  return true;
}

Mode::Mode(){
  UniqueUN=BinData::NameId();
  RecordList={};
  AllRecords={};
  ANSWER={};
  CSNum=1;
}


void Mode::AddToGraph(string GraphName,string Prefix,string Suffix){
  map<string,vector<string>> GraphData=FindOrCreateGraph(GraphName);
  map<string,vector<string>>::iterator GraphDataPos=GraphData.find(Prefix);
  if(GraphDataPos!=GraphData.end()){
    vector<string> Articles;
    //    if(List::List2String(GraphDataPos->second)!="?")
    // Martyn Oct 2018
    if(List::List2String(GraphDataPos->second)!="`")
      Articles=List::SortList(List::SetTheoryAdd(GraphDataPos->second,Suffix));
    else
      Articles=List::String2List(Suffix);
    GraphData[Prefix]=Articles;
  }
  map<string,map<string,vector<string>>>::iterator GraphPos=GrammarGraphs.find(GraphName);
  if(GraphPos==GrammarGraphs.end())
    GrammarGraphs.insert(make_pair(GraphName,GraphData));
  else
    GrammarGraphs[GraphName]=GraphData;
}




/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
*/
