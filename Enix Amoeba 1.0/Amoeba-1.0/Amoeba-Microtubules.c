/*

This file is part of Amoeba.

Amoeba is free software you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Amoeba is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Amoeba.  If not, see <http://www.gnu.org/licenses/>.

      :::.     .        :       ...    .,::::::  :::::::.    :::.          
   ,, ;;`;;    ;;,.    ;;;   .;;;;;;;. ;;;;''''   ;;;'';;'   ;;`;;    ,,   
 ,[[',[[ '[[,  [[[[, ,[[[[, ,[[     \[[,[[cccc    [[[__[[\. ,[[ '[[,  '[[, 
c$  c$$$cc$$$c $$$$$$$$"$$$ $$$,     $$$$$""""    $$""""Y$$c$$$cc$$$c    $c
"8bo,888   888,888 Y88" 888o"888,_ _,88P888oo,__ _88o,,od8P 888   888,,od8"
  "MPYMM   ""` MMM  M'  "MMM  "YMMMMMP" """"YUMMM""YUMMMP"  YMM   ""` YM"  

Programmed By M.S.Braithwaite 2012.

*/


#include "Amoeba-Microtubules.h"

void AL2_CacheFreqUsed(struct Colony *City){
  char ExistU=0,ExistD=0,ExistP=0,ExistT=0;
  char *DATE=NULL;
  struct Blob *Unknown,*Date,*Probability,*Name=NULL;
  unsigned long int UnkPos,DatPos,ProPos,ThisPos=0,NullOffSet=0;
  struct Cytoskelenton *Source=NULL;
  struct Cytoskelenton *UnknownR=NULL;
  struct Cytoskelenton *DateR=NULL;  
  struct Cytoskelenton *ProbabilityR=NULL;
  
  if(!City->CacheProb){
    Name=CreateBlob(strlen("-"),StrCat("","-"));
    City->CachedTime=DATE=Gettime();
    Unknown=CreateBlob(strlen("?"),StrCat("","?"));
    Date=CreateBlob(strlen(DATE),StrCat("",DATE));
    Probability=CreateBlob(strlen("P:1"),StrCat("","P:1"));
    ExistT=0;

    UnkPos= AL2_Recognise(City,&ExistU,Unknown    );
    DatPos= AL2_Recognise(City,&ExistD,Date       );
    ProPos= AL2_Recognise(City,&ExistP,Probability);

    Source=AL2_NullCyto(Name);
    
    if(!ExistU){
      UnknownR=AL2_NullCyto(Unknown);
      NullOffSet++;
    }
    else
      UnknownR=AL1_GetDBEntry(City,UnkPos);

    if(!ExistD){
      DateR=AL2_NullCyto(Date);
      DatPos+=NullOffSet;
      NullOffSet++;
    }
    else
      DateR=AL1_GetDBEntry(City,DatPos);

    if(!ExistP){
      ProbabilityR=AL2_NullCyto(Probability);
      ProPos+=NullOffSet;
    }
    else
      ProbabilityR=AL1_GetDBEntry(City,ProPos);

    AL2_AddLink(City,Source,UnknownR,    AL2_CreateMito(ThisPos,1,0),AL2_CreateMito(UnkPos,0,0));
    AL2_AddLink(City,Source,UnknownR,    AL2_CreateMito(ThisPos,1,1),AL2_CreateMito(UnkPos,0,0));
    AL2_AddLink(City,Source,UnknownR,    AL2_CreateMito(ThisPos,1,2),AL2_CreateMito(UnkPos,0,0));
    AL2_AddLink(City,Source,DateR,       AL2_CreateMito(ThisPos,1,3),AL2_CreateMito(DatPos,0,0));
    AL2_AddLink(City,Source,DateR,       AL2_CreateMito(ThisPos,1,4),AL2_CreateMito(DatPos,0,0));
    AL2_AddLink(City,Source,ProbabilityR,AL2_CreateMito(ThisPos,1,5),AL2_CreateMito(ProPos,0,0));
    AL2_AddLink(City,Source,UnknownR,    AL2_CreateMito(ThisPos,1,6),AL2_CreateMito(UnkPos,0,0));

    if(!ExistU) UnknownR    =AL2_Add2Cyto(UnknownR    ,AL2_DuplicPseudo(Source->Pseudopods[1]));
    if(!ExistD) DateR       =AL2_Add2Cyto(DateR       ,AL2_DuplicPseudo(Source->Pseudopods[1]));
    if(!ExistP) ProbabilityR=AL2_Add2Cyto(ProbabilityR,AL2_DuplicPseudo(Source->Pseudopods[1]));

    if(!ExistU) AL1_AddDBEntry(City,UnknownR);
    else        AL1_RepDBEntry(City,UnknownR,UnkPos);
    if(!ExistD) AL1_AddDBEntry(City,DateR);
    else        AL1_RepDBEntry(City,DateR,DatPos);
    if(!ExistP) AL1_AddDBEntry(City,ProbabilityR);
    else        AL1_RepDBEntry(City,ProbabilityR,ProPos);

    City->CacheUnkn=UnkPos;
    City->CacheDate=DatPos;
    City->CacheProb=ProPos;

    AL1_FreeAmoeba(UnknownR);
    AL1_FreeAmoeba(Source);
    AL1_FreeAmoeba(DateR);
    AL1_FreeAmoeba(ProbabilityR);


    if(ExistU)
      AL1_FreeBlob(Unknown);
    if(ExistP)
      AL1_FreeBlob(Probability);
    if(ExistD)
      free(Date);

  }
}

