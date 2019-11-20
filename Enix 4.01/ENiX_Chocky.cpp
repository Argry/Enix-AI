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

#include "ENiX_Chocky.h"
#include <string.h>
#include <algorithm>
#include <math.h>

#include "ENiX_Settings.h"

Chocky::Chocky(string Input):Poly(){
  PopulateCommandParameters();
  CachedContents={};
  TimeSlice={};
}

Chocky::Chocky(Networking *DBLink,Networking *NNLink,string Input):Poly(){
  PopulateCommandParameters();
  CachedContents={};
  TimeSlice={};
  AmoebaConnection=DBLink;
  NeuralConnection=NNLink;
}

Chocky::Chocky():Poly(){
  PopulateCommandParameters();
  CachedContents={};
  TimeSlice={};
}

Chocky::~Chocky(){
  Mode2WMS();
  Write2Amoeba(CachedContents);
}

void Chocky::ClearVariables(){
  VariableMatrix={};
}

vector<string> Chocky::ListVariables(){
  vector<string> VariableList;
  map<string,string>::iterator ThisVar;
  for(ThisVar=VariableMatrix.begin();ThisVar!=VariableMatrix.end();ThisVar++)
    VariableList.push_back(ThisVar->first);
  return VariableList;
}

string Chocky::GetVariable(string VarName){
  string VariableValue;
  map<string,string>::iterator VariableFound=VariableMatrix.find(VarName);
  if(VariableFound!=VariableMatrix.end())
    VariableValue=VariableFound->second;
  return VariableValue;
}

void Chocky::SetVariable(string VarName,string Value){
  pair<map<string,string>::iterator,bool> Status=VariableMatrix.insert(pair<string,string>(VarName,Value));  
  if(Status.second==false)
    Status.first->second=Value;
}

vector<string> Chocky::GetParamsOf(string Param,vector<string> Input){
  vector<string> Parameters;
  bool Started=false;
  for(size_t Pos=0;Pos<Input.size();++Pos){
    if(Started){
      if(((Input[Pos]).size())&&((Input[Pos]).c_str())[0]==':')
	break;
      Parameters.push_back(Input[Pos]);
    }
    else{
      if(Param==Input[Pos])
	Started=true;
    }
  }
  return Parameters;
}

vector<double> Chocky::GetFloatParams(vector<string> Input){
  vector<double> Converted;
  for(size_t _=0;_<Input.size();++_){
    if(SetTheoryIsElem(CommandName,Input[_]))
      break;
    Converted=List::VecAdd(Converted,GetFloatParam(Input[_]));
  }
  return Converted;
}

vector<int> Chocky::GetIntParams(vector<string> Input){
  vector<int> Converted;
  for(size_t _=0;_<Input.size();++_){
    if(SetTheoryIsElem(CommandName,Input[_]))
      break;
    Converted=List::VecAdd(Converted,GetIntParam(Input[_]));
  }
  return Converted;
}

vector<string> Chocky::GetStringParams(vector<string> Input){
  vector<string> Converted={};
  for(size_t _=0;_<Input.size();++_){
    if(SetTheoryIsElem(CommandName,Input[_]))
      break;
    Converted=List::VecAdd(Converted,GetStringParam(Input[_]));
  }
  return Converted;
}

vector<double> Chocky::GetFloatParam(string Input){
  vector<double> Converted;
  if(Input.size()&&Input.c_str()[0]=='$')
    Converted=List::List2Floats(List::String2List(GetVariable(Input)));
  else
    Converted.push_back(atof(Input.c_str()));
  return Converted;
}

vector<int> Chocky::GetIntParam(string Input){
  vector<int> Converted;
  if(Input.size()&&Input.c_str()[0]=='$')
    Converted=List::List2Ints(List::String2List(GetVariable(Input)));
  else
    Converted.push_back(atoi(Input.c_str()));
  return Converted;
}

vector<string> Chocky::GetStringParam(string Input){
  vector<string> Converted;
  if(Input.size()&&Input.c_str()[0]=='$')
    Converted=List::String2List(GetVariable(Input));
  else
    Converted.push_back(Input);
  return Converted;
}

vector<string> Chocky::Maths2RPN(vector<string> Input){
  if(!MathsSyntaxOK(Input))
    return {};
  
  /* Need to evaluate the inner-most parentheses first - recursion 
     PEMDAS
   */

}

/*
  From ENiX3:
  
  <ENiX> =[Datablock 00114]==========================================                                                                                     
  <ENiX> Name          grammarenglish Grammar                       ?                                                                                     
  <ENiX> Purpose                    ? Emotion                       ?                                                                                     
  <ENiX> First Time          22:41:49-2015/11/08                                                                                                          
  <ENiX> Last Time           22:28:17-2016/01/09                                                                                                          
  <ENiX> Probability                1 Commands startsentence endsentence verb noun pronoun preposition adverb adjective definitearticle indefinitearticle 
  <ENiX> propernoun conjunction questionmark comma fullstop semicolon exclamationmark quotationmark                                                       
  <ENiX> -[Comparisons]----------------------------------------------
*/

vector<string> Chocky::ExpandOnce(vector<string> Covered,string Input){
  if(SetTheoryIsElem(Covered,Input)){
    Errors.push_back("Warning: Ignoring paradox or cyclic definition: "+Input);
    return {};
  }
  vector<string> Expanded={};
  Covered.push_back(Input);

  FindOrCreate(Input);
  WMS_Rec Record=GetFromCache(Input);
  
  string Substitution="`";
  if((Record.Contents.size())&&((Record.Contents[0]).size()>7)){
    string SubsData((Record.Contents[0])[7].Str,(Record.Contents[0])[7].Size);
    Substitution=SubsData;
  }
  if(Substitution=="`")
    Expanded.push_back(Input);
  else{
    vector<string> Expansion=DivideCompletely(Substitution);
    for(size_t _=0;_<Expansion.size();_++){
      if(Expansion[_].c_str()[0]==':')
	Expanded.push_back(Expansion[_]);
      else{
	vector<string> Retrieved=ExpandOnce(Covered,Expansion[_]);
	if(Retrieved.size()==0)
	  Expanded.push_back(Expansion[_]);
	else
	  Expanded=VecAdd(Expanded,Retrieved);
      }
    }
  }
  return Expanded;
}

