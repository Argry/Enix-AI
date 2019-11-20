
/*! \file */

#include "Amoeba.h"

extern const char *FILE1;
extern const char *FILE2;

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 

#include "Amoeba-Colony.h"
#include "Amoeba-Cytoskelenton.h"
#include "Amoeba-Nucleus.h"
#include "Amoeba-Microtubules.h"

extern struct Colony *Nucleii[2];

/*! \brief Create the main handler for Amoeba. 
 *
 */
int main(){

  MainHandler();

  /*  
      DeleteColony(Nucleii[0]);
      DeleteColony(Nucleii[1]);
  */

  return 0;
}