struct Datablock *AL2_GetDBContents(struct Colony *City){
  unsigned long int Pos;
  struct List_Str *Buffer=NULL;
  struct Datablock *Rec=NULL,*R=NULL,*Contents=NULL;
  struct Blob *B=CreateBlob(strlen(":CONTENTS:"),":CONTENTS:");
  Pos=AL1_FindDBEntry(City,B);
  if(Pos+1){
    Contents=Amoeba2WMS(City,Pos);
    for(Buffer=Contents->DS[2];Buffer;Buffer=Buffer->Next){
      Pos=(unsigned long int)atof(Buffer->Str);
      Rec=Amoeba2WMS(City,Pos);
      Rec->Next=R;
      R=Rec;
    }
  }
  return R;
}

void AL2_PutDBContents(struct Datablock *In,struct Colony *City){
  unsigned long int Pos;
  char *Name,*Position;
  struct Datablock *Contents,*Buffer;
  AL1_InitHeader(City);
  Contents=Add_DB_Entry(NULL,":CONTENTS:");
  for(Buffer=In;Buffer;Buffer=Buffer->Next){
    Pos=WMS2Amoeba(City,Buffer);
    Name=Buffer->DS[0]->Str;
    Position=FloatToString((double )Pos);
    Add_DB_Pair(Contents,Name,Position);
  }
  WMS2Amoeba(City,Contents);
}

struct Datablock *Amoeba2WMS(struct Colony *City,unsigned long int RecordID){
  unsigned long int A,B;
  struct Datablock *DB=NULL;
  struct Cytoskelenton *C=NULL;
  DB=(struct Datablock *)malloc(sizeof(struct Datablock));
  C=AL1_GetDBEntry(City,RecordID);
  DB->DS[0]=NULL;
  DB->DS[1]=NULL;
  DB->DS[2]=NULL;
  for(A=1;A<C->Dimension;A++){
    DB->DS[A-1]=NULL;
    for(B=0;B<C->Pseudopods[A]->Dimension;B++){
      DB->DS[A-1]=Ins_List_Element(DB->DS[A-1],AL1_GetRecName(City,C->Pseudopods[A]->Mitochondria[B]->ARef)->Nucleus,B);  
    }
  }
  DB->DS[0]=Ins_List_Element(DB->DS[0],AL1_GetRecName(City,RecordID)->Nucleus,0);
  DB->Next=NULL;
  return DB;
}

unsigned long int WMS2Amoeba(struct Colony *City,struct Datablock *DB){
  char *ExistFlags,ExistT=0;
  char *Unknown,*Date,*Probability;  
  struct Blob *RecordName,*TMP,*Freeable;
  unsigned long int _,*RecordPos,SourcePos,Size=0,Type,Type2,A,Offset;
  struct List_Str *Unique=NULL,*Buffer,*Buffer2,*DS[3],*WMSHeader=NULL;
  struct Cytoskelenton *Source,**Linked;  
  AL2_CacheFreqUsed(City);
  Unknown="?";
  DATE=City->CachedTime;
  Date=DATE;
  Probability="P:1";
  WMSHeader=Ins_List_Element(WMSHeader,StrCat("",Unknown),0);
  WMSHeader=Ins_List_Element(WMSHeader,StrCat("",Unknown),1);
  WMSHeader=Ins_List_Element(WMSHeader,StrCat("",Unknown),2);
  WMSHeader=Ins_List_Element(WMSHeader,StrCat("",Date),3);
  WMSHeader=Ins_List_Element(WMSHeader,StrCat("",Date),4);
  WMSHeader=Ins_List_Element(WMSHeader,StrCat("",Probability),5);
  WMSHeader=Ins_List_Element(WMSHeader,StrCat("",Unknown),6);
  DS[0]=DB->DS[0];
  DS[1]=DB->DS[1];
  DS[2]=DB->DS[2];
  RecordName=CreateBlob(strlen(DS[0]->Str),StrCat("",DS[0]->Str));
  DS[0]=DS[0]->Next;
  for(_=0;_<3;_++)
    for(Buffer=DS[_];Buffer;Buffer=Buffer->Next)
      if(Find_List_Element(Unique,Buffer->Str)==-1)
	Unique=Ins_List_Element(Unique,StrCat("",Buffer->Str),Size++);
  if(Find_List_Element(Unique,Unknown)==-1)
    Unique=Ins_List_Element(Unique,StrCat("",Unknown),Size++);
  if(Find_List_Element(Unique,Date)==-1)
    Unique=Ins_List_Element(Unique,StrCat("",Date),Size++);
  if(Find_List_Element(Unique,Probability)==-1)
    Unique=Ins_List_Element(Unique,StrCat("",Probability),Size++);
  ExistFlags=(char *)malloc(Size*sizeof(char));
  RecordPos=(unsigned long int *)malloc(Size*sizeof(unsigned long int));
  Linked=(struct Cytoskelenton **)malloc(Size*sizeof(struct Cytoskelenton *));
  Offset=-1;
  SourcePos=AL2_Recognise(City,&ExistT,RecordName);
  Source=AL2_NullCyto(RecordName);
  Offset++;
  if(ExistT)
    AL1_DelDBEntry(City,SourcePos);
  Buffer=Unique;
  for(_=0;_<Size;_++){
    ExistFlags[_]=0;
    Linked[_]=NULL;
    TMP=NULL;
    Freeable=NULL;
    if(!strcmp(Buffer->Str,"?")){
      ExistFlags[_]=1;
      RecordPos[_]=City->CacheUnkn;
    }
    if(!strcmp(Buffer->Str,"P:1")){
      ExistFlags[_]=1;
      RecordPos[_]=City->CacheProb;
    }
    if(!strcmp(Buffer->Str,Date)){
      ExistFlags[_]=1;
      RecordPos[_]=City->CacheDate;
    }
    /* if it is another type then see if it exists */
    if(!ExistFlags[_]){
      TMP=CreateBlob(strlen(Buffer->Str),StrCat("",Buffer->Str));
      RecordPos[_]=AL2_Recognise(City,&ExistFlags[_],TMP);
      Freeable=TMP;
    }
    /* if it still does not exist */
    if(!ExistFlags[_]){
      /* this is not computationally the same as combining with the above */      
      Linked[_]=AL2_NullCyto(TMP);
      Offset++;
      RecordPos[_]+=Offset;
    }
    else{
      /* but if it does already exist, then it will have been added already */
      if(Freeable){
	AL1_FreeBlob(TMP);
	TMP=NULL;
      }
      Linked[_]=AL1_GetDBEntry(City,RecordPos[_]);
      /* if we cannot find the record that should be there */
      if(!Linked[_]){
	/* create it! */
	if(!TMP) 
	  TMP=CreateBlob(strlen(Buffer->Str),StrCat("",Buffer->Str));
	Linked[_]=AL2_NullCyto(TMP);
	Offset++;
	RecordPos[_]+=Offset;
      }
    }
    Buffer=Buffer->Next;
  }
  for(_=0;_<3;_++){
    A=0;
    for(Buffer=DS[_];Buffer;Buffer=Buffer->Next){
      Type=Find_List_Element(Unique,Buffer->Str);
      AL2_AddLink(City,Source,Linked[Type],AL2_CreateMito(SourcePos,_+1,A),AL2_CreateMito(RecordPos[Type],0,0));
      A++;
    }
  }
  _=0;
  for(Buffer=Unique;Buffer;Buffer=Buffer->Next)
    if(!ExistFlags[_++]){
      A=0;
      Type=Find_List_Element(Unique,Buffer->Str);
      for(Buffer2=WMSHeader;Buffer2;Buffer2=Buffer2->Next){
	Type2=Find_List_Element(Unique,Buffer2->Str);
	AL2_AddLink(City,Linked[Type],Linked[Type2],AL2_CreateMito(RecordPos[Type],1,A),AL2_CreateMito(RecordPos[Type2],0,0));
	A++;
      }
    }
  if(!ExistT){
    AL1_AddDBEntry(City,Source);
  }
  else{
    AL1_RepDBEntry(City,Source,SourcePos);
  }
  for(_=0;_<Size;_++){
    if(!ExistFlags[_]){
      AL1_AddDBEntry(City,Linked[_]);
    }
    else{
      AL1_RepDBEntry(City,Linked[_],RecordPos[_]);
    }
    AL1_FreeAmoeba(Linked[_]);
  }
  AL1_FreeAmoeba(Source);
  free(Linked);
  FreeList(Unique);
  free(RecordPos);
  free(ExistFlags);
  FreeList(WMSHeader);
  return SourcePos;
}