void Chocky::PopulateCommandParameters(){

  CommandName.push_back("+");
  InstructionSet.push_back(&Chocky::Add);
  Parameters.push_back(3);
  OnlineHelp.push_back("A    +             B      Add B to vector / numbers, A.");

  CommandName.push_back("-");
  InstructionSet.push_back(&Chocky::Subtract);
  Parameters.push_back(3);
  OnlineHelp.push_back("A    -             B      Subtract B from vector / numbers, A.");

  CommandName.push_back("/");
  InstructionSet.push_back(&Chocky::Divide);
  Parameters.push_back(3);
  OnlineHelp.push_back("A    /             B      Divide vector / numbers, A, by B.");

  CommandName.push_back("*");
  InstructionSet.push_back(&Chocky::Multiply);
  Parameters.push_back(3);
  OnlineHelp.push_back("A    *             B      Multiply vector / numbers, A, by B.");

  CommandName.push_back("^");
  InstructionSet.push_back(&Chocky::Exponent);
  Parameters.push_back(3);
  OnlineHelp.push_back("A    ^             B      Raise vector / numbers, A, by B.");

  CommandName.push_back("%");
  InstructionSet.push_back(&Chocky::Modulus);
  Parameters.push_back(3);
  OnlineHelp.push_back("A    %             B      Compute the modulus of A in base, B.");

  CommandName.push_back("=");
  InstructionSet.push_back(&Chocky::Assign);
  Parameters.push_back(3);
  OnlineHelp.push_back("A    =             B      Assign value B to variable or record A.");

  CommandName.push_back("==");
  InstructionSet.push_back(&Chocky::Equals);
  Parameters.push_back(3);
  OnlineHelp.push_back("A    ==            B      Saves 1 to ANSWER if true, 0 otherwise.");

  CommandName.push_back("<>");
  InstructionSet.push_back(&Chocky::NotEqual);
  Parameters.push_back(3);
  OnlineHelp.push_back("A    <>             B      Saves 1 to ANSWER if true, 0 otherwise.");

  CommandName.push_back("<");
  InstructionSet.push_back(&Chocky::LessThan);
  Parameters.push_back(3);
  OnlineHelp.push_back("A    <              B      Saves 1 to ANSWER if true, 0 otherwise.");

  CommandName.push_back("<=");
  InstructionSet.push_back(&Chocky::LessThanEqual);
  Parameters.push_back(3);
  OnlineHelp.push_back("A    <=             B      Saves 1 to ANSWER if true, 0 otherwise.");

  CommandName.push_back(">");
  InstructionSet.push_back(&Chocky::GreaterThan);
  Parameters.push_back(3);
  OnlineHelp.push_back("A    >              B      Saves 1 to ANSWER if true, 0 otherwise.");

  CommandName.push_back(">=");
  InstructionSet.push_back(&Chocky::GreaterThanEqual);
  Parameters.push_back(3);
  OnlineHelp.push_back("A    >=             B      Saves 1 to ANSWER if true, 0 otherwise.");

  CommandName.push_back("<=>");
  InstructionSet.push_back(&Chocky::Swap);
  Parameters.push_back(3);
  OnlineHelp.push_back("A    <=>            B      Move data, A until after the command following the command following B.");

  CommandName.push_back(":aavf");
  InstructionSet.push_back(&Chocky::AddActiveVerbForm);
  Parameters.push_back(3);
  OnlineHelp.push_back("A    :AAVF          B      Sets active verb, B, with name, A.");

  CommandName.push_back(":all");
  InstructionSet.push_back(&Chocky::GetAllRecs);
  Parameters.push_back(0);
  OnlineHelp.push_back("NULL :ALL           NULL   Displays all records in the personality profile.");

  CommandName.push_back(":answer");
  InstructionSet.push_back(&Chocky::PrintAnswer);
  Parameters.push_back(0);
  OnlineHelp.push_back("[A]  :ANSWER        NULL   Shows the answer in the :ANSWER buffer.");

  CommandName.push_back(":apvf");
  InstructionSet.push_back(&Chocky::AddPassiveVerbForm);
  Parameters.push_back(3);
  OnlineHelp.push_back("A    :APVF          B      Sets passive verb, B, with name, A.");

  CommandName.push_back(":baseverb");
  InstructionSet.push_back(&Chocky::BaseVerb);
  Parameters.push_back(3);
  OnlineHelp.push_back("A    :BASEVERB      B      Sets the verbs, A, as having baseverb, B.");

  CommandName.push_back(":crashenix");
  InstructionSet.push_back(&Chocky::CrashENiX);
  Parameters.push_back(0);
  OnlineHelp.push_back("NULL :CRASHENIX     NULL   Force ENiX to segfault.");
  
  CommandName.push_back(":csaddr");
  InstructionSet.push_back(&Chocky::CSAddr);
  Parameters.push_back(1);
  OnlineHelp.push_back("A    :CSADDR        NULL   Saves the CS address into the :ANSWER buffer.");

  CommandName.push_back(":csload");
  InstructionSet.push_back(&Chocky::CSLoad);
  Parameters.push_back(1);
  OnlineHelp.push_back("A    :CSLOAD        NULL   Saves the contents in the address A to the :ANSWER buffer.");

  CommandName.push_back(":csrecon");
  InstructionSet.push_back(&Chocky::CSRecon);
  Parameters.push_back(1);
  OnlineHelp.push_back("A    :CSRECON       NULL   Reconstructs the context address from A and places in :ANSWER.");

  CommandName.push_back(":cssave");
  InstructionSet.push_back(&Chocky::CSSave);
  Parameters.push_back(3);
  OnlineHelp.push_back("A    :CSSAVE        B      Stores data, B in context address, A.");

  CommandName.push_back(":debug");
  InstructionSet.push_back(&Chocky::DebugRec);
  Parameters.push_back(1);
  OnlineHelp.push_back("A    :DEBUG         NULL   Shows the database records, A.");

  CommandName.push_back(":delete");
  InstructionSet.push_back(&Chocky::Delete);
  Parameters.push_back(1);
  OnlineHelp.push_back("A    :DELETE        NULL   Delete database records, A.");

  CommandName.push_back(":domain");
  InstructionSet.push_back(&Chocky::Domain);
  Parameters.push_back(1);
  OnlineHelp.push_back("A    :DOMAIN        NULL   Creates a domain from 2 numbers, A.");

  CommandName.push_back(":endpunct");
  InstructionSet.push_back(&Chocky::EndPunct);
  Parameters.push_back(1);
  OnlineHelp.push_back("A    :ENDPUNCT      NULL   Set the end punctionation in English language.");
  
  CommandName.push_back(":errors");
  InstructionSet.push_back(&Chocky::PrintErrors);
  Parameters.push_back(0);
  OnlineHelp.push_back("NULL :ERRORS        NULL   Shows the errors from the current session.");

  CommandName.push_back(":execute");
  InstructionSet.push_back(&Chocky::NNExecute);
  Parameters.push_back(3);
  OnlineHelp.push_back("A    :EXECUTE       B      Retrieves neural net B, and executes it on domain, A.");

  CommandName.push_back(":help");
  InstructionSet.push_back(&Chocky::CommandHelp);
  Parameters.push_back(0);
  OnlineHelp.push_back("NULL :HELP          [B]    Shows a list of help commands if there is no A specified, otherwise shows the syntax for a specific command.");

  CommandName.push_back(":if");
  InstructionSet.push_back(&Chocky::If);
  Parameters.push_back(1);
  OnlineHelp.push_back("A :IF NULL [:else]  :FI    Run ... if B is true else run the alternate");

  CommandName.push_back(":learn");
  InstructionSet.push_back(&Chocky::Learn);
  Parameters.push_back(3);
  OnlineHelp.push_back("A    :LEARN         B      Learn a neural network from data, A and save it to concept, B.");

  CommandName.push_back(":listvars");
  InstructionSet.push_back(&Chocky::ListVariables);
  Parameters.push_back(0);
  OnlineHelp.push_back("NULL :LISTVARS      NULL   Shows all the data in variables.");

  CommandName.push_back(":mode");
  InstructionSet.push_back(&Chocky::ShowMode);
  Parameters.push_back(0);
  OnlineHelp.push_back("NULL :MODE          NULL   Shows ENiX4 settings.");

  CommandName.push_back(":nnclear");
  InstructionSet.push_back(&Chocky::NNClear);
  Parameters.push_back(1);
  OnlineHelp.push_back("A    :NNCLEAR       NULL   Saves the neural network multiplexor, A.");

  CommandName.push_back(":nnadd");
  InstructionSet.push_back(&Chocky::NNAdd);
  Parameters.push_back(3);
  OnlineHelp.push_back("A    :NNADD         B      Adds :CAUSE... :EFFECT... to multiplexor, A.");

  CommandName.push_back(":nnmul");
  InstructionSet.push_back(&Chocky::NNMul);
  Parameters.push_back(1);
  OnlineHelp.push_back("A    :NNMUL         NULL   Multiplex neural network, A.");

  CommandName.push_back(":nngetcause");
  InstructionSet.push_back(&Chocky::NNGetCause);
  Parameters.push_back(3);
  OnlineHelp.push_back("A    :NNGETCAUSE    B      Calculate cause from effect, B in A.");

  CommandName.push_back(":nngeteffect");
  InstructionSet.push_back(&Chocky::NNGetEffect);
  Parameters.push_back(3);
  OnlineHelp.push_back("A    :NNGETEFFECT   B      Calculate effect from cause, B in A.");

  CommandName.push_back(":nnremove");
  InstructionSet.push_back(&Chocky::NNRemove);
  Parameters.push_back(3);
  OnlineHelp.push_back("A    :NNREMOVE      B      Remove cause, B from A.");

  CommandName.push_back(":oddman");
  InstructionSet.push_back(&Chocky::Oddman);
  Parameters.push_back(1);
  OnlineHelp.push_back("A    :ODDMAN        NULL   Saves the oddman out of sequence A.");

  CommandName.push_back(":output");
  InstructionSet.push_back(&Chocky::PrintOutput);
  Parameters.push_back(0);
  OnlineHelp.push_back("NULL :OUTPUT        NULL   Shows command output from the current session.");

  CommandName.push_back(":polycalc");
  InstructionSet.push_back(&Chocky::CalcPoly);
  Parameters.push_back(3);
  OnlineHelp.push_back("A    :POLYCALC      B      Retrieves a polynomial B, and executes it on domain, A.");

  CommandName.push_back(":polycreate");
  InstructionSet.push_back(&Chocky::CreatePoly);
  Parameters.push_back(3);
  OnlineHelp.push_back("A    :POLYCREATE    B      Creates a polynomial from A and saves it to B.");

  CommandName.push_back(":polyget");
  InstructionSet.push_back(&Chocky::GetPoly);
  Parameters.push_back(1);
  OnlineHelp.push_back("A    :POLYGET       NULL   Displays the polynomial for A.");

  CommandName.push_back(":polyset");
  InstructionSet.push_back(&Chocky::SetPoly);
  Parameters.push_back(3);
  OnlineHelp.push_back("A    :POLYSET       B      Save polynomial A to record B.");

  CommandName.push_back(":predarticles");
  InstructionSet.push_back(&Chocky::PredArticles);
  Parameters.push_back(1);
  OnlineHelp.push_back("NULL :PREDARTICLES  NULL   Specifies a list of predicate articles, A.");

  CommandName.push_back(":resetgrammar");
  InstructionSet.push_back(&Chocky::ResetGrammar);
  Parameters.push_back(0);
  OnlineHelp.push_back("NULL :RESETGRAMMAR  NULL   Resets the language grammars so it can be retrained.");

  CommandName.push_back(":save");
  InstructionSet.push_back(&Chocky::Save);
  Parameters.push_back(0);
  OnlineHelp.push_back("NULL :SAVE          NULL   Save all data to database.");

  CommandName.push_back(":savesettings");
  InstructionSet.push_back(&Chocky::SaveSettings);
  Parameters.push_back(0);
  OnlineHelp.push_back("NULL :SAVESETTINGS  NULL   Shows command output from the current session.");

  CommandName.push_back(":senclass");
  InstructionSet.push_back(&Chocky::ClassifySentence);
  Parameters.push_back(1);
  OnlineHelp.push_back("A    :SENCLASS      NULL   Use directed graphs to classify sentence, A and save the type to :ANSWER.");

  CommandName.push_back(":sentemplate");
  InstructionSet.push_back(&Chocky::SenTemplate);
  Parameters.push_back(2);
  OnlineHelp.push_back("A    :SENTEMPLATE   B      Save the template, A to sentence mood, B.");

  CommandName.push_back(":sepparagraph");
  InstructionSet.push_back(&Chocky::SepParagraph);
  Parameters.push_back(1);
  OnlineHelp.push_back("A    :SEPPARAGRAPH  NULL   Take the paragraph, A, and separate it into sentences.");

  CommandName.push_back(":sentencedebug");
  InstructionSet.push_back(&Chocky::SentenceDebug);
  Parameters.push_back(1);
  OnlineHelp.push_back("A    :SENTENCEDEBUG NULL   Show analytics of sentence, A.");

  CommandName.push_back(":setcondcause");
  InstructionSet.push_back(&Chocky::SetCondCause);
  Parameters.push_back(1);
  OnlineHelp.push_back("A    :SETCONDCAUSE  NULL   Set tokens, A, as conditionals.");

  CommandName.push_back(":setcondeffect");
  InstructionSet.push_back(&Chocky::SetCondEffect);
  Parameters.push_back(1);
  OnlineHelp.push_back("A    :SETCONDEFFECT NULL   Set tokens, A, as conditionals.");

  CommandName.push_back(":setlogic");
  InstructionSet.push_back(&Chocky::SetLogic);
  Parameters.push_back(1);
  OnlineHelp.push_back("A    :SETLOGIC      NULL   Set words, A, as conjunctions including not and neither.");

  CommandName.push_back(":setgrammar");
  InstructionSet.push_back(&Chocky::SetGrammar);
  Parameters.push_back(3);
  OnlineHelp.push_back("A    :SETGRAMMAR    B      Set words, A, as being grammar articles, B.");

  CommandName.push_back(":setarticles");
  InstructionSet.push_back(&Chocky::SetArticles);
  Parameters.push_back(1);
  OnlineHelp.push_back("A    :SETARTICLES   NULL   Register articles, A into the language grammar.");

  CommandName.push_back(":spa");
  InstructionSet.push_back(&Chocky::SetPassiveAgent);
  Parameters.push_back(1);
  OnlineHelp.push_back("A    :SPA           NULL   Set passive agents, A.");

  CommandName.push_back(":setsenstart");
  InstructionSet.push_back(&Chocky::SetSenStart);
  Parameters.push_back(1);
  OnlineHelp.push_back("A    :SETSENSTART   NULL   Register keyword, A as the start sentence token.");

  CommandName.push_back(":setsenend");
  InstructionSet.push_back(&Chocky::SetSenEnd);
  Parameters.push_back(1);
  OnlineHelp.push_back("A    :SETSENEND     NULL   Register keyword, A as the end sentence token.");

  CommandName.push_back(":setwords");
  InstructionSet.push_back(&Chocky::SetWords);
  Parameters.push_back(1);
  OnlineHelp.push_back("A    :SETWORDS      NULL   Register word articles, A into the language grammar.");

  CommandName.push_back(":setmood");
  InstructionSet.push_back(&Chocky::SetMood);
  Parameters.push_back(3);
  OnlineHelp.push_back("A    :SETMOOD       B      Populates grammar graph, B, with sentence data A.");

  CommandName.push_back(":setmoodlist");
  InstructionSet.push_back(&Chocky::SetMoodList);
  Parameters.push_back(1);
  OnlineHelp.push_back("A    :SETMOODLIST   NULL   Populates grammar graph, B, with sentence data A.");

  CommandName.push_back(":showgrammar");
  InstructionSet.push_back(&Chocky::ShowGrammar);
  Parameters.push_back(0);
  OnlineHelp.push_back("NULL :SHOWGRAMMAR   NULL   Displays the grammar graph data.");
  
  CommandName.push_back(":showneural");
  InstructionSet.push_back(&Chocky::ShowNeural);
  Parameters.push_back(1);
  OnlineHelp.push_back("A    :SHOWNEURAL    NULL   Shows a diagram of the neural network stored in record A.");

  CommandName.push_back(":showtemplate");
  InstructionSet.push_back(&Chocky::ShowTemplate);
  Parameters.push_back(0);
  OnlineHelp.push_back("NULL :SHOWTEMPLATE  NULL   Shows the grammar templates.");

  CommandName.push_back(":showverbforms");
  InstructionSet.push_back(&Chocky::ShowVerbForms);
  Parameters.push_back(0);
  OnlineHelp.push_back("NULL :SHOWVERBFORMS NULL   Shows the active and passive verb forms.");

  CommandName.push_back(":sort");
  InstructionSet.push_back(&Chocky::Sort);
  Parameters.push_back(1);
  OnlineHelp.push_back("A    :SORT          NULL   Saves a sorted list of A to :ANSWER.");

  CommandName.push_back(":subjarticles");
  InstructionSet.push_back(&Chocky::SubjArticles);
  Parameters.push_back(1);
  OnlineHelp.push_back("NULL :SUBJARTICLES  NULL   Specifies a list of subject articles, A.");

  CommandName.push_back(":subjectorder");
  InstructionSet.push_back(&Chocky::SubjectOrder);
  Parameters.push_back(1);
  OnlineHelp.push_back("A    :SUBJECTORDER  NULL   Specify the canonical order, A, of the subject.");

  CommandName.push_back(":syntax");
  InstructionSet.push_back(&Chocky::Syntax);
  Parameters.push_back(0);
  OnlineHelp.push_back("NULL :SYNTAX        NULL   Shows language command syntax.");

  CommandName.push_back(":tense");
  InstructionSet.push_back(&Chocky::Tense);
  Parameters.push_back(3);
  OnlineHelp.push_back("A    :TENSE         B      Set words A, as tense, B.");

  CommandName.push_back(":test");
  InstructionSet.push_back(&Chocky::Test);
  Parameters.push_back(0);
  OnlineHelp.push_back("NULL :TEST          NULL   Internal testing function - do not use.");

  CommandName.push_back(":understood");
  InstructionSet.push_back(&Chocky::Understood);
  Parameters.push_back(1);
  OnlineHelp.push_back("A    :UNDERSTOOD    NULL   True if sequence A makes sense, false otherwise.");

  CommandName.push_back(":verborder");
  InstructionSet.push_back(&Chocky::VerbOrder);
  Parameters.push_back(1);
  OnlineHelp.push_back("A    :VERBORDER     NULL   Specify the canonical order, A, of verbs.");

  CommandName.push_back(":while");
  InstructionSet.push_back(&Chocky::While);
  Parameters.push_back(1);
  OnlineHelp.push_back("A :WHILE NULL [:else] :FI  Run ... if B is true else run the alternate");
  
}

