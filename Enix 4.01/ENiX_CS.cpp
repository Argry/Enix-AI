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
                                        


Programmed By M.S.Braithwaite 2019.

*/

#include <string.h>
#include "ENiX_Memory.h"

string Memory::CSRef(vector<string> CSAddr){
  // TODO: remove the : from the contexts

  // Newly added:

  // warn the user of duplicate symbols
  if(List::ContainsDuplicates(CSAddr)){
    printf("ERROR: it is not possible to save the context: ");
    List::PrintList(CSAddr);
    puts("because it has duplicate symbols that will break deferencing.");
    return {};
  }
  
  for(size_t _=0;_<CSAddr.size();++_){
    if((CSAddr[_]).size()&&((CSAddr[_]).at(0)==':')){
      (CSAddr[_]).erase((CSAddr[_]).begin());
    }
  }

  
  // returns the cs number with the : in it.
  if(CSAddr.size()){
    if(CSAddr.size()==1)
      return CSAddr[0];
    string RecordToCreate=CSAddr[0];
    string FirstForwardRef=CSAddr[1];
    string SecondForwardRef;
    string PreviousCreatedRec=CSAddr[0];
    for(size_t _=1;_<CSAddr.size();++_){
      SecondForwardRef=FindTotalCS(RecordToCreate,FirstForwardRef);
      if(SecondForwardRef==""){
	SecondForwardRef=GetLatestCS();
	++CSNum;
      }
      FindOrCreate(RecordToCreate);
      WMS_Rec ForwardRef=GetFromCache(RecordToCreate);
      ForwardRef=AddDBPair(ForwardRef,":CS",FirstForwardRef+" "+SecondForwardRef);
      RepInCache(ForwardRef);      
      RecordToCreate=SecondForwardRef;
      FindOrCreate(SecondForwardRef);
      WMS_Rec BackwardRef=GetFromCache(SecondForwardRef);
      BackwardRef=AddDBPair(BackwardRef,":DEREF",FirstForwardRef+" "+PreviousCreatedRec);
      RepInCache(BackwardRef);
      if((_+1)<CSAddr.size())
	FirstForwardRef=CSAddr[_+1];
      PreviousCreatedRec=RecordToCreate;
    }
    FindOrCreate(CSAddr[0]);
    WMS_Rec BackwardRef=GetFromCache(CSAddr[0]);
    BackwardRef=AddDBPair(BackwardRef,":DEREF",SecondForwardRef);
    RepInCache(BackwardRef);
    return SecondForwardRef;
  }
  return "";
}

vector<string> Memory::CSDeref(string CSRef){
  // The CS reference needs to have the : in, otherwise it could be a symbol
  vector<string> FullRef;
  if((CSRef.size()<3)||(strncmp(CSRef.c_str(),":cs",2))){
    FullRef.push_back(CSRef);
  }
  if((CSRef.size()>1)&&(!strncmp(CSRef.c_str(),(char *)":cs",2))){
    FindOrCreate(CSRef);
    WMS_Rec Rec=GetFromCache(CSRef);
    vector<string> PrevContext=GetAllDBValues(Rec,":DEREF");
    for(size_t _=0;_<PrevContext.size();++_){
      vector<string> SplitDerefs=List::String2List(PrevContext[_]);
      if(SplitDerefs.size()<2)
	continue;
      vector<string> ContextSoFar;
      vector<string> FullContext=RecCSDeref(ContextSoFar,CSRef,SplitDerefs[0],SplitDerefs[1],{});
      if(FullContext.size()){
	reverse(FullContext.begin(),FullContext.end());

	// TODO add the : to the contexts

	for(size_t _=0;_<FullContext.size();++_){
	  string Text=FullContext[_];
	  if((Text.size()>1)&&(!strncmp(Text.c_str(),"cs",2))){
	    FullContext[_]=":"+FullContext[_];
	  }
	}
	
	return FullContext;
      }
    }
  }
  return {};
}

string Memory::CSGet(vector<string> CSAddr){
  // This will need the : to be added before this function
  string RecRef=CSRef(CSAddr);
  if(RecRef=="")
    return "";
  return CSGet(RecRef);
}

string Memory::CSGet(string CSAddr){
  // This will need the : to be added before this function
  FindOrCreate(CSAddr);
  WMS_Rec Rec=GetFromCache(CSAddr);
  return ExtractString(Rec,WMSAPI::Sector::Header,WMSAPI::HeaderSeg::Commands);
}

