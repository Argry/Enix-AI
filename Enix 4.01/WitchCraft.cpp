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

#include <string>

#include "WitchCraft.h"
#include "WitchCraft-Colours.h"
#include "ENiX_Config.h"
#include "ENiX_Disk.h"
#include "ENiX_Net.h"
#include "ENiX_NetInterface.h"
#include "ENiX_Seq.h"
#include "ENiX_Settings.h"

/*
  tput stty to get terminal size ?
 */

/* Global Variates **********************************************************/

int                  ScreenWidth=80;
int                  ScreenHeight=24;
char                 *HorzLine=NULL;
const char           *CommandPrefix=(const char *)"./ENiX3 ";
int                  QUIT=0;
int                  VSynth=0;
int                  Speak=0;
int                  Segfaults=0;
int                  HighSpeedRefresh=1;
int                  CygwinMode=0;
string               PROTOCOL_PREFIX="WitchCraft 2.0";

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

/* Routine Declarations *****************************************************/

/*! \brief WitchCraft handle control C messages and other things.
 *
 */
void ServerSignal(int SignalData){
  unsigned int LocalPid=getpid();
  printf("\nSignal caught in pid %d... Exiting.\n",LocalPid);
  exit(0);
}


/*! \brief WitchCraft connection to server msg.
 *
 */
void Application::Splash(){
  //  puts("ENiX Test Client, connecting to server...");
}

/*! \brief WitchCraft run a command from a client connection.
 *
 */
Packet Application::ExecuteCommand(BinData Input){
  // needs to be a persistent instance!

  Packet Incoming(Input);
  Packet Outgoing;
  BinData OutgoingRecord={};
  string SessionIdentifier=BinData::GetSessionIdentifier();
  if(Incoming.Strings.size()){
    Sequence<int> Seq1={};
    vector<string> ToProcess=Seq1.Convert2StrVec(Incoming.Strings[0]);
    Outgoing=ThisInstance->NetExecuteCommand(ToProcess,Incoming);
    Outgoing.GenerateBinary();
  }
  return Outgoing;
}


/*! \brief WitchCraft shutdown gracefully.
 *
 */
WitchCraft::~WitchCraft(){
  string HistoryName=BinData::GetHomeDir();
  HistoryName+="/.wc_history";
  SaveBuffer(HistoryName.c_str(),true);
}

/*! \brief WitchCraft initialise the library.
 *
 */
WitchCraft::WitchCraft(){
  CmdSetInitialised=false;
  InitialiseInstructions();  
  HostName=WITCHCRAFTHOSTNAME;
  PortNum=WITCHCRAFTPORTNUM;
  Connection=NULL;
  string HomeDir=BinData::GetHomeDir();
  string HistoryFile=HomeDir+"/.wc_history";
  LoadHistory(HistoryFile.c_str());
  string Command="mv "+HistoryFile+" "+HomeDir+"/.wc_backup; tail -100 "+HomeDir+"/.wc_backup > "+HistoryFile+"; rm "+HomeDir+"/.wc_backup";
  /*
    puts(Command.c_str());
    getchar();
  */
  system(Command.c_str());
}

void WitchCraft::LoadHistory(const char *HistoryFile){
  int _=0,Position=0,Len=0,Total=0,Progress=0;
  char *ToRUN=NULL,*G1=NULL,*Executed=NULL,*Command=NULL,*PI=(char *)malloc(7*sizeof(char )),*Old=NULL;
  ToRUN=StrCat(G1=StrCat("grep \"^<PERSON> \" ",HistoryFile)," | sed \"s/^.*<PERSON> //g\" | tail -100");
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
      Command=FilterChars(Old,(char *)"\t");
      free(Old);
      add_history(Command);
      Position=_+1;
      DebugBuffer(ScrFirst);
    }
  }
  if(strlen(Executed+Position)){
    Old=(char *)malloc(((Len=(_-Position))+1)*sizeof(char));
    memcpy(Old,Executed+Position,Len);
    Old[Len]=0;
    Command=FilterChars(Old,(char *)"\t");
    free(Old);
    add_history(Command);
  }
  CLS();
  DebugBuffer(ScrFirst);
}

/*! \brief WitchCraft write the data received from the server to the console buffers.
 *
 */
void WitchCraft::PresentReceivedData(char *Command,Packet Source,size_t StartAt){
  BinData Response;
  string ReturnedData;
  if(Source.Strings.size()>1){
    for(size_t _=StartAt;_<Source.Strings.size();_++){
      string ThisStr(Source.Strings[_].Str,Source.Strings[_].Size);
      ReturnedData+=ThisStr+"\n";
    }
    Session=Add2Buffer(Session,Command,StrCat("",ReturnedData.c_str()));
    Response=Source.Strings[1];
  }
  else{
    Session=Add2Buffer(Session,Command,(char *)"");
    Response="[Response received from remote system, but with no data]";
  }
}

