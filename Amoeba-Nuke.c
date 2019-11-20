/*! \file */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 

#include "Amoeba-Colony.h"
#include "Amoeba-Cytoskelenton.h"
#include "Amoeba-Nuke.h"

/*
extern struct Colony *Nucleii[2];
*/

extern int AllowQueen;

/*! \brief This function is for debugging only it removes the Amoeba colony from SHM after use. This is the function's sole purpose.
 *
 */
int main(){
  int _=0;
  struct Colony Middens[2];
  for(_=0;_<2;_++){
    AllowQueen=1;
    AL1_CreateColony(&Middens[_]);
    AL1_DeleteColony(&Middens[_]);
  }
  puts("SHM CLEARED.");
  return 0;
}
