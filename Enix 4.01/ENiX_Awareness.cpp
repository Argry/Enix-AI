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

#include "ENiX_Settings.h"
#include "ENiX_Awareness.h"

Awareness::Awareness(int argc,char **argv){
  Sequence<int> ConvertInput(argc,argv);
  InitialData=ConvertInput;
  OddmanTPI=-1;
  Understood=0;
}

Awareness::Awareness(Sequence<int> S){
  InitialData=S;
  OddmanTPI=-1;
  Understood=0;
}

Awareness::~Awareness(){
}

/* this will have to be expanded to be more distributed */
int Awareness::OddmanOut(){
  bool First=1;
  int DataSize=InitialData.Size(),TPI=0;
  unsigned long int Deps=0,Comp=0,MinDeps=0,MinComp=0,PositionFound=-1;
  Sequence<int> Complexity(DataSize);
  Sequence<int> Dependencies(DataSize);
  for(TPI=0;TPI<DataSize;TPI++){
    Sequence<int> NeuralInput=InitialData;
    NeuralInput.UnsetValue(TPI);
    ENiX_NN *ANN = new ENiX_NN(NeuralInput);
    Neurals.push_back(*ANN);
    delete(ANN);
  }
  TPI=0;
  for(auto ANN:Neurals){
    ANN.DependencyCheck(1,1);
    ANN.Learn();    
    Comp=ANN.GetTotalComplexity();
    Deps=ANN.GetTotalDeps();
    if(First){
      First=0;
      MinDeps=Deps;
      MinComp=Comp;
      PositionFound=TPI;
    }
    else{
      if(Deps<MinDeps){
	MinDeps=Deps;
	MinComp=Comp;
	PositionFound=TPI;
      }
      else if(Deps==MinDeps)
	if(Comp<MinComp){
	  MinComp=Comp;
	  PositionFound=TPI;
	}
    }
    Complexity.SetValue(Comp,TPI);
    Dependencies.SetValue(Deps,TPI);
    if(VERBOSECORE)
      ANN.PrintNN(ANN);
    TPI++;
  }
  if(VERBOSECORE){
    cout<<"Complexities: "<<Complexity<<"\n";
    cout<<"Dependencies: "<<Dependencies<<"\n";
  }
  if(Complexity.IsConstantSeq(Complexity)&&Dependencies.IsConstantSeq(Dependencies)){
    Understood=1;
    PositionFound=-1;
  }
  else
    Understood=0;  
  OddmanTPI=PositionFound;
  return PositionFound;
}

void Awareness::ExampleUsage(){  
  cout<<"Oddman out found at position: "<<OddmanOut()<<"\n";
}

/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
 */