/*! \brief WitchCraft prepare the data packet for transmission to server.
 *
 */
Packet WitchCraft::Preprocess(char *Command){
  string RecordName="testrecord1";
  string CLIInput=string(Command);
  BinData SendMsg;
  vector<BinData> Binaries={};
  Sequence<int> Seq;
  BinData CLIData(Command);
  vector<string> ToProcess=Seq.Convert2StrVec(CLIData);  
  SendMsg.Change(CLIInput);
  vector<BinData> Strings={};
  Strings.push_back(SendMsg);
  Packet Outgoing(Strings,Binaries);
  return Outgoing;
}

/*! \brief WitchCraft get the help data from the server connection.
 *
 */
void WitchCraft::RequestHelp(Networking *Net){
  char *CLIRawInput=NULL,*Prompt=(char *)"  ";
  char *HelpCmd=(char *)"HELP";
  Packet HelpPacket=Preprocess(HelpCmd);
  Net->Send(HelpPacket.AllData);
  BinData HelpResp=Net->Receive();
  Packet IncomingHelpPacket(HelpResp);
  PresentReceivedData((char *)"",IncomingHelpPacket);
  Buffer2Scr(SeshFirst);
}

/*! \brief WitchCraft create an empty generic packet.
 *
 */
Packet WitchCraft::InitialisePacket(string Response){
  Packet Answer={};
  Answer.Binaries={};
  Answer.Strings={};
  BinData Empty("");
  Answer.Strings.push_back(Empty);
  BinData AddRecStr(Response);
  Answer.Strings.push_back(AddRecStr);
  return Answer;
}

/*! \brief WitchCraft create an instruction set internally.
 *
 */
void WitchCraft::InitialiseInstructions(){
  if(!CmdSetInitialised){

    CommandName.push_back("!CLEAR");
    InstructionSet.push_back(&WitchCraft::Clear);
    Parameters.push_back(0);
    OnlineHelp.push_back("!CLEAR  Clear screen and buffers. ");

    CommandName.push_back("!CLIENT");
    InstructionSet.push_back(&WitchCraft::Client);
    Parameters.push_back(0);
    OnlineHelp.push_back("!CLIENT <server ip> <port>        ");

    CommandName.push_back("!DISCONNECT");
    InstructionSet.push_back(&WitchCraft::Disconnect);
    Parameters.push_back(0);
    OnlineHelp.push_back("!DISCONNECT <DIRECTORY>           ");

    CommandName.push_back("!HELP");
    InstructionSet.push_back(&WitchCraft::Help);
    Parameters.push_back(0);
    OnlineHelp.push_back("!HELP   This menu                 ");

    CommandName.push_back("!LOAD");
    InstructionSet.push_back(&WitchCraft::Load);
    Parameters.push_back(0);
    OnlineHelp.push_back("!LOAD   <buffername>              ");

    CommandName.push_back("!PREFIX");
    InstructionSet.push_back(&WitchCraft::CmdPrefix);
    Parameters.push_back(0);
    OnlineHelp.push_back("!PREFIX <command prefix>          ");

    CommandName.push_back("!QUIT");
    InstructionSet.push_back(&WitchCraft::Quit);
    Parameters.push_back(0);
    OnlineHelp.push_back("!QUIT   Exit the shell            ");

    CommandName.push_back("!REDRAW");
    InstructionSet.push_back(&WitchCraft::Redraw);
    Parameters.push_back(0);
    OnlineHelp.push_back("!REDRAW                           ");

    CommandName.push_back("!SAVE");
    InstructionSet.push_back(&WitchCraft::Save);
    Parameters.push_back(0);
    OnlineHelp.push_back("!SAVE   <buffername>              ");

    CommandName.push_back("!SET");
    InstructionSet.push_back(&WitchCraft::SetConfig);
    Parameters.push_back(0);
    OnlineHelp.push_back("!SET                              ");

    CommandName.push_back("!SLOW");
    InstructionSet.push_back(&WitchCraft::Slow);
    Parameters.push_back(0);
    OnlineHelp.push_back("!SLOW   <ON/OFF>                  ");

    CommandName.push_back("!TEST");
    InstructionSet.push_back(&WitchCraft::Test);
    Parameters.push_back(0);
    OnlineHelp.push_back("!TEST   <DIRECTORY>               ");

    CmdSetInitialised=true;
  }
}

/*! \brief WitchCraft disconnect the shell from the server.
 *
 */
