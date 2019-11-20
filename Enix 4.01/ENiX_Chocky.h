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

#ifndef ENiXCHOCKY
#define ENiXCHOCKY


#include <vector>
#include <string>

#include "ENiX_Memory.h"
#include "ENiX_NNBridge.h"
#include "ENiX_Poly.h"

using namespace std;

class Chocky:public Memory,public Poly{
 public:

  enum Syntax{
    NLPType,
    FuncType,
    IndType
  };


  /* TODO */

  vector<string> Maths2RPN(vector<string> Input);

  /* Complete */

  Chocky(string Input);
  //  Chocky(string HostName,int PortNum,string Input);
  Chocky(Networking *DBLink,Networking *NNLink,string Input);
  Chocky();
  ~Chocky();
  
  string TimeSlice;

  /* Command Interpreter */ 

  typedef vector<string> (Chocky::*ENiXInstructionSet)(vector<string> DataPrefix,vector<string> DataSuffix);
  vector<ENiXInstructionSet> InstructionSet;
  
  
  /************************************************
   *
   * 0 = Both optional parameters
   * 1 = First parameter mandatory
   * 2 = Second parameter mandatory
   * 3 = Both parameters mandatory
   *
   ************************************************/

  vector<int>    Parameters;
  vector<string> CommandName;
  vector<string> OnlineHelp;

  void PopulateCommandParameters();

  /* Commands */
  vector<string> CommandHelp(vector<string> Input1,vector<string> Input2);
  vector<string> PrintAnswer(vector<string> Input1,vector<string> Input2);
  vector<string> PrintErrors(vector<string> Input1,vector<string> Input2);
  vector<string> PrintOutput(vector<string> Input1,vector<string> Input2);
  vector<string> GetAllRecs(vector<string> Input1,vector<string> Input2);
  vector<string> Syntax(vector<string> Input1,vector<string> Input2);
  vector<string> Assign(vector<string> Input1,vector<string> Input2);
  vector<string> ListVariables(vector<string> Input1,vector<string> Input2);
  vector<string> SaveSettings(vector<string> Input1,vector<string> Input2);
  vector<string> DebugRec(vector<string> Input1,vector<string> Input2);
  vector<string> Swap(vector<string> Input1,vector<string> Input2);
  vector<string> Domain(vector<string> Input1,vector<string> Input2);
  vector<string> Sort(vector<string> Input1,vector<string> Input2);
  vector<string> Save(vector<string> Input1,vector<string> Input2);
  vector<string> Test(vector<string> Input1,vector<string> Input2);


  /* Neural */
  vector<string> Learn(vector<string> Input1,vector<string> Input2);
  vector<string> Oddman(vector<string> Input1,vector<string> Input2);
  vector<string> NNExecute(vector<string> Input1,vector<string> Input2);
  vector<string> Understood(vector<string> Input1,vector<string> Input2);
  vector<string> ShowNeural(vector<string> Input1,vector<string> Input2);

  /* Maths */
  vector<string> Add(vector<string> Input1,vector<string> Input2);
  vector<string> Subtract(vector<string> Input1,vector<string> Input2);
  vector<string> Multiply(vector<string> Input1,vector<string> Input2);
  vector<string> Divide(vector<string> Input1,vector<string> Input2);
  vector<string> Exponent(vector<string> Input1,vector<string> Input2);
  vector<string> Modulus(vector<string> Input1,vector<string> Input2);
  
  /* Logic */
  vector<string> Equals(vector<string> Input1,vector<string> Input2);
  vector<string> NotEqual(vector<string> Input1,vector<string> Input2);
  vector<string> GreaterThan(vector<string> Input1,vector<string> Input2);
  vector<string> GreaterThanEqual(vector<string> Input1,vector<string> Input2);
  vector<string> LessThan(vector<string> Input1,vector<string> Input2);
  vector<string> LessThanEqual(vector<string> Input1,vector<string> Input2);

  vector<string> Interpreter(vector<string> Input);
  
  /* Poly */
  vector<string> CreatePoly(vector<string> Input1,vector<string> Input2);
  vector<string> CalcPoly(vector<string> Input1,vector<string> Input2);
  vector<string> SetPoly(vector<string> Input1,vector<string> Input2);
  vector<string> GetPoly(vector<string> Input1,vector<string> Input2);

  /* Control Seq */
  vector<string> If(vector<string> Input1,vector<string> Input2);
  vector<string> While(vector<string> Input1,vector<string> Input2);

  void LearnSelected(vector<string> Input);
  
  /* Context Sensitive Addressing */
  using Memory::CSSave;
  vector<string> CSAddr(vector<string> Input1,vector<string> Input2);
  vector<string> CSLoad(vector<string> Input1,vector<string> Input2);
  vector<string> CSRecon(vector<string> Input1,vector<string> Input2);
  vector<string> CSSave(vector<string> Input1,vector<string> Input2);  

