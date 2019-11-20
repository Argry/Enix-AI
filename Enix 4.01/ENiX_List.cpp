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

#include <string.h>
#include <sstream>
#include <algorithm>
#include "ENiX_List.h"
#include "ENiX_Settings.h"

vector<string> List::Num2List(vector<double> Input){
  vector<string> Output;
  for(size_t _=0;_<Input.size();++_)
    Output.push_back(BinData::Float2String(Input[_]));
  return Output;
}

vector<string> List::Int2List(vector<int> Input){
  vector<string> Output;
  for(size_t _=0;_<Input.size();++_)
    Output.push_back(BinData::Int2String(Input[_]));
  return Output;
}

vector<string> List::SortList(vector<string> Input1){
  std::sort(Input1.begin(),Input1.end());
  return Input1;
}

vector<string> List::SetTheoryExclude(vector<string> Set,vector<string> ToExclude){
  vector<string> Result;
  for(size_t ElPos=0;ElPos<Set.size();++ElPos){
    bool Found=false;
    for(size_t ExPos=0;ExPos<ToExclude.size();++ExPos)
      if(Set[ElPos]==ToExclude[ExPos]){
	Found=true;
	break;
      }
    if(!Found)
      Result.push_back(Set[ElPos]);
  }
  return Result;
}

long int List::FindSameLevel(vector<string> Commands,string StartSymbol,string EndSymbol,string InternSymbol,size_t StartValue){
  size_t Indent=StartValue;
  for(long int _=0;_<Commands.size();++_){
    if(Commands[_]==StartSymbol)
      ++Indent;
    if(Commands[_]==EndSymbol)
      --Indent;
    // the else is "inside" the syntax hence 1 rather than 0.
    if(Indent==1)
      if(Commands[_]==InternSymbol)
	return _+1;
  }
  return -1;  
}

long int List::FindTerminator(vector<string> Commands,string StartSymbol,string EndSymbol,size_t StartValue){
  size_t Indent=StartValue;
  for(long int _=0;_<Commands.size();++_){
    if(Commands[_]==StartSymbol)
      ++Indent;
    if(Commands[_]==EndSymbol)
      --Indent;
    if(Indent==0)
      return _+1;
  }
  return -1;
}

long int List::FindFirst(vector<int> Input,int Token){
  for(size_t _=0;_<Input.size();++_)
    if(Input[_]==Token)
      return (long int)_;
  return -1;
}

long int List::FindFirst(vector<string> Input,string Token){
  for(size_t _=0;_<Input.size();++_)
    if(Input[_]==Token)
      return (long int)_;
  return -1;
}

vector<string> List::GetUpto(vector<string> Input,size_t Finish){
  vector<string> Collected;
  for(size_t _=0;_<Finish;++_)
    Collected.push_back(Input[_]);
  return Collected;
}

vector<string> List::GetFrom(vector<string> Input,size_t Start){
  vector<string> Collected;
  for(size_t _=Start;_<Input.size();++_)
    Collected.push_back(Input[_]);
  return Collected;
}

vector<string> List::GetBetween(vector<string> Input,size_t Start,size_t Finish){
  vector<string> Collected;
  for(size_t _=Start;(_<Input.size())&&(_<Finish);++_)
    Collected.push_back(Input[_]);
  return Collected;
}

string List::List2String(vector<int> Input){
  ostringstream Converted;
  for(size_t Pos=0;Pos<Input.size();Pos++){
    Converted<<Input[Pos];
    if(Pos<Input.size()-1)
      Converted<<" ";
  }
  return Converted.str();
}

string List::List2String(vector<float> Input){
  ostringstream Converted;
  for(size_t Pos=0;Pos<Input.size();Pos++){
    Converted<<Input[Pos];
    if(Pos<Input.size()-1)
      Converted<<" ";
  }
  return Converted.str();
}

