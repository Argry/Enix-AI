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


Programmed By M.S.Braithwaite 2007 - 2012.

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "ENiX_Globals.h"
#include "ENiX_LIST.h"
#include "ENiX_WMS.h"
#include "ENiX_STRING.h"
#include "ENiX_DISK.h"

char             *Filename;

/*! \brief Return the contents of a filehandle "INPUT".
 *
 */
char *LoadFile(FILE *INPUT){
  int _=0;
  char *Contents=NULL;
  fseek(INPUT,0,SEEK_END);
  _=ftell(INPUT);
  rewind(INPUT);
  Contents=(char *)malloc(_+1); 
  if(fread(Contents,_,1,INPUT)){}
  Contents[_]=0; 
  return Contents;
}

/*! \brief Return the contents of a pipehandle "PIPE".
 *
 */
char *LoadPipe(FILE *PIPE){
   int Offset=0,_=0;
   char *Buffer=(char *)malloc(10*sizeof(char *)),*Contents=(char *)malloc(10*sizeof(char *));                  
   memset(Buffer,0,10);
   memset(Contents,0,10);
   while(fgets(Buffer,9,PIPE)!=NULL){
      memcpy(Contents+Offset,Buffer,_=StrLen(Buffer));
      Contents[Offset+_]=0;
      Offset+=_;
      Contents=(char *)realloc(Contents,(Offset+10)*sizeof(char *));
   }
   if(Contents[0]){
      _=StrLen(Contents);
      Contents[_-1]=0;
   }
   return Contents;
}

/*! \brief Write the database to disk as a personality profile.
 *
 */
struct List_Str *Save(struct List_Str *Word_List,struct List_Str *L){
  int C=0;
  char *Memory=NULL,*D1=NULL,*Old=NULL,*New=NULL,*CMD=NULL;
  struct Datablock *Q=NULL;
  struct List_Str *Alpha=NULL;
  D1=Word_List->Str;
  Q=Goto_DB_Entry(Database[1],":mode");
  Alpha=Q->DS[2]; C=0;
  while(Alpha){
    Alpha=Alpha->Next;
    C++;
  }
  if(C>6){
    if(strcmp(Get_List_Element(Q->DS[2],6),D1)){
      Q->DS[1]=Ins_List_Element(Q->DS[1],":personality",6);
      Q->DS[2]=Ins_List_Element(Q->DS[2],D1,6);
    }
  }
  else{
    Q->DS[1]=Ins_List_Element(Q->DS[1],":personality",6);
    Q->DS[2]=Ins_List_Element(Q->DS[2],D1,6);
  }
  Memory=Disk_Buffer(Database);
  Str2File(Filename,Memory);
  Old=Filename;
  New=StrCat(StrCat(StrCat(getenv("HOME"),"/.ENiX/"),D1),".MEM");
  printf("Saving personality profile as %s...\n",D1); 
  CMD=StrCat(StrCat(StrCat("cp ",Old)," "),New);
  if(system(StrCat("rm -f ",New))){}
  if(system(CMD)){}
  exit(0);
}

/*! \brief Lists the personality profiles that can be loaded into ENiX.
 *
 */
struct List_Str *ShowProf(struct List_Str *WordList,struct List_Str *L){
  int _=0,Length=0,Total=StrLen(Filename);
  char *COMMAND=NULL,*Buffer=StrCat("",Filename);
  for(_=0;_<Total;_++)
    if(Filename[_]=='/')
      Length=_;
  Buffer[++Length]=0;
  COMMAND=(char *)malloc((Length=Length+34)*sizeof(char));
  snprintf(COMMAND,Length,"ls %s | grep .MEM | sed \"s/.MEM//g\"",Buffer);
  if(system(COMMAND)){}
  return L;
}

/*! \brief Creates a string to be saved to disk of the database concepts.
 *
 */
