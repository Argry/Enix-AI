/*! \file */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Amoeba-Cytoskelenton.h"
#include "Amoeba-File.h"

/*! \brief Open a file and save to a string.
 *
 */
struct Blob *GetFile(char *Name){
  FILE *F=NULL;
  unsigned long int Size=0;
  struct Blob *R=(struct Blob *)malloc(sizeof(struct Blob));
  if(!(F=fopen(Name,"r"))){
    puts("Error");
    return NULL;
  }
  else{
    fseek(F,0,SEEK_END);
    Size=ftell(F);
    fseek(F,0,SEEK_SET ); 
    R->Nucleus=(char *)malloc((Size)*sizeof(char));
    fread(R->Nucleus,Size,1,F);
    R->Size=Size;
    fclose(F);
  }  
  printf("Loaded %ld bytes\n",Size);
  return R;
}

/*! \brief Save a string to a file.
 *
 */
void PutFile(char *Name,struct Blob *Data){
  FILE *F=NULL;
  if(!(F=fopen(Name,"w"))){
    puts("Error");
  }
  else{
    fwrite(Data->Nucleus,Data->Size,1,F);
    fclose(F);
  }
}