unsigned long int NewRec(struct Colony *City,struct Blob *Name){
  char ExistT;
  unsigned long int UnkPos,DatPos,ProPos,ThisPos,NullOffSet=0;
  struct Cytoskelenton *Source=NULL;  
  struct Cytoskelenton *UnknownR=NULL;
  struct Cytoskelenton *DateR=NULL;  
  struct Cytoskelenton *ProbabilityR=NULL;
  AL2_CacheFreqUsed(City);

  ThisPos=AL2_Recognise(City,&ExistT,Name);  
  UnkPos=City->CacheUnkn;
  DatPos=City->CacheDate;
  ProPos=City->CacheProb;
  if(!ExistT){
    Source=AL2_NullCyto(Name);
    NullOffSet++;
  }
  else
    Source=AL1_GetDBEntry(City,ThisPos);
  UnknownR=AL1_GetDBEntry(City,UnkPos);
  DateR=AL1_GetDBEntry(City,DatPos);
  ProbabilityR=AL1_GetDBEntry(City,ProPos);
  if(!ExistT){
    AL2_AddLink(City,Source,UnknownR,    AL2_CreateMito(ThisPos,1,0),AL2_CreateMito(UnkPos,0,0));
    AL2_AddLink(City,Source,UnknownR,    AL2_CreateMito(ThisPos,1,1),AL2_CreateMito(UnkPos,0,0));
    AL2_AddLink(City,Source,UnknownR,    AL2_CreateMito(ThisPos,1,2),AL2_CreateMito(UnkPos,0,0));
    AL2_AddLink(City,Source,DateR,       AL2_CreateMito(ThisPos,1,3),AL2_CreateMito(DatPos,0,0));
    AL2_AddLink(City,Source,DateR,       AL2_CreateMito(ThisPos,1,4),AL2_CreateMito(DatPos,0,0));
    AL2_AddLink(City,Source,ProbabilityR,AL2_CreateMito(ThisPos,1,5),AL2_CreateMito(ProPos,0,0));
    AL2_AddLink(City,Source,UnknownR,    AL2_CreateMito(ThisPos,1,6),AL2_CreateMito(UnkPos,0,0));
  }
  if(!ExistT) AL1_AddDBEntry(City,Source);
  else        AL1_RepDBEntry(City,Source,ThisPos);

  return ThisPos;
}

unsigned long int AL2_Recognise(struct Colony *City,char *Flag,struct Blob *Name){
  unsigned long int Position=-1;
  if((Position=AL1_FindDBEntry(City,Name))==-1){
    *Flag=0;
    Position=City->Population;
  }
  else{
    /* Need to add additional logic for  updating probabilities and last access times */
    
    *Flag=1;
  }

  return Position;
}

