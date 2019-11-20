/*! \file */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <signal.h>
#include <dirent.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "ENiX_STRING.h"
#include "WitchCraft-Net.h"
#include "WMS_Blob.h"

#include "WitchCraft.h"
#include "WitchCraft-Colours.h"
#include "ENiX_DISK.h"

/*
  tput stty to get terminal size ?
 */

/* Global Variates **********************************************************/

int                  ScreenWidth=80;
int                  ScreenHeight=24;
char                 *HorzLine=NULL;
char                 *CommandPrefix="./ENiX3 ";
int                  QUIT=0;
int                  VSynth=0;
int                  Speak=0;
int                  Segfaults=0;
int                  HighSpeedRefresh=0;
int                  CygwinMode=0;

/* Command and Answer History ***********************************************/

struct Buffer        *Session=NULL;
struct Buffer        *SeshFirst=NULL;
struct Buffer        *SeshLast=NULL;

/* Formatted Screen Data ****************************************************/

struct FormattedLine *ScrSession=NULL;
struct FormattedLine *ScrFirst=NULL;
struct FormattedLine *ScrLast=NULL;

/* Function Prototypes ******************************************************/

int                  IsOSShell=0;

/* Formatting Data **********************************************************/

int                  NumColours=17;
int                  Colour1[]={0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0};
int                  Colour2[]={30,31,32,33,34,35,36,37,30,31,32,33,34,35,36,37,4,5};

int                  SLOWREPLAY=0;
char                 *PERSONFORMAT=NULL;
char                 *SERVERFORMAT=NULL;
char                 *SYSTEMFORMAT=NULL;
char                 *ENiXFORMAT=NULL;
char                 *AlignRight=NULL;
char                 *AlignLeft=NULL;
char                 *BlankLine=NULL;

/* TCP IP *******************************************************************/

int                  ServerMode=0;
int                  ClientMode=0;

/*
struct sockaddr_in   echoClntAddr;
*/

int                  ServerBindId;
int                  CtrlCTrap=0;
char                 *IPAddress=NULL;
int                  Socket2Close;

/* Routine Declarations *****************************************************/

/*! \brief Save configuration file to disk.
 *
 */
void SaveConfig(){
  char Config[4000];
  sprintf(Config,"# ==============================================\n\
# WitchCraft Configuration File.\n\
# There should be no spaces or tabs in this file\n\
# (except after CommandPrefix's value).\n\
# ==============================================\n\n\
VSynth=%d\n\
Speak=%d\n\
CommandPrefix=%s\n\
SlowReplay=%d\n\
HighSpeedRefresh=%d\n\
CygwinMode=%d\n\n",VSynth,Speak,CommandPrefix,SLOWREPLAY,HighSpeedRefresh,CygwinMode);
  Str2File(StrCat(getenv("HOME"),"/.WitchCraft.rc"),Config);
}

/*! \brief Load configuration from disk.
 *
 */
void LoadConfig(){
  char *FileContents=NULL;
  FileContents=File2Str(StrCat(getenv("HOME"),"/.WitchCraft.rc"));
  if(FileContents){
    CommandPrefix=LoadConfigAttr("CommandPrefix=",FileContents);
    VSynth=atoi(LoadConfigAttr("VSynth=",FileContents));
    Speak=atoi(LoadConfigAttr("Speak=",FileContents));
    SLOWREPLAY=atoi(LoadConfigAttr("SlowReplay=",FileContents));
    HighSpeedRefresh=atoi(LoadConfigAttr("HighSpeedRefresh=",FileContents));
    CygwinMode=atoi(LoadConfigAttr("CygwinMode=",FileContents));
  }
}

/*! \brief Remove the trailing spaces after a command entry.
 *
 */
char *RemoveFinalSpaces(char *RAW){
  int _=0;
  if(RAW){
    for(_=strlen(RAW)-1;_>0;_--)
      if(RAW[_]==' ')
	RAW[_]=0;
      else
	return RAW;
  }
  return RAW;
}

/*! \brief Clear the witchcraft buffers and history.
 *
 */
