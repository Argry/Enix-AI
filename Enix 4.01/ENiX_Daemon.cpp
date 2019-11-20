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
                                        


Programmed By M.S.Braithwaite 2018.

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include "ENiX_Daemon.h"
#include "ENiX_Settings.h"

Daemon::Daemon(){
  if(DAEMON_MODE){
    pid_t Pid=fork();
    if(Pid<0)
      exit(EXIT_FAILURE);
    if(Pid>0)
      exit(EXIT_SUCCESS);
    if(setsid()<0)
      exit(EXIT_FAILURE);
    signal(SIGCHLD,SIG_IGN);
    signal(SIGHUP,SIG_IGN);
    Pid=fork();
    if(Pid<0)
      exit(EXIT_FAILURE);
    if(Pid>0)
      exit(EXIT_SUCCESS);
    umask(0);
    chdir("/");
    /*
    int FDId;
    for(FDId=sysconf(_SC_OPEN_MAX);FDId;--FDId){
      close(FDId);
    }
    */
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
  }
}

Daemon::~Daemon(){
  if(DAEMON_MODE){
    exit(EXIT_SUCCESS);
  }
}
