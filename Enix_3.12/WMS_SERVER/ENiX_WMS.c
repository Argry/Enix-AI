/*! \file */

/*

This file is part of ENiX3.

ENiX3 is free software you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

ENiX3 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ENiX3.  If not, see <http://www.gnu.org/licenses/>.

       oooooooooooo ooooo      ooo  o8o  ooooooo  ooooo   .oooo.          
   .dP `888'     `8 `888b.     `8'  `"'   `8888    d8'  .dP""Y88b  Yb     
 .dP    888          8 `88b.    8  oooo     Y888..8P          ]8P'  `Yb   
dP      888oooo8     8   `88b.  8  `888      `8888'         <88b.     `Yb 
Yb      888    "     8     `88b.8   888     .8PY888.         `88b.    .dP 
 `Yb    888       o  8       `888   888    d8'  `888b   o.   .88P   .dP   
   `Yb o888ooooood8 o8o        `8  o888o o888o  o88888o `8bd88P'   dP     


Programmed By M.S.Braithwaite 2007 - 2011.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ENiX_Globals.h"
#include "ENiX_STRING.h"
#include "ENiX_LIST.h"
#include "ENiX_TIME.h"
#include "ENiX_WMS.h"
#include "ENiX_DISK.h"
//#include "WitchCraft-Net.h"
//#include "Amoeba-API.h"

int              ThinkOutLoud;
struct Datablock Datablocks;

int              MiddenService=0;
char             *Filename;
char             *Data;

/* Memory Data */
int DB_Size;
int RB_Size;
int Newly_Learnt;


/* Memory Configuration */
double MemoryLimit;                      /* Maximum number of concepts to hold in memory. -1 = Disabled.              */
double MemoryExp;                        /* Number of days to expire before memory can be considered for removal.     */
double MemoryProbability;                /* Coefficient to multiply occurance probability to compute relevence value. */
double MemoryRate;                       /* Ratio of concepts forgotten divided by concepts learnt.                   */
double MemoryThreshold;                  /* Threshold  probability for consideration for removal.                     */


/*! \brief Saves the file to disk or Amoeba.
 *
 */
void Save2Persistent(char *BackupName){
  int Socket=0;
  if(MiddenService){
    /*
    AL3_Store(Database);
    AL3_SrvDisplay();
    */
  }
  else{
    Data=Disk_Buffer(Database);
    if(system(StrCat("rm -f ",BackupName))){}
    if(system(StrCat(StrCat(StrCat("cp ",Filename)," "),BackupName))){}
    Str2File(Filename,Data);
  }
}

/*! \brief Handles the high level population of memory from disk file or Amoeba.
 *
 */
void Load2RAM(){
  int Socket=0;
  FILE *Disk=NULL;
  struct DB_Wrap *DBW=NULL;
  //  MiddenService=AL3_CheckService();
  MiddenService=0;
  if(MiddenService){
    /* AL3_SrvDisplay(); */
    /*
    DBW=AL3_Acquire(Database);
    Database[0]=DBW->DB[0];
    Database[1]=DBW->DB[1];
    Database[1]=MemoryMode(Database[1]);
    */
  }
  else{
    if(!(Disk=fopen(Filename,"r"))){ 
      printf("Cannot open: %s\n",Filename);
      if(!system(StrCat(StrCat("mkdir -p ",getenv("HOME")),"/.ENiX/"))) 
	puts("Note: ~/.ENiX/ has been successfully created.");
      if(Str2File(Filename,"||")){
	puts("Created blank ~/.ENiX/ENiX.MEM file.\nPlease re-run ENiX for normal operation..."); 
	exit(0);       
      }
      else{
	puts("Unrecoverable error please check disk."); 
	exit(-1); 
      }
    }
    else{
      Data=LoadFile(Disk);
      fclose(Disk);
    }
    Virtualise(Data); 
  }
  Database[1]=MemoryMode(Database[1]);
}

