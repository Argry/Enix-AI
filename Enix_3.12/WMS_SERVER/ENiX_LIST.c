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

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "ENiX_CALC.h"
#include "ENiX_LIST.h"
#include "ENiX_WMS.h"
#include "ENiX_STRING.h"
#include "ENiX_Globals.h"

char             *ConversationSource;
struct Datablock *Database[2];

struct List_Str *ANSWER;

/*! \brief Prints out the list "LS" ending in a newline.
 *
 */
void PrintList(struct List_Str *LS){
   int i=0,Size=0,IsNum=0,DP=0;
   char *Str=NULL;
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

/*! \brief Frees the list "L".
 *
 */
void FreeList(struct List_Str *L){
  struct List_Str *Old=NULL;
  while(L){
    free(L->Str);
    Old=L;
    L=L->Next;
    free(Old);
  }
}

/*! \brief Prints out a formatted list "Prefix" first, finally "Suffix", FinalSep" is the final separator normally and, "Separator" is the separator normally comma and "LS" is the list to format.
 *
 */
void PrintListSpecial(char *Prefix,struct List_Str *LS,char *Separator,char *FinalSep,char *Suffix){
  struct List_Str *Alpha=NULL;
  if(LS){
    printf("%s",Prefix);
    for(Alpha=LS;Alpha;Alpha=Alpha->Next){
      printf("%s",Alpha->Str);
      if(Alpha->Next){
	if(Alpha->Next->Next)
	  printf("%s",Separator);
	else
	  printf("%s",FinalSep);
      }
    }
    printf("%s",Suffix);
  }
}

/*! \brief Returns a copy of the list "LS".
 *
 */
struct List_Str *Cpy_List(struct List_Str *LS){
   int _=0;
   struct List_Str *R=NULL;
   while(LS){
      R=Ins_List_Element(R,StrCat("",LS->Str),_++);
      LS=LS->Next;
   }
   return R;
}

/*! \brief Returns a copy of the list "LS".
 *
 */
struct List_Str *Ins_List_Element(struct List_Str *LS,char *S,int P){
  int _=0;
  struct List_Str *Prev=NULL,*R=NULL;
  if(P&&LS){
    R=LS;
    for(;(LS)&&(_!=P);_++){
      Prev=LS;
      LS=LS->Next;
    }
    if(_<P+1){
      Prev->Next=(struct List_Str *)malloc(sizeof(struct List_Str));
      Prev=Prev->Next;
      Prev->Str=S;
      Prev->Next=LS;
    }
  }
  else{
    R=(struct List_Str *)malloc(sizeof(struct List_Str));
    R->Str=S;
    R->Next=LS;
  }
  return R;
}

/*! \brief Appends a string "Append" to list "List".
 *
 */
struct List_Str  *Append_List_Element(struct List_Str *List,char *Append){
  struct List_Str *Buffer=NULL;
  if(List){
    for(Buffer=List;Buffer->Next;Buffer=Buffer->Next){}
    Buffer->Next=(struct List_Str *)malloc(sizeof(struct List_Str));
    Buffer=Buffer->Next;
  }
  else
    List=Buffer=(struct List_Str *)malloc(sizeof(struct List_Str));
  Buffer->Next=NULL;
  Buffer->Str=Append;
  return List;
}

/*! \brief Returns true if the numbers in "Examples" are same as or are ? in the list "Sequence".
 *
 */
int Same_Examples(struct List_Str *Examples,struct List_Str *Sequence){
  int Same=1;
  struct List_Str *Buffer1=NULL,*Buffer2=NULL;
  for(Buffer1=Examples,Buffer2=Sequence;Buffer1&&Buffer2;Buffer1=Buffer1->Next,Buffer2=Buffer2->Next)
    if(strcmp(Buffer1->Str,"?"))
      if(atoi(Buffer1->Str)!=atoi(Buffer2->Str))
	Same=0;
  return Same;
}

/*! \brief Inserts a list "L2" before position "P", in list "L1".
 *
 */
struct List_Str *Ins_List_List(struct List_Str *L1,struct List_Str *L2,int P){
  int _=0,C=0;
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

/*! \brief Deletes the element "P" of list "LS".
 *
 */
struct List_Str *Del_List_Element(struct List_Str *LS,int P){
  int _=0;
  struct List_Str *Prev=0,*R=LS;
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

/*! \brief Returns the element "P" of list "LS".
 *
 */
char *Get_List_Element(struct List_Str *LS,int P){ 
  int _=0;
  char *R=NULL;
  for(;(LS)&&(_!=P);_++)
    LS=LS->Next;
  if((_==P)&&(LS))
    R=LS->Str;
  return R;
}

/*! \brief Replaces the element at position "P" of list "LS" with string "Data".
 *
 */
struct List_Str  *Rep_List_Element(struct List_Str *LS,char *Data,int P){
   int _=0;
   struct List_Str *R=LS;
   for(;LS;LS=LS->Next)
     if(P==_++)
       LS->Str=Data;
   return R;
}

/*! \brief Returns the position of the element "S" within list "LS".
 *
 */
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

/*! \brief Returns the size of list "LS".
 *
 */
int Size_of_List(struct List_Str *LS){
   int Size=0;
   for(Size=0;LS;LS=LS->Next)
     Size++; 
   return Size;
}

/*! \brief Returns the list "LS" in lower case.
 *
 */
struct List_Str *LowerCaseList(struct List_Str *LS){
  int i=0;
  struct List_Str *CB=LS;
  while(CB){
    for(i=0;CB->Str[i];i++)
      CB->Str[i]=tolower(CB->Str[i]);
    CB=CB->Next;
  }
  return CB;
}

/*! \brief Returns a bubble sorted list "LS".
 *
 */
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

/*! \brief Returns a formatted list. "First" first, finally "Final". "Conj" is the final separator (normally and), "Punct" is the separator (normally comma) and "LS" is the list to format.
 *
 */  
struct List_Str *FormatList(struct List_Str *LS,char *First,char *Punct,char *Conj,char *Final){
  int Position=0;
  struct List_Str *Alpha=NULL,*Beta=NULL;
  if(LS){
    Beta=Ins_List_Element(Beta,First,Position++);  
    for(Alpha=LS;Alpha;Alpha=Alpha->Next){
      Beta=Ins_List_Element(Beta,Alpha->Str,Position++);
      if(Alpha->Next){
	if(Alpha->Next->Next)
	  Beta=Ins_List_Element(Beta,Punct,Position++);
	else
	  Beta=Ins_List_Element(Beta,Conj,Position++);
      }
    }
    Beta=Ins_List_Element(Beta,Final,Position++);
    return Beta;
  }
  return LS;
}

/*! \brief Remove duplicate list elements in list "LS".
 *
 */
struct List_Str *RemoveDups(struct List_Str *LS){
   int C=0;
   struct List_Str *Final=NULL;
   for(;LS;LS=LS->Next){
      if(Find_List_Element(Final,LS->Str)==-1)
	Final=Ins_List_Element(Final,LS->Str,C++);
   }
   return Final;
}

/*! \brief Remote unnecessary trailing zeros etc in list "LS".
 *
 */
struct List_Str  *AutoFormat(struct List_Str *LS){
  struct List_Str *R=NULL;
  int _=0,i=0,Size=0,IsNum=0,DP=0;
  char *Str=NULL;
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

/*! \brief Add list "ToAdd" to array of lists "ARRAY" of size "Size".
 *
 */
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

/*! \brief Converts list "List" into a string.
 *
 */
char *List2Str(struct List_Str *List){
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

/*! \brief Converts string "S" into a list.
 *
 */
struct List_Str *Str2List(char *S){
  int _=0,L=StrLen(S),Start=-1,Finish,P=0; 
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

/*! \brief Returns a list of integers between Start and Finish in "Word_List".
 *
 */
struct List_Str *Domain(struct List_Str *Word_List,struct List_Str *L){
   float Start=0,Finish=0,_=0;
   Start=atof(Word_List->Str);
   ANSWER=NULL;
   if(Word_List->Next){
      Finish=atof((Word_List=Word_List->Next)->Str)+1;
      for(_=Start;_<Finish;_++)
	ANSWER=Ins_List_Element(ANSWER,FloatToString(_),_-Start);
   }
   else{
      puts(":DOMAIN requires two integer inputs!");
      ANSWER=NULL;
   }
   return L;
}

/*! \brief Displays an array of lists "Lists" of size "Size".
 *
 */
void PrintListArray(struct List_Str **Lists,int Size){ 
  int _=0;
  if(Lists){
    for(_=0;_<Size;_++)
      PrintListSpecial("[",Lists[_],"][","][","]\n");
  }
}

/*! \brief Returns the shortest element of list "L".
 *
 */
char *ShortestElement(struct List_Str *L){
  int Len=0,This=0;
  char *Str=NULL;
  struct List_Str *Buffer=NULL;
  for(Buffer=L;Buffer;Buffer=Buffer->Next){
    This=strlen(Buffer->Str);
    if((!Len)||(This<Len)){
      Str=Buffer->Str;
      Len=This;
    }
  }
  return Str;
}

/*! \brief Return true if string "S" is an element of list "LS".
 *
 */
int Is_List_Element(struct List_Str *LS,char *S){
  if(S)
    for(;LS;LS=LS->Next)
      if(!strcmp(LS->Str,S))
	return 1;
  return 0;
}

/*! \brief Returns the size of the list "WordList".
 *
 */
struct List_Str *Size(struct List_Str *WordList,struct List_Str *L){
  ANSWER=Ins_List_Element(NULL,FloatToString((float)Size_of_List(WordList)),0);
  return L;
}

/*! \brief Removes spaces between the list elements "WordList".
 *
 */
struct List_Str *Merge(struct List_Str *WordList,struct List_Str *L){
   int _=0,a=0,b=0;
   char *Word=NULL;
   struct List_Str *Buffered=WordList,*Alpha=NULL;
   ANSWER=NULL;
   if(Buffered){
      Alpha=Buffered;
      while(Buffered){
	 _+=StrLen(Buffered->Str);
	 Buffered=Buffered->Next;
      }
      Word=(char *)malloc(_+2);
      Buffered=Alpha;
      while(Buffered){
	 b=StrLen(Buffered->Str);
	 memcpy(Word+a,Buffered->Str,b);
	 a+=b;
	 Buffered=Buffered->Next;
      }
      Word[_]=0;
      ANSWER=Ins_List_Element(ANSWER,Word,0);
   }
   return L;
}

/*! \brief Reverse the list elements in list "WordList".
 *
 */
struct List_Str *Reverse(struct List_Str *WordList,struct List_Str *L){
   struct List_Str *Buffered=WordList;
   ANSWER=NULL;
   if(Buffered){
      while(Buffered){
	 ANSWER=Ins_List_Element(ANSWER,Buffered->Str,0);
	 Buffered=Buffered->Next;
      }
   }
   return L;
}

/*! \brief Separate out the strings in list "Word_List" into a list of characters.
 *
 */
struct List_Str *Split(struct List_Str *Word_List,struct List_Str *L){
   int _=0,C=0;
   char *String=NULL,Character=0;
   struct List_Str *Buffer=Word_List,*Alpha=NULL,*Beta=NULL;
   Alpha=Buffer;
   while(Alpha){
      _=0;
      while((Character=Alpha->Str[_])){
	 if(Character!=' '){
	    String=(char *)malloc(2);
	    String[0]=Character;
	    String[1]=0;
	    Beta=Ins_List_Element(Beta,String,C);
	    C++;
	 }
	 _++;
      }
      Alpha=Alpha->Next;
   }
   ANSWER=Beta;
   return L;
}

/*! \brief Return a list "List" with the string "Prefix" removed from the front of each element.
 *
 */
struct List_Str  *ListStripPrefix(struct List_Str *List,char *Prefix){
  int _=0,SizeofPrefix=strlen(Prefix);
  struct List_Str *R=NULL;
  for(;List;List=List->Next)
    if(strncmp(List->Str,Prefix,SizeofPrefix))
      R=Ins_List_Element(R,List->Str,_++);
    else
      R=Ins_List_Element(R,List->Str+SizeofPrefix,_++);
  return R;
}

/*! \brief Store the last element of the list in ANSWER.
 *
 */
struct List_Str *Last(struct List_Str *Word_List,struct List_Str *L){
   int Start=0,_=0,Max=Size_of_List(Word_List);
   Start=_=Max-(atoi(List2Nums(Str2List(L->Str))->Str));
   if(_<0)
     Start=_=0;
   for(ANSWER=NULL;(_<Max);_++)
      ANSWER=Ins_List_Element(ANSWER,Get_List_Element(Word_List,_),_-Start);
   return L->Next;
}

/*! \brief Store the first element of the list in ANSWER.
 *
 */
struct List_Str *First(struct List_Str *Word_List,struct List_Str *L){
   int _=0;
   double Max=atof(List2Nums(Str2List(L->Str))->Str);
   for(ANSWER=NULL;(_<Max)&&(Word_List);_++){
      ANSWER=Ins_List_Element(ANSWER,Word_List->Str,_);
      Word_List=Word_List->Next;
   }
   return L->Next;
}

/*! \brief Convert a list "LS" into a list of dereferenced numbers if not already.
 *
 */
struct List_Str *List2Nums(struct List_Str *LS){
   int _=0;
   struct List_Str *R=NULL;
   for(;LS;LS=LS->Next){
     if(isdigit(LS->Str[0])||(LS->Str[0]=='-')||(LS->Str[0]=='.'))
       R=Ins_List_Element(R,LS->Str,_++);
     else
       R=Ins_List_Element(R,Get_DB_Entry(Goto_DB_Entry(Database[1],LS->Str),0,7),_++);
   }
   return R;
}

/*! \brief Marks an unknown variable in a sequence.
 *
 */
struct List_Str  *Unknown(struct List_Str *Word_List,struct List_Str *L){
   int Position=Size_of_List(Word_List);
   ANSWER=Ins_List_Element(Word_List,":unknown",Position++);
   for(;L;L=L->Next){
      if((L->Str[0]==':')&&(strcmp(L->Str,":unknown")))
	return L;
      ANSWER=Ins_List_Element(Word_List,L->Str,Position++);
   }
   return L;
}

/*! \brief Stores the name of the entity talking to ENiX as "SourceName".
 *
 */
struct List_Str  *ExtractConvSource(struct List_Str *L){
  char *SourceName="PERSON";
  if(L){
    if((L->Str[0]=='<')&&(L->Str[strlen(L->Str)-1])=='>'){
      SourceName=(char *)malloc(strlen(L->Str)*sizeof(char *));
      memcpy(SourceName,L->Str+1,strlen(L->Str)-2);
      SourceName[strlen(L->Str)-1]=0;
      L=L->Next;
    }
  }
  ConversationSource=SourceName;
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