/*
  Command to force ENiX to crash. Used for testing purposes only.
 */
vector<string> Chocky::CrashENiX(vector<string> Input1,vector<string> Input2){
  char ShortBuffer[1];
  memset(ShortBuffer,0,512);
  return Input2;
}

vector<string> Chocky::EndPunct(vector<string> Input1,vector<string> Input2){
  vector<string> PrefixSymbols=GetStringParams(Input1);
  EndPunctuation=PrefixSymbols;
  return Input2;
}

vector<string> Chocky::SepParagraph(vector<string> Input1,vector<string> Input2){
  vector<string> PrefixSymbols=GetStringParams(Input1);
  vector<vector<string>> Separated=SeparateParagraph(PrefixSymbols);
  vector<string> Found;
  for(size_t _=0;_<Separated.size();++_){
    Found.push_back(List::List2String(Separated[_]));
  }
  ANSWER=Found;  
  return Input2;
}

vector<string> Chocky::SetCondCause(vector<string> Input1,vector<string> Input2){
  vector<string> PrefixSymbols=GetStringParams(Input1);
  CondCauseTokens=PrefixSymbols;
  return Input2;
}

vector<string> Chocky::SetCondEffect(vector<string> Input1,vector<string> Input2){
  vector<string> PrefixSymbols=GetStringParams(Input1);
  CondEffectTokens=PrefixSymbols;
  return Input2;
}

vector<string> Chocky::BaseVerb(vector<string> Input1,vector<string> Input2){
  vector<string> PrefixSymbols=GetStringParams(Input1);
  vector<string> SuffixSymbols=GetStringParams(Input2);
  for(size_t _=0;_<PrefixSymbols.size();++_){
    string Suffix=SuffixSymbols[0];
    FindOrCreate(PrefixSymbols[_]);
    WMS_Rec VerbRec=GetFromCache(PrefixSymbols[_]);    
    VerbRec=AddDBPair(VerbRec,":baseverb",Suffix);
    RepInCache(VerbRec);
  }
  return Input2;
}

vector<string> Chocky::Tense(vector<string> Input1,vector<string> Input2){
  vector<string> PrefixSymbols=GetStringParams(Input1);
  vector<string> SuffixSymbols=GetStringParams(Input2);
  for(size_t _=0;_<PrefixSymbols.size();++_)
    SetTense(PrefixSymbols[_],SuffixSymbols[0]);
  return Input2;
}

vector<string> Chocky::Delete(vector<string> Input1,vector<string> Input2){
  vector<string> PrefixCommands=GetStringParams(Input1);
  for(size_t _=0;_<PrefixCommands.size();++_){
    DelAmoebaRec(PrefixCommands[_]);
    RemoveFromCache(PrefixCommands[_]);
  }
  return Input2;
}

vector<string> Chocky::Test(vector<string> Input1,vector<string> Input2){

  vector<string> TestData1={"{{test123.?!"};
  vector<string> TestData2=List::SeparateSymbols(TestData1);

  DEBUGMSG;
  List::PrintList(TestData2);
  
  TestData1={"{"};
  TestData2=List::SeparateSymbols(TestData1);

  DEBUGMSG;
  List::PrintList(TestData2);

  TestData1={"}"};
  TestData2=List::SeparateSymbols(TestData1);

  DEBUGMSG;
  List::PrintList(TestData2);

  TestData1={"{}123Test.wib ble!1"};
  TestData2=List::PrepareInput(TestData1);

  DEBUGMSG;
  List::PrintList(TestData2);
  
  /*
  vector<string> TestData={"a"};
  
  List::PrintList(TestData);
  string CSRefData=CSRef(TestData);
  printf("%s\n",CSRefData.c_str());
  
  puts("----");

  TestData={"a","b","c","d"};
  
  List::PrintList(TestData);
  CSRefData=CSRef(TestData);
  
  printf("%s\n",CSRefData.c_str());
  
  DEBUGMSG;
  vector<string> Dereferenced=CSDeref("cs9");
  List::PrintList(Dereferenced);

  TestData={"a","b","c","e"};
  
  List::PrintList(TestData);
  CSRefData=CSRef(TestData);
  
  printf("%s\n",CSRefData.c_str());

  DEBUGMSG;
  Dereferenced=CSDeref("cs10");
  List::PrintList(Dereferenced);

  */
  return Input2;
}

vector<string> Chocky::VerbOrder(vector<string> Input1,vector<string> Input2){
  vector<string> PrefixCommands=GetStringParams(Input1);  
  VerbOrderList=PrefixCommands;
  return Input2;
}

vector<string> Chocky::SubjectOrder(vector<string> Input1,vector<string> Input2){
  vector<string> PrefixCommands=GetStringParams(Input1);
  SubjectOrderList=PrefixCommands;
  return Input2;
}

