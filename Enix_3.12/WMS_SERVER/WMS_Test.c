#include <stdio.h>
#include "WMS_SHM.h"
#include "WMS_Blob.h"
#include "ENiX_WMS.h"
#include "ENiX_LIST.h"

static int TestBlob1(){
  char *Str="Test";
  struct Blob *B=NULL;
  B=WMS_Str2Blob(Str);
  if(strcmp(Str,WMS_Blob2Str(B))){
    WMS_FreeBlob(B);
    return 0;
  }
  WMS_FreeBlob(B);
  return 1;
}

static int TestBlob2(){
  if(WMS_Str2Blob(NULL))
    return 0;
  return 1; 
}

static int TestBlob3(){
  if(WMS_Blob2Str(NULL))
    return 0;
  return 1; 
}

static int CreateSHM(){
  int Pass=0;
  struct DataArea *DB=NULL;
  DB=WMS_SHM_Create(0);
  Pass=DB->Queen;
  WMS_SHM_Delete(DB);
  return Pass;
}

static int SHMAttach(){
  int Pass=0;
  struct DataArea *DB=NULL;
  DB=WMS_SHM_Create(0);
  WMS_SHM_Attach(DB);
  if(DB->Shared)
    Pass=1;
  WMS_SHM_Detach(DB);
  WMS_SHM_Delete(DB);
  return Pass;
}

static int AddOneRec(){
  int Pass=0;
  long int Position=0;
  char *Str="test";
  struct Blob *B=NULL;
  struct DataArea *DB=NULL;
  DB=WMS_SHM_Create(0);
  WMS_SHM_Attach(DB); 
  B=WMS_Str2Blob(Str);
  Position=WMS_AddBinary(DB,B);
  WMS_FreeBlob(B);
  if(!strcmp(WMS_Blob2Str(WMS_GetBinary(DB,0)),Str))
    Pass=1;
  WMS_SHM_Detach(DB);
  WMS_SHM_Delete(DB);
  return Pass;
}

static int GetNonExistent(){
  int Pass=0;
  struct DataArea *DB=NULL;
  DB=WMS_SHM_Create(0);
  WMS_SHM_Attach(DB);  
  if(WMS_Blob2Str(WMS_GetBinary(DB,0))==NULL)
    Pass=1;
  WMS_SHM_Detach(DB);
  WMS_SHM_Delete(DB);
  return Pass;
}

static int AddTwoRec(){
  int Pass=1;
  long int Pos1=0,Pos2=0;
  char *Str1="test1",*Str2="test2";
  struct Blob *B=NULL;
  struct DataArea *DB=NULL;
  DB=WMS_SHM_Create(0);
  WMS_SHM_Attach(DB); 
  B=WMS_Str2Blob(Str1);
  Pos1=WMS_AddBinary(DB,B);
  WMS_FreeBlob(B);
  B=WMS_Str2Blob(Str2);
  Pos2=WMS_AddBinary(DB,B);
  WMS_FreeBlob(B);
  if(strcmp(WMS_Blob2Str(WMS_GetBinary(DB,Pos1)),Str1))
    Pass=0;
  if(strcmp(WMS_Blob2Str(WMS_GetBinary(DB,Pos2)),Str2))
    Pass=0;
  WMS_SHM_Detach(DB);
  WMS_SHM_Delete(DB);
  return Pass;
}

static int DelRec1(){
  int Pass=1;
  long int Pos1=0,Pos2=0;
  char *Str1="test1",*Str2="test2";
  struct Blob *B=NULL;
  struct DataArea *DB=NULL;
  DB=WMS_SHM_Create(0);
  WMS_SHM_Attach(DB); 
  B=WMS_Str2Blob(Str1);
  Pos1=WMS_AddBinary(DB,B);
  WMS_FreeBlob(B);
  B=WMS_Str2Blob(Str2);
  Pos2=WMS_AddBinary(DB,B);
  WMS_FreeBlob(B);
  if(strcmp(WMS_Blob2Str(WMS_GetBinary(DB,Pos1)),Str1))
    Pass=0;
  if(strcmp(WMS_Blob2Str(WMS_GetBinary(DB,Pos2)),Str2))
    Pass=0;
  WMS_DelBinary(DB,0);
  if(strcmp(WMS_Blob2Str(WMS_GetBinary(DB,Pos1)),Str2))
    Pass=0;
  if(WMS_GetBinary(DB,Pos2))
    Pass=0;
  WMS_SHM_Detach(DB);
  WMS_SHM_Delete(DB);
  return Pass;
}

