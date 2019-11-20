#ifndef WMS_BLOB
#define WMS_BLOB

#include "ENiX_BDEFS.h"

struct Blob       *WMS_Str2Blob(char *Str);
struct Blob       *WMS_Bin2Blob(unsigned long int BinLen,char *Str);
int                WMS_BlobSame(struct Blob *B1,struct Blob *B2);
char              *WMS_Blob2Str(struct Blob *B);
void               WMS_FreeBlob(struct Blob *B);
void               WMS_BlobDEBUG(struct Blob *B);

#endif
