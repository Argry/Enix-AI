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

#include <stdio.h>
#include <iostream>
#include <ctime>
#include "Amoeba_CLITest.h"
#include "ENiX_WMSAPI.h"
#include "ENiX_Packet.h"
#include "ENiX_Settings.h"

time_t Start;
time_t Finish;

void Test_Display_Time();

void WMS_API_Test::Test_Packet_001(){
  
  vector<BinData> Binaries={};
  vector<BinData> Strings={};
  
  BinData Wibble1("1234");
  BinData Wibble2("2345");
  BinData Wibble3("abcde");
  Binaries.push_back(Wibble1);
  Binaries.push_back(Wibble2);
  Binaries.push_back(Wibble3);

  puts("1234 converted into Hex is:");
  Wibble1.PrintHex();
  putchar('\n');

  BinData Wobble1("!1234!");
  BinData Wobble2("!2345!");
  BinData Wobble3("!abcde!");
  Strings.push_back(Wobble1);
  Strings.push_back(Wobble2);
  Strings.push_back(Wobble3);

  //  Packet(vector<BinData> SendStr,vector<BinData> SendData);
  Packet CreatePacket(Strings,Binaries);
  puts("Printing actual:");
  CreatePacket.Print();
  puts("Printing raw:");
  CreatePacket.AllData.PrintHex();
  putchar('\n');
  Packet TranslatedPacket(CreatePacket.AllData);
  TranslatedPacket.Print();
  
}

void WMS_API_Test::Test_BinData_001(){
  BinData Test1=(string)"Assignment works.";
  Test1.Print();
  putchar('\n');
  if(Test1==Test1)
    puts("Comparison test 1 works.");
  else
    puts("Comparison test 1 does not work.");
  BinData Test2=(string)"Assignment 2 works.";
  if(Test1==Test2)
    puts("Comparison test 2 fails.");
  else
    puts("Comparison test 2 works.");
  if(Test1<Test2)
    puts("Order failure");
  else
    puts("Order success");
}

void WMS_API_Test::Test_WMS2Amoeba_ModAttr_001(){
  DebugAllocTable();
  vector <string> Test1={"Hello world!","B","C"};
  vector <string> Test2={"Attr1","Attr2","Attr3"};
  vector <string> Test3={"Value1","Value2","Value3"};  
  BinData NewAttr=(string)"Test123";
  BinData FindMe=(string)"Hello world!";
  WMS_Rec NewWMS;
  NewWMS=AddSeg2WMSRec(NewWMS,Test1);
  NewWMS=AddSeg2WMSRec(NewWMS,Test2);
  NewWMS=AddSeg2WMSRec(NewWMS,Test3);
  puts("WMS Record to be added is:");
  PrintWMSRec(NewWMS);
  AddWMSToDB(NewWMS);
  unsigned long int Loc=Search(FindMe);
  if(Loc==-1)
    puts("FAIIIL");
  PrintWMSRec(Amoeba2WMS(Bin2Amoeba(GetData(Loc))));
  RepWMSAttr(NewAttr,Loc,1,1);
  Loc=Search((string)"Hello world!");
  PrintWMSRec(Amoeba2WMS(Bin2Amoeba(GetData(Loc))));
}

void WMS_API_Test::Test_WMS2Amoeba_ModAttr_002(){
  DebugAllocTable();
  vector <string> Test1={"Hello world!","B","C"};
  vector <string> Test2={"Attr1","Attr2","Attr3"};
  vector <string> Test3={"Value1","Value2","Value3"};  
  BinData NewAttr=(string)"Test123";
  BinData FindMe=(string)"Hello world!";
  WMS_Rec NewWMS;
  NewWMS=AddSeg2WMSRec(NewWMS,Test1);
  NewWMS=AddSeg2WMSRec(NewWMS,Test2);
  NewWMS=AddSeg2WMSRec(NewWMS,Test3);
  puts("WMS Record to be added is:");
  PrintWMSRec(NewWMS);
  AddWMSToDB(NewWMS);
  unsigned long int Loc=Search(FindMe);
  if(Loc==-1)
    puts("FAIIIL");
  PrintWMSRec(Amoeba2WMS(Bin2Amoeba(GetData(Loc))));
  RepWMSAttr(NewAttr,Loc,1,0);
  Loc=Search((string)"Test123");
  PrintWMSRec(Amoeba2WMS(Bin2Amoeba(GetData(Loc))));
}