string List::List2String(vector<double> Input){
  ostringstream Converted;
  for(size_t Pos=0;Pos<Input.size();Pos++){
    Converted<<Input[Pos];
    if(Pos<Input.size()-1)
      Converted<<" ";
  }
  return Converted.str();
}

vector<string> List::InsVector(vector<string> &Into,vector<string> &Vector,size_t InsPos){
  size_t CurrentPos=0;
  vector<string> Final={};
  for(CurrentPos=0;(CurrentPos<InsPos)&&(CurrentPos<Into.size());++CurrentPos)
    Final.push_back(Into[CurrentPos]);  
  for(size_t _=0;_<Vector.size();++_)
    Final.push_back(Vector[_]);
  for(;CurrentPos<Into.size();++CurrentPos)
    Final.push_back(Into[CurrentPos]);
  return Final;
}

void List::PrintList(vector<int> &Input){
  unsigned long int C=0;
  for(C=0;C<Input.size();C++)
    printf("%d ",Input[C]);
  putchar('\n');  
}

void List::PrintList(vector<size_t> &Input){
  unsigned long int C=0;
  for(C=0;C<Input.size();C++)
    printf("%zu ",Input[C]);
  putchar('\n');  
}

void List::PrintList(vector<double> &Input){
  unsigned long int C=0;
  for(C=0;C<Input.size();C++)
    printf("%f ",Input[C]);
  putchar('\n');
}

vector<double> List::List2Floats(vector<string> Input){
  vector<double> Converted;
  for(size_t _=0;_<Input.size();_++)
    Converted.push_back(atof(Input[_].c_str()));
  return Converted;
}

vector<int> List::List2Ints(vector<string> Input){
  vector<int> Converted;
  for(size_t _=0;_<Input.size();_++)
    Converted.push_back(atoi(Input[_].c_str()));
  return Converted;
}

vector<double> List::List2Num(vector<string> Input){
  vector<double> Numbers;
  for(size_t _=0;_<Input.size();++_){
    double Num=atof(Input[_].c_str());
    Numbers.push_back(Num);
  }
  return Numbers;
}

vector<int> List::List2Int(vector<string> Input){
  vector<int> Numbers;
  for(size_t _=0;_<Input.size();++_){
    int Num=atoi(Input[_].c_str());
    Numbers.push_back(Num);
  }
  return Numbers;
}

vector<unsigned long int> List::BinDataSetDel(vector<unsigned long int> DataList,size_t Item){
  vector<unsigned long int> Result={};
  for(size_t _=0;_<DataList.size();_++)
    if(!(DataList[_]==Item))
      Result.push_back(DataList[_]);
  return Result;  
}

vector<BinData> List::ListStr2BinList(vector<string> Input){
  vector<BinData> Output={};
  for(size_t _=0;_<Input.size();_++){
    BinData Converted(Input[_].c_str(),Input[_].size());
    Output.push_back(Converted);
  }
  return Output;
}

vector<string> List::BinList2ListStr(vector<BinData> Input){
  vector<string> Output={};
  for(size_t _=0;_<Input.size();_++)
    Output.push_back(Input[_].Convert2Str());
  return Output;
}


vector<BinData> List::String2List(BinData Input,bool ToLower){
  string InputStr=Input.Convert2Str();
  return ListStr2BinList(String2List(InputStr,ToLower));
}

BinData List::List2String(vector<BinData> Input){
  string OutputStr=List2String(BinList2ListStr(Input));
  BinData OutputBin(OutputStr.c_str(),OutputStr.size());
  return OutputBin;
}

vector<BinData> List::BinDataSetAdd(vector<BinData> DataList,vector<BinData> Item){
  for(size_t _=0;_<Item.size();_++)
    DataList=BinDataSetAdd(DataList,Item[_]);
  return DataList;
}

vector<BinData> List::BinDataSetAdd(vector<BinData> DataList,BinData Item){
  bool Found=false;
  for(size_t _=0;_<DataList.size();_++)
    if(DataList[_]==Item){
      Found=true;
      break;
    }
  if(!Found)
    DataList.push_back(Item);
  return DataList;
}