void Clear(void){  
  for(;SeshFirst;SeshFirst=SeshFirst->Next){
    if(SeshFirst->Command)
      free(SeshFirst->Command);
    if(SeshFirst->Answer)
      free(SeshFirst->Answer);
    if(SeshFirst->Prev)
      free(SeshFirst->Prev);
  }
  Segfaults=0;
  Session=NULL;     
  SeshFirst=NULL;
  SeshLast=NULL;
  for(;ScrFirst;ScrFirst=ScrFirst->Next){
    if(ScrFirst->Data)
      free(ScrFirst->Data);
    if(ScrFirst->Prev)
      free(ScrFirst->Prev);
  }
  ScrSession=NULL;
  ScrFirst=NULL;
  ScrLast=NULL;  
}

/*! \brief Run unit tests on all tests in a directory.
 *
 */
void WitchCraftTest(char *Directory){
  char *FullPath=NULL,*SegfaultMessage=NULL;
  DIR *Dir=NULL;
  struct dirent *File;
  if((Dir=opendir(Directory))==NULL){
    printf("%s does not exist - aborting!\n",Directory);
    return ;
  }
  while((File=readdir(Dir))!=NULL){
    if(File->d_name[0]!='.'){
      FullPath=(char *)malloc((sizeof(Directory)+sizeof(File->d_name)+2*sizeof(char)));    
      sprintf(FullPath,"%s/%s",Directory,File->d_name);
      Session=Add2Buffer(Session,StrCat("!LOAD ",FullPath),StrCat("",""));
      SeshFirst->Error=2;
      Buffer2Scr(SeshFirst);
      WC_Load(FullPath);
    }
  }
  SegfaultMessage=(char *)malloc(80*sizeof(char));
  sprintf(SegfaultMessage,"%d Segfaults found during testing.",Segfaults);
  ScrSession=Add2Scr(ScrSession,SegfaultMessage,4);
}

/*! \brief Run in server mode.
 *
 */
void RunServer(){
  int servSock=0;
  ServerMode=1;
  ServConsUpdate();
  servSock=OpenServerSocket();
  CLS();
  fflush(stdout);
  for(;;)
    Incoming(servSock);
}

/*! \brief Accept an incoming connection / data.
 *
 */
void Incoming(int sock){
  int clntSock=0;
  clntSock=AcceptServerCon(sock);
  HandleTCPClient(clntSock);  
}

/*! \brief Process tcp clients.
 *
 */
void HandleTCPClient(int Socket){
  char *Data=NULL,*Answer=NULL;
  struct Blob *Received=NULL,*Sent=NULL;
  Received=NetReceive(Socket);
  Data=Received->Nucleus;
  free(Received);
  Execute(Data);
  ServConsUpdate();
  Answer=SeshFirst->Answer;
  Sent=WMS_Bin2Blob(strlen(Answer),Answer);
  NetTransmit(Sent,Socket);
  free(Sent);
  close(Socket);
}

/*! \brief Wrap the text around the window.
 *
 */
char *Wrap(char *Data){
  int _=0,Previous=0,wasspace=0,Delta=0;
  char *R="",*Str=NULL;
  for(;Data[_];_++){
    if(Data[_]==' '){
      if(!wasspace){
	Str=(char *)malloc(((Delta=(_-Previous))+4)*sizeof(char ));
	memcpy(Str+2,Data+Previous,Delta);
	Str[0]=' ';
	Str[1]='\'';
	Str[Delta+2]='\'';
	Str[Delta+3]=0;
	R=StrCat(R,Str); /* need to free this up not done yet */
	if(Str)
	  free(Str);
      }  
      wasspace=1;
    }
    else{
      if(wasspace)
	Previous=_;
      wasspace=0;
    }
  }
  if((_!=Previous)&&(Data[_-1]!=' ')){
    Str=(char *)malloc(((Delta=(_-Previous))+4)*sizeof(char ));
    memcpy(Str+2,Data+Previous,Delta);
    Str[0]=' ';
    Str[1]='\'';
    Str[Delta+2]='\'';
    Str[Delta+3]=0;
    R=StrCat(R,Str); /* need to free this up not done yet */
    if(Str)
      free(Str);
  }
  if(SLOWREPLAY)
    if(system("sleep 0.1")){}
  return R;
}

/*! \brief Show server mode splash screen.
 *
 */
void ServBG(){
  /*
   int C=(ScreenWidth-47)>>1;
  */
   DrawText(ScreenHeight-2,17,9,  "[SERVER:                ]","");
   DrawText(ScreenHeight-2,26,10,  "% 15s",inet_ntoa(ClientAddress.sin_addr));
   fflush(stdout);
}