unsigned long int RepEntry(struct Colony *City,struct Mitochondrion *M,struct Blob *Attribute){
  unsigned long int Position=M->ARef,_;
  struct Blob *G1,*G2;
  struct Cytoskelenton *Root=NULL,*AddedLeaf=NULL;
  struct Mitochondrion *Remove;
  if(M){
    Root=AL1_GetDBEntry(City,Position);
    G1=Root->Core;
    Remove=AL2_ReturnMito(Root,M);
    _=AL1_FindDBEntry(City,Attribute);
    if(_==-1)
      _=NewRec(City,Attribute);
    AddedLeaf=AL1_GetDBEntry(City,_);
    G2=AddedLeaf->Core;    
    AL2_RepLink(City,Root,AddedLeaf,M,AL2_CreateMito(_,0,0));
    AL1_RepDBEntry(City,Root,Position);
    AL1_RepDBEntry(City,AddedLeaf,_);
    AL1_FreeBlob(G1);
    AL1_FreeBlob(G2);
  }
  else
    puts("RepEntry: Unable to locate mitochondrion. Aborting operation.");
  return 1;
}

unsigned long int AL2_RepLink(struct Colony *City,struct Cytoskelenton *Source,struct Cytoskelenton *Target,struct Mitochondrion *From,struct Mitochondrion *To){
  struct Mitochondrion *M=NULL,*DeleteMe;
  struct Cytoskelenton *C=NULL;
  char *Pos;
  /* Add the entry to the new target */
  if(!Target->Dimension){
    Target->Dimension=1;
    Target->Pseudopods=(struct Pseudopod **)malloc(sizeof(struct Pseudopod *));
    Target->Pseudopods[0]=AL2_Add2Pseudo(NULL,From);
  }
  else{
    Target->Pseudopods[0]=AL2_Add2Pseudo(Target->Pseudopods[0],From);
  }

  /* Delete the entry from the old target */
  M=AL2_ReturnMito(Source,From);
  C=AL1_GetDBEntry(City,M->ARef);
  DeleteMe=AL2_FindMito(C,From);
  Pos=(char *)(C->Pseudopods[0]->Mitochondria[DeleteMe->MRef]);
  C->Pseudopods[0]->Dimension--;
  memmove(Pos,Pos+sizeof(struct Mitochondria *),(C->Pseudopods[0]->Dimension-DeleteMe->MRef)*sizeof(struct Mitochondria *));
  C->Pseudopods[0]->Mitochondria=(struct Mitochondrion **)realloc(C->Pseudopods[0]->Mitochondria,C->Pseudopods[0]->Dimension*sizeof(struct Mitochondrion *));
  AL1_RepDBEntry(City,C,M->ARef);
  free((struct Mitochondrion *)Pos);

  /* Replace the entry at the source */
  Source->Pseudopods[From->PRef]->Mitochondria[From->MRef]=To;
  return 1;
}

unsigned long int AL2_AddLink(struct Colony *City,struct Cytoskelenton *Source,struct Cytoskelenton *Target,struct Mitochondrion *From,struct Mitochondrion *To){
  unsigned long int _,Before,SP=From->PRef;
  if(!Target->Dimension){
    Target->Dimension=1;
    Target->Pseudopods=(struct Pseudopod **)malloc(sizeof(struct Pseudopod *));
    Target->Pseudopods[0]=AL2_Add2Pseudo(NULL,From);
  }
  else{
    Target->Pseudopods[0]=AL2_Add2Pseudo(Target->Pseudopods[0],From);
  }
  if(Source->Dimension<SP+1){
    if(!Source->Dimension){
      Source->Dimension=1;
      Source->Pseudopods=(struct Pseudopod **)malloc(sizeof(struct Pseudopod *));
      Source->Pseudopods[0]=AL2_NullPseudo();
    }
    Before=Source->Dimension;
    Source->Dimension=SP+1;
    Source->Pseudopods=(struct Pseudopod **)realloc(Source->Pseudopods,Source->Dimension*sizeof(struct Pseudopod *));
    Source->Pseudopods[SP]=AL2_Add2Pseudo(NULL,To);
    for(_=Before;_<SP;_++)
      Source->Pseudopods[_]=AL2_NullPseudo();
  }
  else
    Source->Pseudopods[SP]=AL2_Add2Pseudo(Source->Pseudopods[SP],To);
  return 1;
}

unsigned long int AddEntry(struct Colony *City,struct Mitochondrion *M,struct Blob *Attribute){
  char AttribExists=0;
  unsigned long int Position,AttribPosition;
  struct Cytoskelenton *Root=NULL,*Leaf=NULL;
  if(M){
    Position=M->ARef;
    Root=AL1_GetDBEntry(City,Position);

    if(Root){
      AttribPosition=AL2_Recognise(City,&AttribExists,Attribute);
      if(!AttribExists)
	AttribPosition=NewRec(City,Attribute);
      Leaf=AL1_GetDBEntry(City,AttribPosition);

      AL2_AddLink(City,Root,Leaf,M,AL2_CreateMito(AttribPosition,0,0));
      AL1_RepDBEntry(City,Root,Position);
      AL1_RepDBEntry(City,Leaf,AttribPosition);

    }
    else
      printf("AddEntry: Unable to retrieve record. Aborting operation.");

  }
  else
    puts("AddEntry: Unable to locate mitochondrion. Aborting operation.");
  return 1;
}