Packet WitchCraft::Disconnect(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("Disconnected");
  if(Connection){
    Connection->~Networking();
    Connection=NULL;
  }
  ClientMode=0;
  return Answer;
}

/*! \brief WitchCraft nuke the shell history buffer.
 *
 */
Packet WitchCraft::Clear(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("Clear");
  Clear();
  return Answer;
}

/*! \brief WitchCraft set as client to connect to server.
 *
 */
Packet WitchCraft::Client(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("Clear");
  ServerMode=0;

  if(Connection){
    delete Connection;
  }

  HostName=const_cast<char *>(Commands[1].c_str());
  PortNum=atoi(Commands[2].c_str());
  Connection = new Networking(HostName,PortNum,false);
  if(Connection->ConnectedStatus==Connected){
    ClientMode=1;
    BinData Input=Connection->Receive();
    Packet ProtocolData(Input);
    string ProtocolName(ProtocolData.Strings[0].Str,ProtocolData.Strings[0].Size);  
    PresentReceivedData((char *)"",ProtocolData,0);
    for(size_t _=0;_<ProtocolData.Strings.size();++_){
      ScrSession=Add2Scr(ScrSession,StrCat("",ProtocolData.Strings[_].Str),Source::Client);
    }
    RequestHelp(Connection);
    return Answer;
  }
  else{
    ClientMode=0;
    ScrSession=Add2Scr(ScrSession,StrCat("",(char *)"Could not connect to server."),Source::Client);
    return Answer;
  }
}

/*! \brief WitchCraft set the shell command prefix.
 *
 */
Packet WitchCraft::CmdPrefix(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("Clear");
  CommandPrefix=Commands[1].c_str();
  return Answer;
}

/*! \brief WitchCraft list shell help topics.
 *
 */
Packet WitchCraft::Help(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("SENDING HELP");
  for(size_t _=0;_<OnlineHelp.size();_++){
    BinData Text(OnlineHelp[_]);
    Answer.Strings.push_back(Text);
    ScrSession=Add2Scr(ScrSession,StrCat("",OnlineHelp[_].c_str()),Source::Client);
  }
  return Answer;
}

/*! \brief WitchCraft load and execute the session.
 *
 */
Packet WitchCraft::Load(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("Clear");
  WC_Load(Commands[1].c_str());
  return Answer;
}

/*! \brief WitchCraft exit the shell.
 *
 */
Packet WitchCraft::Quit(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("Clear");
  QUIT=1;
  return Answer;
}

/*! \brief WitchCraft update to the resized console.
 *
 */
Packet WitchCraft::Redraw(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("Clear");
  Setup();
  return Answer;
}

/*! \brief WitchCraft save the session.
 *
 */
Packet WitchCraft::Save(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("Clear");
  SaveBuffer(Commands[1].c_str());
  return Answer;
}

/*! \brief WitchCraft save the shell configuration.
 *
 */
Packet WitchCraft::SetConfig(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("CONFIG SET");
  SaveConfig();
  LoadConfig();
  return Answer;
}

/*! \brief WitchCraft slow replay functionality.
 *
 */
Packet WitchCraft::Slow(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("Clear");
  if(!strncmp(Commands[1].c_str(),"ON",2))
    SLOWREPLAY=1;
  if(!strncmp(Commands[1].c_str(),"OFF",3))
    SLOWREPLAY=0;
  return Answer;
}

/*! \brief WitchCraft test function.
 *
 */
Packet WitchCraft::Test(vector<string> Commands,Packet Data){
  Packet Answer=InitialisePacket("TESTING");
  /* this may need to be freed somewhere */
  size_t _=0;
  char *Filename=NULL,*DataBuf=NULL,*FreeMe=NULL;
  DataBuf=StrCat("",Commands[1].c_str());
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
  return Answer;
}

/*! \brief WitchCraft Shell CLI mode command execution.
 *
 */
Packet WitchCraft::CLIExecuteCommand(vector<string> Commands,Packet Data){
  bool Found=false;
  size_t CommandIndex=-1;
  BinData Protocol(PROTOCOL_PREFIX);
  if(Commands.size()){
    for(size_t _=0;_<CommandName.size();_++){
      BinData Converted(Commands[0]);
      if(!strcasecmp(Converted.Str,CommandName[_].c_str())){
	Found=true;
	CommandIndex=_;
	break;
      }
    }
    if(Found&&(CommandIndex!=-1)){
      if(Commands.size()>Parameters[CommandIndex]){
	Packet Retrieved=(this->*InstructionSet[CommandIndex])(Commands,Data);
	Retrieved.Strings[0]=Protocol;
	return Retrieved;
      }
      else{
	Packet Answer=InitialisePacket("ERROR: WRONG COMMAND OPTIONS");
	Answer.Strings[0]=Protocol;
	return Answer;
      }
    }
  }
  Packet Answer=InitialisePacket("ERROR: UNRECOGNISED COMMAND: "+Commands[0]);
  Answer.Strings[0]=Protocol;
  return Answer;
}