  /* NN Multiplexor */
  vector<string> NNClear(vector<string> Input1,vector<string> Input2);
  vector<string> NNAdd(vector<string> Input1,vector<string> Input2);
  vector<string> NNMul(vector<string> Input1,vector<string> Input2);
  vector<string> NNGetCause(vector<string> Input1,vector<string> Input2);  
  vector<string> NNGetEffect(vector<string> Input1,vector<string> Input2);  
  vector<string> NNRemove(vector<string> Input1,vector<string> Input2);  

  /* Grammar */
  vector<string> SetSenStart(vector<string> Input1,vector<string> Input2);
  vector<string> SetSenEnd(vector<string> Input1,vector<string> Input2);
  vector<string> ResetGrammar(vector<string> Input1,vector<string> Input2);
  vector<string> SetGrammar(vector<string> Input1,vector<string> Input2);
  vector<string> SetWords(vector<string> Input1,vector<string> Input2);
  vector<string> SetArticles(vector<string> Input1,vector<string> Input2);
  vector<string> SetMood(vector<string> Input1,vector<string> Input2);
  vector<string> SetMoodList(vector<string> Input1,vector<string> Input2);
  vector<string> ShowGrammar(vector<string> Input1,vector<string> Input2);
  vector<string> ShowMode(vector<string> Input1,vector<string> Input2);
  using Memory::ClassifySentence;
  vector<string> ClassifySentence(vector<string> Input1,vector<string> Input2);
  
  vector<string> SenTemplate(vector<string> Input1,vector<string> Input2);
  vector<string> ShowTemplate(vector<string> Input1,vector<string> Input2);

  vector<string> SentenceDebug(vector<string> Input1,vector<string> Input2);
  
  using Memory::SentenceDebug;
  void AddToDirectedGraph(vector<string> Sentence,string SentenceMood);

  vector<string> SubjArticles(vector<string> Input1,vector<string> Input2);
  vector<string> PredArticles(vector<string> Input1,vector<string> Input2);
  
  vector<string> AddActiveVerbForm(vector<string> Input1,vector<string> Input2);
  vector<string> AddPassiveVerbForm(vector<string> Input1,vector<string> Input2);
  vector<string> ShowVerbForms(vector<string> Input1,vector<string> Input2);

  vector<string> SetPassiveAgent(vector<string> Input1,vector<string> Input2);
  vector<string> SetLogic(vector<string> Input1,vector<string> Input2);
  
  vector<string> VerbOrder(vector<string> Input1,vector<string> Input2);
  vector<string> SubjectOrder(vector<string> Input1,vector<string> Input2);

  vector<string> Delete(vector<string> Input1,vector<string> Input2);
  vector<string> Tense(vector<string> Input1,vector<string> Input2);

  vector<string> BaseVerb(vector<string> Input1,vector<string> Input2);

  vector<string> SetCondCause(vector<string> Input1,vector<string> Input2);
  vector<string> SetCondEffect(vector<string> Input1,vector<string> Input2);

  vector<string> SepParagraph(vector<string> Input1,vector<string> Input2);

  vector<string> EndPunct(vector<string> Input1,vector<string> Input2);
  vector<string> CrashENiX(vector<string> Input1,vector<string> Input2);
  
 private:

  enum Chocky::Syntax ClassifyInputType(vector<string> &Input);
  
  vector<double> GetFloatParams(vector<string> Input);
  vector<int> GetIntParams(vector<string> Input);
  vector<string> GetStringParams(vector<string> Input);

  vector<string> GetParamsOf(string Param,vector<string> Input);

  vector<double> GetFloatParam(string Input);
  vector<int> GetIntParam(string Input);
  vector<string> GetStringParam(string Input);

  /* Variables */
  void ClearVariables();
  vector<string> ListVariables();
  string GetVariable(string VarName);
  void SetVariable(string VarName,string Value);
  map<string,string> VariableMatrix;

  /* TODO */
  bool MathsSyntaxOK(vector<string> Input);
  vector<string> MathsRec(vector<string> Input);
  vector<string> ExpandOnce(vector<string> Covered,string Input);
  vector<string> Execute(vector<string> Pipeline,vector<string> LastCommands);

  vector<string> NLPExe(vector<string> Pipeline,vector<string> LastCommands);
  vector<string> IndExe(vector<string> Pipeline,vector<string> LastCommands,bool TranslateThis=1);
  vector<string> FuncExe(vector<string> Pipeline,vector<string> LastCommands);

  vector<string> ProcessNLP();
  
  /* Complete */
  vector<string> Tokens;

  vector<string> DisplayOutput;
  vector<string> Errors;  
};

#endif

/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
 */
