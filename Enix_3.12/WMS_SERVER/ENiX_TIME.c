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

#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ENiX_TIME.h"
#include "ENiX_WMS.h"
#include "ENiX_LIST.h"
#include "ENiX_Globals.h"
#include "ENiX_NLP.h"

/*! \brief Convert seconds to string.
 *
 */
char *Seconds2Str(){
  char *R=(char *)malloc(12*sizeof(char));
  unsigned long int Seconds=0;
  time_t Time=0;
  time(&Time);
  Seconds=(unsigned long int)Time;
  snprintf(R,11,"%ld",Seconds);
  R[11]=0;
  return R;
}

/*! \brief Format the time into a string.
 *
 */
char *FormatTime(char *Seconds){
  char *R=(char *)malloc(20*sizeof(char));
  time_t Time=(time_t)atol(Seconds);
  struct tm *TimeInfo=NULL;
  TimeInfo=localtime(&Time); 
  strftime(R,20,"%H:%M:%S-%Y/%m/%d",TimeInfo);
  return R;
}

/*! \brief Get schedular data record.
 *
 */
struct Datablock *GetScheduleRec(){
  struct Datablock *DB=NULL;
  if(!(DB=Goto_DB_Entry(Database[1],":schedule")))
    DB=Database[1]=Add_DB_Entry(Database[1],":schedule");
  return DB;
}

/*! \brief Create a new schedule.
 *
 */
struct List_Str *SchedNew(struct List_Str *Word_List,struct List_Str *L){
  unsigned long int TimeNow=0;
  char *NextActiveTime=(char *)malloc(12*sizeof(char));
  struct List_Str *Q=NULL,*Input=Word_List;
  struct Datablock *DB=GetScheduleRec();  
  if(Size_of_List(Input)==3){
    /* 
       1: Name
       2: Source
       3: Next Active (timenow + delay)
       4: Periodicity
    */
    TimeNow=(unsigned long int)time(0);
    TimeNow+=atol(Get_List_Element(Input,1));
    snprintf(NextActiveTime,11,"%ld",TimeNow);
    NextActiveTime[11]=0;
    Input=Rep_List_Element(Input,NextActiveTime,1);
    Q=Ins_List_Element(Word_List,ConversationSource,1);
    Add_DB_Pair(DB,List2Str(Q),List2Str(L));
  }
  else
    printf("ERROR :SCHEDNEW requires 3 parameters. %s:%d\n",__FILE__,__LINE__);
  return NULL;
}

/*! \brief Delete a schedule.
 *
 */
struct List_Str *SchedDel(struct List_Str *Word_List,struct List_Str *L){
  int Position=0;
  char *Name=NULL;
  struct List_Str *Q=NULL,*Buffer=NULL;
  struct Datablock *DB=GetScheduleRec();
  Name=Word_List->Str;
  for(Q=DB->DS[1];Q;){
    Buffer=Str2List(Q->Str);
    if(!strcmp(Name,Buffer->Str)){
      DB->DS[1]=Del_List_Element(DB->DS[1],Position);
      DB->DS[2]=Del_List_Element(DB->DS[2],Position);
      Q=DB->DS[1];
      PrintList(Q);
      Position=0;
    }
    else{
      Q=Q->Next;
      Position++;
    }
  }
  return L;
}

/*! \brief Wait a specified period of seconds.
 *
 */
struct List_Str *Wait(struct List_Str *Word_List,struct List_Str *L){
  unsigned long int TimeStart=0,TimeNow=0,TimeDelta=atol(Word_List->Str);
  for(TimeStart=(unsigned long int)time(0);TimeNow<TimeStart+TimeDelta;TimeNow=(unsigned long int)time(0))
    usleep(500);
  return L;
}

/*! \brief List the records created after "Word_List" seconds ago.
 *
 */
struct List_Str *FirstAfter(struct List_Str *Word_List,struct List_Str *L){
  unsigned long int TimeThen=0,TimeDelta=0,TimeNow=(unsigned long int)time(0);
  struct List_Str *Filtered=NULL,*Buffer=NULL;
  struct Datablock *DB=NULL;
  TimeDelta=atol(L->Str);
  for(Buffer=Word_List;Buffer;Buffer=Buffer->Next){
    if((DB=Goto_DB_Entry(Database[1],Buffer->Str))){
      TimeThen=atol(Get_DB_Entry(DB,0,4));
      if(TimeThen>(TimeNow-TimeDelta))
	Filtered=Append_List_Element(Filtered,Buffer->Str);
    }
  }
  if(!Filtered)
    Filtered=Str2List(":NULL");
  ANSWER=Filtered;
  return L->Next;
}

/*! \brief List the records created before "Word_List" seconds ago.
 *
 */
struct List_Str *FirstBefore(struct List_Str *Word_List,struct List_Str *L){
  unsigned long int TimeThen=0,TimeDelta=0,TimeNow=(unsigned long int)time(0);
  struct List_Str *Filtered=NULL,*Buffer=NULL;
  struct Datablock *DB=NULL;
  TimeDelta=atol(L->Str);
  for(Buffer=Word_List;Buffer;Buffer=Buffer->Next){
    if((DB=Goto_DB_Entry(Database[1],Buffer->Str))){
      TimeThen=atol(Get_DB_Entry(DB,0,4));
      if(TimeThen<(TimeNow-TimeDelta))
	Filtered=Append_List_Element(Filtered,Buffer->Str);
    }
  }
  if(!Filtered)
    Filtered=Str2List(":NULL");
  ANSWER=Filtered;
  return L->Next;
}

/*! \brief List the records last modified after "Word_List" seconds ago.
 *
 */
struct List_Str *LastAfter(struct List_Str *Word_List,struct List_Str *L){
  unsigned long int TimeThen=0,TimeDelta=0,TimeNow=(unsigned long int)time(0);
  struct List_Str *Filtered=NULL,*Buffer=NULL;
  struct Datablock *DB=NULL;
  TimeDelta=atol(L->Str);
  for(Buffer=Word_List;Buffer;Buffer=Buffer->Next){
    if((DB=Goto_DB_Entry(Database[1],Buffer->Str))){
      TimeThen=atol(Get_DB_Entry(DB,0,5));
      if(TimeThen>(TimeNow-TimeDelta))
	Filtered=Append_List_Element(Filtered,Buffer->Str);
    }
  }
  if(!Filtered)
    Filtered=Str2List(":NULL");
  ANSWER=Filtered;
  return L->Next;
}

/*! \brief List the records last modified before "Word_List" seconds ago.
 *
 */
struct List_Str *LastBefore(struct List_Str *Word_List,struct List_Str *L){
  unsigned long int TimeThen=0,TimeDelta=0,TimeNow=(unsigned long int)time(0);
  struct List_Str *Filtered=NULL,*Buffer=NULL;
  struct Datablock *DB=NULL;
  TimeDelta=atol(L->Str);
  for(Buffer=Word_List;Buffer;Buffer=Buffer->Next){
    if((DB=Goto_DB_Entry(Database[1],Buffer->Str))){
      TimeThen=atol(Get_DB_Entry(DB,0,5));
      if(TimeThen<(TimeNow-TimeDelta))
	Filtered=Append_List_Element(Filtered,Buffer->Str);
    }
  }
  if(!Filtered)
    Filtered=Str2List(":NULL");
  ANSWER=Filtered;
  return L->Next;
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