/*! \brief WitchCraft Server mode command execution.
 *
 */
Packet WitchCraft::NetExecuteCommand(vector<string> Commands,Packet Data){
  bool Found=false;
  size_t CommandIndex=-1;
  BinData Protocol(PROTOCOL_PREFIX);

  string Inbound(Data.Strings[0].Str,Data.Strings[0].Size);
  ScrSession=Add2Scr(ScrSession,StrCat("",Inbound.c_str()),Source::Person);

  if(Commands.size()){
    for(size_t _=0;_<CommandName.size();_++){
      BinData Converted(Commands[0]);
      if(Converted==CommandName[_]){
	Found=true;
	CommandIndex=_;
	break;
      }
    }
    if(Found&&(CommandIndex!=-1)){
      if(Commands.size()>Parameters[CommandIndex]){
	Packet Retrieved=(this->*InstructionSet[CommandIndex])(Commands,Data);
	Retrieved.Strings[0]=Protocol;
	return Retrieved;
      }
      else{
	Packet Answer=InitialisePacket("ERROR: WRONG COMMAND OPTIONS");
	Answer.Strings[0]=Protocol;
	return Answer;
      }
    }
  }
  Packet Answer=InitialisePacket("ERROR: UNRECOGNISED COMMAND: "+Commands[0]);
  Answer.Strings[0]=Protocol;
  
  PresentReceivedData((char *)"",Answer);
  ServConsUpdate();

  return Answer;
}


/*! \Brief String cat, appends "S2" onto string "S1".
 *         
 */
char *WitchCraft::StrCat(const char *S1,const char *S2){ 
  int L1=strlen(S1),L2=strlen(S2); 
  char *R=NULL; 
  R=(char *)malloc(L1+L2+1);
  memcpy(R,S1,L1);
  memcpy(R+L1,S2,L2); R[L1+L2]=0; 
  return R;
}

/*! \Brief Strips out the characters "Filter" from string "String".
 *         
 */
char *WitchCraft::FilterChars(char *String,char *Filter){
  int _=0,i=0;
  char *Filtered=(char *)malloc((strlen(String)+1)*sizeof(char));
  for(_=0;String[_];_++)
    if(!strchr(Filter,String[_]))
      Filtered[i++]=String[_];
  Filtered[i]=0;
  return Filtered;
}

/*! \brief Save configuration file to disk.
 *
 */
void WitchCraft::SaveConfig(){
  string ConfigFile=getenv("HOME");
  ConfigFile+="/.WitchCraft.rc";
  Config WCConfig;
  WCConfig.AddToConfig("CommandPrefix",CommandPrefix);
  WCConfig.AddToConfig("VSynth",(long int)VSynth);
  WCConfig.AddToConfig("Speak",(long int)Speak);
  WCConfig.AddToConfig("SlowReplay",(long int)SLOWREPLAY);
  WCConfig.AddToConfig("HighSpeedRefresh",(long int)HighSpeedRefresh);
  WCConfig.AddToConfig("CygwinMode",(long int)CygwinMode);
  WCConfig.WriteConfigFile(ConfigFile);
}

/*! \brief Load configuration from disk.
 *
 */
void WitchCraft::LoadConfig(){
  string ConfigFile=getenv("HOME");
  ConfigFile+="/.WitchCraft.rc";
  Config WCConfig;
  WCConfig.ReadConfigFile(ConfigFile);
  CommandPrefix=WCConfig.GetValue("CommandPrefix").c_str();
  VSynth=atoi(WCConfig.GetValue("VSynth").c_str());
  Speak=atoi(WCConfig.GetValue("Speak").c_str());
  SLOWREPLAY=atoi(WCConfig.GetValue("SlowReplay").c_str());
  HighSpeedRefresh=atoi(WCConfig.GetValue("HighSpeedRefresh").c_str());
  CygwinMode=atoi(WCConfig.GetValue("CygwinMode").c_str());
}

/*! \brief Remove the trailing spaces after a command entry.
 *
 */
char *WitchCraft::RemoveFinalSpaces(char *RAW){
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
void WitchCraft::Clear(void){  
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
void WitchCraft::WitchCraftTest(char *Directory){
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
      SeshFirst->Error=ErrorType::Load;
      Buffer2Scr(SeshFirst);
      WC_Load(FullPath);
    }
  }
  SegfaultMessage=(char *)malloc(80*sizeof(char));
  sprintf(SegfaultMessage,"%d Segfaults found during testing.",Segfaults);
  ScrSession=Add2Scr(ScrSession,SegfaultMessage,Source::Client);
}