/*! \brief Frees a database segment "DB".
 *
 */
void FreeWMS(struct Datablock *DB){
  struct Datablock *Old=0;
  while(DB){
    FreeList(DB->DS[0]);
    FreeList(DB->DS[1]);
    FreeList(DB->DS[2]);
    Old=DB;
    DB=DB->Next;
    free(Old);
  }
}

/*! \brief Initialises a :mode record in database segment "Q".
 *
 */
struct Datablock *MemoryMode(struct Datablock *Q){
   struct List_Str *LS=NULL;
   struct Datablock *B=Q;
   if(!(B=Goto_DB_Entry(Q,":mode"))){
      MemoryLimit=-1;
      MemoryExp=7;
      MemoryProbability=0.9;
      MemoryRate=0.25;
      MemoryThreshold=0.0001;
      Q=Add_DB_Entry(Q,":memory");
      Q=Add_DB_Entry(Q,":mode");
      LS=Ins_List_Element(LS,":memlimit",0);
      LS=Ins_List_Element(LS,":memexp",1);
      LS=Ins_List_Element(LS,":memprob",2);
      LS=Ins_List_Element(LS,":memrate",3);
      LS=Ins_List_Element(LS,":memthresh",4);
      LS=Ins_List_Element(LS,":memnew",5);
      Q->DS[1]=LS;
      LS=NULL;
      LS=Ins_List_Element(LS,FloatToString(MemoryLimit),0);
      LS=Ins_List_Element(LS,FloatToString(MemoryExp),1);  
      LS=Ins_List_Element(LS,FloatToString(MemoryProbability),2);
      LS=Ins_List_Element(LS,FloatToString(MemoryRate),3); 
      LS=Ins_List_Element(LS,FloatToString(MemoryThreshold),4);
      LS=Ins_List_Element(LS,FloatToString(0),5);
      Q->DS[2]=LS;
      Rep_DB_Entry(Q,0,7,"cs0");
      puts("Startup datablock created.");
   }
   else{
      MemoryLimit=atof(Get_DB_Entry(B,2,0));
      MemoryExp=atof(Get_DB_Entry(B,2,1));
      MemoryProbability=atof(Get_DB_Entry(B,2,2));
      MemoryRate=atof(Get_DB_Entry(B,2,3));
      MemoryThreshold=atof(Get_DB_Entry(B,2,4));
      Newly_Learnt=atof(Get_DB_Entry(B,2,5));
      Rep_DB_Entry(B,0,5,Seconds2Str());
   }
   if(!(B=Goto_DB_Entry(Q,":scoperesolution"))){
      Q=Add_DB_Entry(Q,":scoperesolution");
      Rep_DB_Pair(Q,":globalscope","?");
      Rep_DB_Pair(Q,":localscope","?");
      Rep_DB_Entry(Q,0,7,":globalscope");
   }
   return Q;
}

/*! \brief Add a new record called "S" to the database segment "LS".
 *
 */
struct Datablock *Add_DB_Entry(struct Datablock *LS,char *S){
  struct Datablock *D=LS;
  if(S){
    D=(struct Datablock *)malloc(sizeof(Datablocks));
    D->Next=LS;
    D->DS[0]=NULL;
    D->DS[0]=Ins_List_Element(D->DS[0],StrCat("",S),0);
    D->DS[0]=Ins_List_Element(D->DS[0],StrCat("","?"),1);
    D->DS[0]=Ins_List_Element(D->DS[0],StrCat("","?"),2);
    D->DS[0]=Ins_List_Element(D->DS[0],StrCat("","?"),3);
    D->DS[0]=Ins_List_Element(D->DS[0],Seconds2Str(),4);
    D->DS[0]=Ins_List_Element(D->DS[0],Seconds2Str(),5);
    D->DS[0]=Ins_List_Element(D->DS[0],FloatToString(1),6);
    D->DS[0]=Ins_List_Element(D->DS[0],StrCat("","?"),7);
    D->DS[1]=NULL;
    D->DS[2]=NULL;
    Newly_Learnt++;
  }
  return D;
}

