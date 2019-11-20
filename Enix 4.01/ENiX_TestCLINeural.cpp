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

#include "ENiX_CORE.h"
#include "ENiX_Awareness.h"

Packet Application::ExecuteCommand(BinData Inp){}
string PROTOCOL_PREFIX="UNUSED";

void Application::Splash(void){}
void ServerSignal(int SignalData){}

int main(int argc,char **argv){
  if(argc<2){
    puts("This binary is for code demonstration only. Do not use this unless you are");
    puts("testing capabilities for inclusion in other applications.");
    puts("Please consult source code for how to use, or use one of the public binaries.");
    return 0;
  }

  Awareness Oddman(argc,argv);
  Oddman.ExampleUsage();

  /*
  ENiX_NN New(argc,argv);
  New.ExampleUsage();
  */

  return 0;
}



/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
 */
