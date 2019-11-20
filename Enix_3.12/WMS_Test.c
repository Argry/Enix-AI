#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "WMS_SHM.h"
#include "WMS_Blob.h"
#include "ENiX_WMS.h"
#include "ENiX_LIST.h"

char *RandomString(){
  unsigned long int _=0,Size=rand()%9+1;
  char *Str=(char *)malloc(Size*sizeof(char));
  for(_=0;_<Size-1;_++)
    Str[_]= rand()%25+65;
  Str[Size-1]=0;
  return Str;
}

struct Datablock *CreateRandomDB(unsigned long int Size){
  unsigned long int _=0,A=0,Amax=0;
  struct Datablock *DB=NULL,*Original=NULL;
  for(_=0;_<Size;_++){
    DB=(struct Datablock *)malloc(sizeof(struct Datablock));
    DB->DS[0]=NULL;
    DB->DS[1]=NULL;
    DB->DS[2]=NULL;
    for(A=0;A<8;A++){
      DB->DS[0]=Append_List_Element(DB->DS[0],RandomString());
    }
    Amax=rand()%10;
    for(A=0;A<Amax;A++){
      DB->DS[1]=Append_List_Element(DB->DS[1],RandomString());
      DB->DS[2]=Append_List_Element(DB->DS[2],RandomString());
    }
    DB->Next=Original;
    Original=DB;
  }    
  return Original;
}

int TestWMSDBSame(struct Datablock *DB,struct Datablock *Returned){
  int _=0,Pos=0,SizeDB=0,SizeRT=0;
  struct Datablock *Buffer1=NULL,*Buffer2=NULL;
  struct List_Str *L1=NULL,*L2=NULL;
  for(Buffer1=DB;Buffer1;Buffer1=Buffer1->Next)
    SizeDB++;
  for(Buffer2=Returned;Buffer2;Buffer2=Buffer2->Next)
    SizeRT++;
  if(SizeDB!=SizeRT){
    printf("Size difference: %d %d\n",SizeDB,SizeRT);
    exit(0);
  }
  for(Buffer1=DB,Buffer2=Returned;Buffer1&&Buffer2;Buffer1=Buffer1->Next,Buffer2=Buffer2->Next){
    for(_=0;_<3;_++)
      for(L1=Buffer1->DS[_],L2=Buffer2->DS[_];L1&&L2;L1=L1->Next,L2=L2->Next)
	if(strcmp(L1->Str,L2->Str)){
	  printf("%03d ERROR:[%s][%s]\n",Pos,L1->Str,L2->Str);
	}
    Pos++;
  }
  return 1;
}


static int TestBlob1(){
  int Pass=1;
  char *Str="Test";
  struct Blob *B=NULL;
  printf("TestBlob1: ");
  B=WMS_Str2Blob(Str);
  if(strcmp(Str,WMS_Blob2Str(B))){
    WMS_FreeBlob(B);
    Pass=0;
  }
  WMS_FreeBlob(B);
  return Pass;
}

static int TestBlob2(){
  struct Blob *B=NULL;
  printf("TestBlob2: ");
  B=WMS_Str2Blob(NULL);
  if(B->Nucleus||B->Size)
    return 0;
  return 1; 
}

static int TestBlob3(){
  printf("TestBlob3: ");
  if(WMS_Blob2Str(NULL))
    return 0;
  return 1; 
}

static int CreateSHM(){
  int Pass=0;
  struct DataArea *DB=NULL;
  printf("CreateSHM: ");
  DB=WMS_SHM_Create(0);
  Pass=DB->Queen;
  WMS_SHM_Delete(DB);
  return Pass;
}

static int SHMAttach(){
  int Pass=0;
  struct DataArea *DB=NULL;
  printf("SHMAttach: ");
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
  char *Str="test";
  struct Blob *B=NULL;
  struct DataArea *DB=NULL;
  printf("AddOneRec: ");
  DB=WMS_SHM_Create(0);
  WMS_SHM_Attach(DB); 
  B=WMS_Str2Blob(Str);
  /*  WMS_BlobDEBUG(B); */
  WMS_AddBinary(DB,B);
  WMS_FreeBlob(B);
  B=WMS_GetBinary(DB,0);
  /*  WMS_BlobDEBUG(B); */
  if(!strcmp(WMS_Blob2Str(B),Str))
    Pass=1;
  WMS_SHM_Detach(DB);
  WMS_SHM_Delete(DB);
  return Pass;
}