void WMS_API_Test::Test_WMS2Amoeba_DelPair_001(){
  DebugAllocTable();
  vector <string> Test1={"Hello world!","B","C"};
  vector <string> Test2={"Attr1","Attr2","Attr3"};
  vector <string> Test3={"Value1","Value2","Value3"};  
  BinData FindMe=(string)"Hello world!";
  WMS_Rec NewWMS;
  NewWMS=AddSeg2WMSRec(NewWMS,Test1);
  NewWMS=AddSeg2WMSRec(NewWMS,Test2);
  NewWMS=AddSeg2WMSRec(NewWMS,Test3);
  puts("WMS Record to be added is:");
  PrintWMSRec(NewWMS);
  AddWMSToDB(NewWMS);
  unsigned long int Loc=Search(FindMe);
  if(Loc==-1)
    puts("FAIIIL");
  PrintWMSRec(Amoeba2WMS(Bin2Amoeba(GetData(Loc))));
  DelWMSPair(Loc,1);
  PrintWMSRec(Amoeba2WMS(Bin2Amoeba(GetData(Loc))));
}

void WMS_API_Test::Test_WMS2Amoeba_AddPair_001(){
  DebugAllocTable();
  vector <string> Test1={"Hello world!","B","C"};
  vector <string> Test2={"Attr1","Attr2","Attr3"};
  vector <string> Test3={"Value1","Value2","Value3"};  
  BinData FindMe=(string)"Hello world!";
  WMS_Rec NewWMS;
  NewWMS=AddSeg2WMSRec(NewWMS,Test1);
  NewWMS=AddSeg2WMSRec(NewWMS,Test2);
  NewWMS=AddSeg2WMSRec(NewWMS,Test3);
  puts("WMS Record to be added is:");
  PrintWMSRec(NewWMS);
  AddWMSToDB(NewWMS);
  unsigned long int Loc=Search(FindMe);
  if(Loc==-1)
    puts("FAIIIL");
  
  PrintWMSRec(Amoeba2WMS(Bin2Amoeba(GetData(Loc))));
  BinData NewAttr=(string)"wibbleAttr";
  BinData NewValue=(string)"wibbleValue";
  AddWMSPair(NewAttr,NewValue,Loc);
  /*
  Loc=Search(FindMe);
  if(Loc==-1)
    puts("FAIIIL");
  */
  PrintWMSRec(Amoeba2WMS(Bin2Amoeba(GetData(Loc))));
}

void WMS_API_Test::Test_WMS2Amoeba_AddPair_002(){
  DebugAllocTable();
  vector <string> Test1={"Hello world!","B","C"};
  vector <string> Test2={"Attr1","Attr2","Attr3"};
  vector <string> Test3={"Value1","Value2","Value3"};  
  BinData FindMe=(string)"Hello world!";
  WMS_Rec NewWMS;
  NewWMS=AddSeg2WMSRec(NewWMS,Test1);
  NewWMS=AddSeg2WMSRec(NewWMS,Test2);
  NewWMS=AddSeg2WMSRec(NewWMS,Test3);
  puts("WMS Record to be added is:");
  PrintWMSRec(NewWMS);
  AddWMSToDB(NewWMS);
  unsigned long int Loc=Search(FindMe);
  if(Loc==-1)
    puts("FAIIIL");
  PrintWMSRec(Amoeba2WMS(Bin2Amoeba(GetData(Loc))));
  BinData NewAttr=(string)"wibbleAttr";
  BinData NewValue=(string)"wibbleValue";
  AddWMSPair(NewAttr,NewValue,Loc);
  PrintWMSRec(Amoeba2WMS(Bin2Amoeba(GetData(Loc))));
  puts("WMS Record to be added is:");
  PrintWMSRec(NewWMS);
  AddWMSToDB(NewWMS);  
  Loc=Search(FindMe);
  if(Loc==-1)
    puts("FAIIIL");
  PrintWMSRec(Amoeba2WMS(Bin2Amoeba(GetData(Loc))));
}