/*! \brief Show client mode splash screen.
 *
 */
void ClientBG(){
  /*
   int C=(ScreenWidth-44)>>1;
  */
   DrawText(ScreenHeight-2,17,9,  "[CLIENT:                ]","");
   DrawText(ScreenHeight-2,26,10,  "% 15s",IPAddress);
   fflush(stdout);
}

/*! \brief Update the server console.
 *
 */
void ServConsUpdate(){
   DrawText(ScreenHeight-1,(ScreenWidth-30)>>1,12,"- USE ^C TO EXIT SERVER MODE -",NULL);
   DebugBuffer(ScrFirst);
   ServBG();
   DrawText(ScreenHeight-2,10,11," %s","IDLE");   
   fflush(stdout);
}

/*! \brief Generic WitchCraft splash screen.
 *
 */
void Splash(){
  int C=(ScreenWidth-77)>>1;
  DrawText((ScreenHeight>>1)-3,C,10,"_  _  _ _____ _______ _______ _     _ _______  ______ _______ _______ _______","");
  DrawText((ScreenHeight>>1)-2,C, 9,"|  |  |   |      |    |       |_____| |       |_____/ |_____| |______    |   ","");
  DrawText((ScreenHeight>>1)-1,C,13,"|__|__| __|__    |    |_____  |     | |_____  |    \\_ |     | |          |   ","");
  DrawText((ScreenHeight>>1)+1,C, 9,"                       SHELL2 GPL [OvO]wl 2010, ENiX      ","");
}

/*! \brief Execute a command.
 *
 */
void Execute(char *Data){ 
   char *G1=NULL;
   int sock=0;
   char *Returned=NULL;
   struct Blob *Sent=NULL,*Received=NULL;
   add_history(Data);
   if(ClientMode){
     sock=OpenClientSocket(IPAddress);
     Sent=WMS_Bin2Blob(strlen(Data),Data);
     NetTransmit(Sent,sock);
     free(Sent);
     Received=NetReceive(sock);
     Returned=Received->Nucleus;
     Session=Add2Buffer(Session,Data,Returned);
     free(Received);
     close(sock);
   }
   else{
      if(Data[0]=='!'){
	Command(Data);    
      }
      else{
	 if(IsOSShell)
	   Session=Add2Buffer(Session,Data,RUN(G1=StrCat(CommandPrefix,Data)));
	 else
	   Session=Add2Buffer(Session,Data,RUN(G1=StrCat(CommandPrefix,Wrap(Data))));
	 if(G1)
	   free(G1);
	 if(!strncmp("core",G1=RUN("ls core 2> /dev/null"),4)){
	   Segfaults++;
	   ScrSession=Add2Scr(ScrSession,StrCat("",":SHELLERROR"),2);
	   if(system("mkdir -p CORES; mv -f core CORES/ > /dev/null")){}
	   SeshFirst->Error=1;  
	 }
	 else
	   SeshFirst->Error=0;
	 if(G1)
	   free(G1);
	 Buffer2Scr(SeshFirst);
      }
   }
}

/*! \brief Load batch files into WitchCraft and execute them.
 *
 */
void WC_Load(char *Data){
  int _=0,Position=0,Len=0,Total=0,Progress=0;
  char *ToRUN=NULL,*G1=NULL,*Executed=NULL,*Command=NULL,*PI=(char *)malloc(7*sizeof(char )),*Old=NULL;
  ToRUN=StrCat(G1=StrCat("grep \"^<PERSON> \" ",Data)," | sed \"s/^.*<PERSON> //g\"");
  if(G1)
    free(G1);
  Executed=RUN(ToRUN);
  memset(PI,0,6);
  for(;Executed[_];_++)
    if(Executed[_]=='\n')
      Total++;
  for(_=0;Executed[_];_++){
    if(Executed[_]=='\n'){
      Progress++;
      Old=(char *)malloc(((Len=(_-Position))+1)*sizeof(char));
      memcpy(Old,Executed+Position,Len);
      Old[Len]=0;
      Command=FilterChars(Old,"\t");
      free(Old);
      sprintf(PI,"%3d%%",((100*Progress)/Total));
      DrawText(ScreenHeight-1,0,10,"EXE [%s] - ",PI);
      Execute(Command);
      Position=_+1;
      if(HighSpeedRefresh)
	DebugBuffer(ScrFirst);
    }
  }
  if(strlen(Executed+Position)){
    Old=(char *)malloc(((Len=(_-Position))+1)*sizeof(char));
    memcpy(Old,Executed+Position,Len);
    Old[Len]=0;
    Command=FilterChars(Old,"\t");
    free(Old);
    Execute(Command);
  }
  CLS();
  DebugBuffer(ScrFirst);
}