vector<string> Chocky::SetLogic(vector<string> Input1,vector<string> Input2){
  vector<string> PrefixCommands=GetStringParams(Input1);
  LogicList=PrefixCommands;
  return Input2;
}

vector<string> Chocky::SetPassiveAgent(vector<string> Input1,vector<string> Input2){
  vector<string> PrefixCommands=GetStringParams(Input1);
  PassiveAgents=PrefixCommands;
  return Input2;
}

vector<string> Chocky::ShowVerbForms(vector<string> Input1,vector<string> Input2){
  DisplayOutput.push_back(SAVF());
  DisplayOutput.push_back(SPVF());
  return Input2;
}

vector<string> Chocky::AddActiveVerbForm(vector<string> Input1,vector<string> Input2){
  vector<string> PrefixCommands=GetStringParams(Input1);
  vector<string> SuffixCommands=GetStringParams(Input2);  
  AAVF(SuffixCommands,PrefixCommands);
  Input2=ShiftVector(Input2,SuffixCommands.size());
  return Input2;
}

vector<string> Chocky::AddPassiveVerbForm(vector<string> Input1,vector<string> Input2){
  vector<string> PrefixCommands=GetStringParams(Input1);
  vector<string> SuffixCommands=GetStringParams(Input2);
  APVF(SuffixCommands,PrefixCommands);  
  Input2=ShiftVector(Input2,SuffixCommands.size());
  return Input2;
}

vector<string> Chocky::SubjArticles(vector<string> Input1,vector<string> Input2){
  vector<string> PrefixCommands=GetStringParams(Input1);
  SubjectArticles=PrefixCommands;
  return Input2;
}

vector<string> Chocky::PredArticles(vector<string> Input1,vector<string> Input2){
  vector<string> PrefixCommands=GetStringParams(Input1);
  PredicateArticles=PrefixCommands;
  return Input2;
}

vector<string> Chocky::SentenceDebug(vector<string> Input1,vector<string> Input2){
  vector<string> PrefixCommands=GetStringParams(Input1);
  string Data=SentenceDebug(PrefixCommands);
  DisplayOutput.push_back(Data);
  return Input2;
}

vector<string> Chocky::ShowTemplate(vector<string> Input1,vector<string> Input2){
  DisplayOutput.push_back("SentenceTemplates:");
  map<string,vector<vector<string>>>::iterator Templates;
  for(Templates=SentenceTemplates.begin();Templates!=SentenceTemplates.end();++Templates){
    DisplayOutput.push_back(Templates->first);
    vector<vector<string>> TemplateData=Templates->second;
    vector<vector<string>>::iterator TemplateDataIter;    
    string ToPrint;
    for(TemplateDataIter=TemplateData.begin();TemplateDataIter!=TemplateData.end();++TemplateDataIter){
      ToPrint=ToPrint+List::List2String(*TemplateDataIter);
      if(TemplateDataIter+1!=TemplateData.end())
	ToPrint=ToPrint+",";
    }
    if(ToPrint!="")
      DisplayOutput.push_back(ToPrint);
  }
  return Input2;
}

vector<string> Chocky::SenTemplate(vector<string> Input1,vector<string> Input2){
  vector<string> PrefixCommands=GetStringParams(Input1);
  vector<string> SuffixCommands=GetStringParams(Input2);
  if(!AddToTemplates(Input2[0],PrefixCommands)){
    string Error="ERROR: template is not in s/pv/pn notation. Unable to continue.";
    DisplayOutput.push_back(Error);
  }
  Input2=ShiftVector(Input2,SuffixCommands.size());
  return Input2;  
}

vector<string> Chocky::Save(vector<string> Input1,vector<string> Input2){
  Mode2WMS();
  Write2Amoeba(CachedContents);
  return Input2;
}

vector<string> Chocky::ClassifySentence(vector<string> Input1,vector<string> Input2){
  vector<string> PrefixCommands=GetStringParams(Input1);
  vector<vector<string>> SentenceArticles=SentenceToArticles(PrefixCommands);
  string ThisSentenceMood=GetSentenceMood();
  if(ThisSentenceMood!="")
    ANSWER=List::String2List(ThisSentenceMood,false);
  return Input2;
}

vector<string> Chocky::ResetGrammar(vector<string> Input1,vector<string> Input2){
  GrammarGraphs={};
  SentenceTemplates={};
  ActiveVerbForms={};
  PassiveVerbForms={};
  PassiveAgents={};
  return Input2;
}

vector<string> Chocky::SetSenStart(vector<string> Input1,vector<string> Input2){
  vector<string> PrefixCommands=GetStringParams(Input1);
  if(PrefixCommands.size())
    StartSentenceToken=PrefixCommands[0];
  return Input2;
}

vector<string> Chocky::SetSenEnd(vector<string> Input1,vector<string> Input2){
  vector<string> PrefixCommands=GetStringParams(Input1);
  if(PrefixCommands.size())
    EndSentenceToken=PrefixCommands[0];
  return Input2;
}

vector<string> Chocky::ShowMode(vector<string> Input1,vector<string> Input2){
  FindOrCreate(UniqueUN);
  WMS_Rec Rec=GetFromCache(UniqueUN);
  vector<string> RecordData=WMSConversion::WMS2Vec(Rec);
  for(size_t LineId=0;LineId<RecordData.size();++LineId)
    DisplayOutput.push_back(RecordData[LineId]);  
  return Input2;
}

vector<string> Chocky::SetWords(vector<string> Input1,vector<string> Input2){
  vector<string> PrefixCommands=GetStringParams(Input1);
  WordArticleList=PrefixCommands;
  return Input2;
}

vector<string> Chocky::SetGrammar(vector<string> Input1,vector<string> Input2){
  vector<string> SuffixCommands=GetStringParams(Input2);
  vector<string> PrefixCommands=GetStringParams(Input1);
  vector<string> ValidArticles=ArticleList;
  for(size_t _=0;_<SuffixCommands.size();++_){
    if(!List::SetTheoryIsElem(ValidArticles,SuffixCommands[_])){
      string Warning="WARNING: \"";
      Warning=Warning+SuffixCommands[_];
      Warning=Warning+"\" is not registered in :setarticles. Continuing anyway...";
      DisplayOutput.push_back(Warning);
    }
  }  
  string ToReplace=List::List2String(SuffixCommands);
  for(size_t _=0;_<PrefixCommands.size();++_){
    FindOrCreate(PrefixCommands[_]);
    WMS_Rec Concept=GetFromCache(PrefixCommands[_]);
    Concept=RepHeader(Concept,ToReplace,1);
    RepInCache(Concept);
  }
  Input2=ShiftVector(Input2,SuffixCommands.size());
  return Input2;
}

vector<string> Chocky::SetArticles(vector<string> Input1,vector<string> Input2){
  vector<string> PrefixCommands=GetStringParams(Input1);
  ArticleList=PrefixCommands;
  return Input2;
}

vector<string> Chocky::SetMood(vector<string> Input1,vector<string> Input2){
  vector<string> SuffixCommands=GetStringParams(Input2);
  vector<string> PrefixCommands=GetStringParams(Input1);
  if(SuffixCommands.size()){
    AddToDirectedGraph(PrefixCommands,SuffixCommands[0]);
    Input2=ShiftVector(Input2,SuffixCommands.size());
  }
  return Input2;
}

vector<string> Chocky::SetMoodList(vector<string> Input1,vector<string> Input2){
  vector<string> PrefixCommands=GetStringParams(Input1);
  MoodList=PrefixCommands;
  return Input2;
}

vector<string> Chocky::ShowGrammar(vector<string> Input1,vector<string> Input2){
  string Data=DisplayGraphs();
  DisplayOutput.push_back(Data);
  return Input2;
}

vector<string> Chocky::Sort(vector<string> Input1,vector<string> Input2){
  vector<string> PrefixCommands=GetStringParams(Input1);
  ANSWER=List::SortList(PrefixCommands);
  return Input2;
}

vector<string> Chocky::NNClear(vector<string> Input1,vector<string> Input2){
  vector<string> PrefixCommands=GetStringParams(Input1);
  MultiplexClear(PrefixCommands[0]);
  return Input2;
}

vector<string> Chocky::NNAdd(vector<string> Input1,vector<string> Input2){
  vector<string> PrefixCommands=GetStringParams(Input1);
  if(PrefixCommands.size()){
    vector<string> Cause=GetStringParams(GetParamsOf(":cause",Input2));
    vector<string> Effect=GetStringParams(GetParamsOf(":effect",Input2));
    MultiplexAdd(PrefixCommands[0],Cause,Effect);
  }
  return Input2;
}

vector<string> Chocky::NNMul(vector<string> Input1,vector<string> Input2){
  vector<string> PrefixCommands=GetStringParams(Input1);
  if(PrefixCommands.size()){
    string ConceptName=PrefixCommands[0];
    vector<string> NeuralSeqToLearn=MultiplexGen(ConceptName);
    ANSWER=NeuralSeqToLearn;
    string NeuralNet=LearnNN(NeuralSeqToLearn);
    FindOrCreate(ConceptName);
    WMS_Rec Concept=GetFromCache(ConceptName);
    Concept=RepDBPair(Concept,":NEURALNET",NeuralNet);
    RepInCache(Concept);
  }
  return Input2;  
}