static int GetNonExistent(){
  int Pass=0;
  struct DataArea *DB=NULL;
  printf("GetNonExistent: ");
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
  printf("AddTwoRec: ");
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
  printf("DelRec1: ");
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
  printf("DelRec2: ");
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
  printf("RepRec1: ");
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
  printf("RepRec2: ");
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
  printf("RepRec3: ");
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
  printf("GetRecName1: ");
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
  printf("Blob2WMS1: ");
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
  printf("Blob2WMS2: ");
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
  struct DataArea *DB=NULL;
  printf("Search1: ");
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
  struct DataArea *DB=NULL;
  printf("Search2: ");
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
  printf("Search3: ");
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
  char *Str1="test1",*Str2="test2";
  struct Blob *B=NULL;
  struct DataArea *DB=NULL;
  printf("Save1: ");
  DB=WMS_SHM_Create(0);
  WMS_SHM_Attach(DB); 
  B=WMS_Str2Blob(Str1);
  WMS_AddBinary(DB,B);
  WMS_FreeBlob(B);
  B=WMS_Str2Blob(Str2);
  WMS_AddBinary(DB,B);
  WMS_Save_To(DB,"WMS_TEST1.DAT");
  WMS_SHM_Detach(DB);
  WMS_SHM_Delete(DB);
  return Pass;
}

static int Restore1(){
  int Pass=1;
  char *Str1="test1",*Str2="test2";
  struct DataArea *DB=NULL;
  printf("Restore1: ");
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
  char *Str1="test1",*Str2="test2";
  struct Blob *B=NULL;
  struct DataArea *DB=NULL;
  printf("DebugAllocTable1: ");
  DB=WMS_SHM_Create(0);
  WMS_SHM_Attach(DB); 
  B=WMS_Str2Blob(Str1);
  WMS_AddBinary(DB,B);
  WMS_FreeBlob(B);
  B=WMS_Str2Blob(Str2);
  WMS_AddBinary(DB,B);
  
  WMS_ALLOC_DEBUG(DB);

  WMS_SHM_Detach(DB);
  WMS_SHM_Delete(DB);
  return Pass;
}

static int TestBlob4(){
  int Pass=1;
  unsigned long int _=0;
  char *Str1,*Str2;
  struct Blob *B;
  printf("TestBlob4: ");
  for(_=0;_<20000;_++){
    Str1=RandomString();
    B=WMS_Str2Blob(Str1);
    Str2=WMS_Blob2Str(B);
    if(strcmp(Str1,Str2)){
      printf("Error (Iteration %ld): [%s] != [%s]",_,Str1,Str2);
      Pass=0;
    }
    WMS_FreeBlob(B);
    free(Str1);
    free(Str2);
  }
  return Pass;
}

static int TestBlob5(){
  int Pass=1;
  unsigned long int _=0;
  char *Str1,*Str2;
  struct Blob *B;
  printf("TestBlob5: ");
  for(_=0;_<20000;_++){
    Str1=RandomString();
    B=WMS_Bin2Blob(strlen(Str1),Str1);
    Str2=WMS_Blob2Str(B);
    if(strcmp(Str1,Str2)){
      printf("Error (Iteration %ld): [%s] != [%s]",_,Str1,Str2);
      Pass=0;
    }
    WMS_FreeBlob(B);
    free(Str1);
    free(Str2);
  }
  return Pass;
}

static int TestConversion1(){
  int Pass=1,Sector=0;
  struct List_Str *List1=NULL,*List2=NULL;
  struct Datablock *Original=NULL,*Converted=NULL,*DB=NULL;
  struct Blob *BlobTrans=NULL;
  printf("TestConversion1: ");
  Original=CreateRandomDB(1000);
  for(DB=Original;DB;DB=DB->Next){
    BlobTrans=WMS_WMS2Blob(DB);
    Converted=WMS_Blob2WMS(BlobTrans);
    for(Sector=0;Sector<3;Sector++)
      for(List1=DB->DS[Sector],List2=Converted->DS[Sector];List1&&List2;List1=List1->Next,List2=List2->Next)
	if(strcmp(List1->Str,List2->Str))
	  Pass=0;
    if(!Pass){
      PrintList(DB->DS[0]);
      PrintList(DB->DS[1]);
      PrintList(DB->DS[2]);
      WMS_BlobDEBUG(BlobTrans);
      PrintList(Converted->DS[0]);
      PrintList(Converted->DS[1]);
      PrintList(Converted->DS[2]);      
      break;
    }
  }
  return Pass;
}

