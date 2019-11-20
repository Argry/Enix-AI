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

#include <string>
#include <string.h>
#include <iostream>
#include <sstream>
#include <math.h>
#include <limits>
#include "ENiX_Seq.h"
#include "ENiX_Binary.h"

template<typename T> 
double Sequence<T>::GetAnyKnownValue(const Sequence<T> &Seq){
  for(size_t _=0;_<(Seq.Knowns.size())&&(Seq.Values.size());++_)
    if(Seq.Knowns[_]==true)
      return Seq.Values[_];
  return 0;
}

template<typename T> 
bool Sequence<T>::ContainsKnowns(const Sequence<T> &Seq){
  for(size_t _=0;_<Seq.Knowns.size();++_)
    if(Seq.Knowns[_]==true)
      return true;
  return false;
}

template<typename T> 
vector<string> Sequence<T>::Convert2StrVec(BinData Input){
  char *Buffer=(char *)malloc((Input.Size+1)*sizeof(char));
  vector<string> Array(0);
  memcpy(Buffer,Input.Str,Input.Size);
  Buffer[Input.Size]=0;
  for(int i=0;i<Input.Size;i++)
    if(isspace(Buffer[i]))
      Buffer[i]=0;
  for(int i=0;i<Input.Size;i++){
    if(Buffer[i])
      Array.push_back(string(Buffer+i));
    i+=strlen(Buffer+i);
  }
  free(Buffer);
  return Array;
}

template<typename T> 
void Sequence<T>::Print(){
  string Out=Convert2Str(*this);
  puts(Out.c_str());
}

template<typename T> 
string Sequence<T>::Convert2Str(Sequence<T> Data){  
  string Str;
  ostringstream ToConvert;
  for(int C=0;C<Data.Size();C++){
    if(Data.Knowns[C])
      ToConvert<<Data.Values[C]<<" ";
    else
      ToConvert<<"? ";
  }
  Str=ToConvert.str();
  return Str;
}

template<typename T> 
string Sequence<T>::Convert2Str(T Num){
  string Str;
  ostringstream ToConvert;
  ToConvert<<Num;
  Str=ToConvert.str();
  return Str;
}

template<typename T> 
int Sequence<T>::Size() const{
  return (Values.size()<Knowns.size())?Values.size():Knowns.size();
}

template<typename T> 
Sequence<T>::Sequence(const vector<string> V){
  unsigned long int Length=V.size();
  Knowns.resize(Length);
  Values.resize(Length);
  for(unsigned long int C=0;C<Length;C++){
    if(isNum((char *)V[C].c_str())){
      Values[C]=stod(V[C],NULL);
      Knowns[C]=1;
    }
    else{
      Values[C]=0;
      Knowns[C]=0;
    }
  }
}

template<typename T> 
Sequence<T>::Sequence(int argc,char **argv){
  int a=0,i=0;
  char *p=NULL;
  Knowns.resize(argc-1);
  Values.resize(argc-1);
  for(i=1;i<argc;i++){
    if(isNum(argv[i])){
      Values[i-1]=atof(argv[i]);
      Knowns[i-1]=1;
    }
    else{
      Values[i-1]=0;
      Knowns[i-1]=0;
    }
  }
}

template<typename T> 
bool Sequence<T>::isNum(char *str){
  size_t Len=strlen(str);
  for(size_t _=0;_<Len;++_){
    char Character=str[_];
    if((Character=='-')||(Character=='+')||(Character=='.')||isdigit(Character)){}
    else
      return false;
  }
  return true;
}

template<> 
bool Sequence<int>::IsConstantSeq(const Sequence<int> &Seq){
  bool First=true;
  int MinLength=Seq.Size();
  int EqualTo=0;
  for(int i=0;i<MinLength;++i){
    if(Seq.Knowns[i]){
      if(First){
	EqualTo=Seq.Values[i];
	First=false;
      }
      else
	if(EqualTo!=Seq.Values[i])
	  return 0;
    }
  }
  return 1;
}

