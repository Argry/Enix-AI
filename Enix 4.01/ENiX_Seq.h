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

#include<vector>
#include<iostream>
#include "ENiX_Binary.h"

using namespace std;

#ifndef ENIXSEQ
#define ENIXSEQ

template<typename T> class Sequence {
public:
  /* Data */
  vector<T> Values;
  vector<bool> Knowns;

  /* Constructors */
  Sequence(): Values(0),Knowns(0) {};
  Sequence(const int Size);
  Sequence(const vector<T> V,const vector<bool> K);
  Sequence(const vector<T> V);  
  Sequence(int argc,char **argv);
  Sequence(const vector<string> V);
  
  vector<string> Convert2StrVec(BinData Input);
  string Convert2Str(Sequence Data);
  string Convert2Str(T Num);

  /* Operators */
  template<typename U> friend ostream& operator<<(ostream& OutputStream,const Sequence<U>& Seq);
  Sequence &operator+=(const Sequence &Seq);
  Sequence &operator+=(const T Scalar);
  Sequence &operator*=(const Sequence &Seq);
  Sequence &operator*=(const T Scalar);
  Sequence &operator-=(const Sequence &Seq);
  Sequence &operator-=(const T Scalar);
  Sequence &operator/=(const Sequence &Seq);
  Sequence &operator/=(const T Scalar);
  Sequence &operator&=(const Sequence &Seq);
  Sequence &operator&=(const T Scalar);

  void operator()(const int NewSize);
  int Size() const;

  /* Misc */
  static Sequence NonZero(const Sequence& Seq);
  Sequence Zero(const int Size);
  void SetValue(const T Value,const int Pos);
  void UnsetValue(const int Pos);
  static Sequence Delta(const Sequence &Seq);
  static bool IsConstantSeq(const Sequence &Seq);
  static bool IsZeroSeq(const Sequence &Seq);
  static bool isNum(char *str);
  static bool ContainsKnowns(const Sequence &Seq);
  static double GetAnyKnownValue(const Sequence &Seq);
  
  void Print();

  static bool DoubleEqual(double N1,double N2);
  
};


/* This is included in the header to get around linking problems */
template<typename U> 
ostream& operator<<(ostream& OutputStream,const Sequence<U>& Seq){
  vector<bool>::const_iterator it1;
  vector<int>::const_iterator it2;
  for(it1=Seq.Knowns.begin(),it2=Seq.Values.begin();(it1!=Seq.Knowns.end())&&(it2!=Seq.Values.end());++it1,++it2){
    if(*it1)
      OutputStream<<*it2;
    else
      OutputStream<<'?';
    OutputStream<<' ';
  }
  return(OutputStream);
}

#endif

/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
 */