void WMS_API_Test::Test_WMS2Amoeba_Add(){
  DebugAllocTable();
  vector <string> Test1={"Hello world!","B","C"};
  vector <string> Test2={"Attr1","Attr2","Attr3"};
  vector <string> Test3={"Value1","Value2","Value3"};  
  vector <string> Test4={"wibbbbbbbbbbble!","B","C"};
  WMS_Rec NewWMS;
  NewWMS=AddSeg2WMSRec(NewWMS,Test1);
  NewWMS=AddSeg2WMSRec(NewWMS,Test2);
  NewWMS=AddSeg2WMSRec(NewWMS,Test3);
  puts("WMS Record to be added is:");
  PrintWMSRec(NewWMS);
  vector<Amoeba_Rec> Data=WMS2Amoeba(NewWMS);
  DebugAll(Data);
  WriteAll2DB(Data);
  DebugAllocTable();
  puts("Checking the data that was stored in the DB:");
  for(int C=0;C<Data.size();C++)
    Bin2Amoeba(GetData(Data[C].Location)).Print();

  WMS_Rec WMSTest=Amoeba2WMS(Data[0]);
  PrintWMSRec(WMSTest);
}

void WMS_API_Test::Test_WMS2Amoeba_Delete(){
  vector <string> Test1={"Hello world!","B","C"};
  vector <string> Test2={"Attr1","Attr2","Attr3"};
  vector <string> Test3={"Value1","Value2","Value3"};  
  vector <string> Test4={"wibbbbbbbbbbble!","B","C"};
  WMS_Rec NewWMS;
  NewWMS=AddSeg2WMSRec(NewWMS,Test1);
  NewWMS=AddSeg2WMSRec(NewWMS,Test2);
  NewWMS=AddSeg2WMSRec(NewWMS,Test3);
  puts("WMS Record to be added is:");
  PrintWMSRec(NewWMS);
  DebugDatabase();
  DebugAllocTable();
  ShowAllAmoebas();
  //  DebugAll(Data);
  AddWMSToDB(NewWMS);
  DebugDatabase();
  DebugAllocTable();
  ShowAllAmoebas();
  //  DebugAll(Data);
  BinData Name=(string)"Hello world!";
  DelWMSInDB(Name);
  DebugDatabase();
  DebugAllocTable();
  ShowAllAmoebas();
  //  DebugAll(Data);

  /*
  DebugAllocTable();
  vector <string> Test1={"Hello world!","B","C"};
  vector <string> Test2={"Attr1","Attr2","Attr3"};
  vector <string> Test3={"Value1","Value2","Value3"};  
  vector <string> Test4={"wibbbbbbbbbbble!","B","C"};
  WMS_Rec NewWMS;
  NewWMS=AddSeg2WMSRec(NewWMS,Test1);
  NewWMS=AddSeg2WMSRec(NewWMS,Test2);
  NewWMS=AddSeg2WMSRec(NewWMS,Test3);
  puts("WMS Record to be added is:");
  PrintWMSRec(NewWMS);
  vector<Amoeba_Rec> Data=WMS2Amoeba(NewWMS);
  DebugAll(Data);
  WriteAll2DB(Data);
  DebugAllocTable();
  puts("Checking the data that was stored in the DB:");
  for(int C=0;C<Data.size();C++)
    Bin2Amoeba(GetData(Data[C].Location)).Print();

  WMS_Rec WMSTest=Amoeba2WMS(Data[0]);
  PrintWMSRec(WMSTest);

  //  WMS_Rec Amoeba2WMS(struct Amoeba_Rec ToConv);
  */
}