/* Note this will return multiple causes */
vector<string> Chocky::NNGetCause(vector<string> Input1,vector<string> Input2){
  vector<string> PrefixCommands=GetStringParams(Input1);
  if(PrefixCommands.size()){
    vector<string> SuffixCommands=GetStringParams(Input2);

    // We start by encoding the effect
    int Effect=MultiplexEncode(PrefixCommands[0],SuffixCommands,CauseOrEffect::Effect);

    // We need to create the domain
    vector<string> TPIs=List::Int2List(CreateDomain(PrefixCommands[0],CauseOrEffect::Cause));

    // We load in the neural network
    FindOrCreate(PrefixCommands[0]);
    WMS_Rec Concept=GetFromCache(PrefixCommands[0]);
    string NeuralData=GetDBValue(Concept,":NEURALNET");
    vector<string> NeuralNet=List::String2List(NeuralData);

    // And calculate and decode our cause into an effect
    string CalculatedStr=ExecuteNN(NeuralNet,TPIs);
    vector<int> Calculated=List::List2Ints(List::String2List(CalculatedStr));
    if(Calculated.size()){
      ANSWER={};
      for(size_t _=0;_<Calculated.size();++_){
	if(Calculated[_]==Effect){
	  vector<string> Decoded=MultiplexDecode(PrefixCommands[0],_,CauseOrEffect::Cause);
	  if(Decoded.size())
	    ANSWER.push_back(List::List2String(Decoded));
	  else
	    ANSWER.push_back("NOTHING");
	}
      }
    }
    Input2=ShiftVector(Input2,SuffixCommands.size());
  }
  return Input2;  
}

vector<string> Chocky::NNGetEffect(vector<string> Input1,vector<string> Input2){
  vector<string> PrefixCommands=GetStringParams(Input1);
  if(PrefixCommands.size()){
    vector<string> SuffixCommands=GetStringParams(Input2);

    // We start by encoding the cause
    int TPI=MultiplexEncode(PrefixCommands[0],SuffixCommands,CauseOrEffect::Cause);
    vector<string> TPIs=List::String2List(BinData::Int2String(TPI));

    // We load in the neural network
    FindOrCreate(PrefixCommands[0]);
    WMS_Rec Concept=GetFromCache(PrefixCommands[0]);
    string NeuralData=GetDBValue(Concept,":NEURALNET");
    vector<string> NeuralNet=List::String2List(NeuralData);

    // And calculate and decode our cause into an effect
    string CalculatedStr=ExecuteNN(NeuralNet,TPIs);
    vector<int> Calculated=List::List2Ints(List::String2List(CalculatedStr));
    if(Calculated.size()){
      ANSWER=MultiplexDecode(PrefixCommands[0],Calculated[0],CauseOrEffect::Effect);
      if(!ANSWER.size())
	ANSWER=List::String2List("NOTHING");	
    }
    Input2=ShiftVector(Input2,SuffixCommands.size());
  }
  return Input2;  
}

vector<string> Chocky::NNRemove(vector<string> Input1,vector<string> Input2){
  vector<string> PrefixCommands=GetStringParams(Input1);
  if(PrefixCommands.size()){
    vector<string> Cause=GetStringParams(Input2);
    MultiplexRemove(PrefixCommands[0],Cause);
  }
  return Input2;  
}

vector<string> Chocky::CSAddr(vector<string> Input1,vector<string> Input2){
  vector<string> PrefixCommands=GetStringParams(Input1);
  if(PrefixCommands.size()){
    string Data=CSRef(PrefixCommands);
    if(Data!="")
      ANSWER=String2List(Data);
  }
  return Input2;
}

vector<string> Chocky::CSLoad(vector<string> Input1,vector<string> Input2){
  vector<string> PrefixCommands=GetStringParams(Input1);

  DEBUGMSG;

  if(PrefixCommands.size()){
    string Retrieved=CSGet(PrefixCommands);
    if(Retrieved!="")
      ANSWER=String2List(Retrieved);
  }
  return Input2;
}

vector<string> Chocky::CSRecon(vector<string> Input1,vector<string> Input2){
  vector<string> PrefixCommands=GetStringParams(Input1);
  if(PrefixCommands.size()){
    vector<string> Data=CSDeref(PrefixCommands[0]);
    if(Data.size())
      ANSWER=Data;
  }
  return Input2;
}

vector<string> Chocky::CSSave(vector<string> Input1,vector<string> Input2){
  vector<string> SuffixCommands=GetStringParams(Input2);
  vector<string> PrefixCommands=GetStringParams(Input1);

  DEBUGMSG;
  List::PrintList(SuffixCommands);
  List::PrintList(PrefixCommands);

  if(SuffixCommands.size()&&PrefixCommands.size()){
    DEBUGMSG;
    string SuffixData=SuffixCommands[0];
    CSSave(PrefixCommands,SuffixData);
  }
  Input2=ShiftVector(Input2,SuffixCommands.size());
  return Input2;
}

vector<string> Chocky::CreatePoly(vector<string> Input1,vector<string> Input2){
  vector<string> SuffixCommands=GetStringParams(Input2);
  vector<string> PrefixCommands=GetStringParams(Input1);
  ANSWER={};
  if(CreateEquation(PrefixCommands)){
    FindOrCreate(Input2[0]);
    WMS_Rec Concept=GetFromCache(Input2[0]);
    Concept=RepDBPair(Concept,":POLYNOMIAL",List::List2String(GetEquation()));
    RepInCache(Concept);
    ANSWER.push_back("1");
  }
  else
    ANSWER.push_back("0");
  Input2=ShiftVector(Input2,SuffixCommands.size());
  return Input2;
}

vector<string> Chocky::SetPoly(vector<string> Input1,vector<string> Input2){
  vector<string> SuffixCommands=GetStringParams(Input2);
  vector<double> PrefixCommands=GetFloatParams(Input1);
  FindOrCreate(Input2[0]);
  WMS_Rec Concept=GetFromCache(Input2[0]);
  Concept=RepDBPair(Concept,":POLYNOMIAL",List::List2String(PrefixCommands));
  RepInCache(Concept);
  Input2=ShiftVector(Input2,SuffixCommands.size());
  return Input2;
}

vector<string> Chocky::CalcPoly(vector<string> Input1,vector<string> Input2){
  vector<string> SuffixCommands=GetStringParams(Input2);
  vector<double> PrefixCommands=GetFloatParams(Input1);
  FindOrCreate(Input2[0]);
  WMS_Rec Concept=GetFromCache(Input2[0]);
  string PolynomialStr=GetDBValue(Concept,":POLYNOMIAL");
  vector<double> Polynomial=List::List2Num(List::String2List(PolynomialStr));
  SetEquation(Polynomial);
  ANSWER=List::String2List(List::List2String(CalculateSeq(PrefixCommands)));
  Input2=ShiftVector(Input2,SuffixCommands.size());
  return Input2;  
}

vector<string> Chocky::GetPoly(vector<string> Input1,vector<string> Input2){
  vector<string> SuffixCommands=GetStringParams(Input2);
  vector<string> PrefixCommands=GetStringParams(Input1);
  FindOrCreate(Input1[0]);
  WMS_Rec Concept=GetFromCache(Input1[0]);
  string NeuralData=GetDBValue(Concept,":POLYNOMIAL");
  vector<double> Polynomial=List::List2Num(List::String2List(NeuralData));
  SetEquation(Polynomial);
  DisplayOutput.push_back(GetFormula());
  return Input2;  
}

vector<string> Chocky::GreaterThan(vector<string> Input1,vector<string> Input2){
  vector<string> CMDPrefix=GetStringParams(Input1);
  vector<string> CMDSuffix=GetStringParams(Input2);
  if(CMDPrefix.size()&&CMDSuffix.size()){
    if((BinData::IsNumber(CMDPrefix[0])&&(BinData::IsNumber(CMDSuffix[0])))&&(atof(CMDPrefix[0].c_str())>atof(CMDSuffix[0].c_str())))
      ANSWER=List::String2List("1");
    else
      ANSWER=List::String2List("0");
  }
  Input2=ShiftVector(Input2,1);
  return Input2;
}

vector<string> Chocky::GreaterThanEqual(vector<string> Input1,vector<string> Input2){
  vector<string> CMDPrefix=GetStringParams(Input1);
  vector<string> CMDSuffix=GetStringParams(Input2);
  if(CMDPrefix.size()&&CMDSuffix.size()){
    if((BinData::IsNumber(CMDPrefix[0])&&(BinData::IsNumber(CMDSuffix[0])))&&(atof(CMDPrefix[0].c_str())>=atof(CMDSuffix[0].c_str())))
      ANSWER=List::String2List("1");
    else
      ANSWER=List::String2List("0");
  }
  Input2=ShiftVector(Input2,1);
  return Input2;
}

vector<string> Chocky::LessThan(vector<string> Input1,vector<string> Input2){
  vector<string> CMDPrefix=GetStringParams(Input1);
  vector<string> CMDSuffix=GetStringParams(Input2);
  if(CMDPrefix.size()&&CMDSuffix.size()){
    if((BinData::IsNumber(CMDPrefix[0])&&(BinData::IsNumber(CMDSuffix[0])))&&(atof(CMDPrefix[0].c_str())<atof(CMDSuffix[0].c_str())))
      ANSWER=List::String2List("1");
    else
      ANSWER=List::String2List("0");
  }
  Input2=ShiftVector(Input2,1);
  return Input2;
}

vector<string> Chocky::LessThanEqual(vector<string> Input1,vector<string> Input2){
  vector<string> CMDPrefix=GetStringParams(Input1);
  vector<string> CMDSuffix=GetStringParams(Input2);
  if(CMDPrefix.size()&&CMDSuffix.size()){
    if((BinData::IsNumber(CMDPrefix[0])&&(BinData::IsNumber(CMDSuffix[0])))&&(atof(CMDPrefix[0].c_str())<=atof(CMDSuffix[0].c_str())))
      ANSWER=List::String2List("1");
    else
      ANSWER=List::String2List("0");
  }
  Input2=ShiftVector(Input2,1);
  return Input2;
}

vector<string> Chocky::NotEqual(vector<string> Input1,vector<string> Input2){
  vector<string> CMDPrefix=GetStringParams(Input1);
  vector<string> CMDSuffix=GetStringParams(Input2);
  if(CMDPrefix.size()&&CMDSuffix.size()){
    if(CMDPrefix[0]==CMDSuffix[0])
      ANSWER=List::String2List("0");
    else{
      if((BinData::IsNumber(CMDPrefix[0])&&(BinData::IsNumber(CMDSuffix[0])))&&(atof(CMDPrefix[0].c_str())==atof(CMDSuffix[0].c_str())))
	ANSWER=List::String2List("0");
      else
	ANSWER=List::String2List("1");
    }
  }
  Input2=ShiftVector(Input2,1);
  return Input2;
}