/*! \brief Save the history buffer to disk.
 *
 */
void SaveBuffer(char *Data){
  struct Buffer *D=NULL;
  FILE *Write=NULL;
  if((Write=fopen(Data,"w"))){
    for(D=SeshLast;D;D=D->Prev){
      if(D->Error==1)
	fprintf(Write,"<SYSTEM> %s\n","THE FOLLOWING COMMAND SEGFAULTS: ");
      if(D->Error==2)
	fprintf(Write,"<SYSTEM> %s\n",D->Command);
      if((D->Command[0])&&(D->Error!=2))
	fprintf(Write,"<PERSON> %s\n",D->Command);
      if(D->Answer[0])
	fprintf(Write,"<ENiX>   %s\n",D->Answer);
    }
    fclose(Write);
  }
}

/*! \brief WitchCraft help menu.
 *
 */
void Command(char *Data){
  int _=0;
  char *Filename=NULL,*DataBuf=NULL,*FreeMe=NULL;
  ScrSession=Add2Scr(ScrSession,Data,0);
  if(!strncasecmp("!HELP",Data,5)){
    ScrSession=Add2Scr(ScrSession,StrCat("","!HELP   This menu                 "),4);
    ScrSession=Add2Scr(ScrSession,StrCat("","!PREFIX <command prefix>          "),4);
    ScrSession=Add2Scr(ScrSession,StrCat("","!QUIT   Exit the shell            "),4);
    ScrSession=Add2Scr(ScrSession,StrCat("","!CLEAR  Clear screen and buffers. "),4);
    ScrSession=Add2Scr(ScrSession,StrCat("","!SAVE   <buffername>              "),4);    
    ScrSession=Add2Scr(ScrSession,StrCat("","!LOAD   <buffername>              "),4);
    ScrSession=Add2Scr(ScrSession,StrCat("","!SERVER Set shell to be a server. "),4);
    ScrSession=Add2Scr(ScrSession,StrCat("","!CLIENT <server ip>               "),4);
    ScrSession=Add2Scr(ScrSession,StrCat("","!REDRAW                           "),4);
    ScrSession=Add2Scr(ScrSession,StrCat("","!SLOW   <ON/OFF>                  "),4);
    ScrSession=Add2Scr(ScrSession,StrCat("","!TEST   <DIRECTORY>               "),4);
    ScrSession=Add2Scr(ScrSession,StrCat("","!SET                              "),4);
  }
  if(!strncasecmp("!SET",Data,4)){
    SaveConfig();
    LoadConfig();
  }
  if(!strncasecmp("!TEST",Data,5)){
    /* this may need to be freed somewhere */
    DataBuf=StrCat("",Data); 
    Clear();
    for(_=strlen(DataBuf)-1;(DataBuf[_]==' ')||(DataBuf[_]=='/');_--)
      DataBuf[_]=0;
    for(_=5;DataBuf[_]&&(DataBuf[_]==' ');_++){}
    Filename=DataBuf+_;
    WitchCraftTest(Filename);
    SaveBuffer(StrCat("","test.txt"));
    add_history(DataBuf);
    Session=Add2Buffer(Session,DataBuf,RUN(FreeMe=StrCat("","./TEST-RESULTS/Analyse.pl")));
    free(FreeMe);
    Buffer2Scr(SeshFirst);    
  }
  if((!strncasecmp("!PREFIX",Data,7))&&(StrLen(Data)>9)){
    CommandPrefix=Data+7;
  }
  if(!strncasecmp("!QUIT",Data,5)){
    QUIT=1;
  }
  if(!strncasecmp("!CLEAR",Data,6)){
    Clear();
  }
  if(!strncasecmp("!REDRAW",Data,7)){
    Setup(0);
  }
  if((!strncasecmp("!SAVE",Data,5))&&(StrLen(Data)>6)){
    for(_=6;Data[_]&&(Data[_]==' ');_++){}
    Filename=Data+_;
    SaveBuffer(Filename);
  }
  if((!strncasecmp("!LOAD",Data,5))&&(StrLen(Data)>6)){
    for(_=6;Data[_]&&(Data[_]==' ');_++){}
    Filename=Data+_;
    WC_Load(Filename);
    add_history(Data);
  }
  if(!strncasecmp("!SERVER",Data,7)){
    RunServer();
  }
  if(!strncasecmp("!SLOW",Data,5)){
    for(_=6;Data[_]&&(Data[_]==' ');_++){}
    if(!strncmp(Data+_,"ON",2))
      SLOWREPLAY=1;
    if(!strncmp(Data+_,"OFF",3))
      SLOWREPLAY=0;
  }
  if(!strncasecmp("!CLIENT",Data,7)&&(StrLen(Data)>8)){
    ClientMode=1;
    ServerMode=0;
    for(_=8;Data[_]&&(Data[_]==' ');_++){}
    IPAddress=Data+_;
  }
}