template<> 
bool Sequence<double>::IsConstantSeq(const Sequence<double> &Seq){
  bool First=true;
  int MinLength=Seq.Size();
  double EqualTo=0;
  for(int i=0;i<MinLength;++i){
    if(Seq.Knowns[i]){
      if(First){
	EqualTo=Seq.Values[i];
	First=false;
      }
      else
	if(!DoubleEqual(EqualTo,Seq.Values[i]))
	  return 0;
    }
  }
  return 1;
}

template<> 
bool Sequence<int>::IsZeroSeq(const Sequence<int> &Seq){
  bool First=1;
  int MinLength=Seq.Size();
  for(int i=0;i<MinLength;++i)
    if(Seq.Knowns[i])
      if(Seq.Values[i])
	return 0;
  return 1;
}

template<> 
bool Sequence<double>::IsZeroSeq(const Sequence<double> &Seq){
  bool First=1;
  int MinLength=Seq.Size();
  for(int i=0;i<MinLength;++i)
    if(Seq.Knowns[i])
      if(!DoubleEqual(Seq.Values[i],0))
	return 0;
  return 1;
}

template<typename T> 
Sequence<T> Sequence<T>::Delta(const Sequence<T> &Seq){
  int MinLength=Seq.Size();
  class Sequence<T> New(MinLength-1);
  for(int i=0;i<MinLength-1;++i)
    if((Seq.Knowns[i])&&(Seq.Knowns[i+1]))
      New.SetValue(Seq.Values[i+1]-Seq.Values[i],i);
    else
      New.UnsetValue(i);
  return New;
}

template<typename T> 
Sequence<T> &Sequence<T>::operator&=(const T Scalar){
  int MinLength=Size();
  for(int i=0;i<MinLength;++i)
    if(this->Knowns[i])
      this->SetValue(int(this->Values[i])&int(Scalar),i);
    else
      this->UnsetValue(i);
  return *this;
}

template<typename T> 
Sequence<T> &Sequence<T>::operator&=(const Sequence<T> &Seq){
  int MinLength1=Size(),MinLength2=Seq.Size();
  int MinLength=(MinLength1<MinLength2)?MinLength1:MinLength2;
  for(int i=0;i<MinLength;++i)
    if((this->Knowns[i])&&(Seq.Knowns[i]))
      this->SetValue(int(this->Values[i])&int(Seq.Values[i]),i);
    else
      this->UnsetValue(i);
  return *this;
}

template<typename T> 
Sequence<T> &Sequence<T>::operator/=(const T Scalar){
  int MinLength=Size();
  for(int i=0;i<MinLength;++i)
    if(this->Knowns[i])
      this->SetValue((this->Values[i])/Scalar,i);
    else
      this->UnsetValue(i);
  return *this;
}

template<typename T> 
Sequence<T> &Sequence<T>::operator/=(const Sequence<T> &Seq){
  int MinLength1=Size(),MinLength2=Seq.Size();
  int MinLength=(MinLength1<MinLength2)?MinLength1:MinLength2;
  for(int i=0;i<MinLength;++i)
    if((this->Knowns[i])&&(Seq.Knowns[i]))
      this->SetValue((this->Values[i])/Seq.Values[i],i);
    else
      this->UnsetValue(i);
  return *this;
}

template<typename T> 
Sequence<T> &Sequence<T>::operator-=(const T Scalar){
  int MinLength=Size();
  for(int i=0;i<MinLength;++i)
    if(this->Knowns[i])
      this->SetValue((this->Values[i])-Scalar,i);
    else
      this->UnsetValue(i);
  return *this;
}

template<typename T> 
Sequence<T> &Sequence<T>::operator-=(const Sequence<T> &Seq){
  int MinLength1=Size(),MinLength2=Seq.Size();
  int MinLength=(MinLength1<MinLength2)?MinLength1:MinLength2;
  for(int i=0;i<MinLength;++i)
    if((this->Knowns[i])&&(Seq.Knowns[i]))
      this->SetValue((this->Values[i])-Seq.Values[i],i);
    else
      this->UnsetValue(i);
  return *this;
}

