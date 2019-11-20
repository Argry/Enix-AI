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

#include "ENiX_LIST.h"
#include "ENiX_STRING.h"

void PrintList(struct List_Str *LS){
   int i,Size,IsNum,DP=0;
   char *Str;
   while(LS){
      Size=strlen(Str=StrCat("",LS->Str));
      IsNum=1;
      for(i=0;i<Size;i++){
	 if(!(isdigit(Str[i])||(Str[i]=='-')||(Str[i]=='.')))
	   IsNum=0;
	 if(Str[i]=='.')
	   DP=1;
      }
      if(IsNum&&DP){
	 for(i=Size-1;i>0;i--){
	    if(Str[i]=='0')
	      Str[i]=0;
	    else{
	       if(Str[i]=='.')
		 Str[i]=0;
	       i=0;
	    }
	 }
      }
      printf("%s ",Str);
      free(Str);
      LS=LS->Next;
   }
   puts(""); 
}

void FreeList(struct List_Str *L){
  struct List_Str *Old;
  while(L){
    free(L->Str);
    Old=L;
    L=L->Next;
    free(Old);
  }
}

void PrintListSpecial(char *Prefix,struct List_Str *LS,char *Separator,char *FinalSep,char *Suffix){
  printf("%s",Prefix);
  if(LS){
    while(LS){
      printf("%s",LS->Str);
      if(LS->Next){
	if(LS->Next->Next)
	  printf("%s",Separator);
	else
	  printf("%s",FinalSep);
      }
      LS=LS->Next;
    }
  }
  printf("%s",Suffix);
}

struct List_Str *Cpy_List(struct List_Str *LS){
   int _=0;
   struct List_Str *R=NULL;
   while(LS){
      R=Ins_List_Element(R,StrCat("",LS->Str),_++);
      LS=LS->Next;
   }
   return R;
}

struct List_Str *Ins_List_Element(struct List_Str *LS,char *S,int P){
   int _=0;
   struct List_Str *Prev,*R=NULL;
   if(P){
      R=LS;
      for(;(LS)&&(_!=P);_++){
	 Prev=LS;
	 LS=LS->Next;
      }
      if(_<P+1){
	 Prev->Next=malloc(sizeof(struct List_Str));
	 Prev=Prev->Next;
	 Prev->Str=S;
	 Prev->Next=LS;
      }
   }
   else{
      R=malloc(sizeof(struct List_Str));
      R->Str=S;
      R->Next=LS;
   }
   return R;
}

struct List_Str *Ins_List_List(struct List_Str *L1,struct List_Str *L2,int P){
  int _,C=0;
  struct List_Str *R=NULL;
  if(!L1)
    return L2;
  if(!L2)
    return L1;
  if(!P){
    while(L2){
      R=Ins_List_Element(R,L2->Str,C++); 
      L2=L2->Next;
    }
    while(L1){
      R=Ins_List_Element(R,L1->Str,C++);
      L1=L1->Next;
    }
  }
  else{
    for(_=0;(_<P)&&(L1);_++){ 
      R=Ins_List_Element(R,L1->Str,C++);
      L1=L1->Next;
    }
    while(L2){
      R=Ins_List_Element(R,L2->Str,C++);
      L2=L2->Next;
    }
    while(L1){ 
      R=Ins_List_Element(R,L1->Str,C++);
      L1=L1->Next;
    }
  }
  return R;
}

struct List_Str *Del_List_Element(struct List_Str *LS,int P){
  int _=0;
  struct List_Str *Prev,*R=LS;
  if(LS){ 
    if(P){
      for(;(LS)&&(_!=P);_++){
	Prev=LS;
	LS=LS->Next;
      }
      if((_==P)&&(LS)){
	Prev->Next=LS->Next;
	free(LS);
      } 
    }
    else{
      R=LS->Next;
      free(LS);
    }
  }
  return R;
}

char *Get_List_Element(struct List_Str *LS,int P){ 
  int _=0;
  char *R=NULL;
  for(;(LS)&&(_!=P);_++)
    LS=LS->Next;
  if((_==P)&&(LS))
    R=LS->Str;
  return R;
}

struct List_Str  *Rep_List_Element(struct List_Str *LS,int P,char *Data){
   int _=0;
   struct List_Str *R=LS;
   for(;LS;LS=LS->Next)
     if(P==_++)
       LS->Str=Data;
   return R;
}

int Find_List_Element(struct List_Str *LS,char *S){
  int _=0;
  if(!S)
    return -1;
  for(;LS;_++){
    if(!strcmp(LS->Str,S))
      return _;
    LS=LS->Next;
  }
  return -1;
}

int Size_of_List(struct List_Str *LS){
   int Size;
   for(Size=0;LS;LS=LS->Next)
     Size++; 
   return Size;
}

struct List_Str *LowerCaseList(struct List_Str *LS){
  int i;
  struct List_Str *CB=LS;
  while(CB){
    for(i=0;CB->Str[i];i++)
      CB->Str[i]=tolower(CB->Str[i]);
    CB=CB->Next;
  }
  return CB;
}

