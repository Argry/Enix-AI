/*! \file */

#ifndef WITCHCRAFT_MAIN
#define WITCHCRAFT_MAIN

/*
  GPL [OvO]wl 2010
*/

struct Buffer {
  char                 *Command;
  char                 *Answer;
  int                  Error; /* 1=segfault 0=normal 2=load*/
  struct Buffer        *Prev;
  struct Buffer        *Next;
};

struct FormattedLine {
  char                 *Data;
  int                  Type;  /* 0=person 1=enix */
  struct FormattedLine *Prev;
  struct FormattedLine *Next;
};

void                 DetectTermSize();
struct Buffer        *Add2Buffer(struct Buffer *Data,char *C,char *A);
struct FormattedLine *Add2Scr(struct FormattedLine *Data,char *C,int Type);
void                 DebugBuffer(struct FormattedLine *Data);
unsigned long int    LengthofBuffer(struct Buffer *Data);
void                 Buffer2Scr(struct Buffer *Data);
void                 DrawText(int X,int Y,int C,char *Format,char *Data);
void                 CLS(void);
void                 Clear(void);
void                 Input();
void                 LineWrap(char *Data,int Type);
char                 *RUN(char *Data);
void                 Command(char *Data);
void                 SaveBuffer(char *Data);
void                 WC_Load(char *Data);
void                 Execute(char *Data);
void                 Splash();
void                 MainLoop();
char                 *Wrap(char *Data);
void                 ModeBar();

void                 DetectOSMode(char *Data);
void                 Setup(int Signal);

void                 RunServer();
void                 Incoming(int sock);
void                 HandleTCPClient(int Socket);

void                 WitchSignal(int Sig);

void                 RunServer();
void                 ServConsUpdate();

void                 ServBG();
void                 ClientBG();

void                 WitchCraftTest(char *Directory);

char                 *RemoveFinalSpaces(char *RAW);

void                 SaveConfig();
void                 LoadConfig();


#endif