template<typename T> 
Sequence<T> &Sequence<T>::operator*=(const T Scalar){
  int MinLength=Size();
  for(int i=0;i<MinLength;++i)
    if(this->Knowns[i])
      this->SetValue((this->Values[i])*Scalar,i);
    else
      this->UnsetValue(i);
  return *this;
}

template<typename T> 
Sequence<T> &Sequence<T>::operator*=(const Sequence<T> &Seq){
  int MinLength1=Size(),MinLength2=Seq.Size();
  int MinLength=(MinLength1<MinLength2)?MinLength1:MinLength2;
  for(int i=0;i<MinLength;++i)
    if((this->Knowns[i])&&(Seq.Knowns[i]))
      this->SetValue((this->Values[i])*Seq.Values[i],i);
    else
      this->UnsetValue(i);
  return *this;
}

template<typename T> 
Sequence<T> &Sequence<T>::operator+=(const T Scalar){
  int MinLength=Size();
  for(int i=0;i<MinLength;++i)
    if(this->Knowns[i])
      this->SetValue((this->Values[i])+Scalar,i);
    else
      this->UnsetValue(i);
  return *this;
}

template<typename T> 
Sequence<T> &Sequence<T>::operator+=(const Sequence<T> &Seq){
  int MinLength1=Size(),MinLength2=Seq.Size();
  int MinLength=(MinLength1<MinLength2)?MinLength1:MinLength2;
  for(int i=0;i<MinLength;++i)
    if((this->Knowns[i])&&(Seq.Knowns[i]))
      this->SetValue((this->Values[i])+Seq.Values[i],i);
    else
      this->UnsetValue(i);
  return *this;
}

template<typename T> 
void Sequence<T>::SetValue(const T Value,const int Pos){
  Values[Pos]=Value;
  Knowns[Pos]=1;
}

template<typename T> 
void Sequence<T>::UnsetValue(const int Pos){
  Values[Pos]=0;
  Knowns[Pos]=0;
}

template<typename T> 
Sequence<T> Sequence<T>::Zero(const int Size){
  class Sequence<T> New(Size);
  for(int i=0;i<Size;++i)
    New.SetValue(0,i);
  return New;
}

template<typename T> 
Sequence<T> Sequence<T>::NonZero(const Sequence<T>& Seq){
  int MinLength=Seq.Size();
  class Sequence<T> New(MinLength);
  for(int i=0;i<MinLength;++i)
    if(Seq.Knowns[i])
      New.SetValue(!!Seq.Values[i],i);
    else
      New.UnsetValue(i);
  return New;
}

template<typename T> 
Sequence<T>::Sequence(const int Size){
  if(Size>0){
    Values.resize(Size);
    Knowns.resize(Size);
    for(int i=0;i<Size;++i){
      Values[i]=0;
      Knowns[i]=0;
    }
  }
  else
  {
    Values.resize(0);
    Knowns.resize(0);
  }
}

template<typename T> 
Sequence<T>::Sequence(const vector<T> V,const vector<bool> K){ 
  Values=V;
  Knowns=K;
}

template<typename T> 
Sequence<T>::Sequence(const vector<T> V){
  int Pos=0;
  Values=V;
  Knowns.resize(V.size());
  for(auto i:V){
    Knowns[Pos]=1;
    Pos++;
  }
}

template<typename T> 
void Sequence<T>::operator()(const int NewSize){
  vector<T> ValBuf=Values;
  vector<bool> KnBuf=Knowns;
  Values.resize(NewSize);
  Knowns.resize(NewSize);
  for(int i=0;i<NewSize;++i){
    Values[i]=(i<ValBuf.size())?ValBuf[i]:0;
    Knowns[i]=(i<KnBuf.size())?KnBuf[i]:0;
  }
}

template<typename T>
bool Sequence<T>::DoubleEqual(double N1,double N2){
  double Epsilon=numeric_limits<double>::epsilon();
  /*
    The following is recommended in the art of programming, not enabling here though
    it seems to be causing serious problems.

    //return fabs(N1-N2) <= ((fabs(N1)<fabs(N2))?fabs(N2):fabs(N1))*Epsilon;
  */
  return (fabs(N1-N2)<=Epsilon);
}

template class Sequence<int>;
template class Sequence<double>;


/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
 */
