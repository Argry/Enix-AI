#ifndef WMS_SHM
#define WMS_SHM

#include <sys/shm.h> 
#include "ENiX_BDEFS.h"

#define SHMSIZE       10485760
#define FATSIZE       3000000
#define FATSTART      200
#define SHMKEY        575
#define ALLACCESS     666
#define SHMSPACING    0
#define FRAGMENTATION 0
#define SPINTIME      5000
#define MAXTHREADS    2
#define ALLOCSECTIONS 4

struct DataArea{
  char                LockAlloc;
  int                 Sector;
  unsigned long int   Size;
  key_t               Key;
  char                *Shared;
  int                 Queen;
  unsigned long int   CacheProb;
  unsigned long int   CacheUnkn;
  unsigned long int   CacheDate;
  char                *CachedTime;
  unsigned long int   Population;
  char                *StartPos;
  char                *FinishPos;
  char                *Cached;
  char                *Lock;
  unsigned long int   LastPos;
  int                 SHM_ID; 
  struct shmid_ds     *shmid_ds;
};

struct DataArea Sector[2];

void               MainHandler();
void               Queen();
void               Worker();

struct DataArea   *WMS_SHM_Create(unsigned int Sector);
unsigned int       WMS_SHM_Delete(struct DataArea *DB);
unsigned int       WMS_SHM_Attach(struct DataArea *DB);
unsigned int       WMS_SHM_Detach(struct DataArea *DB);

void               WMS_SHM_Debug(struct DataArea *DB);

unsigned int       WMS_SHM_Clear();

unsigned int       WMS_Defrag(struct DataArea *DB);

long int           WMS_AddBinary(struct DataArea *DB,struct Blob *B);
unsigned int       WMS_DelBinary(struct DataArea *DB,unsigned long int Position);
unsigned long int  WMS_RepBinary(struct DataArea *DB,unsigned long int Position,struct Blob *B);
struct Blob       *WMS_GetBinary(struct DataArea *DB,unsigned long int Position);

struct Blob       *WMS_WMS2Blob(struct Datablock *WMS);
struct Datablock  *WMS_Blob2WMS(struct Blob *B);

char              *WMS_GetRecName(struct Blob *B);
long int           WMS_FindRec(struct DataArea *DB,char *RecordName);

unsigned int       WMS_Save_To(struct DataArea *DB,char *FileName);
struct DataArea   *WMS_Read_From(struct DataArea *DB,char *FileName,unsigned int Sector);

void               WMS_LockRec(struct DataArea *DB,unsigned long int Position);
void               WMS_UnlockRec(struct DataArea *DB,unsigned long int Position);

void               WMS_LockDB(struct DataArea *DB);
void               WMS_UnlockDB(struct DataArea *DB);

#endif