void Test_Display_Time(){
  long TimeDiff=Finish-Start;
  printf("Time taken: %ld seconds\n",TimeDiff);
}

Amoeba_Rec WMS_API_Test::Create_Amoeba_Rec(string Name){
  int A=0,B=0,MaxSegs=3,MaxElements=4;
  struct Amoeba_Rec CreatedAmoeba={};
  vector <unsigned long int> SegData;
  BinData PrimaryKey(Name); 
  CreatedAmoeba.PrimaryKey=PrimaryKey;
  CreatedAmoeba.RefSeg.resize(0);
  for(A=0;A<MaxSegs;A++){
    SegData.resize(0);
    for(B=0;B<MaxElements;B++)
      SegData.push_back(B);
    CreatedAmoeba.RefSeg.push_back(SegData);
  }
  return CreatedAmoeba;
}

void WMS_API_Test::Print_Amoeba_Rec(Amoeba_Rec Rec){
  int A=0,B=0;
  vector <unsigned long int> SegData;
  printf("Primary Key: ");
  Rec.PrimaryKey.Print();
  putchar('\n');  
  for(A=0;A<Rec.RefSeg.size();A++){
    SegData=Rec.RefSeg[A];
    for(B=0;B<SegData.size();B++)
      printf("[%09lu]",(unsigned long int)SegData[B]);
    putchar('\n');
  }
}

void Amoeba_SHM_Test::Test_Search_Ref(unsigned long int Ref){
  puts("Retrieving data...");
  BinData Retrieved=GetData(Ref);
  if(Retrieved.Size){
    Retrieved.Print();
    putchar('\n');
  }
  else{
    puts("Entry blank");
  }
}

unsigned long int Amoeba_SHM_Test::Test_Add_Entry(const string Data){
  puts("Adding data...");
  BinData Msg(Data);
  unsigned long int Index=AddData(Msg);
  return Index;
}

void Amoeba_SHM_Test::Test_Delete_Ref(unsigned long int Ref){
  DelData(Ref);
}

void Amoeba_SHM_Test::Test_Rep_Ref(unsigned long int Ref,string NewData){
  puts("Replacing data...");
  BinData Msg(NewData);
  RepData(Msg,Ref);
}

void Amoeba_SHM_Test::Test_DB_Full_001(){
  puts("Filling Amoeba Alloc Area...");
  unsigned long int Pos=0;
  BinData Msg("Hello world!");
  while(Pos!=-1){
    Pos=AddData(Msg);
  }
  DebugDatabase();
  Test_Search_Ref(0);
}

void Amoeba_SHM_Test::Test_DB_Full_002(){
  puts("Filling Amoeba Data Area...");
  unsigned long int Pos=0;
  BinData Msg("Hello world!kjsfdhjkasdfhjksdfhjkasdfhkjasdfhjksadfhjkasdhfjksdfhjkadhfjkasdhfjkasdhfjkasdfhjkasdhfjkasdhfjkasdhfjkasdfhjkasdfhjkasdfhjkasdfhjkafhjkasdfhasjkdfhjkasdfhjkasdfhjkasdfhjkasdfhjkasdhfjkasdhfjkasdfhjkasdfhjkasdhfjkasdhfjkasdhfjkasdfhajksdfhjkasdfhasjkdf");
  while(Pos!=-1){
    Pos=AddData(Msg);
  }
  DebugDatabase();
}

void Amoeba_SHM_Test::Test_Defrag_001(){
  DebugAllocTable();
  Test_Add_Entry("Hello world!");
  DebugAllocTable();
  Test_Search_Ref(0);
  Test_Search_Ref(1);
  Test_Search_Ref(2);
  Test_Search_Ref(3);
  Test_Rep_Ref(2,"Hello Owl11111111111111!");
  Test_Search_Ref(0);
  DebugAllocTable();
  puts("Defragging DB...");
  Defrag();
  DebugAllocTable();
}