vector<string> Chocky::Equals(vector<string> Input1,vector<string> Input2){
  vector<string> CMDPrefix=GetStringParams(Input1);
  vector<string> CMDSuffix=GetStringParams(Input2);
  if(CMDPrefix.size()&&CMDSuffix.size()){
    if(CMDPrefix[0]==CMDSuffix[0])
      ANSWER=List::String2List("1");
    else{
      if((BinData::IsNumber(CMDPrefix[0])&&(BinData::IsNumber(CMDSuffix[0])))&&(atof(CMDPrefix[0].c_str())==atof(CMDSuffix[0].c_str())))
	ANSWER=List::String2List("1");
      else
	ANSWER=List::String2List("0");
    }
  }
  Input2=ShiftVector(Input2,1);
  return Input2;
}

vector<string> Chocky::ShowNeural(vector<string> Input1,vector<string> Input2){
  vector<string> SuffixCommands=GetStringParams(Input2);
  vector<string> PrefixCommands=GetStringParams(Input1);
  FindOrCreate(Input1[0]);
  WMS_Rec Concept=GetFromCache(Input1[0]);
  string NeuralData=GetDBValue(Concept,":NEURALNET");
  vector<string> NeuralNet=List::String2List(NeuralData);  
  string Calculated=DisplayNeuralSymbols(Input1[0]);
  Calculated=Calculated+ShowNN(NeuralNet);
  DisplayOutput.push_back(Calculated);  
  return Input2;
}

vector<string> Chocky::Learn(vector<string> Input1,vector<string> Input2){
  vector<string> SuffixCommands=GetStringParams(Input2);
  vector<string> PrefixCommands=GetStringParams(Input1);
  string NeuralNet=LearnNN(PrefixCommands);
  FindOrCreate(Input2[0]);
  WMS_Rec Concept=GetFromCache(Input2[0]);
  Concept=RepDBPair(Concept,":NEURALNET",NeuralNet);
  RepInCache(Concept);
  ANSWER=List::String2List(NeuralNet);
  Input2=ShiftVector(Input2,SuffixCommands.size());
  return Input2;
}

vector<string> Chocky::Oddman(vector<string> Input1,vector<string> Input2){
  vector<string> SuffixCommands=GetStringParams(Input2);
  vector<string> PrefixCommands=GetStringParams(Input1);
  ANSWER=List::String2List(OddmanNN(PrefixCommands));
  return Input2;
}

vector<string> Chocky::NNExecute(vector<string> Input1,vector<string> Input2){
  vector<string> SuffixCommands=GetStringParams(Input2);
  vector<string> PrefixCommands=GetStringParams(Input1);
  FindOrCreate(Input2[0]);
  WMS_Rec Concept=GetFromCache(Input2[0]);
  string NeuralData=GetDBValue(Concept,":NEURALNET");
  vector<string> NeuralNet=List::String2List(NeuralData);
  string Calculated=ExecuteNN(NeuralNet,PrefixCommands);
  ANSWER=List::String2List(Calculated);
  Input2=ShiftVector(Input2,SuffixCommands.size());
  return Input2;
}

vector<string> Chocky::Understood(vector<string> Input1,vector<string> Input2){
  vector<string> SuffixCommands=GetStringParams(Input2);
  vector<string> PrefixCommands=GetStringParams(Input1);
  ANSWER=List::String2List(UnderstoodNN(PrefixCommands));
  return Input2;
}

vector<string> Chocky::Swap(vector<string> Input1,vector<string> Input2){
  vector<string> SuffixCommands=GetStringParams(Input2);
  vector<string> PrefixCommands=GetStringParams(Input1);
  size_t LocatedPos=SuffixCommands.size();
  if(LocatedPos!=-1)
    Input2=List::InsVector(Input2,PrefixCommands,LocatedPos+1);
  return Input2;  
}

vector<string> Chocky::Syntax(vector<string> Input1,vector<string> Input2){
  DisplayOutput.push_back(" <=>                      Swap pipelines.");
  DisplayOutput.push_back(" :                        Use indicative paradigm.");
  DisplayOutput.push_back(" $                        Use functional paradigm.");
  DisplayOutput.push_back(" #                        Use NLP paradigm.");
  DisplayOutput.push_back(" { }                      Do not evaluate at this level - like quotation marks.");
  DisplayOutput.push_back(" ~                        Do not evaluate next symbol at this level - like quotation mark.");
  DisplayOutput.push_back(" ( )                      Syntactical block markers.");
  DisplayOutput.push_back(" =                        Assignment operator.");
  DisplayOutput.push_back(" ==, <>, >, <, >=, <=     Equality operators.");
  DisplayOutput.push_back(" +, -, /, *, ^, %         Maths operators."); 
  DisplayOutput.push_back(" $var                     Represents a variable called $var.");  
  DisplayOutput.push_back(" :if { $var } [:else] :fi Conditional on $var being true.");
  DisplayOutput.push_back(" :while {$var}      :wend Loop conditional on $var being true.");
  return Input2;
}

vector<string> Chocky::If(vector<string> Input1,vector<string> Input2){
  long int FiPos=List::FindTerminator(Input2,":if",":fi",1);
  if(FiPos==-1){
    DisplayOutput.push_back("Error: :IF must be terminated with a :FI.");
    return Input2;
  }
  else{
    int ConditionParam=0;
    vector<string> IfClause;
    vector<string> ElseClause;
    long int ElsePos=FindSameLevel(Input2,":if",":fi",":else",1);
    vector<int> ExpandedConditions=GetIntParam(Input2[0]);
    if(ExpandedConditions.size())
      ConditionParam=ExpandedConditions[0];    
    if(ElsePos==-1){
      IfClause=List::GetBetween(Input2,1,FiPos-1);
      ElseClause={};
    }
    else{
      IfClause=List::GetBetween(Input2,1,ElsePos-1);
      ElseClause=GetBetween(Input2,ElsePos,FiPos-1);
    }
    if(ConditionParam)
      IndExe(Input1,IfClause,false);
    else{
      if(ElseClause.size())
	IndExe(Input1,ElseClause,false);
    }
    Input2=GetFrom(Input2,FiPos);    
  }
  return Input2;
}

vector<string> Chocky::While(vector<string> Input1,vector<string> Input2){
  long int WendPos=List::FindTerminator(Input2,":while",":wend",1);
  if(WendPos==-1){
    DisplayOutput.push_back("Error: :WHILE must be terminated with a :WEND.");
    return Input2;
  }
  else{
    int ConditionParam=0;
    vector<string> WhileClause=List::GetBetween(Input2,1,WendPos-1);
    while(1){
      vector<int> ExpandedConditions=GetIntParam(Input2[0]);
      if((!ExpandedConditions.size())||(ExpandedConditions.size()&&(ExpandedConditions[0]==0)))
	break;
      IndExe(Input1,WhileClause,false);            
    }
    Input2=GetFrom(Input2,WendPos);
  }  
  return Input2;
}

vector<string> Chocky::Domain(vector<string> Input1,vector<string> Input2){
  vector<int> CMDPrefix=GetIntParams(Input1);
  vector<string> Range={};
  if(CMDPrefix.size()==2){
    for(size_t _=CMDPrefix[0];_<CMDPrefix[1];++_)
      Range.push_back(BinData::Int2String(_));
    ANSWER=Range;
  }
  else
    DisplayOutput.push_back("Error: :DOMAIN needs two parameters.");
  return Input2;
}

vector<string> Chocky::DebugRec(vector<string> Input1,vector<string> Input2){
  for(size_t _=0;_<Input1.size();++_){
    FindOrCreate(Input1[_]);
    WMS_Rec Record=GetFromCache(Input1[_]);
    vector<string> RecordData=WMSConversion::WMS2Vec(Record);
    for(size_t LineId=0;LineId<RecordData.size();++LineId)
      DisplayOutput.push_back(RecordData[LineId]);
  }
  return Input2;
}

vector<string> Chocky::ListVariables(vector<string> Input1,vector<string> Input2){
  vector<string> VariableList=ListVariables();
  for(size_t _=0;_<VariableList.size();++_){
    string Value=Set2String(String2List(GetVariable(VariableList[_])));
    string VariablePair=VariableList[_]+" = "+Value;
    DisplayOutput.push_back(VariablePair);
  }
  return Input2;
}

vector<string> Chocky::SaveSettings(vector<string> Input1,vector<string> Input2){
  Mode2WMS();
  return Input2;
}

vector<string> Chocky::Assign(vector<string> Input1,vector<string> Input2){
  vector<string> SuffixCommands=GetStringParams(Input2);
  string CMDSuffix=List::List2String(SuffixCommands);
  for(size_t _=0;_<Input1.size();++_){
    if(Input1[_].size()&&(Input1[_].c_str()[0]=='$')){
      SetVariable(Input1[_],CMDSuffix);
    }
    else{
      size_t RecPos=FindInCache(Input1[_]);
      if(RecPos!=-1)
	CachedContents[RecPos]=RepHeader(CachedContents[RecPos],CMDSuffix,7);
    }
  }  
  Input2=ShiftVector(Input2,SuffixCommands.size());
  return Input2;
}

vector<string> Chocky::Add(vector<string> Input1,vector<string> Input2){
  vector<double> CMDPrefix=GetFloatParams(Input1);
  vector<string> Result;
  vector<double> CMDSuffix=GetFloatParams(Input2);
  if(CMDSuffix.size()==1){
    for(size_t _=0;_<CMDPrefix.size();++_)
      Result.push_back(Float2String(CMDPrefix[_]+CMDSuffix[0]));
  }
  else if(CMDSuffix.size()==CMDPrefix.size()){
    for(size_t _=0;_<CMDPrefix.size();++_)
      Result.push_back(Float2String(CMDPrefix[_]+CMDSuffix[_]));
  }
  else{
    return Input2;
  }
  if(Result.size())
    ANSWER=Result;
  Input2=ShiftVector(Input2,CMDSuffix.size());
  return Input2;
}