char *Disk_Buffer(struct Datablock *Q[2]){
  char *R=NULL;
  int A=0,B=0,New_Size=1,Append_Size=0;
  struct Datablock *BF[2];
  struct List_Str *LS=NULL;
  BF[0]=Q[0];
  BF[1]=Q[1];
  R=(char *)malloc(2);
  R[0]='|';
  R[1]=0; 
  if(BF[0]||BF[1]){
    New_Size=1; 
    for(A=0;A<2;A++){ 
      for(;BF[A];BF[A]=BF[A]->Next){
	if(A){
	  R=SuperStrCat(R,"D|",New_Size,2);
	  New_Size+=2;
	}
	else{
	  R=SuperStrCat(R,"R|",New_Size,2);
	  New_Size+=2;
	}
	for(B=0;B<3;B++){
	  if(BF[A]->DS[B]){
	    for(LS=BF[A]->DS[B];LS;LS=LS->Next){
	      Append_Size=StrLen(LS->Str);
	      R=SuperStrCat(R,LS->Str,New_Size,Append_Size);
	      New_Size+=Append_Size;
	      if(LS->Next){
		R=SuperStrCat(R,"$",New_Size,1);
		New_Size++;
	      }
	    }
	    R=SuperStrCat(R,"|",New_Size,1);
	    New_Size++;
	  }
	}
      }
    } 
  }
  else
    R=SuperStrCat(R,"|",New_Size,1);
  return R;
}

/*! \brief Populate the database contents from a string loaded from an ENiX memory file.
 *
 */
void Virtualise(char *S){
  int Word_Start=0,Reserved=0,i=0,Side=1,Segment=0,Position=0,Size=0;
  char *D=NULL,*Word=NULL,*Buffer=(char *)malloc((Size=StrLen(S)+1));
  struct List_Str *List[3];
  struct Datablock *DB[2];
  List[0]=List[1]=List[2]=NULL;
  DB_Size=RB_Size=0;
  Database[0]=Database[1]=NULL;
  DB[0]=DB[1]=NULL;
  memcpy(Buffer,S,Size);
  Buffer[Size-1]=0;
  while(S[i]){
    D=S+i;
    if((!strncmp("R|",D,2))||(!strncmp("D|",D,2))||(D[0]=='|')||(D[0]=='@')||(D[0]=='$')){
      Buffer[i]=0;
      if(Reserved==1)
	List[Segment]=Ins_List_Element(List[Segment],Word,Position);
      Reserved=0;
      if((D[0]=='R')||(D[0]=='D')){ 
	if(List[0]){
	  DB[Side]=(struct Datablock *)malloc(sizeof(struct Datablock));
	  DB[Side]->DS[0]=List[0];
	  DB[Side]->DS[1]=List[1];
	  DB[Side]->DS[2]=List[2];
	  DB[Side]->Next=Database[Side];
	  Database[Side]=DB[Side];
	  if(Side)
	    DB_Size++;
	  else
	    RB_Size++;
	}
	List[0]=List[1]=List[2]=NULL;
	Side=(D[0]=='R')?0:1;
	Segment=-1;
	Position=0;  
      }
      if(D[0]=='|'){
	Segment++;
	Position=0;
      }
      if(D[0]=='$')
	Position++;
    }
    else{
      if(Reserved==0){
	Reserved=1;
	Word_Start=i;
	Word=Buffer+Word_Start;
      }
    }
    i++;
  }
  if(List[0]){
    DB[Side]=(struct Datablock *)malloc(sizeof(struct Datablock));
    DB[Side]->DS[0]=List[0];
    DB[Side]->DS[1]=List[1];
    DB[Side]->DS[2]=List[2];
    DB[Side]->Next=Database[Side];
    Database[Side]=DB[Side];
    if(Side)
      DB_Size++;
    else
      RB_Size++;
  }
}

/*! \brief Loads concepts and dependencies from one personality profile into the current.
 *
 */