void Amoeba_SHM_Test::Test_Search_001(){
  unsigned long int Pos=0;
  vector <string> Test1={"Hello world!","B","C"};
  vector <string> Test2={"Attr1","Attr2","Attr3"};
  vector <string> Test3={"Value1","Value2","Value3"};  
  WMS_Rec NewWMS;
  NewWMS=AddSeg2WMSRec(NewWMS,Test1);
  NewWMS=AddSeg2WMSRec(NewWMS,Test2);
  NewWMS=AddSeg2WMSRec(NewWMS,Test3);
  PrintWMSRec(NewWMS);
  BinData NewD=WMS2Binary(NewWMS);
  AddData(NewD);
  AddData(NewD);
  AddData(NewD);
  AddData(NewD);
  DebugAllocTable();
  BinData Msg1("Hello world!2");
  Pos=Search(Msg1);
  BinData Retrieved=GetData(Pos);
  if(Retrieved.Size){
    Retrieved.Print();
    putchar('\n');
  }
  else{
    puts("Entry blank");
  }
  /* Should not find the data */
}

void Amoeba_SHM_Test::Test_Search_002(){
  unsigned long int Pos=0;
  vector <string> Test1={"Hello world!","B","C"};
  vector <string> Test2={"Attr1","Attr2","Attr3"};
  vector <string> Test3={"Value1","Value2","Value3"};  
  vector <string> Test4={"wibbbbbbbbbbble!","B","C"};
  WMS_Rec NewWMS;
  NewWMS=AddSeg2WMSRec(NewWMS,Test1);
  NewWMS=AddSeg2WMSRec(NewWMS,Test2);
  AddSeg2WMSRec(NewWMS,Test3);
  PrintWMSRec(NewWMS);  
  BinData NewD=WMS2Binary(NewWMS);
  AddData(NewD);
  AddData(NewD);
  AddData(NewD);
  AddData(NewD);
  DebugAllocTable();
  BinData Msg1("Hello world!");
  Pos=Search(Msg1);
  BinData Retrieved=GetData(Pos);
  if(Retrieved.Size){
    Retrieved.Print();
    putchar('\n');
  }
  else{
    puts("Entry blank");
  }
  WMSAPI NewRec(ThisDBConfig);
  WMS_Rec NewWMS2;
  NewRec.AddSeg2WMSRec(NewWMS2,Test4);
  NewRec.AddSeg2WMSRec(NewWMS2,Test2);
  NewRec.AddSeg2WMSRec(NewWMS2,Test3);
  NewRec.PrintWMSRec(NewWMS2);
  BinData NewD2=NewRec.WMS2Binary(NewWMS2);
  AddData(NewD2);
  BinData Msg2("wibbbbbbbbbbble!");
  Pos=Search(Msg2);
  BinData Retrieved2=GetData(Pos);
  if(Retrieved2.Size){
    Retrieved2.Print();
    putchar('\n');
  }
  else{
    puts("Entry blank");
  }
  /* should find both records */
}

void Amoeba_SHM_Test::Test_Search_003(){
  unsigned long int Pos=0;
  vector <string> Test1={"Hello world!","B","C"};
  vector <string> Test2={"Attr1","Attr2","Attr3"};
  vector <string> Test3={"Value1","Value2","Value3"};  
  vector <string> Test4={"wibbbbbbbbbbble!","B","C"};
  WMS_Rec NewWMS;
  AddSeg2WMSRec(NewWMS,Test1);
  AddSeg2WMSRec(NewWMS,Test2);
  AddSeg2WMSRec(NewWMS,Test3);
  PrintWMSRec(NewWMS);
  BinData NewD=WMS2Binary(NewWMS);
  for(int C=0;C<32;C++){
    AddData(NewD);
  }
  DebugAllocTable();
  BinData Msg1("Hello world!");
  Pos=Search(Msg1);
  BinData Retrieved=GetData(Pos);
  if(Retrieved.Size){
    Retrieved.Print();
    putchar('\n');
  }
  else{
    puts("Entry blank");
  }
  WMSAPI NewRec(ThisDBConfig);
  WMS_Rec NewWMS2;
  NewRec.AddSeg2WMSRec(NewWMS2,Test4);
  NewRec.AddSeg2WMSRec(NewWMS2,Test2);
  NewRec.AddSeg2WMSRec(NewWMS2,Test3);
  NewRec.PrintWMSRec(NewWMS2);
  BinData NewD2=NewRec.WMS2Binary(NewWMS2);
  AddData(NewD2);
  BinData Msg2("wibbbbbbbbbbble!");
  Pos=Search(Msg2);
  BinData Retrieved2=GetData(Pos);
  if(Retrieved2.Size){
    Retrieved2.Print();
    putchar('\n');
  }
  else{
    puts("Entry blank");
  }

  DebugAllocTable();
  
  /* should find both records */
}