vector<BinData> List::BinDataSetDel(vector<BinData> DataList,BinData Item){
  vector<BinData> Result={};
  for(size_t _=0;_<DataList.size();_++)
    if(!(DataList[_]==Item))
      Result.push_back(DataList[_]);
  return Result;
}

vector<BinData> List::BinDataSetDel(vector<BinData> DataList,size_t Pos){
  vector<BinData> Result={};
  for(size_t _=0;_<DataList.size();_++)
    if(_!=Pos)
      Result.push_back(DataList[_]);
  return Result;
}

void List::PrintSentences(vector<string> &Input){
  unsigned long int C=0;
  for(C=0;C<Input.size();C++)
    printf("%s\n",(Input[C]).c_str());
}

bool List::SetTheoryIsSame(vector<string> Set1,vector<string> Set2){
  if(Set1.size()==Set2.size())
    return SetTheoryIsSubset(Set1,Set2);
  else
    return false;
}

vector<string> List::SetTheoryUnion(vector<string> Set1,vector<string> Set2){
  vector<string> Union=Set2;
  for(size_t _=0;_<Set1.size();_++)
    Union=SetTheoryAdd(Union,Set1[_]);
  return Union;
}

bool List::SetTheoryIsElem(vector<string> Set,string Element){
  for(size_t _=0;_<Set.size();_++)
    if(Set[_]==Element)
      return true;
  return false;
}

vector<string> List::SetTheoryIntersect(vector<string> Set1,vector<string> Set2){
  vector<string> Intersect={};
  for(size_t _=0;_<Set1.size();_++){
    if(SetTheoryIsElem(Set2,Set1[_]))
      Intersect.push_back(Set1[_]);
  }
  return Intersect;
}

vector<string> List::SetTheoryAdd(vector<string> Set,string Element){
  size_t _,Size=Set.size();
  bool Found=false;
  for(_=0;_<Size;_++){
    if(Element==(Set[_])){
      Found=1;
      break;
    }
  }
  if(!Found)
    Set.push_back(Element);
  return Set;
}

vector<string> List::ShiftVector(vector<string> &Input1,size_t Offset){
  size_t Remove=0,InputSize=Input1.size();
  Remove=(InputSize>Offset)?Offset:InputSize;
  Input1.erase(Input1.begin(),Input1.begin()+Remove);
  return Input1;
}

vector<string> List::VecAdd(vector<string> Input,vector<string> ToAdd){
  size_t _=0;
  for(_=0;_<ToAdd.size();_++)
    Input.push_back(ToAdd[_]);
  return Input;
}

vector<int> List::VecAdd(vector<int> Input,vector<int> ToAdd){
  size_t _=0;
  for(_=0;_<ToAdd.size();_++)
    Input.push_back(ToAdd[_]);
  return Input;
}

vector<double> List::VecAdd(vector<double> Input,vector<double> ToAdd){
  size_t _=0;
  for(_=0;_<ToAdd.size();_++)
    Input.push_back(ToAdd[_]);
  return Input;
}

bool List::SetTheoryIsSubset(vector<string> Coverset,vector<string> Subset){
  for(size_t _=0;_<Subset.size();_++)
    if(!SetTheoryIsElem(Coverset,Subset[_]))
      return false;
  return true;
}

bool List::SetTheoryIsIntersect(vector<string> Set1,vector<string> Set2){
  for(size_t _=0;_<Set2.size();_++)
    if(SetTheoryIsElem(Set1,Set2[_]))
      return true;
  return false;
}

string List::Convert2Lower(string &InStr){
  transform(InStr.begin(),InStr.end(),InStr.begin(),::tolower);
  return InStr;
}

string List::Convert2Lower(string InStr){
  transform(InStr.begin(),InStr.end(),InStr.begin(),::tolower);
  return InStr;
}

void List::PrintList(vector<string> &Input){
  unsigned long int C=0;
  for(C=0;C<Input.size();C++)
    printf("%s ",(Input[C]).c_str());
  putchar('\n');
}

