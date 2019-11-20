/*! \file */

#ifndef WITCHCRAFT_MAIN
#define WITCHCRAFT_MAIN

/*
  GPL [OvO]wl 2010
*/

#include "ENiX_Packet.h"
#include "ENiX_Net.h"

using namespace std;

enum ErrorType{
  Segfault=0,
  Normal,
  Load,
  DontSave
};

enum Source{
  Person=0,
  ENiX,       //1
  Loaded,     //2
  Ignore,     //3
  Client,     //4
  Unspecified //5
};

struct Buffer {
  char                 *Command;
  char                 *Answer;
  ErrorType            Error;
  struct Buffer        *Prev;
  struct Buffer        *Next;
};

struct FormattedLine {
  char                 *Data;
  Source               Type;
  struct FormattedLine *Prev;
  struct FormattedLine *Next;
};

class WitchCraft;
static WitchCraft    *ThisInstance;

class WitchCraft{
 public:
  /* Instruction set: */
  WitchCraft();
  ~WitchCraft();
  
  void InitialiseInstructions(void);
  
  typedef Packet (WitchCraft::*ENiXInstructionSet)(vector<string> Commands,Packet Data);
  Packet CLIExecuteCommand(vector<string> Commands,Packet Data);
  
  Packet NetExecuteCommand(vector<string> Commands,Packet Data);

  Packet Clear(vector<string> Commands,Packet Data);
  Packet Client(vector<string> Commands,Packet Data);
  Packet CmdPrefix(vector<string> Commands,Packet Data);
  Packet Disconnect(vector<string> Commands,Packet Data);
  Packet Help(vector<string> Commands,Packet Data);
  Packet Load(vector<string> Commands,Packet Data);
  Packet Quit(vector<string> Commands,Packet Data);
  Packet Redraw(vector<string> Commands,Packet Data);
  Packet Save(vector<string> Commands,Packet Data);
  Packet SetConfig(vector<string> Commands,Packet Data);
  Packet Slow(vector<string> Commands,Packet Data);

  Packet Test(vector<string> Commands,Packet Data);

  vector<ENiXInstructionSet> InstructionSet;

  bool CmdSetInitialised;

  vector<int>    Parameters;
  vector<string> CommandName;
  vector<string> OnlineHelp;

  Packet InitialisePacket(string Response);

  /* Interface */
  void                 DetectTermSize();
  struct Buffer        *Add2Buffer(struct Buffer *Data,char *C,char *A);
  struct FormattedLine *Add2Scr(struct FormattedLine *Data,const char *C,Source Type);
  void                 DebugBuffer(struct FormattedLine *Data);
  unsigned long int    LengthofBuffer(struct Buffer *Data);
  void                 Buffer2Scr(struct Buffer *Data);
  void                 DrawText(int X,int Y,int C,const char *Format,const char *Data);
  void                 CLS(void);
  void                 Clear(void);
  void                 Input();
  void                 LineWrap(char *Data,Source Type);
  char                 *RUN(char *Data);
  void                 ShellCommand(char *Data);
  void                 SaveBuffer(const char *Data,bool Append=false);
  void                 WC_Load(const char *Data);
  void                 ExeCmdInScript(char *Data);
  void                 Splash();
  void                 MainLoop();
  char                 *Wrap(char *Data);
  void                 ModeBar();
  void                 PresentReceivedData(char *Command,Packet Source,size_t StartAt=1);

  void                 DetectOSMode(const char *Data);

  void                 ServConsUpdate();

  void                 ServBG();
  void                 ClientBG();

  void                 WitchCraftTest(char *Directory);

  char                 *RemoveFinalSpaces(char *RAW);

  void                 WitchSignal();
  void                 Setup();

  static void          WitchHandler(int Sig){
    ThisInstance->WitchSignal();
  };
  static void          SetupHandler(int Sig){
    ThisInstance->Setup();
  };

  void                 SaveConfig();
  void                 LoadConfig();
  void                 LoadHistory(const char *HistoryFile);

  /* Legacy stuff which should be replaced. */
  char                 *StrCat(const char *S1,const char *S2);
  char                 *FilterChars(char *String,char *Filter);
  char                 *RemoveTrailingChars(char *String,char *ToRemove=(char *)" ");

 private:
  string               HostName;
  int                  PortNum;
  string               ProtocolName;
  Networking           *Connection; 
  Networking           *ServerConn;
  void                 RequestHelp(Networking *Net);
  Packet               Preprocess(char *Command);
};

#endif
