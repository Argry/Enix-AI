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
                                        


Programmed By M.S.Braithwaite 2016.

*/

#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <algorithm>

#include <sys/types.h>
#include <pwd.h>

#include "ENiX_Binary.h"
#include "ENiX_Settings.h"

char *BinData::SepStartSymbols=(char *)"{\'\"";
char *BinData::SepLastSymbols=(char *)".?!\'\"}:,;";
char *BinData::SepSplitSymbols=(char *)"{}()[], \n\t\r-+*/";

/*! \brief Get the name of the session.
 *
 */
string BinData::GetSessionIdentifier(){
  string SessionIdentifier="[Session:";
  SessionIdentifier+=BinData::Int2String(getpid());
  SessionIdentifier+="] ";
  return SessionIdentifier;
}



bool BinData::ContainsLastCharacters(string String){
  if(String.size()>1){
    char LastChar=(String.c_str())[String.size()-1];
    for(size_t _=0;_<strlen(SepLastSymbols);++_){
      if(LastChar==SepLastSymbols[_]){
	return true;
      }
    }
  }
  return false;
}

bool BinData::ContainsFirstCharacters(string String){
  if(String.size()>1){
    char FirstChar=(String.c_str())[0];
    for(size_t _=0;_<strlen(SepStartSymbols);++_){
      if(FirstChar==SepStartSymbols[_]){
	return true;
      }
    }
  }
  return false;
}

bool BinData::ContainsSplitCharacters(string String){
  if(String.size()>1){
    for(size_t _=0;_<String.size();++_){
      char CurrentChar=(String.c_str())[_];
      for(size_t SpecialChar=0;SpecialChar<strlen(SepSplitSymbols);++SpecialChar){
	if(CurrentChar==SepSplitSymbols[SpecialChar]){
	  return true;
	}
      }
    }
  }
  return false;
}

string BinData::FormatString(const char *Format,...){
  // We need to make this variable in size!
  va_list Args;
  va_start(Args,Format);
  size_t SizeNeeded=1024;
  char *SizedBuffer=(char *)malloc((SizeNeeded)*sizeof(char));
  size_t MadeSize=vsnprintf(SizedBuffer,SizeNeeded,Format,Args);  
  string FormattedData(SizedBuffer,MadeSize);  
  va_end(Args);
  if(SizedBuffer){
    free(SizedBuffer);
  }
  return FormattedData;
}

bool BinData::IsNumber(string &Input){
  size_t Len=Input.size();
  for(size_t _=0;_<Len;++_){
    char Character=Input.c_str()[_];
    if((Character=='-')||(Character=='+')||(Character=='.')||isdigit(Character)){}
    else
      return false;
  }
  return true;
}

string BinData::GetHomeDir(){
  struct passwd *Password=getpwuid(getuid());
  const char *HomedirStr=Password->pw_dir;
  string Homedir=HomedirStr;
  return Homedir;
}

bool BinData::CompareWithin(BinData &Larger, BinData &Smaller){
  if(Larger.Size>=Smaller.Size){
    size_t SmallestSize=Smaller.Size;
    if(!memcmp(Larger.Str,Smaller.Str,SmallestSize))
      return true;
  }
  return false;
}

void BinData::Change(const char *InitStr,int InSize){
  if(Str)
    free(Str);
  Size=InSize;
  Str=(char *)malloc((Size+1)*sizeof(char));
  memcpy(Str,InitStr,Size);
  Str[Size]=0;
}

BinData::BinData(const BinData &Input){
  (*this).Size=Input.Size;
  (*this).Str=(char *)malloc((Input.Size+1)*sizeof(char));
  memcpy((*this).Str,Input.Str,Input.Size);
  (*this).Str[Input.Size]=0;
}

string BinData::ToLower(string &Input){
  string Output=Input;
  transform(Output.begin(),Output.end(),Output.begin(),::tolower);
  return Output;
}

BinData BinData::ToLower(BinData &Input){
  BinData Output=Input;
  for(size_t _=0;_<Output.Size;_++)
    Output.Str[_]=tolower(Output.Str[_]);
  return Output;
}

string BinData::Convert2Str(){
  string ConvertedString(Str,Size);  
  return ConvertedString;
}

