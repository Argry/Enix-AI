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


#include "ENiX_STRING.h"
#include "ENiX_LIST.h"
#include "ENiX_WMS.h"

struct Datablock *Database[2];
struct Datablock Datablocks;

extern struct List_Str *ANSWER;

void FreeWMS(struct Datablock *DB){
  struct Datablock *Old;
  while(DB){
    FreeList(DB->DS[0]);
    FreeList(DB->DS[1]);
    FreeList(DB->DS[2]);
    Old=DB;
    DB=DB->Next;
    free(Old);
  }
}

struct List_Str *CSRecon(struct List_Str *Word_List,struct List_Str *L){
  int _,Length=0,Found=0,STOP=0;
  char *CSNUM=NULL,*OLD=NULL;
  struct List_Str *DSDATA;
  struct Datablock *DB=Database[1];
  OLD=Word_List->Str;
  if(strncmp(OLD,"cs",2)){
    if(isdigit(OLD[0])){
      CSNUM=(char *)malloc((Length=3+StrLen(OLD))*sizeof(char));
      snprintf(CSNUM,Length,"cs%s",OLD);
    }
    else{
      ANSWER=Word_List;
      return L;
    }
  }
  else{
    CSNUM=OLD;
  }
  ANSWER=NULL;
  while(!STOP){
    DB=Database[1];
    Found=0;
    while(DB){
      DSDATA=DB->DS[2];
      if((_=Find_List_Element(DSDATA,CSNUM))>-1){
	if(strncmp(Get_DB_Entry(DB,1,_),"cs",2)){
	   Found=1;
	   ANSWER=Ins_List_Element(ANSWER,Get_List_Element(DB->DS[1],_),0);
	   CSNUM=DB->DS[0]->Str;
	   DB=NULL;
	}	
      }
      if(DB) DB=DB->Next;
    }
    if(!Found) STOP=1;
    else{
      if(strncmp(CSNUM,"cs",2)){
	ANSWER=Ins_List_Element(ANSWER,CSNUM,0);
	STOP=1;
      }
    }
  }
  return L;
}

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
      Rep_DB_Entry(B,0,5,Gettime());
   }
   if(!(B=Goto_DB_Entry(Q,":scoperesolution"))){
      Q=Add_DB_Entry(Q,":scoperesolution");
      Add_DB_Pair(Q,":globalscope","?");
      Add_DB_Pair(Q,":localscope","?");
      Rep_DB_Entry(Q,0,7,":globalscope");
   }
   return Q;
}

void PrintHalf(struct Datablock *Q){
  int B;
  struct Datablock *BF;
  BF=Q;
  while(BF){ 
    for(B=0;B<3;B++)
      if(BF->DS[B])
	PrintList(BF->DS[B]);
    BF=BF->Next;
  }
}

void PrintDatabase(struct Datablock *Q[2]){
  int A,B;
  struct Datablock *BF[2];
  BF[0]=Q[0];
  BF[1]=Q[1];
  for(A=0;A<2;A++){
    while(BF[A]){ 
      if(A)
	printf("Data: "); 
      else
	printf("Rels: ");
      for(B=0;B<3;B++)
	if(BF[A]->DS[B])
	  PrintList(BF[A]->DS[B]);
      BF[A]=BF[A]->Next;
    }
  }
}