struct List_Str *SortList(struct List_Str *LS){
  int C=0,D=0;
  struct List_Str *R=Cpy_List(LS),*Buffer1=NULL,*Buffer2=NULL;
  char *B=NULL;
  for(Buffer1=R;Buffer1;Buffer1=Buffer1->Next){
    D=0;
    for(Buffer2=R;Buffer2;Buffer2=Buffer2->Next){
      if(C>D){
	if(strcmp(Buffer1->Str,Buffer2->Str)<0){
	  B=Buffer2->Str;
	  Buffer2->Str=Buffer1->Str;
	  Buffer1->Str=B;
	}
      }
      D++;
    }
    C++;
  }
  return R;
}
  
struct List_Str *FormatList(struct List_Str *LS,char *First,char *Punct,char *Conj,char *Final){
  int Position=0;
  struct List_Str *Alpha,*Beta=NULL;
  if(LS->Next){
    for(Alpha=LS;Alpha;Alpha=Alpha->Next){
      if(!Position)
	if(First)
	  Beta=Ins_List_Element(Beta,First,Position++);
      Beta=Ins_List_Element(Beta,Alpha->Str,Position++);
      if(Alpha->Next){
	if(Alpha->Next->Next)
	  Beta=Ins_List_Element(Beta,Punct,Position++);
	else
	  Beta=Ins_List_Element(Beta,Conj,Position++);
      }
    }
    if(Final)
      Beta=Ins_List_Element(Beta,Final,Position++);
    return Beta;
  }
  else
    return LS;
}

struct List_Str *RemoveDups(struct List_Str *LS){
   int C=0;
   struct List_Str *Final=NULL;
   for(;LS;LS=LS->Next){
      if(Find_List_Element(Final,LS->Str)==-1)
	Final=Ins_List_Element(Final,LS->Str,C++);
   }
   return Final;
}

struct List_Str  *AutoFormat(struct List_Str *LS){
  struct List_Str *R=NULL;
  int _=0,i,Size,IsNum,DP=0;
  char *Str;
  for(;LS;LS=LS->Next){
    Size=strlen(Str=StrCat("",LS->Str));
    IsNum=1;
    for(i=0;i<Size;i++){
      if(!(isdigit(Str[i])||(Str[i]=='-')||(Str[i]=='.')))
	IsNum=0;
      if(Str[i]=='.')
	DP=1;
    }
    if(IsNum&&DP){
      for(i=Size-1;i>0;i--){
	if(Str[i]=='0')
	  Str[i]=0;
	else{
	  if(Str[i]=='.')
	    Str[i]=0;
	  i=0;
	}
      }
    }
    R=Ins_List_Element(R,Str,_++);
  }
  return R;
}

struct List_Str **Add_To_Array(struct List_Str **ARRAY,struct List_Str *ToAdd,int Size){
   if(!Size){
      ARRAY=(struct List_Str **)malloc(sizeof(struct List_Str *));
      ARRAY[0]=ToAdd;
   }
   else{
      ARRAY=(struct List_Str **)realloc(ARRAY,(Size+1)*sizeof(struct List_Str *));
      ARRAY[Size]=ToAdd;
   }
   return ARRAY;
}

char *List2Str(struct List_Str *List){ /* home of the imfamous ampersand bug (fixed)!!! */
  int Length=0,Current=0,C_Length=0;
  char *R=NULL; 
  struct List_Str *Root=List; 
  while(List){
    Length+=StrLen(List->Str)+1;
    List=List->Next;
  } 
  List=Root; R=(char *)malloc(Length); 
  while(List){
    C_Length=StrLen(List->Str);
    memcpy(R+Current,List->Str,C_Length);
    Current+=C_Length+1;
    R[Current-1]=' ';
    List=List->Next;
  }
  if(!Length)
    R=0;
  else
    R[Length-1]=0;        
  return R;
}

struct List_Str *Str2List(char *S){
  int _,L=StrLen(S),Start=-1,Finish,P=0; 
  char *Word=NULL; 
  struct List_Str *R=NULL;
  for(_=0;_<L;_++){ 
    if((S[_]==' ')||(S[_]=='\n')||(S[_]=='\t')||(S[_]=='\r')){ 
      Finish=_; 
    if(Start>-1){
      Word=(char *)malloc(Finish-Start+1);
      memcpy(Word,S+Start,Finish-Start);
      Word[Finish-Start]=0; 
      R=Ins_List_Element(R,Word,P++);
      Start=-1;
    }
    } 
    else{
      if(_==L-1){
	Finish=_; 
	if(Start>-1){
	  Word=(char *)malloc(Finish-Start+2);
	  memcpy(Word,S+Start,Finish-Start+1);
	  Word[Finish-Start+1]=0;
	  R=Ins_List_Element(R,Word,P++);
	  Start=-1;
	}
      } 
      if(Start==-1)
	Start=_;
    }
  } 
  if(Start>-1){
    if(_==Start+1){
      if(Start-1<0){
	Word=(char *)malloc(2);
	memcpy(Word,S+Start,1);
	Word[1]=0;
	R=Ins_List_Element(R,Word,P++);
      } 
      else{ 
	if(S[Start-1]==' '){
	  Word=(char *)malloc(2);
	  memcpy(Word,S+Start,1);
	  Word[1]=0;
	  R=Ins_List_Element(R,Word,P++);
	}
      }
    }
  } 
  return R;
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