struct Mitochondrion *FindEntry(struct Colony *City,unsigned long int Position,struct Blob *Pattern){
  unsigned long int A,B;
  struct Blob *Name;
  struct Cytoskelenton *C=NULL;
  struct Pseudopod     *P=NULL;
  struct Mitochondrion *M=NULL,*R=(struct Mitochondrion *)malloc(sizeof(struct Mitochondrion));
  C=AL1_GetDBEntry(City,Position);
  R->ARef=-1;
  R->PRef=0;
  R->MRef=0;
  for(A=0;A<C->Dimension;A++){
    P=C->Pseudopods[A];
    for(B=0;B<P->Dimension;B++){
      M=P->Mitochondria[B];
      Name=AL1_GetRecName(City,M->ARef);
      if(BlobSame(Pattern,Name)){
	R->ARef=Position;
	R->PRef=A;
	R->MRef=B;
	A=C->Dimension;
	B=P->Dimension;
      }
    }
  }
  return R;
}

struct Blob *GetEntry(struct Colony *City,struct Mitochondrion *M){
  struct Blob *R=NULL;
  struct Cytoskelenton *C=NULL;
  C=AL1_GetDBEntry(City,M->ARef);
  if(C){
    if(C->Dimension>M->PRef)
      if(C->Pseudopods[M->PRef]->Dimension>M->MRef)
	R=AL1_GetRecName(City,C->Pseudopods[M->PRef]->Mitochondria[M->MRef]->ARef);
  }
  else
    printf("GetEntry: Record, [%ld] not found! Aborting operation.\n",M->ARef);
  return R;
}

unsigned long int RepPair(struct Colony *City,unsigned long int Position,struct Blob *Qual,struct Blob *Value){
  struct Blob *D=NULL;
  struct Mitochondrion *Location=FindPair(City,Position,Qual,D);
  if(Location->ARef+1)
    DelPair(City,Location);
  AddPair(City,Position,Qual,Value);
  return 1;
}

unsigned long int DelPair(struct Colony *City, struct Mitochondrion *M){
  struct Cytoskelenton *Root=NULL,*Leaf1=NULL,*Leaf2=NULL,*A,*B,*C;
  struct Mitochondrion *D=NULL,*E=NULL,*N=NULL,*G=NULL,*H=NULL;
  Root=AL1_GetDBEntry(City,M->ARef);
  if((Root)&&(M->PRef>1)){
    N=AL2_CreateMito(M->ARef,5-M->PRef,M->MRef);
    D=AL2_ReturnMito(Root,M);
    E=AL2_ReturnMito(Root,N);
    A=DeleteEntry(City,Root,M);
    Leaf1=AL1_GetDBEntry(City,D->ARef);
    if(Leaf1){
      G=AL2_FindMito(Leaf1,M);
      G->ARef=D->ARef;
      B=DeleteEntry(City,Leaf1,G);
    }
    Leaf2=AL1_GetDBEntry(City,E->ARef);
    if(Leaf2){
      H=AL2_FindMito(Leaf2,N);
      H->ARef=E->ARef;
      C=DeleteEntry(City,Leaf2,H);
    }
  }
  AL1_RepDBEntry(City,A,M->ARef);
  AL1_RepDBEntry(City,B,D->ARef);
  AL1_RepDBEntry(City,C,E->ARef);
  return 0;
}

struct Mitochondrion *FindPair(struct Colony *City,unsigned long int Position,struct Blob *Qual,struct Blob *Value){
  unsigned long int _,Location=0;
  int Pos;
  struct Blob *Name,*Search;
  char Found=0;
  struct Cytoskelenton *C=NULL;
  struct Pseudopod     *P=NULL;
  struct Mitochondrion *R=(struct Mitochondrion *)malloc(sizeof(struct Mitochondrion ));  
  R->ARef=-1;
  if(!(Qual)&&!(Value))
    puts("FindPair: Need at least one search reference - Aborting operation.");
  else{
    C=AL1_GetDBEntry(City,Position);
    if(C){
      if(Qual){
	Pos=2;
	Search=Qual;
      }
      else{
	Pos=3;
	Search=Value;
      }
      P=C->Pseudopods[Pos];
      for(_=0;_<P->Dimension;_++){
	Name=AL1_GetRecName(City,P->Mitochondria[_]->ARef);
	if(BlobSame(Name,Search)){
	  Found=1;
	  Location=_;
	}
      }
      if(Found){
	R->ARef=Position;
	R->PRef=Pos;
	R->MRef=Location;
      }
    }
    else
      puts("FindPair: Can't find the record - Aborting operation.");
  }
  return R;
}

unsigned long int AddPair(struct Colony *City,unsigned long int Position,struct Blob *Qual,struct Blob *Value){
  unsigned long int L1,L2;
  struct Cytoskelenton *Root=NULL,*Leaf1=NULL,*Leaf2=NULL;
  struct Blob *G1,*G2,*G3;
  Root=AL1_GetDBEntry(City,Position);
  L1=AL1_FindDBEntry(City,Qual);
  if(L1==-1)
    L1=NewRec(City,Qual);
  Leaf1=AL1_GetDBEntry(City,L1);
  L2=AL1_FindDBEntry(City,Value);
  if(L2==-1)
    L2=NewRec(City,Value);
  Leaf2=AL1_GetDBEntry(City,L2);
  AL2_AddLink(City,Root,Leaf1,AL2_CreateMito(Position,2,Root->Pseudopods[2]->Dimension),AL2_CreateMito(L1,0,0));
  AL2_AddLink(City,Root,Leaf2,AL2_CreateMito(Position,3,Root->Pseudopods[3]->Dimension),AL2_CreateMito(L2,0,0));
  G1=Root->Core;
  G2=Leaf1->Core;
  G3=Leaf2->Core;
  AL1_RepDBEntry(City,Root,Position);
  AL1_RepDBEntry(City,Leaf1,L1);
  AL1_RepDBEntry(City,Leaf2,L2);
  AL1_FreeBlob(G1);
  AL1_FreeBlob(G2);
  AL1_FreeBlob(G3);
  return 0;
}