bool Memory::CSSave(string CSAddr,string Data){
  // This will need the : to be added before this function
  long int Pos=FindOrCreate(CSAddr);
  if(Pos==-1)
    return false;
  WMS_Rec Rec=GetFromCache(CSAddr);
  Rec=RepHeader(Rec,Data,7);
  RepInCache(Rec);
  return true;
}

bool Memory::CSSave(vector<string> CSAddr,string Data){
  // This will need the : to be added before this function
  string RecRef=CSRef(CSAddr);
  if(RecRef!="")
    return CSSave(RecRef,Data);
  return false;
}




string Memory::FindTotalCS(string ContextPrefix,string ContextSuffix){
  long int Pos=FindOrCreate(ContextPrefix);
  if(Pos==-1) return "";
  vector<string> Contexts=GetAllDBValues(GetFromCache(ContextPrefix),":CS");
  for(size_t _=0;_<Contexts.size();++_){
    vector<string> Split=String2List(Contexts[_]);
    if((Split[0]==ContextSuffix)&&(Split.size()>1))
      return Split[1];
  }
  return "";
}

string Memory::GetLatestCS(){
  string TotalCS=":cs";
  TotalCS+=to_string(CSNum);
  return TotalCS;
}

bool Memory::CSPairExists(string Prefix,string Suffix){
  
}

vector<string> Memory::RecCSDeref(vector<string> ContextSoFar,string FinalCS,string CurrentCSSym,string NextCSSym,vector<string> RecsSoFar){
  ContextSoFar.push_back(CurrentCSSym);
  vector<string>::iterator FoundPos=find(RecsSoFar.begin(),RecsSoFar.end(),CurrentCSSym);
  if(FoundPos!=RecsSoFar.end()){
    return {};
  }
  RecsSoFar.push_back(CurrentCSSym);
  FindOrCreate(NextCSSym);
  WMS_Rec Rec=GetFromCache(NextCSSym);
  vector<string> PrevContext=GetAllDBValues(Rec,":DEREF");
  for(size_t _=0;_<PrevContext.size();++_){
    if(PrevContext[_]==FinalCS){
      ContextSoFar.push_back(NextCSSym);
      return ContextSoFar;
    }
    vector<string> Separated=String2List(PrevContext[_]);
    if(Separated.size()>1){
      vector<string> FullContext=RecCSDeref(ContextSoFar,FinalCS,Separated[0],Separated[1],RecsSoFar);
      if(FullContext.size())
	return FullContext;
    }
    else{
      vector<string> FullContext=RecCSDeref(ContextSoFar,FinalCS,Separated[0],"",RecsSoFar);
      if(FullContext.size())
	return FullContext;
    }
  }
  return {};
}