static int DelRec2(){
  int Pass=1;
  long int Pos1=0,Pos2=0;
  char *Str1="test1",*Str2="test2";
  struct Blob *B=NULL;
  struct DataArea *DB=NULL;
  DB=WMS_SHM_Create(0);
  WMS_SHM_Attach(DB); 
  B=WMS_Str2Blob(Str1);
  Pos1=WMS_AddBinary(DB,B);
  WMS_FreeBlob(B);
  B=WMS_Str2Blob(Str2);
  Pos2=WMS_AddBinary(DB,B);
  WMS_FreeBlob(B);
  if(strcmp(WMS_Blob2Str(WMS_GetBinary(DB,Pos1)),Str1))
    Pass=0;
  if(strcmp(WMS_Blob2Str(WMS_GetBinary(DB,Pos2)),Str2))
    Pass=0;
  WMS_DelBinary(DB,1);
  if(strcmp(WMS_Blob2Str(WMS_GetBinary(DB,Pos1)),Str1))
    Pass=0;
  if(WMS_GetBinary(DB,Pos2))
    Pass=0;
  WMS_SHM_Detach(DB);
  WMS_SHM_Delete(DB);
  return Pass;
}

static int RepRec1(){
  int Pass=1;
  long int Pos1=0,Pos2=0;
  char *Str1="test1",*Str2="test2",*Str3="test3";
  struct Blob *B=NULL;
  struct DataArea *DB=NULL;
  DB=WMS_SHM_Create(0);
  WMS_SHM_Attach(DB); 
  B=WMS_Str2Blob(Str1);
  Pos1=WMS_AddBinary(DB,B);
  WMS_FreeBlob(B);
  B=WMS_Str2Blob(Str2);
  Pos2=WMS_AddBinary(DB,B);
  WMS_FreeBlob(B);
  if(strcmp(WMS_Blob2Str(WMS_GetBinary(DB,Pos1)),Str1))
    Pass=0;
  if(strcmp(WMS_Blob2Str(WMS_GetBinary(DB,Pos2)),Str2))
    Pass=0;
  B=WMS_Str2Blob(Str3);
  WMS_RepBinary(DB,0,B);
  WMS_FreeBlob(B);
  if(strcmp(WMS_Blob2Str(WMS_GetBinary(DB,Pos1)),Str3))
    Pass=0;
  if(strcmp(WMS_Blob2Str(WMS_GetBinary(DB,Pos2)),Str2))
    Pass=0;
  WMS_SHM_Detach(DB);
  WMS_SHM_Delete(DB);
  return Pass;
}

static int RepRec2(){
  int Pass=1;
  long int Pos1=0,Pos2=0;
  char *Str1="test1",*Str2="test2",*Str3="test32";
  struct Blob *B=NULL;
  struct DataArea *DB=NULL;
  DB=WMS_SHM_Create(0);
  WMS_SHM_Attach(DB); 
  B=WMS_Str2Blob(Str1);
  Pos1=WMS_AddBinary(DB,B);
  WMS_FreeBlob(B);
  B=WMS_Str2Blob(Str2);
  Pos2=WMS_AddBinary(DB,B);
  WMS_FreeBlob(B);
  if(strcmp(WMS_Blob2Str(WMS_GetBinary(DB,Pos1)),Str1))
    Pass=0;
  if(strcmp(WMS_Blob2Str(WMS_GetBinary(DB,Pos2)),Str2))
    Pass=0;
  B=WMS_Str2Blob(Str3);
  WMS_RepBinary(DB,0,B);
  WMS_FreeBlob(B);
  if(strcmp(WMS_Blob2Str(WMS_GetBinary(DB,Pos1)),Str3))
    Pass=0;
  if(strcmp(WMS_Blob2Str(WMS_GetBinary(DB,Pos2)),Str2))
    Pass=0;
  WMS_SHM_Detach(DB);
  WMS_SHM_Delete(DB);
  return Pass;
}