void List::PrintList(vector<BinData> &Input){
  unsigned long int C=0;
  for(C=0;C<Input.size();C++){
    (Input[C]).Print();
    putchar('\n');
  }
}

void List::DebugList(vector<string> &Input){
  unsigned long int C=0;
  for(C=0;C<Input.size();C++)
    printf("[%s]{%d}",(Input[C]).c_str(),ClassifyToken(Input[C]));
  putchar('\n');
}

int List::ClassifyToken(string &Token){
  int Classification=0;
  unsigned long int C=0,Size=Token.size();
  const char *TokenStr=Token.c_str();  
  for(C=0;C<Size;C++){

    /*
      HASQUOTES  1
      HASSPACES  2
      HASNUMBERS 4
      HASSYMBOLS 8
      HASLETTERS 16
    */

    Classification|=(HASQUOTES*((TokenStr[C]=='"')||(TokenStr[C]=='\'')));
    Classification|=(HASSPACES*(!!isspace(TokenStr[C])));
    Classification|=(HASNUMBERS*(!!isdigit(TokenStr[C])));
    Classification|=(HASSYMBOLS*((!!ispunct(TokenStr[C]))&&(!((TokenStr[C]=='"')||(TokenStr[C]=='\'')))));
    Classification|=(HASLETTERS*(!!isalpha(TokenStr[C])));
  }
  return Classification;
}

// should ignore quotes!
vector<string> List::String2List(string Input,bool ToLower){
  bool IsSpace=false,WasChar=false;
  int QuoteLevel=0;
  unsigned long int InputSize=Input.size(),C=0,Start=0,Finish=0;
  const char *InputData=Input.c_str();
  vector<string> TokenList={};
  for(C=0;C<InputSize;C++){
    if(isspace(InputData[C])){
      IsSpace=true;
    }
    else{

      if((InputData[C]=='"')||(InputData[C]=='\'')){
	if(WasChar)
	  --QuoteLevel;
	else
	  ++QuoteLevel;
      }

      IsSpace=false;
      WasChar=true;
    }
    if(IsSpace&&(!QuoteLevel)){
      Finish=C;
      if(WasChar){
	unsigned long int Size=Finish-Start;
	char *Token=(char *)malloc((Size+1)*sizeof(char));
	if(Token){
	  memcpy(Token,InputData+Start,Size);
	  Token[Size]=0;
	  string TokenString;
	  if(ToLower)
	    TokenString=Convert2Lower(string(Token));
	  else
	    TokenString=Token;
	  TokenList.push_back(TokenString);
	  free(Token);
	}
      }
      Start=Finish+1;
      WasChar=false;
    }
  }
  Finish=C;
  if(WasChar){
    unsigned long int Size=Finish-Start;
    char *Token=(char *)malloc((Size+1)*sizeof(char));
    if(Token){
      memcpy(Token,InputData+Start,Size);
      Token[Size]=0;
      string TokenString;
      if(ToLower)
	TokenString=Convert2Lower(string(Token));
      else
	TokenString=Token;
      TokenList.push_back(TokenString);
      free(Token);
    }
  }
  return TokenList;
}

vector<string> List::String2List(string Input,char SplitOn){
  bool IsSpace=false,WasChar=false;
  unsigned long int InputSize=Input.size(),C=0,Start=0,Finish=0;
  const char *InputData=Input.c_str();
  vector<string> TokenList={};
  for(C=0;C<InputSize;C++){
    if(InputData[C]==SplitOn){
      IsSpace=true;
    }
    else{
      IsSpace=false;
      WasChar=true;
    }
    if(IsSpace){
      Finish=C;
      if(WasChar){
	unsigned long int Size=Finish-Start;
	char *Token=(char *)malloc((Size+1)*sizeof(char));
	if(Token){
	  memcpy(Token,InputData+Start,Size);
	  Token[Size]=0;
	  string TokenString;
	  /*
	  if(ToLower)
	    TokenString=Convert2Lower(string(Token));
	  else
	  */
	  TokenString=Token;
	  TokenList.push_back(TokenString);
	  free(Token);
	}
      }
      Start=Finish+1;
      WasChar=false;
    }
  }
  Finish=C;
  if(WasChar){
    unsigned long int Size=Finish-Start;
    char *Token=(char *)malloc((Size+1)*sizeof(char));
    if(Token){
      memcpy(Token,InputData+Start,Size);
      Token[Size]=0;
      string TokenString;
      TokenString=Token;
      TokenList.push_back(TokenString);
      free(Token);
    }
  }
  return TokenList;
  
}