struct List_Str *Meld(struct List_Str *Word_List,struct List_Str *L){
  FILE *DiskTarget=NULL;
  int C=0,Number=0;
  char *Concept_SRC=NULL,*Cached=NULL,*T=NULL;
  struct List_Str *Concepts=NULL,*Buffer=NULL,*Translated=NULL;
  struct Datablock *Backup1[2],*DBBackup=NULL,*To=NULL,*From=NULL;
  Concept_SRC=StrCat(getenv("HOME"),StrCat("/.ENiX/",StrCat(L->Str,".MEM")));
  L=L->Next;
  Concepts=Word_List;
  if(!(DiskTarget=fopen(Concept_SRC,"r"))){
    printf("Sorry, I cannot open: %s\n",Concept_SRC);
    return L;
  }
  else{
    Cached=LoadFile(DiskTarget);
    fclose(DiskTarget);
    Backup1[0]=Database[0];
    Backup1[1]=Database[1];
    Virtualise(Cached);
    Database[1]=MemoryMode(Database[1]);
  }
  Buffer=Concepts;
  while(Buffer){
    Buffer=Buffer->Next;
    Number++;
  }
  Buffer=Concepts;
  while(Buffer){
    Translated=NULL;
    if((DBBackup=Goto_DB_Entry(Database[1],Buffer->Str))){
      if(strcmp(T=Get_DB_Entry(DBBackup,0,7),"?")){
	Translated=Str2List(T);
	while(Translated){
	  if((Translated->Str[0]!=':')&&(!isdigit(Translated->Str[0]))&&(!Is_List_Element(Concepts,Translated->Str)))
	    Concepts=Ins_List_Element(Concepts,Translated->Str,Number++); 
	  Translated=Translated->Next;
	}
      }
    }
    Buffer=Buffer->Next;
  }
  Buffer=Concepts; 
  puts("=[:MELD]========================================[Overwrite][Copy]");
  while(Buffer){
    printf("%-48s",Buffer->Str);
    To=Goto_DB_Entry(Backup1[1],Buffer->Str);
    From=Goto_DB_Entry(Database[1],Buffer->Str);
    if(From){
      if(To)
	printf(" ......Yes ");
      else{
	printf(" .......No ");
	To=Add_DB_Entry(Backup1[1],Buffer->Str);
	Backup1[1]=To;
	To=Goto_DB_Entry(Backup1[1],Buffer->Str);
      }
      puts(" .Yes");
      for(C=0;C<2;C++)
	To->DS[C]=From->DS[C];
    }
    else{
      if(To)
	printf(" ......Yes ");
      else
	printf(" .......No ");
      puts(" ..No");
    }
    Buffer=Buffer->Next;
  }
  Database[0]=Backup1[0];
  Database[1]=Backup1[1];
  puts("==========================================================[Done]=");
  return L;
}

/*! \brief Abandons the running personality profile and replaces it with a user specified one.
 *
 */
struct List_Str *Load(struct List_Str *Word_List,struct List_Str *L){
  char *Data=NULL,*Old=NULL,*New=NULL,*CMD=NULL;
  Data=Word_List->Str;
  New=Filename;
  Old=StrCat(StrCat(StrCat(getenv("HOME"),"/.ENiX/"),Data),".MEM");
  printf("Loading personality profile: %s...\n",Data);
  CMD=StrCat(StrCat(StrCat("cp ",Old)," "),New);
  if(system(StrCat("rm -f ",New))){}
  if(system(CMD)){}
  puts("Program killed by :load.");
  exit(0);
  return L;
}

/*! \brief Writes a string "Data" to disk as file "FileName".
 *
 */
int Str2File(char *FileName,char *Data){
  FILE *Disk=NULL;
  if((Disk=fopen(FileName,"w"))==NULL){
    printf("Unable to write to file %s\n",FileName);
    return 0;
  }
  else{
    fprintf(Disk,"%s",Data); 
    fclose(Disk); 
    return 1;
  }
}

/*! \brief Reads a file called "FineName" to a string.
 *
 */
char *File2Str(char *FileName){
  char *R=NULL;
  FILE *Disk=NULL;
  if((Disk=fopen(FileName,"r")))
    R=LoadFile(Disk);
  return R;
}

/*! \brief Loads a configuration attribute from the memory file.
 *
 */
char *LoadConfigAttr(char *Attribute,char *FileData){
  char *Start=NULL,*Finish=NULL,*NewSearch=NULL,*R="0";
  if((NewSearch=strstr(FileData,Attribute))){
    if((Start=strstr(NewSearch,"="))){
      Start=Start+1;
      Finish=strstr(Start,"\n");
      R=(char *)malloc((Finish-Start+1)*sizeof(char));
      memcpy(R,Start,Finish-Start);
      R[Finish-Start]=0;
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