void WMS_API_Test::Test_WMS_Rec_Constr(){
  vector <string> Test1={"A","B","C"};
  vector <string> Test2={"Attr1","Attr2","Attr3"};
  vector <string> Test3={"Value1","Value2","Value3"};
  WMS_Rec NewWMS;
  AddSeg2WMSRec(NewWMS,Test1);
  AddSeg2WMSRec(NewWMS,Test2);
  AddSeg2WMSRec(NewWMS,Test3);
  PrintWMSRec(NewWMS);
}

void WMS_API_Test::Test_Conv2WMS(){
  vector <string> Test1={"A","B","C"};
  vector <string> Test2={"Attr1","Attr2","Attr3"};
  vector <string> Test3={"Value1","Value2","Value3"};  
  WMS_Rec NewWMS;
  AddSeg2WMSRec(NewWMS,Test1);
  AddSeg2WMSRec(NewWMS,Test2);
  AddSeg2WMSRec(NewWMS,Test3);
  PrintWMSRec(NewWMS);
  BinData NewD=WMS2Binary(NewWMS);
  Binary2WMS(NewD);
  PrintWMSRec(NewWMS);
}

void WMS_API_Test::Test_AmoebaConv_001(){
  Amoeba_Rec Data=Create_Amoeba_Rec("Hello world!");
  Print_Amoeba_Rec(Data);
}

void WMS_API_Test::Test_AmoebaConv_002(){
  Amoeba_Rec NewData={};
  Amoeba_Rec Data=Create_Amoeba_Rec("Hello world2!");
  BinData ConvertedData=Amoeba2Bin(Data);
  NewData=Bin2Amoeba(ConvertedData);
  Print_Amoeba_Rec(NewData);
}

void WMS_API_Test::Test_CreateBlankRecord_001(){
  string RecordName="RecordName";
  DebugDatabase();
  DebugAllocTable();
  ShowAllAmoebas();
  AddEmptyAmoebaRec(RecordName);
  DebugDatabase();
  DebugAllocTable();
  ShowAllAmoebas();
  unsigned long int Loc=Search(RecordName);
  if(Loc==-1)
    puts("Unable to find record");
  else{
    PrintWMSRec(Amoeba2WMS(Bin2Amoeba(GetData(Loc))));    
  }
}

int main(){
  SHMConfig ConfigurationData(AMOEBABACKUP,AMOEBAALLOCSIZE,AMOEBAOFFSET,AMOEBASHMKEY,AMOEBADBSIZE);
  puts("Amoeba Test");
  Start=time(0);
  
  Amoeba_SHM_Test Amoeba(ConfigurationData);
  
  Amoeba.Test_CreateBlankRecord_001();

  //  Amoeba.Test_Packet_001();

  //Amoeba.Test_WMS2Amoeba_ModAttr_002();
  //Amoeba.Test_WMS2Amoeba_DelPair_001();
  //  Amoeba.Test_WMS2Amoeba_AddPair_002();

  //  WMS_API_Test WMS;
  
  //  WMS.Test_WMS_Rec_Constr();
  //  WMS.Test_Conv2WMS();

  
  
  Finish=time(0);
  Test_Display_Time();
}


/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
 */