static int SHMTest1(){
  int Pass=1;
  unsigned long int _=0;
  struct Blob **B=NULL,*Tmp=NULL;
  struct DataArea *DB=NULL;
  printf("SHMTest1: ");
  DB=WMS_SHM_Create(0);
  WMS_SHM_Attach(DB); 
  B=(struct Blob **)malloc(2000*sizeof(struct Blob *));
  for(_=0;_<2000;_++){
    B[_]=WMS_Str2Blob(RandomString());
    WMS_AddBinary(DB,B[_]);
  }
  for(_=0;_<2000;_++){
    Tmp=WMS_GetBinary(DB,_);
    if(Tmp->Size!=B[_]->Size){
      puts("Different size!");
      Pass=0;
    }
    if(strcmp(Tmp->Nucleus,B[_]->Nucleus)){
      puts("Different string!");
      Pass=0;
    }
    if(memcmp(Tmp->Nucleus,B[_]->Nucleus,Tmp->Size)){
      puts("Different binary!");
      Pass=0;
    }
  }
  WMS_SHM_Detach(DB);
  WMS_SHM_Delete(DB);
  return Pass;
}

static int SHMTest2(){
  int Pass=1;
  unsigned long int _=0;
  struct Blob **B=NULL,*Tmp=NULL;
  struct DataArea *DB=NULL;
  printf("SHMTest2: ");
  DB=WMS_SHM_Create(0);
  WMS_SHM_Attach(DB); 
  B=(struct Blob **)malloc(2000*sizeof(struct Blob *));
  for(_=0;_<2000;_++){
    WMS_AddBinary(DB,B[_]);
  }
  WMS_SHM_Clear(DB);
  for(_=0;_<2000;_++){
    B[_]=WMS_Str2Blob(RandomString());
    WMS_AddBinary(DB,B[_]);
  }
  for(_=0;_<2000;_++){
    Tmp=WMS_GetBinary(DB,_);
    if(Tmp->Size!=B[_]->Size){
      puts("Different size!");
      Pass=0;
    }
    if(strcmp(Tmp->Nucleus,B[_]->Nucleus)){
      puts("Different string!");
      Pass=0;
    }
    if(memcmp(Tmp->Nucleus,B[_]->Nucleus,Tmp->Size)){
      puts("Different binary!");
      Pass=0;
    }
  }
  WMS_SHM_Detach(DB);
  WMS_SHM_Delete(DB);
  return Pass;
}

static int TestConversion2(){
  int Pass=1,A=0,Sector=0;
  unsigned long int _=0;
  struct List_Str *List1=NULL,*List2=NULL;
  struct Datablock *Original=NULL,*Converted=NULL,*DB=NULL;
  struct Blob *BlobTrans=NULL;
  printf("TestConversion2: ");
  for(_=0;_<1000;_++){
    DB=(struct Datablock *)malloc(sizeof(struct Datablock));
    DB->DS[0]=NULL;
    for(A=0;A<8;A++){
      DB->DS[0]=Append_List_Element(DB->DS[0],RandomString());
    }
    DB->DS[1]=NULL;
    DB->DS[2]=NULL;
    DB->Next=Original;
    Original=DB;
  }  
  for(DB=Original;DB;DB=DB->Next){
    BlobTrans=WMS_WMS2Blob(DB);
    Converted=WMS_Blob2WMS(BlobTrans);
    for(Sector=0;Sector<3;Sector++)
      for(List1=DB->DS[Sector],List2=Converted->DS[Sector];List1&&List2;List1=List1->Next,List2=List2->Next)
	if(strcmp(List1->Str,List2->Str))
	  Pass=0;
    if(!Pass){
      PrintList(DB->DS[0]);
      PrintList(DB->DS[1]);
      PrintList(DB->DS[2]);
      WMS_BlobDEBUG(BlobTrans);
      PrintList(Converted->DS[0]);
      PrintList(Converted->DS[1]);
      PrintList(Converted->DS[2]);      
      break;
    }
  }
  return Pass;
}