/*! \brief Delete the record at position "P" from the database segment "LS".
 *
 */
struct Datablock *Del_DB_Entry(struct Datablock *LS,int P){
  int _=0;
  struct Datablock *Prev,*R=LS;
  if(LS){
    if(P){
      for(;(LS)&&(_!=P);_++){
	Prev=LS;
	LS=LS->Next;
      }
      if((_==P)&&(LS)){
	Prev->Next=LS->Next;
	while(LS->DS[0])
	  LS->DS[0]=Del_List_Element(LS->DS[0],0);
	while(LS->DS[1])
	  LS->DS[1]=Del_List_Element(LS->DS[1],0);
	while(LS->DS[2])
	  LS->DS[2]=Del_List_Element(LS->DS[2],0);
	free(LS);
      }
    }
    else{
      R=LS->Next;
      while(LS->DS[0])
	LS->DS[0]=Del_List_Element(LS->DS[0],0);
      while(LS->DS[1])
	LS->DS[1]=Del_List_Element(LS->DS[1],0);
      while(LS->DS[2])
	LS->DS[2]=Del_List_Element(LS->DS[2],0);
      free(LS);
    }
  }
  return R;
}

/*! \brief Locate the record called "S" in database segment "LS".
 *
 */
int Find_DB_Entry(struct Datablock *LS,char *S){
  int R=0;
  struct Datablock *DB=LS;
  while(DB){
    if(!strcmp(DB->DS[0]->Str,S))
      return R;
    DB=DB->Next;
    R++;
  }
  return -1;
}

/*! \brief Extract the string at position "Position" of sector "Type" of database segment "LS".
 *
 */
char *Get_DB_Entry(struct Datablock *LS,int Type,int Position){
  char *R=NULL;
  struct List_Str *List=NULL;
  if(LS){
    List=LS->DS[Type];
    R=Get_List_Element(List,Position);
  }
  return R;
}

/*! \brief Replace the string at position "Position" of sector "Type" of database segment "LS", with "S".
 *
 */
void Rep_DB_Entry(struct Datablock *LS,int Type,int Position,char *S){
  int _=0;
  struct List_Str *L1=NULL,*List=NULL;
  if(LS){
    List=LS->DS[Type];
    L1=List;
    for(;(List)&&(_!=Position);_++)
      List=List->Next;
    if((_==Position)&&(List)){
      List->Str=S;
      LS->DS[Type]=L1;
    }
  } 
}

/*! \brief Return a location marker of the record called "S" in the database segment "LS".
 *
 */
struct Datablock *Goto_DB_Entry(struct Datablock *LS,char *S){
  struct Datablock *DB=NULL;
  for(DB=LS;DB;DB=DB->Next)
    if(!strcmp(DB->DS[0]->Str,S)){ 
      Rep_DB_Entry(DB,0,6,FloatToString(1));
      return DB;
    }
  return NULL;
}

/*! \brief Replace (or add) an attribute "Qual" / value "Des" to the database segment "LS".
 *
 */
int Rep_DB_Pair(struct Datablock *LS,char *Qual,char *Des){
   int _=0;
   if(LS&&Qual&&Des){
     if((_=Find_List_Element(LS->DS[1],Qual))==-1){
       LS->DS[1]=Ins_List_Element(LS->DS[1],Qual,0);
       LS->DS[2]=Ins_List_Element(LS->DS[2],Des,0);
     }
     else
       Rep_DB_Entry(LS,2,_,Des);
   }
   else{
     if(ThinkOutLoud)
       puts("ERROR: Erroneous call to Rep_DB_Pair.");
     return 0;
   }
   return (_!=-1);
}

/*! \brief Add an attribute "Qual" / value "Des" to the database segment "LS".
 *
 */