vector<string> List::DivideCompletely(string &Input){
  bool IsPunct=false,WasChar=false;
  int QuoteLevel=0;
  unsigned long int InputSize=Input.size(),C=0,Start=0,Finish=0;
  const char *InputData=Input.c_str();
  vector<string> TokenList={};

  int Classification=ClassifyToken(Input);
  if((Classification&HASQUOTES)||(Classification&HASNUMBERS)||(Input==":")||(Input=="#")||(Input=="$"))
    TokenList.push_back(Input);
  else{
    for(C=0;C<InputSize;C++){
      if(!isalnum(InputData[C])){
	IsPunct=true;
      }
      else{
	IsPunct=false;
	WasChar=true;
      }
      if(IsPunct&&(WasChar||(InputData[C]!=':'))){
	Finish=C;
	if(WasChar){
	  unsigned long int Size=Finish-Start;
	  char *Token=(char *)malloc((Size+1)*sizeof(char));
	  if(Token){
	    memcpy(Token,InputData+Start,Size);
	    Token[Size]=0;
	    string TokenString=string(Token);
	    TokenList.push_back(TokenString);
	    free(Token);
	  }
	}
	char *Token=(char *)malloc(2*sizeof(char));
	Token[0]=InputData[C];
	Token[1]=0;
	string TokenString=string(Token);
	TokenList.push_back(TokenString);
	free(Token);
	Start=Finish+1;
	WasChar=false;
      }
    }
    Finish=C;
    if(WasChar){
      unsigned long int Size=Finish-Start;
      char *Token=(char *)malloc((Size+1)*sizeof(char));
      if(Token){
	memcpy(Token,InputData+Start,Size);
	Token[Size]=0;
	string TokenString=string(Token);
	TokenList.push_back(TokenString);
	free(Token);
      }
    }
    
  }
  return TokenList;
}

vector<string> List::DivideInput(string &Input){
  size_t C=0,i=0;
  vector<string> SeparatedFirstPass={};
  vector<string> SeparatedSecondPass={};
  vector<string> SeparatedFinalPass={};

  SeparatedFirstPass=String2List(Input,true);

  for(C=0;C<SeparatedFirstPass.size();C++){
    SeparatedSecondPass=DivideCompletely(SeparatedFirstPass[C]);
    for(i=0;i<SeparatedSecondPass.size();i++){
      SeparatedFinalPass.push_back(SeparatedSecondPass[i]);
    }
  }

  return SeparatedFinalPass;
}

string List::List2String(vector<string> Input,string Separator){
  string Converted="";
  for(size_t Pos=0;Pos<Input.size();Pos++){
    Converted=Converted+Input[Pos];
    if(Pos<Input.size()-1)
      Converted=Converted+Separator;
  }
  return Converted;
}

string List::Set2String(vector<string> Input){
  string Converted="{";
  for(size_t Pos=0;Pos<Input.size();Pos++){
    Converted=Converted+Input[Pos];
    if(Pos<Input.size()-1)
      Converted=Converted+",";
  }
  Converted+="}";
  return Converted;
}