static int RepRec3(){
  int Pass=1;
  long int Pos1=0,Pos2=0,Pos3=0;
  char *Str1="test1",*Str2="test2",*Str3="test3";
  struct Blob *B=NULL;
  struct DataArea *DB=NULL;
  DB=WMS_SHM_Create(0);
  WMS_SHM_Attach(DB); 
  B=WMS_Str2Blob(Str1);
  Pos1=WMS_AddBinary(DB,B);
  WMS_FreeBlob(B);
  B=WMS_Str2Blob(Str2);
  Pos2=WMS_AddBinary(DB,B);
  WMS_FreeBlob(B);
  if(strcmp(WMS_Blob2Str(WMS_GetBinary(DB,Pos1)),Str1))
    Pass=0;
  if(strcmp(WMS_Blob2Str(WMS_GetBinary(DB,Pos2)),Str2))
    Pass=0;
  B=WMS_Str2Blob(Str3);
  Pos3=WMS_RepBinary(DB,4,B);
  WMS_FreeBlob(B);
  if(strcmp(WMS_Blob2Str(WMS_GetBinary(DB,Pos1)),Str1))
    Pass=0;
  if(strcmp(WMS_Blob2Str(WMS_GetBinary(DB,Pos2)),Str2))
    Pass=0;
  if(strcmp(WMS_Blob2Str(WMS_GetBinary(DB,Pos3)),Str3))
    Pass=0;
  WMS_SHM_Detach(DB);
  WMS_SHM_Delete(DB);
  return Pass;
}

static int GetRecName1(){
  int Pass=1;
  char *Str1="test1";
  struct Blob *B=NULL;
  struct Datablock *WMSRec1=NULL;
  WMSRec1=Add_DB_Entry(WMSRec1,Str1);
  B=WMS_WMS2Blob(WMSRec1);
  if(strcmp(Str1,WMS_GetRecName(B)))
    Pass=0;
  return Pass;
}

static int Blob2WMS1(){
  int Pass=1;
  char *Str1="test1";
  struct Blob *B=NULL;
  struct List_Str *L1=NULL,*L2=NULL;
  struct Datablock *WMSRec1=NULL,*Output=NULL;
  WMSRec1=Add_DB_Entry(WMSRec1,Str1);
  B=WMS_WMS2Blob(WMSRec1);
  Output=WMS_Blob2WMS(B);
  for(L1=Output->DS[0],L2=WMSRec1->DS[0];L1&&L2;L1=L1->Next,L2=L2->Next){
    if(strcmp(L1->Str,L2->Str)){
      Pass=0;
      break;
    }
  }
  if(Output->DS[1])
    Pass=0;
  if(Output->DS[2])
    Pass=0;
  return Pass;
}

static int Blob2WMS2(){
  int Pass=1,Sector=0;
  char *Str1="test1";
  struct Blob *B=NULL;
  struct List_Str *L1=NULL,*L2=NULL;
  struct Datablock *WMSRec1=NULL,*Output=NULL;
  WMSRec1=Add_DB_Entry(WMSRec1,Str1);
  WMSRec1->DS[1]=Append_List_Element(WMSRec1->DS[1],"alpha");
  WMSRec1->DS[2]=Append_List_Element(WMSRec1->DS[2],"beta");
  B=WMS_WMS2Blob(WMSRec1);
  Output=WMS_Blob2WMS(B);
  for(Sector=0;Sector<3;Sector++)
    for(L1=Output->DS[Sector],L2=WMSRec1->DS[Sector];L1&&L2;L1=L1->Next,L2=L2->Next)
      if(strcmp(L1->Str,L2->Str))
	return 0;
  return Pass;
}

static int Search1(){
  int Pass=1;
  struct Blob *B=NULL;
  struct DataArea *DB=NULL;
  DB=WMS_SHM_Create(0);
  WMS_SHM_Attach(DB); 
  if(WMS_FindRec(DB,NULL)!=-1)
    Pass=0;
  WMS_SHM_Detach(DB);
  WMS_SHM_Delete(DB);
  return Pass;
}