/*

//  - Original code:

string Memory::FindTotalCS(string ContextPrefix,string ContextSuffix){
  long int Pos=FindOrCreate(ContextPrefix);
  if(Pos==-1) return "";
  vector<string> Contexts=GetAllDBValues(GetFromCache(ContextPrefix),":CS");
  for(size_t _=0;_<Contexts.size();++_){
    vector<string> Split=String2List(Contexts[_]);
    if((Split[0]==ContextSuffix)&&(Split.size()>1))
      return Split[1];
  }
  return "";
}

string Memory::GetLatestCS(){
  string TotalCS="cs";
  TotalCS+=to_string(CSNum);
  return TotalCS;
}

bool Memory::CSPairExists(string Prefix,string Suffix){
  
}

string Memory::CSRef(vector<string> CSAddr){
  if(CSAddr.size()){
    if(CSAddr.size()==1)
      return CSAddr[0];
    string RecordToCreate=CSAddr[0];
    string FirstForwardRef=CSAddr[1];
    string SecondForwardRef;
    string PreviousCreatedRec=CSAddr[0];
    for(size_t _=1;_<CSAddr.size();++_){
      SecondForwardRef=FindTotalCS(RecordToCreate,FirstForwardRef);
      if(SecondForwardRef==""){
	SecondForwardRef=GetLatestCS();
	++CSNum;
      }
      FindOrCreate(RecordToCreate);
      WMS_Rec ForwardRef=GetFromCache(RecordToCreate);
      ForwardRef=AddDBPair(ForwardRef,":CS",FirstForwardRef+" "+SecondForwardRef);
      RepInCache(ForwardRef);      
      RecordToCreate=SecondForwardRef;
      FindOrCreate(SecondForwardRef);
      WMS_Rec BackwardRef=GetFromCache(SecondForwardRef);
      BackwardRef=AddDBPair(BackwardRef,":DEREF",FirstForwardRef+" "+PreviousCreatedRec);
      RepInCache(BackwardRef);
      if((_+1)<CSAddr.size())
	FirstForwardRef=CSAddr[_+1];
      PreviousCreatedRec=RecordToCreate;
    }
    FindOrCreate(CSAddr[0]);
    WMS_Rec BackwardRef=GetFromCache(CSAddr[0]);
    BackwardRef=AddDBPair(BackwardRef,":DEREF",SecondForwardRef);
    RepInCache(BackwardRef);
    return SecondForwardRef;
  }
  return "";
}

vector<string> Memory::CSDeref(string CSRef){
  vector<string> FullRef;
  if((CSRef.size()<3)||(strncmp(CSRef.c_str(),"cs",2))){
    FullRef.push_back(CSRef);
  }
  if((CSRef.size()>1)&&(!strncmp(CSRef.c_str(),(char *)"cs",2))){
    FindOrCreate(CSRef);
    WMS_Rec Rec=GetFromCache(CSRef);
    vector<string> PrevContext=GetAllDBValues(Rec,":DEREF");
    for(size_t _=0;_<PrevContext.size();++_){
      vector<string> SplitDerefs=List::String2List(PrevContext[_]);
      if(SplitDerefs.size()<2)
	continue;
      vector<string> ContextSoFar;
      vector<string> FullContext=RecCSDeref(ContextSoFar,CSRef,SplitDerefs[0],SplitDerefs[1],{});
      if(FullContext.size()){
	reverse(FullContext.begin(),FullContext.end());
	return FullContext;
      }
    }    
  }
  return {};
}

vector<string> Memory::RecCSDeref(vector<string> ContextSoFar,string FinalCS,string CurrentCSSym,string NextCSSym,vector<string> RecsSoFar){
  ContextSoFar.push_back(CurrentCSSym);
  vector<string>::iterator FoundPos=find(RecsSoFar.begin(),RecsSoFar.end(),CurrentCSSym);
  if(FoundPos!=RecsSoFar.end())
    return {};
  RecsSoFar.push_back(CurrentCSSym);
  FindOrCreate(NextCSSym);
  WMS_Rec Rec=GetFromCache(NextCSSym);
  vector<string> PrevContext=GetAllDBValues(Rec,":DEREF");
  for(size_t _=0;_<PrevContext.size();++_){
    if(PrevContext[_]==FinalCS){
      ContextSoFar.push_back(NextCSSym);
      return ContextSoFar;
    }
    vector<string> Separated=String2List(PrevContext[_]);
    if(Separated.size()>1){
      vector<string> FullContext=RecCSDeref(ContextSoFar,FinalCS,Separated[0],Separated[1],RecsSoFar);
      if(FullContext.size())
	return FullContext;
    }
    else{
      vector<string> FullContext=RecCSDeref(ContextSoFar,FinalCS,Separated[0],"",RecsSoFar);
      if(FullContext.size())
	return FullContext;
    }
  }
  return {};
}

string Memory::CSGet(vector<string> CSAddr){
  string RecRef=CSRef(CSAddr);
  if(RecRef=="")
    return "";
  return CSGet(RecRef);
}

string Memory::CSGet(string CSAddr){
  FindOrCreate(CSAddr);
  WMS_Rec Rec=GetFromCache(CSAddr);
  return ExtractString(Rec,WMSAPI::Sector::Header,WMSAPI::HeaderSeg::Commands);
}

bool Memory::CSSave(string CSAddr,string Data){

  DEBUGMSG;
  printf("Address:%s Value:%s\n",CSAddr.c_str(),Data.c_str());


  long int Pos=FindOrCreate(CSAddr);
  if(Pos==-1)
    return false;
  WMS_Rec Rec=GetFromCache(CSAddr);
  
  WMSConversion::PrintWMSRec(Rec);

  Rec=RepHeader(Rec,Data,7);
  
  WMSConversion::PrintWMSRec(Rec);
  
  RepInCache(Rec);
  return true;
}

bool Memory::CSSave(vector<string> CSAddr,string Data){
  string RecRef=CSRef(CSAddr);
  if(RecRef!="")
    return CSSave(RecRef,Data);
  return false;
}
 */


/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
*/
