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

#include <sstream>
#include <string.h>
#include "ENiX_Config.h"
#include "ENiX_Disk.h"

Config::Config(){
  ConfigSettings={};  
}

string Config::ConfigHeader(string Input){
  return Input;
}

void Config::ReadConfigFile(string FileName){
  ReadConfigData(Disk::ReadFromDisk(FileName));
}

void Config::ReadConfigData(BinData Contents){
  ConfigContents=Contents;
  ParseConfig();
}

void Config::WriteConfigFile(string FileName){
  MakeConfigData();
  Disk::WriteToDisk(ConfigContents,FileName);
}

BinData Config::MakeConfigData(){
  string Config="";
  vector<string> DumpedConfig=DumpConfig();
  for(size_t _=0;_<DumpedConfig.size();_++)
    Config+=DumpedConfig[_]+'\n';
  BinData ConvertedConfig(Config);
  return ConvertedConfig;
}

void Config::ParseConfig(){
  BinData Contents=ConfigContents;
  ConfigSettings={};
  size_t StartPos=0;
  for(size_t _=0;_<Contents.Size;++_){
    if(Contents.Str[_]=='\n'){
      Contents.Str[_]=0;
      char *AttrName=Contents.Str+StartPos;
      char *AttrValue=strstr(AttrName,"=");
      if(AttrValue){
	AttrValue[0]=0;
	++AttrValue;
	string ConvertedAttrName=AttrName;
	string ConvertedAttrValue=AttrValue;	
	AddToConfig(ConvertedAttrName,ConvertedAttrValue);
      }
      StartPos=_+1;
    }
  }  
}

string Config::GetValue(string AttrName){
  map<string,string>::const_iterator Location=ConfigSettings.find(AttrName);
  if(Location==ConfigSettings.end())
    return "";
  else
    return Location->second;
}

void Config::AddToConfig(string AttrName,string AttrValue){
  ConfigSettings.insert(make_pair(AttrName,AttrValue));  
}

void Config::AddToConfig(string AttrName,long int AttrValue){
  ostringstream Converted;
  Converted<<AttrValue;
  AddToConfig(AttrName,Converted.str());
}

void Config::AddToConfig(string AttrName,double AttrValue){
  ostringstream Converted;
  Converted<<AttrValue;
  AddToConfig(AttrName,Converted.str());
}

void Config::DebugConfig(string Prefix){
  vector<string> DumpedConfig=DumpConfig();
  for(size_t _=0;_<DumpedConfig.size();_++)
    printf("%s%s\n",Prefix.c_str(),DumpedConfig[_].c_str());
}

vector<string> Config::DumpConfig(){
  vector<string> Output={};
  map<string,string>::const_iterator MapWalk=ConfigSettings.begin();
  for(map<string,string>::const_iterator MapWalk=ConfigSettings.begin();MapWalk!=ConfigSettings.end();++MapWalk)
    Output.push_back(MapWalk->first+"="+MapWalk->second);
  return Output;
}

/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
*/