/*! \brief Wrap the text around the window.
 *
 */
char *WitchCraft::Wrap(char *Data){
  int _=0,Previous=0,wasspace=0,Delta=0;
  char *R=(char *)"",*Str=NULL;
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
void WitchCraft::ServBG(){

  DrawText(ScreenHeight-2,17,9,  (char *)"[SERVER:                ]",(char *)"");
  DrawText(ScreenHeight-2,26,10,  (char *)"% 15s",(char *)"-");
  fflush(stdout);

}

/*! \brief Show client mode splash screen.
 *
 */
void WitchCraft::ClientBG(){
  /*
   int C=(ScreenWidth-44)>>1;
  */
  DrawText(ScreenHeight-2,17,9,  (char *)"[CLIENT:                ]",(char *)"");
  DrawText(ScreenHeight-2,26,10,(char *)"% 15s",HostName.c_str());
  fflush(stdout);
}

/*! \brief Update the server console.
 *
 */
void WitchCraft::ServConsUpdate(){
  DrawText(ScreenHeight-1,(ScreenWidth-30)>>1,12,(char *)"- USE ^C TO EXIT SERVER MODE -",NULL);
  DebugBuffer(ScrFirst);
  ServBG();
  DrawText(ScreenHeight-2,10,11,(char *)" %s",(char *)"IDLE");
  fflush(stdout);
}


/*! \brief Generic WitchCraft splash screen.
 *
 */
void WitchCraft::Splash(){
  int C=(ScreenWidth-77)>>1;
  DrawText((ScreenHeight>>1)-3,C,10,(char *)"_  _  _ _____ _______ _______ _     _ _______  ______ _______ _______ _______",(char *)"");
  DrawText((ScreenHeight>>1)-2,C, 9,(char *)"|  |  |   |      |    |       |_____| |       |_____/ |_____| |______    |   ",(char *)"");
  DrawText((ScreenHeight>>1)-1,C,13,(char *)"|__|__| __|__    |    |_____  |     | |_____  |    \\_ |     | |          |   ",(char *)"");
  DrawText((ScreenHeight>>1)+1,C, 9,(char *)"                       SHELL2 GPL [OvO]wl 2010, ENiX      ",(char *)"");
}

/*! \brief Execute a command. Only called from within scripts.
 *
 */
void WitchCraft::ExeCmdInScript(char *Data){
  char *G1=NULL;
  int sock=0;
  char *Returned=NULL;
  struct Blob *Sent=NULL,*Received=NULL;
  add_history(Data);
  if(ClientMode){
    if(Data[0]=='!'){
      ShellCommand(Data);
    }
    else{
      vector<BinData> Binaries={};
      vector<BinData> Strings={};
      BinData InputInBin(Data,strlen(Data));
      Strings.push_back(InputInBin);
      Packet Outgoing(Strings,Binaries);
      DrawText(ScreenHeight-2,10,11,(char *)" %s",(char *)"BUSY");
      fflush(stdout);
      Connection->Send(Outgoing.AllData);
      BinData Test=Connection->Receive();
      Packet Incoming(Test);
      PresentReceivedData(Data,Incoming);       
      Buffer2Scr(SeshFirst);
    }
  }
  else{
    if(Data[0]=='!'){
      ShellCommand(Data); 
    }
    else{
      if(IsOSShell)
	Session=Add2Buffer(Session,Data,RUN(G1=StrCat(CommandPrefix,Data)));
      else
	Session=Add2Buffer(Session,Data,RUN(G1=StrCat(CommandPrefix,Wrap(Data))));
      if(G1)
	free(G1);
      if(!strncmp((char *)"core",G1=RUN((char *)"ls core 2> /dev/null"),4)){
	Segfaults++;
	ScrSession=Add2Scr(ScrSession,StrCat("",":SHELLERROR"),Source::Loaded);
	if(system("mkdir -p CORES; mv -f core CORES/ > /dev/null")){}
	SeshFirst->Error=ErrorType::Segfault;
      }
      else{
	SeshFirst->Error=ErrorType::Load;
      }
      if(G1)
	free(G1);
      Buffer2Scr(SeshFirst);
    }
  }
}

/*! \brief Load a batch file into WitchCraft and execute all the commands within it.
 *
 */
void WitchCraft::WC_Load(const char *Data){
  int _=0,Position=0,Len=0,Total=0,Progress=0;
  char *ToRUN=NULL,*G1=NULL,*Executed=NULL,*Command=NULL,*PI=(char *)malloc(7*sizeof(char )),*Old=NULL;
  ToRUN=StrCat(G1=StrCat("grep \"^<PERSON> \" ",Data)," | sed \"s/^.*<PERSON> //g\"");
  if(G1)
    free(G1);
  Executed=RUN(ToRUN);
  memset(PI,0,6);
  for(;Executed[_];_++)
    if(Executed[_]=='\n')
      ++Total;
  for(_=0;Executed[_];_++){
    if(Executed[_]=='\n'){
      Progress++;
      Old=(char *)malloc(((Len=(_-Position))+1)*sizeof(char));
      memcpy(Old,Executed+Position,Len);
      Old[Len]=0;
      char *FilteredChars=FilterChars(Old,(char *)"\t");
      Command=RemoveTrailingChars(FilteredChars,(char *)" ");
      if(FilteredChars){
	free(FilteredChars);
      }
      free(Old);
      sprintf(PI,"%3d%%",((100*Progress)/Total));
      DrawText(ScreenHeight-1,0,10,(char *)"EXE [%s] - ",PI);
      ExeCmdInScript(Command);
      add_history(Command);
      Position=_+1;
      DebugBuffer(ScrFirst);
    }
  }
  if(strlen(Executed+Position)){
    Old=(char *)malloc(((Len=(_-Position))+1)*sizeof(char));
    memcpy(Old,Executed+Position,Len);
    Old[Len]=0;
    char *FilteredChars=FilterChars(Old,(char *)"\t");      
    Command=RemoveTrailingChars(FilteredChars,(char *)" ");
    if(FilteredChars){
      free(FilteredChars);
    }
    free(Old);
    ExeCmdInScript(Command);
    add_history(Command);
  }
  CLS();
  DebugBuffer(ScrFirst);
}

/*! \brief Save the history buffer to disk.
 *
 */
void WitchCraft::SaveBuffer(const char *Data,bool Append){
  struct Buffer *Line=NULL;
  FILE *Write=NULL;
  const char *WriteAccess=NULL;
  if(Append)
    WriteAccess="a";
  else
    WriteAccess="w";

  if((Write=fopen(Data,WriteAccess))){
    for(Line=SeshLast;Line;Line=Line->Prev){
      if(Line->Error==ErrorType::DontSave){
	fprintf(Write,"<SYSTEM> %s\n",Line->Command);
	continue;
      }
      if(Line->Error==ErrorType::Normal){
	fprintf(Write,"<PERSON> %s\n",Line->Command);
      }
      if(Line->Error==ErrorType::Segfault){
	fprintf(Write,"<SYSTEM> %s\n","THE FOLLOWING COMMAND SEGFAULTS: ");
      }
      if(Line->Error==ErrorType::Load){
	fprintf(Write,"<PERSON> %s\n",Line->Command);
      }
      if((Line->Command[0])&&(Line->Error!=ErrorType::Load)){
	fprintf(Write,"<PERSON> %s\n",Line->Command);
      }
      if(Line->Answer[0]){
	fprintf(Write,"<ENiX>   %s\n",Line->Answer);
      }
    }
    fclose(Write);
  }
}

/*! \brief WitchCraft command menu. This calls commands prefixed with "!"
 *
 */
void WitchCraft::ShellCommand(char *Data){
  string Input(Data);
  Packet FakePacket=InitialisePacket(Input);
  Sequence<int> Seq1={};
  vector<string> ToProcess=Seq1.Convert2StrVec(Input);
  Session=Add2Buffer(Session,Data,StrCat("",""));
  ScrSession=Add2Scr(ScrSession,StrCat("",Data),Source::Loaded);
  if(strlen(Data)>5){
    if((!strncasecmp(Data,"!save",5))||(!strncasecmp(Data,"!load",5))){
      SeshFirst->Error=ErrorType::DontSave;
    }
    else{
      SeshFirst->Error=ErrorType::Load;
    }
  }
  else{
    SeshFirst->Error=ErrorType::Load;
  }
  CLIExecuteCommand(ToProcess,FakePacket);
}

/*! \brief Clear the screen.
 *
 */
void WitchCraft::CLS(void){
   printf(WC_CLS);
   DrawText(ScreenHeight-2,1,9,(char *)"%s",HorzLine);
   printf("\r");
   if(ServerMode)
     ServBG();
   if(ClientMode)
     ClientBG();
}

/*! \brief Add data to the history + screen buffer.
 *
 */
struct Buffer *WitchCraft::Add2Buffer(struct Buffer *Data,char *C,char *A){
   struct Buffer *New=(struct Buffer *)malloc(sizeof(struct Buffer));
   New->Command=C;
   New->Answer=A;
   New->Error=ErrorType::Normal;
   New->Next=Data;
   New->Prev=NULL;
   if(Data){
     Data->Prev=New;
   }
   else{
     SeshLast=New;
   }
   SeshFirst=New;
   return New;
}

/*! \brief Add data to the screen buffer.
 *
 */
struct FormattedLine *WitchCraft::Add2Scr(struct FormattedLine *Data,const char *C,Source Type){
  struct FormattedLine *New=(struct FormattedLine *)malloc(sizeof(struct FormattedLine));
  New->Data=const_cast<char *>(C);
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
void WitchCraft::LineWrap(char *Data,Source Type){
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
void WitchCraft::Buffer2Scr(struct Buffer *Data){
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
	LineWrap(Stuff,Source::Person);
      }
    }
    if(Stuff)
      free(Stuff);
    Stuff=(char *)malloc((_-OldP+1)*sizeof(char)); 
    memcpy(Stuff,Raw+OldP,(_-OldP));
    Stuff[_-OldP]=0; 
    LineWrap(Stuff,Source::Person);
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
	LineWrap(Stuff,Source::ENiX);
      }
    }
    if(Stuff)
      free(Stuff);
    Stuff=(char *)malloc((_-OldP+1)*sizeof(char)); 
    memcpy(Stuff,Raw+OldP,(_-OldP));
    Stuff[_-OldP]=0; 
    LineWrap(Stuff,Source::ENiX);
  }
}

