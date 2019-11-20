/* 

This file is part of ENiXHybrid1.

ENiXHybrid1 is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

ENiXHybrid1 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ENiXHybrid1.  If not, see <http://www.gnu.org/licenses/>.

Programmed By M.S.Braithwaite December 2008.                            

*/

#include "ENiX_Hybrid.h"

/********************************************************************************************************************************************************
 *                                                                    START OF ALGORITHM:                                                               *
 ********************************************************************************************************************************************************/

/*[CTRL SEQ]***********************************/ /*[COMMAND SEQ]*****************************************************************************************/

float *ENiXHybridOr(float *Vector){
  float *Method=ENiXPolyGenFlt(Vector);
  if(Method[0]){
    ENiXPrintHybrid("","\n",Method=ENiXHybridFabric(Method,ENiXBlankEquation));
    ENiXHybridSel=1;
    return Method;
  }
  Method=ENiXRecGen(Vector);
  if((Method[0])&&(!Method[(int)Method[0]-1])){
    ENiXPrintHybrid("","\n",Method=ENiXHybridFabric(ENiXBlankEquation,Method));
    ENiXHybridSel=0;
    return Method;
  }
  ENiXHybridSel=-1; 
  return ENiXBlankEquation;
}

float *ENiXHybridCom(float *Vector,float *Method){
  float *Sub,*Remainder,*Store;
  Store=ENiXRecCalVecBas(Vector,Method);  
  Remainder=ENiXSubVecFlt(Vector,Store);
  free(Store);
  if((ENiXIsEqVecFlt(Remainder))&&(!Remainder[1])){
    if(ENiXHybridFilter(Method)){
      ENiXPrintHybrid(" -> ","\n",Method=ENiXHybridFabric(ENiXBlankEquation,Method));
      ENiXHybridSel=-1;
      free(Remainder);
      free(Method);
      return NULL;
    }
  }
  Remainder[1]=(int)Vector[1];
  if(ENiXHybridFilter(Method)){
    if(USEOMEGA){
      if(ENiXOmegaTest(Remainder)){
	Sub=ENiXPolyGenFlt(Remainder);
	if(Sub[0]){
	  ENiXPrintHybrid(" -> ","\n",Method=ENiXHybridFabric(Sub,Method));
	  ENiXHybridSel=-1;
	  free(Remainder);
	  free(Sub);
	  free(Method);
	  return NULL;
	}
	free(Sub);
      }
    }
    else{
      Sub=ENiXPolyGenFlt(Remainder);
      if(Sub[0]){
	ENiXPrintHybrid(" -> ","\n",Method=ENiXHybridFabric(Sub,Method));
	ENiXHybridSel=-1;
	free(Remainder);
	free(Sub);
	free(Method);
	return NULL;
      }
      free(Sub);
    }
  }
  ENiXHybridSel=-2;
  free(Remainder);
  return ENiXBlankEquation;
}

float *ENiXHybrid(float *Vector){ /* Vector is a sequence of points */
  int Terminate=0,Found=0,Size,*R,*Template,*Zero,*Free;
  float *Method;
  Size=Vector[0]-2;
  ENiXBlankEquation=ENiXNewBlankEq();
  if(!IDENTITYGEN){
    Method=ENiXHybridOr(Vector);
    if(ENiXHybridSel>-1){
      Found=1;
      free(ENiXBlankEquation);
      return Method;
    }
  }
  Template=ENiXFillVecInt(SCANRADIUS,ENiXDefVecInt(Size));
  Zero=ENiXFillVecInt((SCANRADIUS>>1),ENiXDefVecInt(Size));
  R=ENiXChaosInit(Size++);
  while(!Terminate){
    ENiXChaosGen(Template,R);
    if(R[0]==0)
      Terminate++;
    else{
      Method=ENiXConvertI2F(Free=ENiXSubVecInt(R,Zero)); 
      free(Free);
      ENiXHybridCom(Vector,Method);
      free(Method);
      if((AUTOSTOP)&&(ENiXHybridSel==-1)) {
	free(R);
	free(Zero);
	free(Template);
	free(ENiXBlankEquation);
	return NULL;
      }
      if(ENiXHybridSel==-1)
	Found=1;
    }
  }
  if(!Found){
    ENiXHybridSel=-2;
    /*
      puts("No solutions found; try increasing search radius or alternatively increase examples,");
      puts("Avoid both unless you are sure. Increasing search radius involves modifying the SCANRADIUS");
      puts("in ENiX_Hybrid.h and recompiling. Don't report bugs to me for polynomial-recurrence hybrids");
      puts("unless you have first tried these modifications and you have checked the sequence against");
      puts("SeqGen - M.S.Braithwaite 2008.");*/ 
    puts("Bug?");
  }
  free(R);
  free(Zero);
  free(Template);
  free(ENiXBlankEquation);
  return NULL;
}

float *ENiXHybridCal(int Size,int R0,float *Vector){
  int C,Size1=Vector[1],Size2=Vector[2];
  float *Ply,*Rec,*R=ENiXDefVecFlt(Size);
  
  Ply=ENiXCpySecFlt(3,3+Size1,Vector);
  Rec=ENiXCpySecFlt(3+Size1,3+Size1+Size2,Vector);
  R[1]=R0;
  for(C=1;C<Size;C++)
    R[C+1]=ENiXPolyCalValFlt(C,Ply)+ENiXRecCalVal(C,R,Rec);
  free(Ply);
  free(Rec);
  return R;
}

float ENiXHybridFilter(float *Eq){
  int C,Size=Eq[0]+1,HalfLen=Size>>1;
  if(AGGRESSIVEFILTER)
    for(C=HalfLen;C<Size;C++) 
      if(Eq[C])
	return 0;
  return 1;
}

float *ENiXHybridFabric(float *Poly,float *Rec){
  int A,C,PolySize=Poly[0],RecSize=Rec[0],Start1,Start2,Finish;
  float *R;
  R=ENiXDefVecFlt(PolySize+RecSize+3); 
  R[1]=PolySize;
  R[2]=RecSize;
  Start1=3;
  Start2=Start1+PolySize;
  Finish=Start2+RecSize;
  A=1-Start1;
  for(C=Start1;C<Start2;C++)
    R[C]=Poly[C+A];
  A=1-Start2;
  for(C=Start2;C<Finish;C++)
    R[C]=Rec[C+A];
  return R;
}

/*
                                                                                                               
    _/_/_/_/  _/      _/  _/  _/      _/      _/    _/            _/                  _/        _/        _/   
   _/        _/_/    _/        _/  _/        _/    _/  _/    _/  _/_/_/    _/  _/_/        _/_/_/      _/_/    
  _/_/_/    _/  _/  _/  _/      _/          _/_/_/_/  _/    _/  _/    _/  _/_/      _/  _/    _/        _/     
 _/        _/    _/_/  _/    _/  _/        _/    _/  _/    _/  _/    _/  _/        _/  _/    _/        _/      
_/_/_/_/  _/      _/  _/  _/      _/      _/    _/    _/_/_/  _/_/_/    _/        _/    _/_/_/        _/       
                                                         _/                                                    
                                                    _/_/                                                       
*/
