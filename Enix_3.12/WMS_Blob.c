/*! \file */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "WMS_Blob.h"


/*! \brief Convert binary data into a blob.
 *
 */
struct Blob *WMS_Bin2Blob(unsigned long int BinLen,char *Str){
  struct Blob *R;
  R=(struct Blob *)malloc(sizeof(struct Blob ));
  R->Size=BinLen;
  R->Nucleus=(char *)malloc((BinLen+1)*sizeof(char));
  if(!Str){
    memset(R->Nucleus,'\0',BinLen);
  }
  else{
    memcpy(R->Nucleus,Str,BinLen);
    R->Nucleus[BinLen]=0;
  }
  return R;
}

/*! \brief Determine if two blobs are equal.
 *
 */
int WMS_BlobSame(struct Blob *B1,struct Blob *B2){
  unsigned long MinLen=0;
  if(B1->Size!=B2->Size)
    return 0;
  MinLen=(B1->Size>B2->Size)?B2->Size:B1->Size;
  if(memcmp(B1->Nucleus,B2->Nucleus,MinLen))
    return 0;
  return 1;
}

/*! \brief Convert a string into a blob.
 * 
 */
struct Blob *WMS_Str2Blob(char *Str){
  /* DO NOT MODIFY */
  unsigned long int Size=0;
  struct Blob *B=NULL;
  B=(struct Blob *)malloc(sizeof(struct Blob));
  if(Str){
    Size=strlen(Str);
    B->Size=Size;
    B->Nucleus=(char *)malloc((Size+1)*sizeof(char));
    memcpy(B->Nucleus,Str,Size);
    B->Nucleus[Size]=0;
  }
  else{
    B->Size=0;
    B->Nucleus=NULL;
  }
  return B;
}

/*! \brief Convert a blob into a string.
 * 
 */
char *WMS_Blob2Str(struct Blob *B){
  char *R=NULL;
  if(B){
    R=(char *)malloc((B->Size+1)*sizeof(char));
    memcpy(R,B->Nucleus,B->Size);
    R[B->Size]=0;
    return R;
  }
  return NULL;
}

/*! \brief Free up a blob.
 * 
 */
void WMS_FreeBlob(struct Blob *B){
  if(B){
    free(B->Nucleus);
    free(B);
  }
}

/*! \brief Show a blob data.
 *  - For debugging only.
 */
void WMS_BlobDEBUG(struct Blob *B){
  int Sectors=0,Terminated=0;
  unsigned long int _=0;
  if(B){
    printf("DEBUG Blob (size: %ld): [",B->Size);
    for(_=0;_<B->Size;_++){
      switch(B->Nucleus[_]){
      case 0:
	putchar('^');
	break;
      case 1:
	putchar('|');
	Sectors++;
	break;
      case 2:
	putchar(']');
	Terminated=1;
	break;
      default:
	putchar(B->Nucleus[_]);
      }
      fflush(stdout);
    }
    putchar('\n');
    if((Sectors!=3||!Terminated)&&B->Size>32){
      printf("ERROR: blob looks corrupt! (%d/%d)\n",Sectors,Terminated);
      exit(0);
    }
  }
}


/*! \brief Check to see if a blob looks fishy.
 *  - For debugging only.
 */
void WMS_BlobCheck(struct Blob *B){
  int Sectors=0,Terminated=0;
  unsigned long int _=0;
  if(B){
    for(_=0;_<B->Size;_++){
      switch(B->Nucleus[_]){
      case 1:
	Sectors++;
	break;
      case 2:
	Terminated=1;
	break;
      }
      fflush(stdout);
    }
    if((Sectors!=3||!Terminated)&&B->Size>32){
      WMS_BlobDEBUG(B);
      printf("ERROR: blob looks corrupt! (%d/%d)\n",Sectors,Terminated);
      exit(0);
    }
  }
}