/*! \brief Show the buffer on the screen.
 *
 */
void WitchCraft::DebugBuffer(struct FormattedLine *Data){
  Source LastType=Source::Unspecified;
  struct FormattedLine *Line=Data;
  for(size_t _=ScreenHeight-3;Line&&_;_--){
    if(LastType==Line->Type){
      if(Line->Type==Source::ENiX)
	DrawText(_,1,9,ENiXFORMAT,Line->Data);
      if(Line->Type==Source::Person){
	if(ServerMode)
	  DrawText(_,1,3,SERVERFORMAT,Line->Data);
	else
	  DrawText(_,1,5,PERSONFORMAT,Line->Data);
      }
      if(Line->Type==Source::Loaded){
	DrawText(_,1,5,SYSTEMFORMAT,Line->Data);
	//DrawText(_,1,15,SYSTEMFORMAT,StrCat("","THE FOLLOWING COMMAND SEGFAULTS: "));
      }
      if(Line->Type==Source::Ignore){}
      if(Line->Type==Source::Client)
	DrawText(_,1,15,SYSTEMFORMAT,Line->Data);
      LastType=Line->Type;
      Line=Line->Next;
    }
    else{
      DrawText(_,1,15,BlankLine,(char *)"");
      LastType=Line->Type;
    }     
  }
}