unsigned long int RepRec(struct Colony *City,struct Datablock *DB,unsigned long int RecordID){
  unsigned long int R=0;
  if(RecordID+1)
    DelRec(City,RecordID);
  WMS2Amoeba(City,DB);
  return R;
}

struct Cytoskelenton *DeleteEntry(struct Colony *City,struct Cytoskelenton *C,struct Mitochondrion *M){
  unsigned long int _=0;
  struct Cytoskelenton *R=C;
  struct Pseudopod     *P=NULL,*Q=NULL;
  char *PP,*QP;
  if(C->Dimension>=M->PRef){
    if(C->Pseudopods[M->PRef]->Dimension>=M->MRef){
      if(M->PRef==0){
	P=C->Pseudopods[M->PRef];	
	if(P->Dimension>M->MRef){
	  PP=(char *)P->Mitochondria+(M->MRef)*sizeof(struct Mitochondrion *);
	  memmove(PP,PP+sizeof(struct Mitochondrion *),(P->Dimension-1-(M->MRef))*sizeof(struct Mitochondrion *));
	}
	P->Dimension--;
	P->Mitochondria=(struct Mitochondrion **)realloc(P->Mitochondria,P->Dimension*sizeof(struct Mitochondrion *));
	C->Pseudopods[M->PRef]=P;
      }
      if(M->PRef==1){
	_=AL1_FindDBEntry(City,CreateBlob(strlen("?"),"?"));
	C->Pseudopods[M->PRef]->Mitochondria[M->MRef]=AL2_CreateMito(_,0,0);
      }
      if((M->PRef>1)&&(M->PRef<4)){
	_=5-M->PRef;
	P=C->Pseudopods[M->PRef];
	Q=C->Pseudopods[_];
	if(P->Dimension>M->MRef){
	  PP=(char *)P->Mitochondria+(M->MRef)*sizeof(struct Mitochondrion *);
	  QP=(char *)Q->Mitochondria+(M->MRef)*sizeof(struct Mitochondrion *);
	  memmove(PP,PP+sizeof(struct Mitochondrion *),(P->Dimension-1-(M->MRef))*sizeof(struct Mitochondrion *));
	  memmove(QP,QP+sizeof(struct Mitochondrion *),(Q->Dimension-1-(M->MRef))*sizeof(struct Mitochondrion *));
	}
	P->Dimension--;
	Q->Dimension--;
	P->Mitochondria=(struct Mitochondrion **)realloc(P->Mitochondria,P->Dimension*sizeof(struct Mitochondrion *));
	Q->Mitochondria=(struct Mitochondrion **)realloc(Q->Mitochondria,Q->Dimension*sizeof(struct Mitochondrion *));
	C->Pseudopods[_]=Q;
	C->Pseudopods[M->PRef]=P;
      }
    }
    else{
      puts("DeleteEntry: cannot complete operation! Abort.");
      return NULL;
    }
  }
  else{
    puts("DeleteEntry: cannot complete operation! Abort.");
    return NULL;
  }
  return R;
}


unsigned long int DelRec(struct Colony *City,unsigned long int RecordID){
  unsigned long int A=0,_=0,Dimension=0,*Positions=NULL,*ReplacePos=NULL;
  char *Name=NULL;
  struct Cytoskelenton *C=NULL,**Dependencies=NULL;
  struct Mitochondrion *M=NULL;
  struct List_Str *L=NULL;
  C=AL1_GetDBEntry(City,RecordID);
  if(!strcmp(C->Core->Nucleus,"?")){
    printf("Refusing to delete \"%s\".\n",C->Core->Nucleus);
    return 0;
  }
  A=0;
  for(_=0;_<C->Pseudopods[0]->Dimension;_++){
    M=C->Pseudopods[0]->Mitochondria[_];
    Name=AL1_GetRecName(City,M->ARef)->Nucleus;
    if(Find_List_Element(L,Name)==-1)
      L=Ins_List_Element(L,Name,A++);
  }
  Dimension=Size_of_List(L);
  Positions=(unsigned long int *)malloc(_*sizeof(unsigned long int));
  Dependencies=(struct Cytoskelenton **)malloc((Dimension+1)*sizeof(struct Cytoskelenton *));
  ReplacePos=(unsigned long int *)malloc((Dimension+1)*sizeof(unsigned long int));
  L=NULL;
  A=0;
  for(_=0;_<C->Pseudopods[0]->Dimension;_++){
    M=C->Pseudopods[0]->Mitochondria[_];
    Name=AL1_GetRecName(City,M->ARef)->Nucleus;
    if((Positions[_]=Find_List_Element(L,Name))==-1){
      L=Ins_List_Element(L,Name,A);
      Positions[_]=A;
      ReplacePos[A]=M->ARef;
      Dependencies[A]=AL1_GetDBEntry(City,M->ARef);
      A++;
    }
  }
  for(_=0;_<C->Pseudopods[0]->Dimension;_++){
    M=C->Pseudopods[0]->Mitochondria[_];
    Dependencies[Positions[_]]=DeleteEntry(City,Dependencies[Positions[_]],M);
  }
  for(_=0;_<Dimension;_++){
    AL1_RepDBEntry(City,Dependencies[_],ReplacePos[_]);
  }
  AL1_DelDBEntry(City,RecordID);
  return 1;
}