static int TestAlloc1(){
  int Pass=1;
  char *Str1="test1",*Str2="test2";
  struct Blob *B=NULL;
  struct DataArea *DB=NULL;
  printf("TestAlloc1: ");
  DB=WMS_SHM_Create(0);
  WMS_SHM_Attach(DB); 
  B=WMS_Str2Blob(Str1);
  WMS_AddBinary(DB,B);
  WMS_FreeBlob(B);
  B=WMS_Str2Blob(Str2);
  WMS_AddBinary(DB,B);
  WMS_FreeBlob(B);
  if(*(unsigned long int *)(DB->InPos)!=0)
    Pass=0;
  if(*(unsigned long int *)(DB->InPos+sizeof(unsigned long int))!=1)
    Pass=0;
  WMS_SHM_Detach(DB);
  WMS_SHM_Delete(DB);  
  return Pass;
}

static int TestAlloc2(){
  int Pass=1;
  long int Pos1=0;
  char *Str1="test1",*Str2="test2";
  struct Blob *B=NULL;
  struct DataArea *DB=NULL;
  printf("TestAlloc2: ");
  DB=WMS_SHM_Create(0);
  WMS_SHM_Attach(DB); 
  B=WMS_Str2Blob(Str1);
  Pos1=WMS_AddBinary(DB,B);
  WMS_FreeBlob(B);
  B=WMS_Str2Blob(Str2);
  WMS_AddBinary(DB,B);
  WMS_FreeBlob(B);
  WMS_DelBinary(DB,Pos1);
  if(*(unsigned long int *)(DB->InPos)!=0)
    Pass=0;
  WMS_SHM_Detach(DB);
  WMS_SHM_Delete(DB);  
  return Pass;
}

static int TestAlloc3(){
  int Pass=1;
  long int Pos1=0;
  char *Str1="test1",*Str2="test2",*Str3="longtest";
  struct Blob *B=NULL;
  struct DataArea *DB=NULL;
  printf("TestAlloc3: ");
  DB=WMS_SHM_Create(0);
  WMS_SHM_Attach(DB); 
  B=WMS_Str2Blob(Str1);
  Pos1=WMS_AddBinary(DB,B);
  WMS_FreeBlob(B);
  B=WMS_Str2Blob(Str2);
  WMS_AddBinary(DB,B);
  WMS_FreeBlob(B);
  B=WMS_Str2Blob(Str3);
  WMS_RepBinary(DB,Pos1,B);
  if(*(unsigned long int *)(DB->InPos)!=1)
    Pass=0;
  if(*(unsigned long int *)(DB->InPos+sizeof(unsigned long int))!=0)
    Pass=0;
  WMS_SHM_Detach(DB);
  WMS_SHM_Delete(DB);  
  return Pass;
}

static int TestDefrag1(){
  int Pass=1;
  long int _=0,FinalPos=FATSIZE;
  struct Blob *B=NULL;
  struct DataArea *DB=NULL;
  printf("TestDefrag1: ");
  DB=WMS_SHM_Create(0);
  WMS_SHM_Attach(DB); 
  for(_=0;_<100;_++){
    B=WMS_Str2Blob(RandomString());
    WMS_AddBinary(DB,B);
    WMS_FreeBlob(B);
  }  
  for(_=0;_<100;_+=2)
    WMS_DelBinary(DB,_);
  WMS_Defrag(DB);
  for(_=0;_<DB->Population;_++){
    if(*(unsigned long int *)(DB->StartPos+(_*sizeof(unsigned long int)))!=FinalPos){
      Pass=0;
      break;
    }
    FinalPos=*(unsigned long int *)(DB->FinishPos+(_*sizeof(unsigned long int)));
  }
  WMS_SHM_Detach(DB);
  WMS_SHM_Delete(DB);
  return Pass;
}

static int TestAllocFull1(){
  int Pass=1;
  long int _=0;
  struct DataArea *DB=NULL;
  struct Blob *B=NULL;
  printf("TestAllocFull1: ");
  DB=WMS_SHM_Create(0);
  WMS_SHM_Attach(DB); 
  while(_>=0){
    B=WMS_Str2Blob(RandomString());
    _=WMS_AddBinary(DB,B);
    WMS_FreeBlob(B);
  }
  if(_!=-1)
    Pass=0;
  WMS_SHM_Detach(DB);
  WMS_SHM_Delete(DB);
  return Pass;
}