/*! \brief Calculate the length of the buffer.
 *
 */
unsigned long int WitchCraft::LengthofBuffer(struct Buffer *Data){
  unsigned long int Length=0;
  for(;Data;Data=Data->Next)
    Length++;
  return Length;
}

/*! \brief Low level draw text API.
 *
 */
void WitchCraft::DrawText(int X,int Y,int C,const char *Format,const char *Data){
   printf("\033[%02d;%02df\033[%02d;%02dm",X,Y,Colour1[C],Colour2[C]);
   if(Format&&Data)
     printf(Format,Data);
}

/*! \brief Run commands through ENiX.
 *
 */
char *WitchCraft::RUN(char *Data){
  char *Output=NULL;
  FILE *CMD_OUT=NULL;
  DrawText(ScreenHeight-2,10,11,(char *)" %s",(char *)"BUSY");
  fflush(stdout);
  if((CMD_OUT=popen(Data,"r"))){
    Output=Disk::LoadPipe(CMD_OUT);
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
void WitchCraft::ModeBar(){
  DrawText(ScreenHeight-2,1,9,(char *)"%s",HorzLine);
   if(ServerMode)
     ServBG();
   if(ClientMode)
     ClientBG();   
   DrawText(ScreenHeight-2,10,11,(char *)" %s",(char *)"IDLE");
   DrawText(ScreenHeight-1,1,10,(char *)"%s",(char *)"");
}

/*! \brief Handle the command line input.
 *
 */
void WitchCraft::Input(){
   char *Data=NULL,*G1=NULL;
   int sock=0;
   char *Returned=NULL;
   struct Blob *Received=NULL,*Sent=NULL;
   ModeBar();
   printf("\033[?25h");
   Data=RemoveFinalSpaces(readline((char *)"WITCHCRAFT - "));
   printf("\033[?25l");
   ModeBar();
   DrawText(ScreenHeight-1,0,10,(char *)"%s",(char *)"EXECUTING... ");
   if(Data){
      add_history(Data);
      if(Data[0]=='!')
	ShellCommand(Data);
      else{
	if(ClientMode){
	  vector<BinData> Binaries={};
	  vector<BinData> Strings={};
	  BinData InputInBin(Data,strlen(Data));
	  Strings.push_back(InputInBin);
	  Packet Outgoing(Strings,Binaries);
	  DrawText(ScreenHeight-2,10,11,(char *)" %s",(char *)"BUSY");
	  fflush(stdout);
	  Connection->Send(Outgoing.AllData);
	  BinData Test=Connection->Receive();
	  Packet Incoming(Test);
	  PresentReceivedData(StrCat("",Data),Incoming);
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
	   if(!strncmp((char *)"core",G1=RUN((char *)"ls core 2> /dev/null"),4)){
	     Segfaults++;
	     ScrSession=Add2Scr(ScrSession,StrCat((char *)"",(char *)":SHELLERROR"),Source::Loaded);
	     if(system("mkdir -p CORES; mv -f core CORES/ > /dev/null")){}
	     SeshFirst->Error=ErrorType::Segfault;
	   }
	   else{
	     SeshFirst->Error=ErrorType::Normal;
	   }
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
void WitchCraft::MainLoop(){
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
void WitchCraft::DetectOSMode(const char *Data){
   int flag=1;
   for(int _=0;Data[_];_++)
     if(Data[_]!=' ')
       flag=0;
   IsOSShell=flag;
}

/*! \brief Setup the shell.
 *
 */
void WitchCraft::Setup(){
  printf(WC_TITLE);
  
  LoadConfig();

  if(!CygwinMode)
    DetectTermSize();
  
  AlignRight=(char *)malloc(10*sizeof(char));
  memset(AlignRight,0,10);
  sprintf(AlignRight,"%%%ds",ScreenWidth-9);
  AlignLeft=(char *)malloc(10*sizeof(char));
  memset(AlignLeft,0,10);
  sprintf(AlignLeft,"%%-%ds",ScreenWidth-9);
  BlankLine=(char *)malloc((ScreenWidth+1)*sizeof(char));
  memset(BlankLine,' ',ScreenWidth);
  BlankLine[ScreenWidth]=0;
  ENiXFORMAT=StrCat((char *)"  <ENiX> ",AlignLeft);
  PERSONFORMAT=StrCat((char *)"<PERSON> ",AlignRight);
  SERVERFORMAT=StrCat((char *)"<SERVER> ",AlignRight);
  SYSTEMFORMAT=StrCat((char *)"<SYSTEM> ",AlignRight);
  HorzLine=(char *)malloc(ScreenWidth*sizeof(char));
  memset(HorzLine,'-',ScreenWidth);
  HorzLine[1]='[';
  HorzLine[14]=']';
  memcpy(HorzLine+2,"STATUS:      ",12);   
}

/*! \brief Handle WitchCraft exit, handle the trapped signals etc.
 *
 */
void WitchCraft::WitchSignal(){
  if(!ClientMode){
    printf("\rExiting, please wait... ");
    fflush(stdout);

    puts("Saved :)");
    printf("\033[?25h");
  }
  else{
    printf("\033[?25h");
  }
  printf(WC_RESET);
  ThisInstance->~WitchCraft();
  exit(0);
}

/*! \brief remove trailing spaces from scripts to stop formatting issues
 *
 */
char *WitchCraft::RemoveTrailingChars(char *String,char *ToRemove){
  if(!ToRemove)
    return strdup(String);
  if(!String)
    return strdup((char *)"");
  char *NewString=strdup(String);
  size_t Length=strlen(String);
  if(!Length)
    return strdup((char *)"");
  for(size_t _=Length;_>0;_--)
    if(strchr(ToRemove,NewString[_]))
      NewString[_]=0;
    else
      break;
  return NewString;
}

/*! \brief Detect the terminal size.
 *
 */
void WitchCraft::DetectTermSize(){
  struct winsize WindowSize;
  if(ioctl(STDIN_FILENO,TIOCGWINSZ,&WindowSize)==-1){
    puts("ioctl");
  }
  ScreenWidth=WindowSize.ws_col;
  ScreenHeight=WindowSize.ws_row;
}

/*! \brief Bootstrap the WitchCraft shell.
 *
 */
int main(int argc,char *argv[]){
  WitchCraft Init;

  Init.Setup();

  signal(SIGINT,WitchCraft::WitchHandler);
  signal(SIGWINCH,WitchCraft::SetupHandler);

  ThisInstance=&Init;

  Init.CLS();
  Init.DetectOSMode(CommandPrefix);
  printf("\033[?25l");
  if(system("mkdir -p CORES; mv -f core CORES/")){}
  Init.MainLoop();
  printf(WC_RESET);
  return 0;
}
