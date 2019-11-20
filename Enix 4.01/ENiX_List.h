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

#ifndef ENiXLIST
#define ENiXLIST

#include <vector>
#include <string>
#include <algorithm>
#include "ENiX_Binary.h"

using namespace std;

#define HASQUOTES  1
#define HASSPACES  2
#define HASNUMBERS 4
#define HASSYMBOLS 8
#define HASLETTERS 16

class List: public BinData{
 public:
  static vector<string> ShiftVector(vector<string> &Input1,size_t Offset);
  static vector<string> VecAdd(vector<string> Input,vector<string> ToAdd);
  static vector<int> VecAdd(vector<int> Input,vector<int> ToAdd);
  static vector<double> VecAdd(vector<double> Input,vector<double> ToAdd);
  static vector<string> InsVector(vector<string> &Into,vector<string> &Vector,size_t Pos);
  static vector<string> SortList(vector<string> Input1);

  static string Convert2Lower(string &InStr);
  static string Convert2Lower(string InStr);
  static void PrintList(vector<string> &Input);
  static void PrintList(vector<vector<string>> &Input);
  static void PrintList(vector<BinData> &Input);
  static void PrintList(vector<int> &Input);
  static void PrintList(vector<size_t> &Input);
  static void PrintList(vector<double> &Input);

  static void PrintSentences(vector<string> &Input);
  static void DebugList(vector<string> &Input);
  static int ClassifyToken(string &Token);  

  static vector<string> DivideInput(string &Input);
  static vector<string> String2List(string Input,bool ToLower=false);
  static string List2String(vector<string> Input,string Separator=(char *)" ");
  static string Set2String(vector<string> Input);
  static vector<string> DivideCompletely(string &Input);

  static vector<BinData> String2List(BinData Input,bool ToLower);
  static BinData List2String(vector<BinData> Input);
  static string List2String(vector<int> Input);
  static string List2String(vector<float> Input);
  static string List2String(vector<double> Input);

  static vector<string> String2List(string Input,char SplitOn);

  static vector<BinData> ListStr2BinList(vector<string>  Input);
  static vector<string> BinList2ListStr(vector<BinData> Input);

  static vector<double> List2Num(vector<string> Input);
  static vector<int> List2Int(vector<string> Input);
  static vector<string> Num2List(vector<double> Input);
  static vector<string> Int2List(vector<int> Input);

  static vector<double> List2Floats(vector<string> Input);
  static vector<int> List2Ints(vector<string> Input);

  static vector<string> GetUpto(vector<string> Input,size_t Finish);
  static vector<string> GetFrom(vector<string> Input,size_t Start);
  static vector<string> GetBetween(vector<string> Input,size_t Start,size_t Finish);
  static long int FindFirst(vector<string> Input,string Token);
  static long int FindFirst(vector<int> Input,int Token);

  /* Separation */
  static bool ContainsDuplicates(vector<string> Input);
  static vector<string> SeparateSymbols(vector<string> Symbols);
  static vector<string> PrepareInput(vector<string> Symbols);
  
  /* Syntax parsing */
  static long int FindTerminator(vector<string> Commands,string StartSymbol,string EndSymbol,size_t StartValue=0);
  static long int FindSameLevel(vector<string> Commands,string StartSymbol,string EndSymbol,string InternSymbol,size_t StartValue=0);

  /* Set Theory Operators */
  static bool           SetTheoryIsSame(vector<string> Set1,vector<string> Set2);
  static bool           SetTheoryIsIntersect(vector<string> Set1,vector<string> Set2);
  static bool           SetTheoryIsSubset(vector<string> Coverset,vector<string> Subset);
  static bool           SetTheoryIsElem(vector<string> Set,string Element);
  static vector<string> SetTheoryAdd(vector<string> Set,string Element);
  static vector<string> SetTheoryUnion(vector<string> Set1,vector<string> Set2);
  static vector<string> SetTheoryIntersect(vector<string> Set1,vector<string> Set2);
  static vector<string> SetTheoryExclude(vector<string> Set,vector<string> ToExclude);
  
  static vector<BinData> BinDataSetAdd(vector<BinData> DataList,BinData Item);
  static vector<BinData> BinDataSetAdd(vector<BinData> DataList,vector<BinData> Item);
  static vector<BinData> BinDataSetDel(vector<BinData> DataList,size_t Item);
  static vector<unsigned long int> BinDataSetDel(vector<unsigned long int> DataList,size_t Pos);
  static vector<BinData> BinDataSetDel(vector<BinData> DataList,BinData Item);

 private:
  static vector<string> SplitOffPrefix(string &Symbol);
  static vector<string> SplitOffSuffix(string &Symbol);
  static vector<string> SplitSymbol(string &Symbol);


};



#endif

/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
*/