/*! \brief Clear the screen.
 *
 */
void CLS(void){
   printf(WC_CLS);
   DrawText(ScreenHeight-2,1,9,"%s",HorzLine);
   printf("\r");
   if(ServerMode)
     ServBG();
   if(ClientMode)
     ClientBG();
}

/*! \brief Add data to the history + screen buffer.
 *
 */
struct Buffer *Add2Buffer(struct Buffer *Data,char *C,char *A){
   struct Buffer *New=(struct Buffer *)malloc(sizeof(struct Buffer));
   New->Command=C;
   New->Answer=A;
   New->Next=Data;
   New->Prev=NULL;
   if(Data)
     Data->Prev=New;
   else
     SeshLast=New;
   SeshFirst=New;
   return New;
}

/*! \brief Add data to the screen buffer.
 *
 */
struct FormattedLine *Add2Scr(struct FormattedLine *Data,char *C,int Type){
  struct FormattedLine *New=(struct FormattedLine *)malloc(sizeof(struct FormattedLine));
  New->Data=C;    
  New->Type=Type;
  New->Next=Data; 
  New->Prev=NULL;
  if(Data)
    Data->Prev=New;
  else
    ScrLast=New;
  ScrFirst=New;
  return New;
}

/*! \brief Calculate the line wraps.
 *
 */
void LineWrap(char *Data,int Type){
   int A=0,B=0,C=0,Max=strlen(Data),Width=ScreenWidth-9;
   char *B2=NULL;
   A=(Max/Width)+!!(Max%Width);
   for(B=0;B<A;B++){
     B2=(char *)malloc((Width+1)*sizeof(char));
     if((C=(Max-(B*Width)))>Width){
       memcpy(B2,Data+(B*Width),Width); 
       B2[Width]=0;
     }
     else{
       memcpy(B2,Data+(B*Width),C); 
       B2[C]=0;
     }
     ScrSession=Add2Scr(ScrSession,B2,Type);
   }
}

/*! \brief Blit the history buffer to the terminal.
 *
 */
void Buffer2Scr(struct Buffer *Data){
   int _=0,Max=0,OldP=0;
   char *Stuff=NULL,*Raw=NULL;
   struct Buffer *NewBuf=Data;
   if(NewBuf){
     Max=strlen(Raw=NewBuf->Command); 
     OldP=0;
      for(_=0;_<Max;_++){
	 if(Raw[_]=='\n'){
	   if(Stuff)
	     free(Stuff);
	   Stuff=(char *)malloc((_-OldP+1)*sizeof(char));
	   memcpy(Stuff,Raw+OldP,(_-OldP));
	   Stuff[_-OldP]=0;
	   OldP=_+1;
	   LineWrap(Stuff,0);
	 }
      }
      if(Stuff)
	free(Stuff);
      Stuff=(char *)malloc((_-OldP+1)*sizeof(char)); 
      memcpy(Stuff,Raw+OldP,(_-OldP));
      Stuff[_-OldP]=0; 
      LineWrap(Stuff,0); 
      Max=strlen(Raw=NewBuf->Answer); 
      OldP=0;
      for(_=0;_<Max;_++){
	 if(Raw[_]=='\n'){
	    if(Stuff)
	      free(Stuff);
	    Stuff=(char *)malloc((_-OldP+1)*sizeof(char)); 
	    memcpy(Stuff,Raw+OldP,(_-OldP));
	    Stuff[_-OldP]=0;
	    OldP=_+1;
	    LineWrap(Stuff,1);
	 }
      }
      if(Stuff)
	free(Stuff);
      Stuff=(char *)malloc((_-OldP+1)*sizeof(char)); 
      memcpy(Stuff,Raw+OldP,(_-OldP));
      Stuff[_-OldP]=0; 
      LineWrap(Stuff,1);
   }
}

