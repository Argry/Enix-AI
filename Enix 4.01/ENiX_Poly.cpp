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

#include "ENiX_Poly.h"
#include "ENiX_Settings.h"
#include "ENiX_List.h"
#include <math.h>
#include <sstream>
#include <map>

bool Poly::CreateEquation(vector<string> Seq){
  bool Continue=true;
  bool Success=false;
  Equation={};
  PolyFound=false;
  vector<double> Domain;
  for(size_t _=0;_<Seq.size();++_)
    Domain.push_back(_);
  Sequence<double> OriginalSeq(Seq);
  while(Continue){
    vector<double> ModelledSoFar=CalculateSeq(Domain);
    Sequence<double> MSF(ModelledSoFar);
    Sequence<double> ToFind=OriginalSeq;
    ToFind-=MSF;
    if(Sequence<double>::IsZeroSeq(ToFind)){
      PolyFound=true;
      return true;
    }
    Sequence<double> Delta=ToFind;
    size_t Exponent=0;
    double Coefficient=0;
    bool First=true;
    while(Sequence<double>::ContainsKnowns(Delta)){
      if(Sequence<double>::IsConstantSeq(Delta)){
	Coefficient=Sequence<double>::GetAnyKnownValue(Delta);
	Coefficient/=Factorial(Exponent);
	if(First){
	  Equation.resize(Exponent+1);
	  First=false;
	}
	Equation[Exponent]=Coefficient;
	break;
      }
      Delta=Sequence<double>::Delta(Delta);
      ++Exponent;
    }
    if(!Sequence<double>::ContainsKnowns(Delta))
      return false;
  }
  return true;
}

string Poly::GetFormula(){
  bool First=true;
  ostringstream Converted;
  for(size_t _=0;_<Equation.size();++_){
    if(Equation[_]){
      if((!First)&&(Equation[_]>0))
	Converted<<"+";
      if(First||(Equation[_]!=1))
	Converted<<Equation[_];
      if(_)
	Converted<<"x";
      if(_>1)
	Converted<<"^"<<_;
      First=false;
    }    
  }
  Converted<<endl;
  return Converted.str();
}

double Poly::ExecutePoly(vector<double> Fx,double x){
  double Result=0;
  for(size_t _=0;_<Fx.size();++_)
    Result+=(Fx[_]*pow(x,_));
  return Result;
}

vector<double> Poly::CalculateSeq(vector<double> Input){
  vector<double> Result;
  for(size_t _=0;_<Input.size();++_)
    Result.push_back(ExecutePoly(Equation,Input[_]));
  return Result;
}

void Poly::SetEquation(vector<double> Eq){
  Equation=Eq;
}

vector<double> Poly::GetEquation(){
  return Equation;
}

size_t Poly::Factorial(size_t N){
  size_t Result=1;
  for(size_t _=1;_<=N;++_)
    Result*=_;
  return Result;
}

/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
*/