void List::PrintList(vector<vector<string>> &Input){
  putchar('{');
  for(size_t Outer=0;Outer<Input.size();++Outer){
    putchar('{');
    for(size_t Inner=0;Inner<(Input[Outer]).size();++Inner){
      if(Inner<(Input[Outer]).size()-1){
	printf("%s,",(Input[Outer])[Inner].c_str());
      }
      else{
	printf("%s",(Input[Outer])[Inner].c_str());
      }
    } 
    putchar('}');
  }
  puts("}");
}

vector<string> List::SplitOffPrefix(string &Symbol){
  vector<string> Expanded;
  string Pending=Symbol;
  while(BinData::ContainsFirstCharacters(Pending)){
    string First=Pending.substr(0,1);
    Pending=Pending.substr(1);
    Expanded.push_back(First);
  }  
  Expanded.push_back(Pending);
  return Expanded;
}

vector<string> List::SplitOffSuffix(string &Symbol){
  vector<string> Expanded;
  string Pending=Symbol;
  while(BinData::ContainsLastCharacters(Pending)){
    size_t Length=Pending.size();
    string Last=Pending.substr(Length-1);
    Pending=Pending.substr(0,Length-1);
    Expanded.push_back(Last);
  }
  Expanded.push_back(Pending);  
  reverse(Expanded.begin(),Expanded.end());
  return Expanded;
}

vector<string> List::SplitSymbol(string &Symbol){
  vector<string> Expanded;
  string Pending;
  bool WasSplit=false;
  if(BinData::ContainsSplitCharacters(Symbol)){
    for(size_t _=0;_<Symbol.size();++_){
      char PendingChar=(Symbol.c_str())[_];      
      bool ToSplit=false;
      for(size_t SepChar=0;SepChar<strlen(BinData::SepSplitSymbols);++SepChar){
	if(PendingChar==BinData::SepSplitSymbols[SepChar]){
	  ToSplit=true;
	  break;
	}
      }
      if(ToSplit){
	if(Pending.size())
	  Expanded.push_back(Pending);
	if(!isspace(PendingChar)){
	  string NewString;	  
	  NewString+=PendingChar;
	  Expanded.push_back(NewString);
	}
	Pending="";
	continue;
      }
      else{
	if(WasSplit){
	  if(Pending.size())
	    Expanded.push_back(Pending);
	  if(!isspace(PendingChar)){
	    string NewString;
	    NewString+=PendingChar;
	    Expanded.push_back(NewString);
	  }
	  Pending="";
	  continue;
	}
      }
      Pending+=PendingChar;
      WasSplit=ToSplit;
    }    
    if(Pending.size())
      Expanded.push_back(Pending);
  }
  else{
    Expanded.push_back(Symbol);
  }
  return Expanded;
}

/*
  This is used to split up the data in ENiX ready for NLP. 
  Keywords: Separate / Split / Format
 */
vector<string> List::SeparateSymbols(vector<string> Symbols){
  vector<string> Expanded;
  for(size_t _=0;_<Symbols.size();++_){
    vector<string> First=SplitOffPrefix(Symbols[_]);
    vector<string> Second=SplitOffSuffix(*(First.end()-1));
    First.pop_back();
    Expanded=VecAdd(Expanded,First);
    Expanded=VecAdd(Expanded,Second);    
  }
  return Expanded;
}


/*
  This is used to split up the input into ENiX initially. 
  Keywords: Separate / Split / Format
 */
vector<string> List::PrepareInput(vector<string> Symbols){
  vector<string> Expanded;
  for(size_t _=0;_<Symbols.size();++_){
    vector<string> ThisExpansion=SplitSymbol(Symbols[_]);
    Expanded=VecAdd(Expanded,ThisExpansion);
  }
  return Expanded;
}

/*
  Returns true if there are duplicates in Input. False if it's a set.
 */
bool List::ContainsDuplicates(vector<string> Input){
  for(size_t Outter=0;Outter<Input.size();++Outter)
    for(size_t Inner=Outter+1;Inner<Input.size();++Inner)
      if(Input[Inner]==Input[Outter])
	return true;
  return false;
}

/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
*/