static int TestSHMFull1(){
  int Pass=1;
  long int _=0;
  char *Str1="Looooooooooong cat is very loooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooong";
  struct DataArea *DB=NULL;
  struct Blob *B=NULL;
  printf("TestSHMFull1: ");
  DB=WMS_SHM_Create(0);
  WMS_SHM_Attach(DB);
  while(_>=0){
    B=WMS_Str2Blob(Str1);
    _=WMS_AddBinary(DB,B);
    WMS_FreeBlob(B);
  }
  if(_!=-2)
    Pass=0;
  WMS_SHM_Detach(DB);
  WMS_SHM_Delete(DB);
  return Pass;
}

static int TestAllocCache1(){
  int Pass=1;
  char *Str1="test1",*Str2="test2";
  struct Blob *B=NULL;
  struct DataArea *DB=NULL;
  printf("TestAllocCache1: ");
  DB=WMS_SHM_Create(0);
  WMS_SHM_Attach(DB); 
  B=WMS_Str2Blob(Str1);
  WMS_AddBinary(DB,B);
  WMS_FreeBlob(B);
  B=WMS_Str2Blob(Str2);
  WMS_AddBinary(DB,B);
  WMS_FreeBlob(B);
  WMS_DelBinary(DB,1);
  if(*(unsigned long int *)(DB->Cached)!=0)
    Pass=0;
  /*
  WMS_ALLOC_DEBUG(DB);
  */
  WMS_SHM_Detach(DB);
  WMS_SHM_Delete(DB);  
  return Pass;
}


static void TestDirect(){
  int Tests=35,_=0;
  puts("Testing Direct SHM Connection:");
  int (*Test[])()={
    TestBlob1,TestBlob2,TestBlob3,CreateSHM,SHMAttach,AddOneRec,GetNonExistent,AddTwoRec,DelRec1,DelRec2,
    RepRec1,RepRec2,RepRec3,GetRecName1,Blob2WMS1,Blob2WMS2,Search1,Search2,Search3,Save1,Restore1,DebugAllocTable1,
    TestBlob4,TestBlob5,TestConversion1,SHMTest1,SHMTest2,TestConversion2,TestAlloc1,TestAlloc2,TestAlloc3,TestDefrag1,
    TestAllocFull1,TestSHMFull1,TestAllocCache1
  };
  for(_=0;_<Tests;_++){
    /*    printf("Test %02d: ",_); */
    if(Test[_]())
      puts("Pass");
    else
      puts("Fail");
  }
}

static int TestComp1(){
  int Pass=1;
  struct Datablock *DB[2];

  DB[1]=Database[1]=CreateRandomDB(200);
  DB[0]=Database[0]=CreateRandomDB(10);

  if(!TestWMSDBSame(DB[0],Database[0]))
    Pass=0;
  if(!TestWMSDBSame(DB[1],Database[1]))
    Pass=0;
  return Pass;

}

static int TestRelay1(){
  int Pass=1,_=0;
  struct Datablock *DB[2],*FirstRet[2];
  Load2RAM();
  Purge(NULL,NULL);
  FirstRet[1]=CreateRandomDB(0);
  FirstRet[0]=CreateRandomDB(200);
  Database[0]=FirstRet[0];
  Database[1]=FirstRet[1];
  Save2Persistent("Test");
  Load2RAM();
  DB[1]=Database[1];
  DB[0]=Database[0];
  for(_=0;_<100;_++){
    Load2RAM();
    if(!TestWMSDBSame(DB[0],Database[0])){
      Pass=0;
      break;
    }
    if(!TestWMSDBSame(DB[1],Database[1])){
      Pass=0;
      break;
    }
  }
  return Pass;
}


static void TestNetwork(){
  int Tests=2,_=0;
  puts("Testing Network:");
  int (*Test[])()={
    TestComp1,TestRelay1
  };
  for(_=0;_<Tests;_++){
    printf("Test %02d: ",_);
    if(Test[_]())
      puts("Pass");
    else
      puts("Fail");
  }
}

int main(int argc, char *argv[]){
  int Success=!!(argc-1);
  if(Success){
    if(!strcmp(argv[1],"DIRECT"))
      TestDirect();
    else if(!strcmp(argv[1],"CLIENT"))
      TestNetwork();
    else
      Success=0;    
    if(Success)
      return 0;
  }
  printf("Usage: %s DIRECT|CLIENT\n",argv[0]);
  return -1;
}
