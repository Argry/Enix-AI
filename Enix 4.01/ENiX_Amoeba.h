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

#ifndef ENiXAMOEBA
#define ENiXAMOEBA

#include "ENiX_SHM.h"
#include "ENiX_Settings.h"

struct SearchThreadData{
  unsigned long int *FoundLocation;
  unsigned long int Offset;
  BinData *SearchData;
  bool *Found;
  char *DataArea;
  unsigned long int *RecordNum;
  unsigned long int *UsableAllocSpace;
  bool (*ComparisonFunc)(unsigned long int A,unsigned long int B);
};

/*
  The allocation table is divided in 5 parts:
  1) Not fragmented, always full shared between:
    a) Optimised Search list
    b) Free list
  2) Fragmented pointer list (for referenced entries)
  3) Data start positions
  4) Data end positions
  5) Defrag accelerator
 */
class Amoeba:public SHM{
 public:
  Amoeba(){};
  Amoeba(SHMConfig DBConfig);
  ~Amoeba();
  
  void CreateEmptyDB();
  unsigned long int AddData(BinData Data);
  BinData GetData(unsigned long int Reference);
  BinData GetName(unsigned long int StaticReference);
  bool RepData(BinData Data,unsigned long int Reference);
  bool DelData(unsigned long int Reference);
  bool Defrag();
  unsigned long int Search(BinData Search);
  unsigned long int PartSearch(BinData Search);
  unsigned long int GenericSearch(BinData Search,bool (*ComparisonFunc)(unsigned long int A,unsigned long int B));

  void DebugAllocTable(string Prefix);
  void DebugAllocGeneral(string Prefix);
  void DebugDatabase(string Prefix);

  void DebugAllocTable();
  void DebugAllocGeneral();
  void DebugDatabase();

  unsigned long int GetIndexFromNumber(unsigned long int RecordNum);
  unsigned long int RecordNum;

  void PutDBInfo();
  void GetDBInfo();
  void InitFreeList(string Prefix="");

  void Statistics();
  void Statistics(string Prefix);

 protected:
  static SHMConfig MemConfig;
  
  void ClearSHM();
  unsigned long int WriteAllocPos(void *DataToWrite,size_t DataSize,unsigned long int Offset);
  unsigned long int FreeRecordNum;
  unsigned long int MaxRecordNum;
  unsigned long int LastDataWrittenPos;
  unsigned long int LastAllocWrittenPos;
  unsigned long int LastDefragAccel;
  unsigned long int UsableAllocSpace;
  /* Search Data */
  unsigned long int FoundLocation; 
  bool Found;
  static void *GenericSearchThread(void *SearchTD);
  static bool EqualTo(unsigned long int A,unsigned long int B);
  static bool GreaterOrEqual(unsigned long int A,unsigned long int B);
  bool Queen;

  unsigned long int FirstFreeStartPos();
};

#endif


/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
 */