/*! \brief Show the buffer on the screen.
 *
 */
void DebugBuffer(struct FormattedLine *Data){
  int _=0,LastType=-1;
  struct FormattedLine *B=Data;
  for(_=ScreenHeight-3;B&&_;_--){
    if(LastType==B->Type){
      if(B->Type==1)
	DrawText(_,1,9,ENiXFORMAT,B->Data);
      if(B->Type==0){
	if(ServerMode)
	  DrawText(_,1,3,SERVERFORMAT,B->Data);
	else
	  DrawText(_,1,5,PERSONFORMAT,B->Data);
      }
      if(B->Type==2)
	DrawText(_,1,15,SYSTEMFORMAT,StrCat("","THE FOLLOWING COMMAND SEGFAULTS: "));
      if(B->Type==3){}
      if(B->Type==4)
	DrawText(_,1,15,SYSTEMFORMAT,B->Data);
      LastType=B->Type;
      B=B->Next;
    }
    else{
      DrawText(_,1,15,BlankLine,"");
      LastType=B->Type;
    }     
  }
}

/*! \brief Calculate the length of the buffer.
 *
 */
unsigned long int LengthofBuffer(struct Buffer *Data){
  unsigned long int Length=0;
  for(Length=0;Data;Data=Data->Next)
    Length++;
  return Length;
}

/*! \brief Low level draw text API.
 *
 */
void DrawText(int X,int Y,int C,char *Format,char *Data){
   printf("\033[%02d;%02df\033[%02d;%02dm",X,Y,Colour1[C],Colour2[C]);
   if(Format&&Data)
     printf(Format,Data);
}

/*! \brief Run commands through ENiX.
 *
 */
char *RUN(char *Data){
  char *Output=NULL;
  FILE *CMD_OUT=NULL;
  DrawText(ScreenHeight-2,10,11," %s","BUSY");
  fflush(stdout);
  if((CMD_OUT=popen(Data,"r"))){
    Output=LoadPipe(CMD_OUT);
    pclose(CMD_OUT);
  }
  if(Speak && Output){
    if(system(StrCat(StrCat("echo \"",Output),"\" | festival --tts &"))){}
  }
  return Output;
}

/*! \brief Display the modebar.
 *
 */
void ModeBar(){
   DrawText(ScreenHeight-2,1,9,"%s",HorzLine);
   if(ServerMode)
     ServBG();
   if(ClientMode)
     ClientBG();   
   DrawText(ScreenHeight-2,10,11," %s","IDLE");
   DrawText(ScreenHeight-1,1,10,"%s","");
}

/*! \brief Handle the command line input.
 *
 */
void Input(){
   char *Data=NULL,*G1=NULL;
   int sock=0;
   char *Returned=NULL;
   struct Blob *Received=NULL,*Sent=NULL;
   ModeBar();
   printf("\033[?25h");
   Data=RemoveFinalSpaces(readline("WITCHCRAFT - "));
   printf("\033[?25l");
   ModeBar();
   DrawText(ScreenHeight-1,0,10,"%s","EXECUTING... ");
   if(Data){
      add_history(Data);
      if(Data[0]=='!')
	Command(Data);    
      else{
	 if(ClientMode){
	   sock=OpenClientSocket(IPAddress);
	   Sent=WMS_Bin2Blob(strlen(Data),Data);
	   NetTransmit(Sent,sock);
	   free(Sent);
	   Received=NetReceive(sock);
	   Returned=Received->Nucleus;
	   Session=Add2Buffer(Session,Data,Returned);
	   free(Received);
	   close(sock);
	 }
	 else{
	   if(Speak==2 && Data){
	     if(system(StrCat(StrCat("echo \"",Data),"\" | festival --tts"))){}
	   }
	   if(IsOSShell)
	     Session=Add2Buffer(Session,Data,RUN(G1=StrCat(CommandPrefix,Data)));
	   else
	     Session=Add2Buffer(Session,Data,RUN(G1=StrCat(CommandPrefix,Wrap(Data))));
	   if(G1)
	     free(G1);
	   if(!strncmp("core",G1=RUN("ls core 2> /dev/null"),4)){
	     Segfaults++;
	     ScrSession=Add2Scr(ScrSession,StrCat("",":SHELLERROR"),2);
	     if(system("mkdir -p CORES; mv -f core CORES/ > /dev/null")){}
	     SeshFirst->Error=1;
	   }
	   else
	     SeshFirst->Error=0;
	   if(G1)
	     free(G1);
	 }
	 Buffer2Scr(SeshFirst);
      }
   }
}

