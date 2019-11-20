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

#ifndef ENiXSTORELOGIC
#define ENiXSTORELOGIC

#include <vector>
#include <string>

using namespace std;

class NLPStoreLogic{
 public:
  enum Overlap{
    None=0,
    Partial,
    AllOld,
    AllNew,
    Full
  };

  static size_t NumOverlapping;
  
  enum SLogic{
    AND=0,
    OR,
    NOR,
    XOR
  };

  enum Type{
    Subject=0,
    PredicateNouns
  };
  
  static int NLP_L_NOTHING; // leave any old symbols and logic there
  static int NLP_L_ADD_NEW; // add the new symbols and logic
  static int NLP_L_REPLACE; // replace old symbols and logic with new
  static int NLP_L_COMBINE; // add the new symbols to the existing logic and content
  static int NLP_L_DELETE;  // delete what is already tehre
  static int NLP_L_LOG_NEW; // change the logic to the new
  static int NLP_L_LOG_OLD; // change the logic to the old
  static int NLP_L_OVERLAP; // replace with the overlap symbols
  static int NLP_L_OVL_ONE; // replace with the overlap symbols if there are only one - else deletes

  static int NLP_L_NOVLNEW; // add new non-overlapping
  static int NLP_L_NOVLOLD; // add old non-overlapping

  static int AND_STATES[3][4][4];
  static int OR_STATES[3][4][4];

  static NLPStoreLogic::Overlap GetOverlap(vector<string> Original,vector<string> New);

  /*
  typedef vector<string>(NLPStoreLogic::*PrepFns)(vector<string> &OldLogic,vector<string> &NewLogic);
  static vector<PrepFns> PreprocessorFns;
  static void PopulatePrep();
  */

  static vector<string> ChangeLog2New(vector<string> &OldLogic,vector<string> &NewLogic);
  static vector<string> ChangeLog2Old(vector<string> &OldLogic,vector<string> &NewLogic);
  static vector<string> GetOverlapData(vector<string> &OldLogic,vector<string> &NewLogic);
  static vector<string> GetOverlapIfOne(vector<string> &OldLogic,vector<string> &NewLogic);
  static vector<string> GetNewNonOverlap(vector<string> &OldLogic,vector<string> &NewLogic);
  static vector<string> GetOldNonOverlap(vector<string> &OldLogic,vector<string> &NewLogic);
  static vector<string> GetCombined(vector<string> &OldLogic,vector<string> &NewLogic);

  static vector<string> ChangeLogicTo(vector<string> &Logic,string &ChangeTo);

  static void DebugOverlap(NLPStoreLogic::Overlap Input);
  static void DebugSLogic(NLPStoreLogic::SLogic Input);

  static int StateCalculation(NLPStoreLogic::Type PhraseType,NLPStoreLogic::Overlap OverlapType,NLPStoreLogic::SLogic OldLogic,NLPStoreLogic::SLogic NewLogic);

  static NLPStoreLogic::SLogic GetLogicCode(string Operand);
  static string GetLogicName(NLPStoreLogic::SLogic Input);

  static bool QuestionPhraseFound(NLPStoreLogic::SLogic OldLogic,NLPStoreLogic::SLogic NewLogic,NLPStoreLogic::Overlap OverlapType,size_t SizeofOverlap);

  //  static WMS_Rec GetRecUpdate(WMS_Rec &Rec,int EntryNum,int Action,NLPStoreLogic::SLogic Old,vector<string> OldPhrase,NLPStoreLogic::SLogic New,vector<string> NewPhrase);

};


#endif

/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
*/