void Print_DB_Entry(struct Datablock *LS,int P){
  int _=0;
  struct List_Str *L1=NULL,*L2=NULL; 
  for(;LS;_++){ 
    if(_==P){
      if(strncmp(LS->DS[0]->Str,"cs",2)){
	printf("=[Datablock %05d]==========================================\n",_); 
	L1=LS->DS[0];
	printf("Name %23s Grammar %23s\n",Get_List_Element(L1,0),Get_List_Element(L1,1));
	printf("Purpose %20s Emotion %23s\n",Get_List_Element(L1,2),Get_List_Element(L1,3));
	printf("First Time %28s\n",Get_List_Element(L1,4)); 
	printf("Last Time %29s\n",Get_List_Element(L1,5));
	printf("Probability    %13s Commands %22s\n",List2Str(AutoFormat(Str2List(Get_List_Element(L1,6)))),List2Str(AutoFormat(Str2List(Get_List_Element(L1,7)))));
	if((LS->DS[1])&&(LS->DS[2])){ 
	  puts("-[Comparisons]----------------------------------------------");
	  puts("Qualifier:                                            Value:"); 
	  L1=LS->DS[1]; 
	  L2=LS->DS[2];
	  while(L1&&L2){ 
	    printf("%-30s%30s\n",L1->Str,List2Str(AutoFormat(Str2List(L2->Str)))); 
	    L1=L1->Next; 
	    L2=L2->Next; 
	  } 
	} 
	puts("============================================================"); 
      }
      else{
	printf("=[Metablock %05d]==========================================\n",_);
	L1=LS->DS[0];
	CSRecon(Str2List(Get_List_Element(L1,0)),Ins_List_Element(NULL,"ERROR",0));
	printf("Address %52s\n",List2Str(ANSWER));
	printf("Grammar %52s\n",Get_List_Element(L1,1));
	printf("Purpose %20s Emotion %23s\n",Get_List_Element(L1,2),Get_List_Element(L1,3));
	printf("First Time %28s\n",Get_List_Element(L1,4)); 
	printf("Last Time %29s\n",Get_List_Element(L1,5));
	printf("Probability    %13s Commands %22s\n",Get_List_Element(L1,6),Get_List_Element(L1,7));
	if((LS->DS[1])&&(LS->DS[2])){ 
	  puts("-[Comparisons]----------------------------------------------");
	  puts("CS Qualifier Ref:                              CS Value Ref:"); 
	  L1=LS->DS[1];
	  L2=LS->DS[2];
	  while(L1&&L2){ 
	    if(strncmp(L1->Str,"cs",2))
	      printf("%-60s\n",L1->Str);
	    else{
	      CSRecon(Str2List(L1->Str),Ins_List_Element(NULL,"ERROR",0));
	      printf("%-60s\n",List2Str(ANSWER));
	    }
	    if(strncmp(L2->Str,"cs",2))
	      printf("%60s\n",L2->Str);
	    else{
	      CSRecon(Str2List(L2->Str),Ins_List_Element(NULL,"ERROR",0));
	      printf("%60s\n",List2Str(ANSWER));
	    }
	    L1=L1->Next;
	    L2=L2->Next;
	  } 
	} 
	puts("============================================================"); 
	
      }
    }
    LS=LS->Next;  
  }
}

struct Datablock *Add_DB_Entry(struct Datablock *LS,char *S){
   struct Datablock *D=LS;
   if(S){
      D=malloc(sizeof(Datablocks));
      D->Next=LS;
      D->DS[0]=NULL;
      D->DS[0]=Ins_List_Element(D->DS[0],StrCat("",S),0);
      D->DS[0]=Ins_List_Element(D->DS[0],StrCat("","?"),1);
      D->DS[0]=Ins_List_Element(D->DS[0],StrCat("","?"),2);
      D->DS[0]=Ins_List_Element(D->DS[0],StrCat("","?"),3);
      D->DS[0]=Ins_List_Element(D->DS[0],Gettime(),4);
      D->DS[0]=Ins_List_Element(D->DS[0],Gettime(),5);
      D->DS[0]=Ins_List_Element(D->DS[0],FloatToString(1),6);
      D->DS[0]=Ins_List_Element(D->DS[0],StrCat("","?"),7);
      D->DS[1]=NULL;
      D->DS[2]=NULL;
      Newly_Learnt++;
   }
   return D;
}

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

char *Get_DB_Entry(struct Datablock *LS,int Type,int Position){
  char *R=NULL;
  struct List_Str *List=NULL;
  if(LS){
    List=LS->DS[Type];
    R=Get_List_Element(List,Position);
  }
  return R;
}

void Rep_DB_Entry(struct Datablock *LS,int Type,int Position,char *S){
  int _=0;
  struct List_Str *L1,*List=NULL;
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

struct Datablock *Goto_DB_Entry(struct Datablock *LS,char *S){
  struct Datablock *DB=LS;
  while(DB){
     if(!strcmp(DB->DS[0]->Str,S)){ 
	Rep_DB_Entry(DB,0,6,FloatToString(1));
	return DB;
     }
     DB=DB->Next;
  }
  return NULL;
}

void Add_DB_Pair(struct Datablock *LS,char *Qual,char *Des){
   int _;
   if((_=Find_List_Element(LS->DS[1],Qual))==-1){
     LS->DS[1]=Ins_List_Element(LS->DS[1],Qual,0);
     LS->DS[2]=Ins_List_Element(LS->DS[2],Des,0);
   }
   else
     Rep_DB_Entry(LS,2,_,Des);
}

char *Lookup_DB_Attr(struct Datablock *DB,char *Qual,char *Des){
   int _;
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

void Print_DB_Duplicate(struct Datablock *LS,char *S){
   struct Datablock *DB;
   printf("DUP SCAN: ");
   for(DB=LS;DB;DB=DB->Next)
     printf("%s ",DB->DS[0]->Str);
   puts("");
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