int Add_DB_Pair(struct Datablock *LS,char *Qual,char *Des){
  struct List_Str *Buffer1=NULL,*Buffer2=NULL;
  for(Buffer1=LS->DS[1],Buffer2=LS->DS[2];Buffer1&&Buffer2;Buffer1=Buffer1->Next,Buffer2=Buffer2->Next)
    if((!strcmp(Buffer1->Str,Qual))&&(!strcmp(Buffer2->Str,Des)))
      return 1;
  LS->DS[1]=Ins_List_Element(LS->DS[1],Qual,0);
  LS->DS[2]=Ins_List_Element(LS->DS[2],Des,0);
  return 0;
}

/*! \brief Retrieve an attribute "Qual" / value "Des" to the database segment "LS".
 *  - the attribute or value is NULL if it is required.
 */
char *Lookup_DB_Attr(struct Datablock *DB,char *Qual,char *Des){
   int _=0;
   char *Found=NULL;
   if(Qual||Des){
      if(Qual){
	 if((_=Find_List_Element(DB->DS[1],Qual))+1)
	   Found=Get_List_Element(DB->DS[2],_);
      }
      else{
	 if((_=Find_List_Element(DB->DS[2],Des))+1)
	   Found=Get_List_Element(DB->DS[1],_);
      }
      return Found;
   }
   else
     return NULL;
}

/*! \brief Search database segment "LS" for duplicates called "S".
 *
 */
void Print_DB_Duplicate(struct Datablock *LS,char *S){
   struct Datablock *DB=NULL;
   printf("DUP SCAN: ");
   for(DB=LS;DB;DB=DB->Next)
     printf("%s ",DB->DS[0]->Str);
   puts("");
}

/*! \brief Get a language parameter with attribute "Attribute".
 *
 */
char *GetValue(char *Attribute){
  int _=0;
  struct Datablock *DB=GetLang();
  if((_=Find_List_Element(DB->DS[1],Attribute))+1)
    return Get_List_Element(DB->DS[2],_);
  else
    return NULL;
}

/*! \brief Get the position in the database of the language configuration record.
 *
 */
struct Datablock *GetLang(){
  return Goto_DB_Entry(Database[1],Get_DB_Entry(Goto_DB_Entry(Database[1],":mode"),0,1));
}

/*! \brief Add word references to associate similar words.
 *
 */
struct List_Str *AddWRef(struct List_Str *Word_List,struct List_Str *L){
   struct List_Str *Input=NULL;
   struct Datablock *DB=NULL;
   for(Input=Word_List;Input;Input=Input->Next)   
     Rep_DB_Pair(Goto_DB_Entry(Database[1],Input->Str),":baseword",L->Str);
   if((DB=Goto_DB_Entry(Database[1],L->Str))){
     Rep_DB_Pair(DB,":similarwords",List2Str(Word_List));
   }
   return L->Next;
}

/*! \brief Kill switch for higher ENiX functionality.
 *
 */
int Detect_Sentiency(){
   int _=0,C=0;
   char *Language=NULL;
   struct List_Str *Alpha=NULL,*Beta=NULL;
   struct Datablock *DB=Database[1];
   Language=Get_DB_Entry(Goto_DB_Entry(Database[1],":mode"),0,1);
   if(!strcmp(Language,"?"))
     return 0;
   DB=Goto_DB_Entry(Database[1],Language);
   Alpha=DB->DS[1];
   Beta=DB->DS[2];
   _=Find_List_Element(Alpha,":sentiency");
   if(_<0)
     return 0;
   for(C=0;C<_;C++){
     Alpha=Alpha->Next;
     Beta=Beta->Next;
   }
   if(!strcmp(Beta->Str,"off"))
     return 0;
   return
     1;
}

/*! \brief Determine which of list "Word_List" have not yet been grammatically classified.
 *
 */