vector<string> Chocky::Subtract(vector<string> Input1,vector<string> Input2){
  vector<double> CMDPrefix=GetFloatParams(Input1);
  vector<string> Result;
  vector<double> CMDSuffix=GetFloatParams(Input2);
  if(CMDSuffix.size()==1){
    for(size_t _=0;_<CMDPrefix.size();++_)
      Result.push_back(Float2String(CMDPrefix[_]-CMDSuffix[0]));
  }
  else if(CMDSuffix.size()==CMDPrefix.size()){
    for(size_t _=0;_<CMDPrefix.size();++_)
      Result.push_back(Float2String(CMDPrefix[_]-CMDSuffix[_]));
  }
  else
    return Input2;
  if(Result.size())
    ANSWER=Result;
  Input2=ShiftVector(Input2,CMDSuffix.size());
  return Input2;
}

vector<string> Chocky::Multiply(vector<string> Input1,vector<string> Input2){
  vector<double> CMDPrefix=GetFloatParams(Input1);
  vector<string> Result;
  vector<double> CMDSuffix=GetFloatParams(Input2);
  if(CMDSuffix.size()==1){
    for(size_t _=0;_<CMDPrefix.size();++_)
      Result.push_back(Float2String(CMDPrefix[_]*CMDSuffix[0]));
  }
  else if(CMDSuffix.size()==CMDPrefix.size()){
    for(size_t _=0;_<CMDPrefix.size();++_)
      Result.push_back(Float2String(CMDPrefix[_]*CMDSuffix[_]));
  }
  else
    return Input2;
  if(Result.size())
    ANSWER=Result;
  Input2=ShiftVector(Input2,CMDSuffix.size());
  return Input2;
}

vector<string> Chocky::Modulus(vector<string> Input1,vector<string> Input2){
  vector<int> CMDPrefix=GetIntParams(Input1);
  vector<string> Result;
  vector<int> CMDSuffix=GetIntParams(Input2);
  if(CMDSuffix.size()==1){
    for(size_t _=0;_<CMDPrefix.size();++_)
      Result.push_back(Float2String(CMDPrefix[_]%CMDSuffix[0]));
  }
  else if(CMDSuffix.size()==CMDPrefix.size()){
    for(size_t _=0;_<CMDPrefix.size();++_)
      Result.push_back(Float2String(CMDPrefix[_]%CMDSuffix[_]));
  }
  else
    return Input2;
  if(Result.size())
    ANSWER=Result;
  Input2=ShiftVector(Input2,CMDSuffix.size());
  return Input2;
}

vector<string> Chocky::Divide(vector<string> Input1,vector<string> Input2){
  vector<double> CMDPrefix=GetFloatParams(Input1);
  vector<string> Result;
  vector<double> CMDSuffix=GetFloatParams(Input2);
  if(CMDSuffix.size()==1){
    for(size_t _=0;_<CMDPrefix.size();++_)
      Result.push_back(Float2String(CMDPrefix[_]/CMDSuffix[0]));
  }
  else if(CMDSuffix.size()==CMDPrefix.size()){
    for(size_t _=0;_<CMDPrefix.size();++_)
      Result.push_back(Float2String(CMDPrefix[_]/CMDSuffix[_]));
  }
  else
    return Input2;
  if(Result.size())
    ANSWER=Result;
  Input2=ShiftVector(Input2,CMDSuffix.size());
  return Input2;
}

vector<string> Chocky::Exponent(vector<string> Input1,vector<string> Input2){
  vector<double> CMDPrefix=GetFloatParams(Input1);
  vector<string> Result;
  vector<double> CMDSuffix=GetFloatParams(Input2);
  if(CMDSuffix.size()==1){
    for(size_t _=0;_<CMDPrefix.size();++_)
      Result.push_back(Float2String(pow(CMDPrefix[_],CMDSuffix[0])));
  }
  else if(CMDSuffix.size()==CMDPrefix.size()){
    for(size_t _=0;_<CMDPrefix.size();++_)
      Result.push_back(Float2String(pow(CMDPrefix[_],CMDSuffix[_])));
  }
  else
    return Input2;
  if(Result.size())
    ANSWER=Result;
  Input2=ShiftVector(Input2,CMDSuffix.size());
  return Input2;
}

vector<string> Chocky::CommandHelp(vector<string> Input1,vector<string> Input2){
  size_t NumberofCommands=CommandName.size();
  if(!Input2.size()){
    for(size_t _=0;_<NumberofCommands;++_){
      printf("%s\n",OnlineHelp[_].c_str());
      DisplayOutput.push_back(OnlineHelp[_]);
    }
  }
  else{
    bool Found=false;
    size_t InputCommandLength=Input2[0].size();
    for(size_t _=0;_<NumberofCommands;++_){
      size_t InstructionLength=CommandName[_].size();
      if((InputCommandLength<=InstructionLength)&&(!strncmp(CommandName[_].c_str(),Input2[0].c_str(),InputCommandLength))){
	printf("%s\n",OnlineHelp[_].c_str());
	DisplayOutput.push_back(OnlineHelp[_]);
	Found=true;
      }
    }
    if(!Found){
      string Output="NULL :HELP [B] Shows a list of help commands if there is no A specified, otherwise shows the syntax for a specific command.";
      printf("%s\n",Output.c_str());
      DisplayOutput.push_back(Output);
      Input2=ShiftVector(Input2,1);
    }
    else{
      Input2=ShiftVector(Input2,1);
    }
  }
  return Input2;
}

vector<string> Chocky::PrintAnswer(vector<string> Input1,vector<string> Input2){
  vector<string> AnswerData={};
  if(Input1.size()==0)
    AnswerData=ANSWER;
  else
    AnswerData=Input1;
  AnswerData=GetStringParams(AnswerData);
  DisplayOutput.push_back(List2String(AnswerData));
  ANSWER=AnswerData;
  return Input2;
}

vector<string> Chocky::PrintErrors(vector<string> Input1,vector<string> Input2){
  PrintSentences(Errors);
  return Input2;
}

vector<string> Chocky::PrintOutput(vector<string> Input1,vector<string> Input2){
  PrintSentences(DisplayOutput);
  Input2=ShiftVector(Input2,1);
  return Input2;
}

vector<string> Chocky::GetAllRecs(vector<string> Input1,vector<string> Input2){
  ANSWER=AllRecords;
  return Input2;
}

vector<string> Chocky::Execute(vector<string> Pipeline,vector<string> LastCommands){
  bool ValidSyntax=true;
  size_t NumberofCommands=CommandName.size();
  for(size_t _=0;_<NumberofCommands;_++){
    if(CommandName[_]==LastCommands[0]){
      size_t In1Size=Pipeline.size();
      size_t In2Size=LastCommands.size();
      if((Parameters[_]==1)&&(!In1Size))
	ValidSyntax=false;
      if((Parameters[_]==2)&&(!In2Size))
	ValidSyntax=false;
      if((Parameters[_]==3)&&((!In2Size)||(!In1Size)))
	ValidSyntax=false;
      if(ValidSyntax){
	LastCommands.erase(LastCommands.begin());	
	LastCommands=(this->*InstructionSet[_])(Pipeline,LastCommands);
      }
      else
	Errors.push_back("Invalid command syntax for "+LastCommands[0]);
      break;
    }
  }
  return LastCommands;
}

vector<string> Chocky::ProcessNLP(){
  vector<string> Result;
  if(SentenceMoodFound=="indicative"){ 
    DEBUGMSG;
   
    SaveSentences();
  }
  if(SentenceMoodFound=="imperative"){
    DEBUGMSG;    
    
  }
  if(SentenceMoodFound=="ynq"){
    DEBUGMSG;
    
    Result={};
    if(YesNoQuestion()){
      DEBUGMSG;
      puts("Yes");
      DisplayOutput.push_back("Yes");
    }
    else{
      DEBUGMSG;
      puts("No");
      DisplayOutput.push_back("No");
    }
    ANSWER=Result;
  }
  if(SentenceMoodFound=="whq"){
    if(SentenceVoiceFound==Sentence::SentenceVoiceType::Active){
      DisplayOutput=WHQuestion(Memory::PhraseType::IsSubject);
    }
    if(SentenceVoiceFound==Sentence::SentenceVoiceType::Passive){
      DisplayOutput=WHQuestion(Memory::PhraseType::IsPredicate);
    }
    if(SentenceVoiceFound==Sentence::SentenceVoiceType::Unknown){
      DisplayOutput.push_back("Please rephrase the sentence.");
    }
  }
  if(SentenceMoodFound==""){
    DisplayOutput.push_back("Please rephrase the sentence.");
  }
  return {};  
}

