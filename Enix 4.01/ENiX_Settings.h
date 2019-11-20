/*! \file */

/*

This file is part of ENiX4.

ENiX4 is free software you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

ENiX4 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ENiX4.  If not, see <http://www.gnu.org/licenses/>.

  __    _______   ___ _  __    __ ____  
 / /   / ____/ | / (_) |/ /   / // /\ \ 
/ /   / __/ /  |/ / /|   /   / // /_ \ \
\ \  / /___/ /|  / //   |   /__  __/ / /
 \_\/_____/_/ |_/_//_/|_|     /_/   /_/ 
                                        


Programmed By M.S.Braithwaite 2014.

*/

#ifndef ENiXSETTINGS
#define ENiXSETTINGS

#include<string>

using namespace std;

/*****************************************/
/*   Length of hex pairs in printouts    */
/*****************************************/
#define HEXLENGTH           20

/*****************************************/
/*        1: Run as linux daemon         */
/*        0: Run in foreground           */
/*****************************************/
#define DAEMON_MODE         0

/*****************************************/
/*        1: Use ANN Tree Topology       */
/*        0: Use ANN Step Topology       */
/*****************************************/
#define ENABLETREE          1

/*****************************************/
/*    0: Dont Extend ANN Dependencies    */
/*    1: Extend ANN between Min and Max  */
/*    2: Extend ANN between 0 and Max    */
/*    3: Extend ANN between 0 and Cap    */
/*****************************************/
#define EXTENDDEPS          3

/* Maximum dependencies limit ************/
#define DEPMAX              8

/* Verbosity of core system processing ***/
#define VERBOSECORE         0

/*****************************************/
/* Local process distribution            */
/* Do NOT set this to less than 1...     */
/* If you do, bad stuff will happen!     */
/*****************************************/
#define TOTALTHREADS        2

/*****************************************/
/* Enable debug for server this will     */
/* enable multiple process handling for  */
/* server connection processing          */
/* 0: Enable multiple processes          */
/* 1: Turn off multiple processes        */
/*****************************************/
#define DEBUGSERVER         0

/*****************************************/
/* Neural Network Server IPC Controls    */
/* Make sure NNSHMSIZE is large enough   */
/* to contain the map data!              */ 
/*****************************************/
#define NNSHMSIZE           1024
#define NNSHMKEY            575
#define NNACCESS            0666
#define SPINTIME            5000
#define POLLINTERVAL        100000

/*****************************************/
/* Amoeba Database Configuration         */
/*****************************************/
#define SEARCHTHREADS       2
#define AMOEBAPORTNUM       575
#define AMOEBAHOSTNAME      "localhost"
#define AMOEBADBSIZE        33554432
#define AMOEBAALLOCSIZE     16777216
#define AMOEBAOFFSET        128
#define AMOEBASHMKEY        666
#define AMOEBABACKUP        "AMOEBA_Backup_"
#define AMOEBAPARTITION     false

/*****************************************/
/* Chocky Configuration                  */
/*****************************************/
#define CHOCKYPORTNUM       576
#define CHOCKYHOSTNAME      "localhost"

/*****************************************/
/* CDS Configuration                     */
/*****************************************/
#define CDSPORTNUM          577
#define CDSHOSTNAME         "localhost"
#define CDSSHMKEY           667
#define CDSDBSIZE           262144
#define CDSALLOCSIZE        131072
#define CDSOFFSET           128
#define CDSBACKUP           "CDS_Backup_"
#define MAXPEERS            1000

/*****************************************/
/* NN Processor Configuration            */
/*****************************************/
#define NNPPORTNUM          578
#define NNPHOSTNAME         "localhost"

/*****************************************/
/* WitchCraft Configuration              */
/*****************************************/
#define WITCHCRAFTPORTNUM   579
#define WITCHCRAFTHOSTNAME  "localhost"

/*****************************************/
/* System Limits                         */
/*****************************************/
#define MAXHOSTNAME         64
#define MAXUSERNAME         32

/*****************************************/
/* Debugging Assistance                  */
/*****************************************/
#define DEBUGMSG            printf("DEBUG: %s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);

/*****************************************/
/* SHM Cached Configuration              */
/*****************************************/

class SHMConfig{
 public:
  SHMConfig(){};
  SHMConfig(string BackupSrc,size_t AllocSize,size_t Offset,int SHMKey,size_t DBSize){
    BACKUP=BackupSrc;
    DBALLOCSIZE=AllocSize;
    DBOFFSET=Offset;
    DBSHMKEY=SHMKey;
    DBSIZE=DBSize;
  }
  string BACKUP;
  size_t DBALLOCSIZE;
  size_t DBOFFSET;
  int    DBSHMKEY;
  size_t DBSIZE;
};

#endif

/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
 */