static int Search2(){
  int Pass=1;
  struct Blob *B=NULL;
  struct DataArea *DB=NULL;
  DB=WMS_SHM_Create(0);
  WMS_SHM_Attach(DB); 
  if(WMS_FindRec(DB,"test1")!=-1)
    Pass=0;
  WMS_SHM_Detach(DB);
  WMS_SHM_Delete(DB);
  return Pass;
}

static int Search3(){
  int Pass=1;
  long int Pos1=0,Pos2=0;
  char *Str1="test1",*Str2="test2";
  struct Blob *B=NULL;
  struct DataArea *DB=NULL;
  DB=WMS_SHM_Create(0);
  WMS_SHM_Attach(DB); 
  B=WMS_Str2Blob(Str1);
  Pos1=WMS_AddBinary(DB,B);
  WMS_FreeBlob(B);
  B=WMS_Str2Blob(Str2);
  Pos2=WMS_AddBinary(DB,B);
  if(WMS_FindRec(DB,Str1)!=Pos1)
    Pass=0;
  if(WMS_FindRec(DB,Str2)!=Pos2)
    Pass=0;
  WMS_SHM_Detach(DB);
  WMS_SHM_Delete(DB);
  return Pass;
}

static int Save1(){
  int Pass=1;
  long int Pos1=0,Pos2=0;
  char *Str1="test1",*Str2="test2";
  struct Blob *B=NULL;
  struct DataArea *DB=NULL;
  DB=WMS_SHM_Create(0);
  WMS_SHM_Attach(DB); 
  B=WMS_Str2Blob(Str1);
  Pos1=WMS_AddBinary(DB,B);
  WMS_FreeBlob(B);
  B=WMS_Str2Blob(Str2);
  Pos2=WMS_AddBinary(DB,B);
  WMS_Save_To(DB,"WMS_TEST1.DAT");
  WMS_SHM_Detach(DB);
  WMS_SHM_Delete(DB);
  return Pass;
}

static int Restore1(){
  int Pass=1;
  long int Pos1=0,Pos2=0;
  char *Str1="test1",*Str2="test2";
  struct Blob *B=NULL;
  struct DataArea *DB=NULL;
  DB=WMS_SHM_Create(0);
  WMS_SHM_Attach(DB); 
  DB=WMS_Read_From(DB,"WMS_TEST1.DAT",0);
  if(strcmp(WMS_Blob2Str(WMS_GetBinary(DB,0)),Str1))
    Pass=0;
  if(strcmp(WMS_Blob2Str(WMS_GetBinary(DB,1)),Str2))
    Pass=0;
  WMS_SHM_Detach(DB);
  WMS_SHM_Delete(DB);
  return Pass;
}

static int DebugAllocTable1(){
  int Pass=1;
  long int Pos1=0,Pos2=0;
  char *Str1="test1",*Str2="test2";
  struct Blob *B=NULL;
  struct DataArea *DB=NULL;
  DB=WMS_SHM_Create(0);
  WMS_SHM_Attach(DB); 
  B=WMS_Str2Blob(Str1);
  Pos1=WMS_AddBinary(DB,B);
  WMS_FreeBlob(B);
  B=WMS_Str2Blob(Str2);
  Pos2=WMS_AddBinary(DB,B);
  
  WMS_ALLOC_DEBUG(DB);

  WMS_SHM_Detach(DB);
  WMS_SHM_Delete(DB);
  return Pass;
}

int Tests=22;
int (*Test[])()={
  TestBlob1,TestBlob2,TestBlob3,CreateSHM,SHMAttach,AddOneRec,GetNonExistent,AddTwoRec,DelRec1,DelRec2,
  RepRec1,RepRec2,RepRec3,GetRecName1,Blob2WMS1,Blob2WMS2,Search1,Search2,Search3,Save1,Restore1,DebugAllocTable1
};

int main(){
  int _=0;
  for(_=0;_<Tests;_++){
    printf("Test %02d: ",_);
    if(Test[_]())
      puts("Pass");
    else
      puts("Fail");
  }
  return 0;
}
