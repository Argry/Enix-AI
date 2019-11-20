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

#ifndef AMOEBACLITEST
#define AMOEBACLITEST

#include "ENiX_Amoeba.h"
#include "ENiX_WMSAPI.h"
#include "ENiX_Settings.h"

class WMS_API_Test:public WMSAPI{
 public:
 WMS_API_Test(class SHMConfig DBConfig):WMSAPI(DBConfig){
    ThisDBConfig=DBConfig;
  };
  void Test_WMS_Rec_Constr();
  void Test_Conv2WMS();
  Amoeba_Rec Create_Amoeba_Rec(string Name);
  void Print_Amoeba_Rec(Amoeba_Rec Rec);
  void Test_AmoebaConv_001();
  void Test_AmoebaConv_002();
  void Test_BinData_001();
  void Test_WMS2Amoeba_Add();
  void Test_WMS2Amoeba_Delete();
  void Test_WMS2Amoeba_ModAttr_001();
  void Test_WMS2Amoeba_ModAttr_002();
  void Test_WMS2Amoeba_DelPair_001();
  void Test_WMS2Amoeba_AddPair_001();
  void Test_WMS2Amoeba_AddPair_002();
  void Test_Packet_001();
  void Test_CreateBlankRecord_001();
  
  SHMConfig ThisDBConfig;
};

class Amoeba_SHM_Test:public WMS_API_Test{
 public:
  //  Amoeba_SHM_Test():Amoeba(666){};
  Amoeba_SHM_Test(class SHMConfig DBConfig):WMS_API_Test(DBConfig){};
  void Test_Search_Ref(unsigned long int Ref);
  unsigned long int Test_Add_Entry(string Data);
  void Test_Delete_Ref(unsigned long int Ref);
  void Test_Rep_Ref(unsigned long int Ref,string NewData);
  void Test_DB_Full_001();
  void Test_DB_Full_002();
  void Test_Defrag_001();
  void Test_Search_001();
  void Test_Search_002();
  void Test_Search_003();
};


#endif


/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
 */
