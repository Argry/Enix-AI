#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "WMS_Blob.h"

struct Blob *WMS_Bin2Blob(unsigned long int BinLen,char *Str){
  struct Blob *R=(struct Blob *)malloc(sizeof(struct Blob ));
  if(BinLen<0||BinLen>10000){
    printf("CreateBlob Size: %ld. Looks weird, aborting!!\n",BinLen);
    exit(1);
  }
  R->Size=BinLen;
  R->Nucleus=Str;
  return R;
}

int WMS_BlobSame(struct Blob *B1,struct Blob *B2){
  unsigned long MinLen=0;
  if(B1->Size!=B2->Size)
    return 0;
  MinLen=(B1->Size>B2->Size)?B2->Size:B1->Size;
  if(memcmp(B1->Nucleus,B2->Nucleus,MinLen))
    return 0;
  return 1;
}

struct Blob *WMS_Str2Blob(char *Str){
  struct Blob *B=NULL;
  if(Str){
    B=(struct Blob *)malloc(sizeof(struct Blob));
    B->Size=strlen(Str);
    B->Nucleus=strdup(Str);
    return B;
  }
  return NULL;
}

char *WMS_Blob2Str(struct Blob *B){
  char *R=NULL;
  if(B){
    R=(char *)malloc((B->Size+1)*sizeof(char));
    memcpy(R,B->Nucleus,B->Size);
    B->Nucleus[B->Size]=0;
    return R;
  }
  return NULL;
}

void WMS_FreeBlob(struct Blob *B){
  free(B->Nucleus);
  free(B);
}

void WMS_BlobDEBUG(struct Blob *B){
  unsigned long int _=0;
  if(B){
    printf("DEBUG Blob (size: %ld): [",B->Size);
    for(_=0;_<B->Size;_++){
      switch(B->Nucleus[_]){
      case 0:
	putchar(' ');
	break;
      case 1:
	putchar('!');
	break;
      case 2:
	putchar(']');
	break;
      default:
	putchar(B->Nucleus[_]);
      }
      fflush(stdout);
    }
    putchar('\n');
  }
}

