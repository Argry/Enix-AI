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
                                        


Programmed By M.S.Braithwaite 2014.

*/

#ifndef ENiXBINDATA
#define ENiXBINDATA

#include <string>

using namespace std;

class BinData{
public:
  BinData();
  ~BinData();
  BinData(const char *InitStr,int InSize);
  BinData(string InStr);
  BinData(const BinData &Input);
  BinData(size_t InSize);
  void Change(string Input);
  void Change(const char *InitStr,int InSize);
  char *Str;
  unsigned long int Size;
  void Print();
  void PrintIfASCII(unsigned long int Characters=-1);
  void PrintHex();
  string Convert2Str();
  BinData &operator=(const BinData &Data);
  BinData &operator=(const string &Data);
  bool operator==(const BinData &Data);
  bool operator<(const BinData &Data);

  static bool CompareWithin(BinData &Larger, BinData &Smaller);
  static string GetHomeDir();
  static string Int2String(int Input);
  static string Float2String(double Input);
  static string NameId();
  static string GetSessionIdentifier();
  static bool IsNumber(string &Input);

  static string FormatString(const char *Format,...);
  static string RemovePrefix(string &OriginalData,const string &Prefix);
  
  string ToLower(string &Input);
  BinData ToLower(BinData &Input);

  static char *SepStartSymbols;
  static char *SepLastSymbols;
  static char *SepSplitSymbols;

  static bool ContainsLastCharacters(string String);
  static bool ContainsFirstCharacters(string String);
  static bool ContainsSplitCharacters(string String);

  
};

#endif

/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
*/