struct List_Str *Unknown_Words(struct List_Str *Word_List){
  struct List_Str *Unknowns=NULL,*Buffer=Word_List;
  while(Buffer){
     if(!strcmp(Get_DB_Entry(Goto_DB_Entry(Database[1],Buffer->Str),0,1),"?"))
       Unknowns=Ins_List_Element(Unknowns,Buffer->Str,0);
     Buffer=Buffer->Next;
  }
  return Unknowns;
}

/*! \brief Determine if any parameter in the record called "Concept" has an attribute called "Qualifier" which is on.
 *
 */
int IsEnabled(char *Concept,char *Qualifier){
  int C=0;
  char *B=NULL;
  struct Datablock *DB=NULL;
  return (DB=Goto_DB_Entry(Database[1],Concept))?!(C=(B=Get_List_Element(DB->DS[2],Find_List_Element(DB->DS[1],Qualifier)))?strcmp(B,"on"):1):0;
} 

/*! \brief List all database records that are relationships.
 *
 */
struct List_Str *AllRels(struct List_Str *Word_List,struct List_Str *L){
  struct Datablock *DB1=Database[0];
   ANSWER=NULL;
   while(DB1){
      ANSWER=Ins_List_Element(ANSWER,DB1->DS[0]->Str,0);
      DB1=DB1->Next;
   }
   return L;
}

/*! \brief Check if the database pair attribute: "Qual" and value: "Des" appear in the database record "LS".
 *
 */
int Is_DB_Pair(struct Datablock *LS,char *Qual,char *Des){
  struct List_Str *Buffer1=NULL,*Buffer2=NULL;
  for(Buffer1=LS->DS[1],Buffer2=LS->DS[2];Buffer1&&Buffer2;Buffer1=Buffer1->Next,Buffer2=Buffer2->Next)
    if((!strcmp(Buffer1->Str,Qual))&&(!strcmp(Buffer2->Str,Des)))
      return 1;
  return 0;
}

/*! \brief Remove database pair attribute: "Qual" and value: "Des" from database record "LS".
 *
 */
int Del_DB_Pair(struct Datablock *LS,char *Qual,char *Des){
  int C=0;
  struct List_Str *Buffer1=NULL,*Buffer2=NULL;
  for(Buffer1=LS->DS[1],Buffer2=LS->DS[2];Buffer1&&Buffer2;Buffer1=Buffer1->Next,Buffer2=Buffer2->Next,C++)
    if((!strcmp(Buffer1->Str,Qual))&&(!strcmp(Buffer2->Str,Des))){
      LS->DS[1]=Del_List_Element(LS->DS[1],C);
      LS->DS[2]=Del_List_Element(LS->DS[2],C);
      return 1;
    }
  return 0;
}

/*! \brief Totally erase the database.
 *
 */
struct List_Str *Purge(struct List_Str *Word_List,struct List_Str *L){
  int Socket=0;
  if(MiddenService){
    /*
    AL3_Purge();
    exit(0);
    */
  }
  else{
   while(Database[0])
     Database[0]=Del_DB_Entry(Database[0],0);
   while(Database[1])
     Database[1]=Del_DB_Entry(Database[1],0); 
  }
  puts("Memory destroyed.");
  return L;
}

/*

            88 oooooooooooo ooooo      ooo  o8o  ooooooo  ooooo   .oooo.          
   .dP     .8' `888'     `8 `888b.     `8'  `"'   `8888    d8'  .dP""Y88b  Yb     
 .dP      .8'   888          8 `88b.    8  oooo     Y888..8P          ]8P'  `Yb   
dP       .8'    888oooo8     8   `88b.  8  `888      `8888'         <88b.     `Yb 
Yb      .8'     888    "     8     `88b.8   888     .8PY888.         `88b.    .dP 
 `Yb   .8'      888       o  8       `888   888    d8'  `888b   o.   .88P   .dP   
   `Yb 88      o888ooooood8 o8o        `8  o888o o888o  o88888o `8bd88P'   dP     


 */