/*! \brief Main loop for processing entry.
 *
 */
void MainLoop(){
   if(ServerMode)
     close(ServerBindId);
   ServerMode=0;
   while(!QUIT){
      CLS();
      DebugBuffer(ScrFirst);
      if(!ScrFirst)
	 Splash();
      Input(); 
   }  
   printf("\033[?25h");
}

/*! \brief Detect if OS flag.
 *
 */
void DetectOSMode(char *Data){
   int _=0,flag=1;
   for(_=0;Data[_];_++)
     if(Data[_]!=' ')
       flag=0;
   IsOSShell=flag;
}

/*! \brief Setup the shell.
 *
 */
void Setup(int Signal){
  printf(WC_TITLE);
  
  LoadConfig();

  if(!CygwinMode)
    DetectTermSize(0);
  
  AlignRight=(char *)malloc(10*sizeof(char));
  memset(AlignRight,0,10);
  sprintf(AlignRight,"%%%ds",ScreenWidth-9);
  AlignLeft=(char *)malloc(10*sizeof(char));
  memset(AlignLeft,0,10);
  sprintf(AlignLeft,"%%-%ds",ScreenWidth-9);
  BlankLine=(char *)malloc((ScreenWidth+1)*sizeof(char));
  memset(BlankLine,' ',ScreenWidth);
  BlankLine[ScreenWidth]=0;
  ENiXFORMAT=StrCat("  <ENiX> ",AlignLeft);
  PERSONFORMAT=StrCat("<PERSON> ",AlignRight);
  SERVERFORMAT=StrCat("<SERVER> ",AlignRight);
  SYSTEMFORMAT=StrCat("<SYSTEM> ",AlignRight);
  HorzLine=(char *)malloc(ScreenWidth*sizeof(char));
  memset(HorzLine,'-',ScreenWidth);
  HorzLine[1]='[';
  HorzLine[14]=']';
  memcpy(HorzLine+2,"STATUS:      ",12);   
}

/*! \brief Handle WitchCraft exit, handle the trapped signals etc.
 *
 */
void WitchSignal(int Sig){
  if(!ClientMode){
    printf("\rExiting, please wait... ");
    fflush(stdout);
    close(Socket2Close);    
    puts("Saved :)");
    printf("\033[?25h");
  }
  exit(0);
}

/*! \brief Detect the terminal size.
 *
 */
void DetectTermSize(){
  struct winsize WindowSize;
  if(ioctl(STDIN_FILENO,TIOCGWINSZ,&WindowSize)==-1){
    puts("ioctl");
  }
  ScreenWidth=WindowSize.ws_col;
  ScreenHeight=WindowSize.ws_row;

  /*
  // Old version
  char *Cols=NULL,*Rows=NULL;
  FILE *CMD_OUT=NULL;
  if((CMD_OUT=popen("tput cols","r"))){
    Cols=LoadPipe(CMD_OUT);
    pclose(CMD_OUT);
  }  
  if((CMD_OUT=popen("tput lines","r"))){
    Rows=LoadPipe(CMD_OUT);
    pclose(CMD_OUT);
  }  
  ScreenWidth=atoi(Cols);
  ScreenHeight=atoi(Rows);
  */
}

/*! \brief Bootstrap the WitchCraft shell.
 *
 */
int main(int argc,char *argv[]){
  ServerID=0;
  Setup(0);
  signal(SIGINT,WitchSignal);
  signal(SIGWINCH,Setup);
  CLS();
  DetectOSMode(CommandPrefix);
  printf("\033[?25l");
  if(system("mkdir -p CORES; mv -f core CORES/")){}
  MainLoop();
  printf(WC_RESET);
  return 0;
}