void BinData::PrintIfASCII(unsigned long int Characters){
  unsigned long int C=0;
  bool Binary=0;
  for(C=0;C<Size;C++)
    if((Str[C]>126)||(Str[C]<32)){
      Binary==true;
      break;
    }  
  if(Binary)
    printf("Binary");
  else{
    if(Characters==-1)
      Print();
    else
      for(C=0;(C<Size)&&(C<Characters);C++)
	putchar(Str[C]);
  }
}

bool BinData::operator==(const BinData &Data){
  if(this->Size!=Data.Size)
    return false;
  if(memcmp(Data.Str,this->Str,Data.Size))
    return false;
  return true;
}

bool BinData::operator<(const BinData &Data){
  unsigned long int Size=0;
  Size=((this->Size)<Data.Size)?(this->Size):Data.Size;
  if(memcmp(this->Str,Data.Str,Size)<0)
    return true;
  else
    return false;
}

string BinData::Float2String(double Input){
  ostringstream Converted;
  Converted<<Input;
  return Converted.str();
}

string BinData::Int2String(int Input){
  ostringstream Converted;
  Converted<<Input;
  return Converted.str();
}

string BinData::NameId(){
  char Hostname[MAXHOSTNAME];
  char Username[MAXUSERNAME];
  memset(Hostname,0,MAXHOSTNAME);
  memset(Username,0,MAXUSERNAME);
  gethostname(Hostname,MAXHOSTNAME);
  getlogin_r(Username,MAXUSERNAME);
  string HostN(Hostname);
  string UserN(Username);
  return UserN+"@"+HostN;
}

BinData &BinData::operator=(const BinData &Data){
  Size=Data.Size;
  Str=(char *)malloc((Size+1)*sizeof(char));
  memcpy(Str,Data.Str,Size);
  Str[Size]=0;
  return *this;
}

BinData &BinData::operator=(const string &Data){
  Size=Data.size();
  Str=(char *)malloc((Size+1)*sizeof(char));
  memcpy(Str,Data.data(),Size);
  Str[Size]=0;
  return *this;
}

void BinData::Print(){
  char *Output=(char *)malloc(((Size)+1)*sizeof(char));
  memcpy(Output,Str,Size);
  Output[Size]=0;
  printf("%s",Output);
  if(Str)
    free(Output);
}

void BinData::PrintHex(){
  unsigned long int Pos=0;
  for(Pos=0;(Pos<Size)&&(Pos<HEXLENGTH);Pos++)
    printf("%02x ",Str[Pos]&255);
}

BinData::BinData(string InStr){
  Size=InStr.size();
  Str=(char *)malloc((Size+1)*sizeof(char));
  memcpy(Str,InStr.c_str(),Size);
  Str[Size]=0;
}

void BinData::Change(string Input){
  if(Str)
    free(Str);
  Size=Input.size();
  Str=(char *)malloc((Size+1)*sizeof(char));
  memcpy(Str,Input.c_str(),Size);
  Str[Size]=0;
}

BinData::BinData(const char *InitStr,int InSize){
  Size=InSize;
  if(InSize&&InitStr){
    Str=(char *)malloc((Size+1)*sizeof(char));
    memcpy(Str,InitStr,Size);
    Str[Size]=0;
  }
  else{
    Str=(char *)malloc(sizeof(char));
    Str[0]=0;
  }
}

BinData::BinData(size_t InSize){
  Size=InSize;
  if(InSize){
    Str=(char *)malloc((Size+1)*sizeof(char));
  }
  else{
    Str=(char *)malloc(sizeof(char));
    Str[0]=0;
  }
}

BinData::BinData(){
  Str=NULL;
  Size=0;
}

BinData::~BinData(){
  if(Str&&Size){
    free(Str);
    Str=NULL;
  }
  Size=0;
}

string BinData::RemovePrefix(string &OriginalData,const string &Prefix){
  const char *StartPos=strstr(OriginalData.c_str(),Prefix.c_str());
  if(StartPos){
    const char *SuffixP=StartPos+Prefix.size();
    string Suffix(SuffixP);
    return Suffix;    
  }
  return OriginalData;
}

/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
*/
