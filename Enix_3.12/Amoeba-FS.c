/*! \file */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <signal.h>
#include <unistd.h>

#include "ENiX_STRING.h"
#include "ENiX_LIST.h"
#include "ENiX_WMS.h"
#include "Amoeba-File.h"
#include "Amoeba-Colony.h"
#include "Amoeba-Cytoskelenton.h"
#include "Amoeba-Microtubules.h"
#include "Amoeba-Nucleus.h"

#include "Amoeba-FS.h"



extern struct Colony *Nucleii[2];
extern struct Colony Middens[2];

/*! \brief Displays filesystem help.
 *
 */
void Help(char *Name){
  printf("%s          [HELP] - This screen\n",Name);
  printf("%s CONTENTS        - Display contents\n",Name);
  printf("%s GET     <FILES> - Get files from Amoeba\n",Name);
  printf("%s GET     ALL     - Get all files from Amoeba\n",Name);
  printf("%s STORE   <FILES> - Add files to Amoeba\n",Name);
  printf("%s VIEW    <FILES> - Show file contents\n",Name);
}

/*! \brief Displays filesystem contents.
 *
 */
void Contents(){
  struct Datablock *DB=NULL;
  Amoeba_Con();
  DB=FSContents("CONTENTS");
  if(DB)
    Amoeba_P_DB_Entry(DB,0);
  else
    puts("Amoeba contains no files.");
  Amoeba_Dis();
}

/*! \brief Obtains the contents in WMS format.
 *
 */
struct Datablock *FSContents(char *Name){
  unsigned long int _=0;
  _=AL1_FindDBEntry(Nucleii[0],CreateBlob(strlen(Name),Name));
  if(_+1)
    return Amoeba2WMS(Nucleii[0],_);
  else
    return NULL;
}

/*! \brief Connect to SHM.
 *
 */
void Amoeba_Con(){
  int _=0;
  for(_=0;_<2;_++){
    AL1_CreateColony(&Middens[_]);
    AL1_AttachColony(&Middens[_]);
    AL1_ReadHeader(&Middens[_]);
    /*
      Nucleii[_]=&Middens[_];
    */
  }
}

/*! \brief Disconnect from SHM.
 *
 */
void Amoeba_Dis(){
  int _=0;
  for(_=0;_<2;_++)
    AL1_DetachColony(&Middens[_]);
}

/*! \brief Main handler for processing filesystem control.
 *
 */
int main(int argc, char *argv[]){
  unsigned long int _=0,Pos=0;
  struct Blob *D=NULL;
  struct Datablock     *DB=NULL;
  struct Cytoskelenton *C=NULL;
  struct List_Str      *L1,*L2;
  if(argc==1){
    Help(argv[0]);
  }
  else{
    if(!strcasecmp(argv[1],"help"))
      Help(argv[0]);
    if(!strcasecmp(argv[1],"contents"))
      Contents();
    if((!strcasecmp(argv[1],"store"))&&(argc>2)){
      Amoeba_Con();
      DB=FSContents("CONTENTS");
      if(!DB)
	DB=Add_DB_Entry(DB,"CONTENTS");
      for(_=2;_<argc;_++){
	printf("Adding %s... ",argv[_]);
	D=GetFile(argv[_]);
	Pos=NewRec(Nucleii[0],D);
	Add_DB_Pair(DB,argv[_],FloatToString((float)Pos));
	C=AL1_GetDBEntry(Nucleii[0],Pos);
      }
      WMS2Amoeba(Nucleii[0],DB);
      Amoeba_Dis();
    }
    if((!strcasecmp(argv[1],"get"))&&(argc>2)){
      Amoeba_Con();
      DB=FSContents("CONTENTS");
      if(!strcasecmp(argv[2],"all")){
	L1=DB->DS[1];
	for(L2=DB->DS[2];L2;L2=L2->Next){
	  D=AL1_GetRecName(Nucleii[0],(unsigned long)atof(L2->Str));
	  if(D)
	    PutFile(L1->Str,D);
	  L1=L1->Next;
	}
      }
      else{
	for(_=2;_<argc;_++){      
	  Pos=(unsigned long)atof(Lookup_DB_Attr(DB,argv[_],NULL));
	  D=AL1_GetRecName(Nucleii[0],Pos);
	  if(D)
	    PutFile(argv[_],D);
	}
      }
      Amoeba_Dis();
    }
    if((!strcasecmp(argv[1],"view"))&&(argc>2)){
      Amoeba_Con();
      DB=FSContents("CONTENTS");
      for(_=2;_<argc;_++){      
	Pos=(unsigned long)atof(Lookup_DB_Attr(DB,argv[_],NULL));
	D=AL1_GetRecName(Nucleii[0],Pos);
	if(D)
	  puts(D->Nucleus);
      }
      Amoeba_Dis();
    }
  }
  return 0;
}
