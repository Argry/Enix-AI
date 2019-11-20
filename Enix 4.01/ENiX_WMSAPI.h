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

#ifndef ENiXWMSAPI
#define ENiXWMSAPI

#include <vector>
#include "ENiX_Amoeba.h"
#include "ENiX_List.h"
#include "ENiX_Settings.h"

class Amoeba_Rec{
 public:
  BinData PrimaryKey;
  vector<vector<unsigned long int>> RefSeg;
  unsigned long int Location;
  bool Target;
  Amoeba_Rec &operator=(const string &Data);
  Amoeba_Rec &operator=(const BinData &Data);
  Amoeba_Rec Add2Segment(class Amoeba_Rec Source,unsigned long int SegId,unsigned long int Pos);
  Amoeba_Rec AddSegment(class Amoeba_Rec Source,vector<unsigned long int> Seg);
  Amoeba_Rec DelSegment(class Amoeba_Rec Source,unsigned long int DelSeg);
  Amoeba_Rec RepSegment(class Amoeba_Rec Source,vector<unsigned long int> Seg,unsigned long int RepSeg);
  void Print();
  void Print(string Prefix);
};

class WMS_Rec{
 public:
  vector<vector<BinData>> Contents;
  bool Accessed;
};

class WMSConversion{
 public:
  static WMS_Rec Binary2WMS(BinData Data);
  static BinData WMS2Binary(WMS_Rec Rec);
  static void PrintWMSRec(WMS_Rec Rec);
  static void PrintWMSRec(string Prefix,WMS_Rec Rec);
  static vector<string> WMS2Vec(WMS_Rec Rec);
  static void ProperPrintWMS(WMS_Rec Rec);
};

class WMSAPI:public Amoeba,public WMSConversion,public List{
 public:

  enum Sector{
    Header,
    Attributes,
    Values
  };

  enum HeaderSeg{
    Name,
    Grammar,
    Purpose,
    Emotion,
    FirstTime,
    LastTime,
    Probability,
    Commands
  };
  
  WMSAPI(class SHMConfig DBConfig);

  /* Top Level */
  void AddWMSToDB(WMS_Rec ToAdd);
  bool DelWMSInDB(string ToDelete);
  bool DelWMSInDB(BinData ToDelete);
  bool DelWMSInDB(unsigned long int Pos);

  void ShowAllAmoebas(string Prefix);
  void ShowAllWMS(string Prefix);

  void ShowAllAmoebas();
  void ShowAllWMS();

  void AddEmptyAmoebaRec(BinData Name);
  void AddEmptyAmoebaRec(string Name);

  bool RepWMSAttr(BinData ReplaceWith,unsigned long int RefId,unsigned long int SegId,unsigned long int AttrId);
  bool DelWMSPair(unsigned long int RefId,unsigned long int AttrId);
  bool DelWMSPair(unsigned long int RefId,BinData AttrName);

  bool AddWMSPair(BinData AddAttr,BinData AddValue,unsigned long int RefId);

  bool DeleteAmoeba(unsigned long int StaticId);

  unsigned long int GetAttrRef(unsigned long int RefId,BinData DataPrefix);
  
  /* Components */
  WMS_Rec Amoeba2WMS(struct Amoeba_Rec ToConv);
  Amoeba_Rec NewAmoeba(BinData Name);
  vector<Amoeba_Rec> WMS2Amoeba(WMS_Rec ToConv);

  BinData Amoeba2Bin(struct Amoeba_Rec ToConv);
  struct Amoeba_Rec Bin2Amoeba(BinData ToConv);
  void DebugAll(vector <Amoeba_Rec> Inp);
  void WriteAll2DB(vector <Amoeba_Rec> Inp);

  Amoeba_Rec Delink(Amoeba_Rec Inp,unsigned long int RefId);
  Amoeba_Rec Delink(Amoeba_Rec Inp,unsigned long int RefId,unsigned long int SegId,unsigned long int AttrId);

  WMS_Rec AddSeg2WMSRec(WMS_Rec Original,vector<BinData> Segment);
  WMS_Rec AddSeg2WMSRec(WMS_Rec Original,vector<string> Segment);

  /* Personality Profiles [:pprofile] */
  vector<string> GetSegment(BinData RecName,unsigned long int Seg);
  vector<string> GetSegment(string RecName,unsigned long int Seg);
  void SetPP(BinData PPName);
  void SetPP(string PPName);
  vector<string> GetPPList();
  vector<unsigned long int> GetAllPPRec();
  void AddRec2PP(WMS_Rec Rec2Add);
  void PopulateFilter(BinData PPName);
  void DeletePP(string PPName);
  void DeletePP(BinData PPName);

  vector<BinData> RecFilter;
  vector<BinData> AttrFilter;

  BinData ActivePP;

  /* Filtration */
  WMS_Rec InFilter(WMS_Rec Source);
  WMS_Rec OutFilter(WMS_Rec Source);
  WMS_Rec MergeFilters(WMS_Rec InRec,WMS_Rec OutRec);
  WMS_Rec MergeFilteredWMSToDB(WMS_Rec Source);
  bool MergeFilteredWMSPair(BinData AddAttr,BinData AddValue,unsigned long int RefId);
  bool DelFromPP(BinData ToDelete);
  bool DelFromPP(unsigned long int DelPos);

 private:  
  Amoeba_Rec FillWithBlankSegments(Amoeba_Rec Amoeba_Record);

};

#endif


/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
 */