struct Pseudopod *AL2_DuplicPseudo(struct Pseudopod *P){
  unsigned long int _;
  struct Pseudopod *New=(struct Pseudopod *)malloc(sizeof(struct Pseudopod ));
  New->Dimension=P->Dimension;
  New->Mitochondria=(struct Mitochondrion **)malloc(New->Dimension*sizeof(struct Mitochondrion *));
  for(_=0;_<New->Dimension;_++){
    New->Mitochondria[_]=AL2_DuplicMito(P->Mitochondria[_]);
  }
  return New;
}

unsigned long int AL2_DelLink(struct Colony *City,struct Mitochondrion *From){
  unsigned long int SourcePos,TargetPos;
  struct Mitochondrion *TM,*SM;
  struct Cytoskelenton *Source, *Target;
  Source=AL1_GetDBEntry(City,From->ARef);
  TM=AL2_ReturnMito(Source,From);
  TargetPos=TM->ARef;
  SourcePos=From->ARef;
  Target=AL1_GetDBEntry(City,TM->ARef);
  Source=AL2_DeleteMito(Source,From);
  SM=AL2_FindMito(Target,From);
  SM->ARef=From->ARef;
  Target=AL2_DeleteMito(Target,SM);
  AL1_RepDBEntry(City,Source,SourcePos);
  AL1_RepDBEntry(City,Target,TargetPos);
  return 1;
}

struct Mitochondrion *AL2_FindMito(struct Cytoskelenton *C,struct Mitochondrion *Token){
  char Found=0;
  unsigned long int A,B;
  struct Mitochondrion *D,*M=(struct Mitochondrion *)malloc(sizeof(struct Mitochondrion ));
  M->ARef=-1;  
  for(A=0;(A<C->Dimension)&&(!Found);A++)
    for(B=0;(B<C->Pseudopods[A]->Dimension)&&(!Found);B++){
      D=C->Pseudopods[A]->Mitochondria[B];
      if((D->ARef==Token->ARef)&&(D->PRef==Token->PRef)&&(D->MRef==Token->MRef)){
	M->PRef=A;
	M->MRef=B;
	Found=1;
      }
    }
  return M;
}

struct Cytoskelenton *AL2_DeleteMito(struct Cytoskelenton *C,struct Mitochondrion *Pos){
  if(C)
    if(C->Dimension>=Pos->PRef)
      if(C->Pseudopods[Pos->PRef]->Dimension>=Pos->MRef){
	free(C->Pseudopods[Pos->PRef]->Mitochondria[Pos->MRef]);
	memmove(C->Pseudopods[Pos->PRef]->Mitochondria[Pos->MRef],C->Pseudopods[Pos->PRef]->Mitochondria[Pos->MRef]+sizeof(struct Mitochondrion *),(C->Pseudopods[Pos->PRef]->Dimension-Pos->MRef)*sizeof(struct Mitochondrion *));
	C->Pseudopods[Pos->PRef]->Dimension--;
	C->Pseudopods[Pos->PRef]->Mitochondria=(struct Mitochondrion **)realloc(C->Pseudopods[Pos->PRef]->Mitochondria,C->Pseudopods[Pos->PRef]->Dimension*sizeof(struct Mitochondrion *));
      }
  return C;
}

struct Mitochondrion *AL2_ReturnMito(struct Cytoskelenton *C,struct Mitochondrion *Pos){
  struct Mitochondrion *M=NULL;
  if(C)
    if(C->Dimension>=Pos->PRef)
      if(C->Pseudopods[Pos->PRef]->Dimension>=Pos->MRef)
	M=C->Pseudopods[Pos->PRef]->Mitochondria[Pos->MRef];
  return M;
}

struct Mitochondrion *AL2_DuplicMito(struct Mitochondrion *M){
  struct Mitochondrion *R;
  R=(struct Mitochondrion *)malloc(sizeof(struct Mitochondrion));
  memcpy(R,M,sizeof(struct Mitochondrion));
  return R;
}

struct Cytoskelenton *AL2_NullCyto(struct Blob *Name){
  struct Cytoskelenton *C=NULL;
  if(Name){
    C=(struct Cytoskelenton *)malloc(sizeof(struct Cytoskelenton));
    C->Dimension=0;
    C->Size=Name->Size;
    C->Core=Name;
  }
  return C;
}

struct Cytoskelenton *AL2_Add2Cyto(struct Cytoskelenton *C,struct Pseudopod *P){
  if(C){
    if(C->Dimension){
      C->Dimension++;
      C->Pseudopods=(struct Pseudopod **)realloc(C->Pseudopods,C->Dimension*sizeof(struct Pseudopod *));
      C->Pseudopods[C->Dimension-1]=P;
    }
    else{
      C->Dimension++;
      C->Pseudopods=(struct Pseudopod **)malloc(C->Dimension*sizeof(struct Pseudopod *));
      C->Pseudopods[C->Dimension-1]=P;
    }
  }
  return C;
}

struct Pseudopod *AL2_NullPseudo(){
  struct Pseudopod *P=NULL;
  P=(struct Pseudopod *)malloc(sizeof(struct Pseudopod));
  P->Dimension=0;
  P->Mitochondria=(struct Mitochondrion **)malloc(sizeof(struct Mitochondrion *));
  return P;
}