vector<string> Chocky::NLPExe(vector<string> Pipeline,vector<string> LastCommands){

  DEBUGMSG;
  List::PrintList(LastCommands);
  DEBUGMSG;
  List::PrintList(Pipeline);

  bool Error=false;
  bool IsQuestion=false;
  
  vector<vector<string>> Sentences=SeparateParagraph(LastCommands);
  for(size_t _=0;_<Sentences.size();++_){
    vector<string> ThisSentence=Sentences[_];

    if(IsCondSentence(ThisSentence)){
      // need to split up conditional sentence

      ConditionalParagraphData SentenceConditions=SplitConditional(ThisSentence);

      DEBUGMSG;
      SentenceConditions.Debug();

      // go through and check that the sentences are valid

      vector<string> Causes={};
      vector<string> Effects={};
      
      for(size_t _=0;_<SentenceConditions.Causes.size();++_){

	DEBUGMSG;
	List::PrintList(SentenceConditions.Causes[_]);
	
	if(SentenceConditions.Causes[_].size()&&((SentenceConditions.Causes[_])[0]!="?")){	
	  PrepareSentence(SentenceConditions.Causes[_]);
	  if(IsSentenceValid()){
	    Sentence ThisCauseSentence=*GetSentence();
	    SentenceConditions.ProcessedCauses.push_back(ThisCauseSentence);	  
	    Causes=GetCSLocationList(ThisCauseSentence,SentenceConditions.CSCauseSyms);
	  }
	  else{

	    DEBUGMSG;
	    puts("Invalid Sentence: ");
	    List::PrintList(SentenceConditions.Causes[_]);
	    
	    Error=true;
	    break;
	  }
	}
      }
      for(size_t _=0;_<SentenceConditions.Effects.size();++_){

	DEBUGMSG;
	List::PrintList(SentenceConditions.Effects[_]);
	
	if(SentenceConditions.Effects[_].size()&&((SentenceConditions.Effects[_])[0]!="?")){
	  PrepareSentence(SentenceConditions.Effects[_]);
	  if(IsSentenceValid()){
	    Sentence ThisEffectSentence=*GetSentence();
	    SentenceConditions.ProcessedEffects.push_back(ThisEffectSentence);
	    Effects=GetCSLocationList(ThisEffectSentence,SentenceConditions.CSEffectSyms);
	  }
	  else{

	    DEBUGMSG;
	    puts("Invalid Sentence: ");
	    List::PrintList(SentenceConditions.Effects[_]);
	    
	    Error=true;
	    break;
	  }
	}
      }
      if(Error){
	ANSWER={};
	DEBUGMSG;
	ANSWER.push_back("Please rephrase the sentence.");
	break;
      }

      string CSCause="";
      string CSEffect="";
      
      if(Causes.size()){
	string CauseLogic=NLPStoreLogic::GetLogicName(SentenceConditions.CauseLogic);
	Causes.insert(Causes.begin(),CauseLogic);
	CSCause=CSRef(Causes);
      }
      if(Effects.size()){
	string EffectLogic=NLPStoreLogic::GetLogicName(SentenceConditions.EffectLogic);
	Effects.insert(Effects.begin(),EffectLogic);
	CSEffect=CSRef(Effects);
      }


      DEBUGMSG;
      List::PrintList(Causes);
      DEBUGMSG;
      List::PrintList(Effects);

      
      // is it a question ?

      string CondSentenceMood=GetCondSentenceMood(ThisSentence);

      if(CondSentenceMood=="whq"){
	IsQuestion=true;
	

	vector<string> WHQResult=AskWHQConditional(CSCause,CSEffect);

	DEBUGMSG;
	List::PrintList(WHQResult);

	for(size_t SymbolNum=0;SymbolNum<WHQResult.size();++SymbolNum){
	  // expand out the symbols
	  vector<string> Expanded=CSDeref(WHQResult[SymbolNum]);

	  DEBUGMSG;
	  List::PrintList(Expanded);
	  
	}

	
      }
      else if(CondSentenceMood=="ynq"){
	IsQuestion=true;
	
	
	DEBUGMSG;
	printf("[%s -> %s]\n",CSCause.c_str(),CSEffect.c_str());
	
	bool YNQResult=AskYNConditional(CSCause,CSEffect);
      }
      else if(CondSentenceMood=="indicative"){
	StoreConditional(CSCause,CSEffect);	
      }      
    }
    else{
      PrepareSentence(ThisSentence);      
    }
    if(!IsQuestion){
      vector<string> Processed=ProcessNLP();
    }
  }

  DEBUGMSG;
  
  return {};
}

vector<string> Chocky::IndExe(vector<string> Pipeline,vector<string> Input,bool TranslateThis){
  bool LongQuote=false,ShortQuote=false,LastIsCommand=true;
  vector<string> Translated={};
  while(Input.size()){
    if((Input[0]=="#")||((Input[0].size()==1)&&(Input[0]=="$"))||(Input[0]==":")){
      break;
    }
    else if((!ShortQuote)&&(Input[0]=="{"))
      LongQuote=true;
    else if((!ShortQuote)&&(Input[0]=="}"))
      LongQuote=false;
    else if((!ShortQuote)&&(Input[0]=="~"))
      ShortQuote=true;
    else{
      if((!LongQuote)&&(!ShortQuote)&&((Input[0].c_str())[0]!=':')&&TranslateThis)
	Translated=VecAdd(Translated,ExpandOnce({},Input[0]));
      else
	Translated.push_back(Input[0]);
      ShortQuote=false;
    }
    Input.erase(Input.begin());
  }

  DEBUGMSG;
  List::PrintList(Translated);

  vector<string> LastAnswer=ANSWER;
  ANSWER={};
  while(Translated.size()){
    if(List::SetTheoryIsElem(CommandName,Translated[0])){
      if(!LastIsCommand){
	LastAnswer=ANSWER;
	LastIsCommand=true;
      }
      Translated=Execute(LastAnswer,Translated);
      if(ANSWER.size()==0)
	ANSWER=LastAnswer;
      else
	LastAnswer=ANSWER;
    }
    else{
      if(((Translated[0])[0]!=':')||((Translated[0].size()>2)&&(!strncmp((Translated[0]).c_str(),":cs",3)))){
	if(LastIsCommand){
	  ANSWER={};
	  LastIsCommand=false;
	}
	ANSWER.push_back(Translated[0]);
      }
      else
	LastIsCommand=true;
      Translated.erase(Translated.begin());
    }
  }
  return Input;
}

vector<string> Chocky::FuncExe(vector<string> Pipeline,vector<string> LastCommands){
  DEBUGMSG;

  // recursion

  //  return LastCommands;
  return {};
}

enum Chocky::Syntax Chocky::ClassifyInputType(vector<string> &Input){
  for(size_t _=0;_<Input.size();++_){
    char FirstChar=0;
    if(Input[_].size())
      FirstChar=(Input[_].c_str())[0];
    if(FirstChar==':')
      return Chocky::Syntax::IndType;
    if(FirstChar=='$')
      return Chocky::Syntax::FuncType;
    if(FirstChar=='#')
      return Chocky::Syntax::NLPType;
  }
  return Chocky::Syntax::NLPType;
}

vector<string> Chocky::Interpreter(vector<string> Input){
  vector<string> ToParse=Input;
  enum Chocky::Syntax SynType=ClassifyInputType(ToParse);  
  DisplayOutput={};
  while(ToParse.size()){
    vector<string> LastAnswer=ANSWER;
    if(ToParse[0]==":"){
      ToParse.erase(ToParse.begin());
      continue;
    }
    else if(ToParse[0]=="$"){
      ToParse.erase(ToParse.begin());
      continue;
    }
    else if(ToParse[0]=="#"){
      ToParse.erase(ToParse.begin());
      continue;
    }
    if(SynType==Syntax::NLPType){
      DEBUGMSG;
      ToParse=NLPExe(LastAnswer,ToParse);
    }
    if(SynType==Syntax::FuncType){
      DEBUGMSG;
      ToParse=FuncExe(LastAnswer,ToParse);
    }
    if(SynType==Syntax::IndType){
      DEBUGMSG;
      ToParse=IndExe(LastAnswer,ToParse);
    }
    if(ANSWER.size()==0){
      DEBUGMSG;
      ANSWER=LastAnswer;
    }
  }  
  return DisplayOutput;
}

vector<string> Chocky::MathsRec(vector<string> Input){
  size_t InputSize=Input.size(),C=0,StartPos=0,EndPos=0;
  
  while(true){

    for(C=0;C<InputSize;C++)
      if(!strcmp(Input[C].c_str(),"("))
	StartPos=C;

    for(C=StartPos+1;C<InputSize;C++){
      
      
    }
    
  }  
}

bool Chocky::MathsSyntaxOK(vector<string> Input){
  size_t InputSize=Input.size(),C=0;
  int OpenBrackets=0;

  /* Check parentheses first */

  for(C=0;C<InputSize;C++){
    if(!strcmp(Input[C].c_str(),"("))
      OpenBrackets++;
    if(!strcmp(Input[C].c_str(),")")){
      if(!OpenBrackets)
	return false;
      OpenBrackets--;
    }
  }
  if(OpenBrackets)
    return false;

  /* TODO: check operand syntax */


  return true;
}

void Chocky::LearnSelected(vector<string> Input){
  vector<string> Filtered;
  string Unwanted="{ }";
  vector<string> ToRemove=List::SetTheoryUnion(List::String2List(Unwanted),CommandName);
  Filtered=List::SetTheoryExclude(Input,ToRemove);
  LearnWords(Filtered);
}

void Chocky::AddToDirectedGraph(vector<string> Sentence,string SentenceMood){
  FindOrCreate(UniqueUN);
  WMS_Rec Mode=GetFromCache(UniqueUN);
  vector<string> GraphList=List::String2List(GetDBValue(Mode,":moodlist"));
  if(!List::SetTheoryIsElem(GraphList,SentenceMood)){
    string Error="ERROR: \"";
    Error=Error+SentenceMood;
    Error=Error+"\" is not registered in :moodlist. Cannot continue.";
    DisplayOutput.push_back(Error);
    return;
  }
  vector<string> ConvertedSentence={};
  vector<vector<string>> ArticlesExtracted=SentenceToArticles(Sentence);
  for(size_t _=0;_<ArticlesExtracted.size();++_){
    if((ArticlesExtracted[_]).size()){
      if(((ArticlesExtracted[_]).size()>1)&&(_>0)){
	string Error="ERROR: \"";
	Error=Error+Sentence[_-1];
	Error=Error+"\" is ambiguously defined. Stopping processing.";
	DisplayOutput.push_back(Error);
	return;
      }
      ConvertedSentence.push_back((ArticlesExtracted[_])[0]);
    }
  }
  map<string,vector<string>> Graph=FindOrCreateGraph(SentenceMood);
  string LastArticle=ConvertedSentence[0];
  for(size_t _=1;_<ConvertedSentence.size();++_){
    AddToGraph(SentenceMood,LastArticle,ConvertedSentence[_]);
    LastArticle=ConvertedSentence[_];
  }
}

/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
 */
