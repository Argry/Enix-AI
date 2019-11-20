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
                                        

Programmed By M.S.Braithwaite 2019.

*/

#ifndef ENiXSEMANTICS
#define ENiXSEMANTICS

#include "ENiX_Memory.h"

// These functions call into Memory *GlobalMemoryAccess.

class Semantics{
 public:

  enum Overlap{
    None=0,
    One,
    AllOld,
    AllNew,
    Partial,
    Full
  };
  
  static bool ConditionalOverlap(NLPStoreLogic::SLogic SpokenLogic,vector<string> &SpokenContext,string &CSRec);
  static bool ConditionalOverlapStates[6][4][4];
  static Semantics::Overlap SemanticOverlap(vector<string> &Old,vector<string> &New);
  
};

#endif

/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
*/