struct Pseudopod *AL2_Add2Pseudo(struct Pseudopod *P,struct Mitochondrion *M){  
  if(P){
    P->Dimension++;
    P->Mitochondria=(struct Mitochondrion **)realloc(P->Mitochondria,P->Dimension*sizeof(struct Mitochondrion *));
    P->Mitochondria[P->Dimension-1]=M;
  }
  else{
    P=(struct Pseudopod *)malloc(sizeof(struct Pseudopod));
    P->Dimension=1;
    P->Mitochondria=(struct Mitochondrion **)malloc(sizeof(struct Mitochondrion *));
    P->Mitochondria[0]=M;
  }
  return P;
}

struct Mitochondrion *AL2_CreateMito(unsigned long int ARef,unsigned short int PRef,unsigned short int MRef){
  struct Mitochondrion *M;
  M=(struct Mitochondrion *)malloc(sizeof(struct Mitochondrion));
  M->ARef=ARef;
  M->PRef=PRef;
  M->MRef=MRef;
  return M;
}

struct Pseudopod *AL2_CreateHeader(struct Colony *City,unsigned long int RecordID){

  return NULL;
}

unsigned long int AL2_CreateBlankR(struct Colony *City,struct Blob *Name){
  unsigned long int Created=-1;
  struct Cytoskelenton *C;
  printf(" + CreateBlankR %s\n",Name->Nucleus);
  if((Created=AL1_FindDBEntry(City,Name))==-1){
    C=(struct Cytoskelenton *)malloc(sizeof(struct Cytoskelenton));
    C->Dimension=0;
    C->Pseudopods=(struct Pseudopod **)malloc(sizeof(struct Pseudopod *));
    C->Core=Name;
    C->Size=Name->Size;    
    if(AL1_AddDBEntry(City,C)!=-1)
      Created=City->Population;
  }
  return Created;
}

void Amoeba_P_DB_Entry(struct Datablock *LS,int P){
  int _=0;
  struct List_Str *L1=NULL,*L2=NULL; 
  for(;LS;_++){ 
    if(_==P){
      printf("=[Datablock %05d]==========================================\n",_); 
      L1=LS->DS[0];
      printf("Name %23s Grammar %23s\n",Get_List_Element(L1,0),Get_List_Element(L1,1));
      printf("Purpose %20s Emotion %23s\n",Get_List_Element(L1,2),Get_List_Element(L1,3));
      printf("First Time %28s\n",Get_List_Element(L1,4)); 
      printf("Last Time %29s\n",Get_List_Element(L1,5));
      printf("Probability    %13s Commands %22s\n",Get_List_Element(L1,6),Get_List_Element(L1,7));
      
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
    LS=LS->Next;
  }
}


struct List_Str *AL2_ReportReferences(struct Colony *City,struct Blob *Name){
  unsigned long int _,A,B;
  char *D;
  struct List_Str *R=NULL;
  struct Cytoskelenton *C=NULL;  
  _=AL1_FindDBEntry(City,Name);
  C=AL1_GetDBEntry(City,_);
  for(B=0;B<C->Pseudopods[0]->Dimension;B++)
    if((D=AL1_GetRecName(City,C->Pseudopods[0]->Mitochondria[B]->ARef)->Nucleus))
      R=Ins_List_Element(R,D,0);
  for(A=0;A<C->Dimension;A++){
    for(B=0;B<C->Pseudopods[A]->Dimension;B++)
      free(C->Pseudopods[A]->Mitochondria[B]);
    free(C->Pseudopods[A]);
  }
  free(C);
  return R;
}

struct List_Str *AL2_ReportLinks(struct Colony *City,struct Blob *Name){
  unsigned long int _,A,B;
  char *D;
  struct List_Str *R=NULL;
  struct Cytoskelenton *C=NULL;
  _=AL1_FindDBEntry(City,Name);
  C=AL1_GetDBEntry(City,_);
  for(A=1;A<C->Dimension;A++)
    for(B=0;B<C->Pseudopods[A]->Dimension;B++)
      if((D=AL1_GetRecName(City,C->Pseudopods[A]->Mitochondria[B]->ARef)->Nucleus))
	R=Ins_List_Element(R,D,0);
  for(A=0;A<C->Dimension;A++){
    for(B=0;B<C->Pseudopods[A]->Dimension;B++)
      free(C->Pseudopods[A]->Mitochondria[B]);
    free(C->Pseudopods[A]);
  }
  free(C);
  return R;
}

struct List_Str *AL2_ReportContents(struct Colony *City){
  unsigned long int _;
  char *D;
  struct List_Str *R=NULL;
  for(_=City->Population;_>0;_--)
    if((D=AL1_GetRecName(City,AL1_GetCacheHeader(City,_))->Nucleus))
      R=Ins_List_Element(R,D,0);
  return R;
}

/*

          /:`:::.     .        :       ...    .,::::::  :::::::.    :::.          
   ,,    /;` ;;`;;    ;;,.    ;;;   .;;;;;;;. ;;;;''''   ;;;'';;'   ;;`;;    ,,   
 ,[['   n[' ,[[ '[[,  [[[[, ,[[[[, ,[[     \[[,[[cccc    [[[__[[\. ,[[ '[[,  '[[, 
c$     c$" c$$$cc$$$c $$$$$$$$"$$$ $$$,     $$$$$""""    $$""""Y$$c$$$cc$$$c    $c
"8bo, o8"   888   888,888 Y88" 888o"888,_ _,88P888oo,__ _88o,,od8P 888   888,,od8"
  "MPmM"    YMM   ""` MMM  M'  "MMM  "YMMMMMP" """"YUMMM""YUMMMP"  YMM   ""` YM"  

*/
