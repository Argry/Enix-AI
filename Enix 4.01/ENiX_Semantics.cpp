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

#include "ENiX_Semantics.h"

/*

  NLPStoreLogic::SLogic
  AND,OR,NOR,XOR

  Horizontal is old
  Vertical is new
  --------------------->  \
  |                        |
  |                        |
  |                        | Semantics::Overlap
  |                        | None,One,AllOld,AllNew,Partial,Full
  |                        |
  |                        |
  V                       /

 */

bool Semantics::ConditionalOverlapStates[6][4][4]={
  {
    // Overlap::None
    {false,false,false,false},
    {false,false,false,false},
    {false,false,false,false},
    {false,false,false,false},
  },
  {
    // Overlap::One
    {true ,true, false,false},
    {true ,true, false,true },
    {false,false,true, false},
    {false,true, false,true },
  },
  {
    // Overlap::AllOld
    {false,false,false,false},
    {false,true, false,true },
    {false,false,true, false},
    {false,false,false,true },
  },
  {
    // Overlap::AllNew
    {false,false,false,false},
    {false,true, false,false},
    {false,false,true, false},
    {false,true, false,true },
  },
  {
    // Overlap::Partial
    {false,false,false,false},
    {false,true, false,false},
    {false,false,true, false},
    {false,false,false,true },
  },
  {
    // Overlap::Full
    {true, false,false,false},
    {false,true, false,false},
    {false,false,true, false},
    {false,false,false,true },
  },
};

bool Semantics::ConditionalOverlap(NLPStoreLogic::SLogic SpokenLogic,vector<string> &SpokenContext,string &CSRec){
  vector<string> CSCauses=GlobalMemoryAccess->CSDeref(CSRec);
  if(!CSCauses.size())
    return false;
  NLPStoreLogic::SLogic RecCauseLogic=NLPStoreLogic::GetLogicCode(CSCauses[0]);
  CSCauses.erase(CSCauses.begin());
  Semantics::Overlap CauseOverlap=Semantics::SemanticOverlap(CSCauses,SpokenContext);

  /*
  DEBUGMSG;
  List::PrintList(CSCauses);
  List::PrintList(SpokenContext);
  printf("Overlap code: %d, NewLog: %d, OldLog: %d => Result: %d\n",(int)CauseOverlap,(int)SpokenLogic,(int)RecCauseLogic,(int)Semantics::ConditionalOverlapStates[(int)CauseOverlap][(int)SpokenLogic][(int)RecCauseLogic]);
  */
  
  return Semantics::ConditionalOverlapStates[(int)CauseOverlap][(int)SpokenLogic][(int)RecCauseLogic];
}

Semantics::Overlap Semantics::SemanticOverlap(vector<string> &Old,vector<string> &New){
  size_t OldSize=Old.size();
  size_t NewSize=New.size();
  size_t OverlapSize=List::SetTheoryIntersect(Old,New).size();
  if(!OverlapSize)
    return Semantics::Overlap::None;
  if(OverlapSize==1)
    return Semantics::Overlap::One;
  if((OverlapSize==OldSize)&&(OverlapSize==NewSize))
    return Semantics::Overlap::Full;
  if((OverlapSize==OldSize))
    return Semantics::Overlap::AllOld;
  if((OverlapSize==NewSize))
    return Semantics::Overlap::AllNew;
  return Semantics::Overlap::Partial;
}

/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
*/
