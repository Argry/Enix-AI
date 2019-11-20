/*

This file is part of Amoeba.

Amoeba is free software you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Amoeba is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Amoeba.  If not, see <http://www.gnu.org/licenses/>.

      :::.     .        :       ...    .,::::::  :::::::.    :::.          
   ,, ;;`;;    ;;,.    ;;;   .;;;;;;;. ;;;;''''   ;;;'';;'   ;;`;;    ,,   
 ,[[',[[ '[[,  [[[[, ,[[[[, ,[[     \[[,[[cccc    [[[__[[\. ,[[ '[[,  '[[, 
c$  c$$$cc$$$c $$$$$$$$"$$$ $$$,     $$$$$""""    $$""""Y$$c$$$cc$$$c    $c
"8bo,888   888,888 Y88" 888o"888,_ _,88P888oo,__ _88o,,od8P 888   888,,od8"
  "MPYMM   ""` MMM  M'  "MMM  "YMMMMMP" """"YUMMM""YUMMMP"  YMM   ""` YM"  

Programmed By M.S.Braithwaite 2012.

*/


#ifndef CYTOSKELENTON
#define CYTOSKELENTON

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Amoeba-Colony.h"

struct Colony;

struct Mitochondrion{
  unsigned long int  ARef;             /* Cytoskelenton ID                     */
  unsigned short int PRef;             /* Pseudopod ID                         */
  unsigned short int MRef;             /* Mitochondria ID                      */
};

struct Pseudopod{
  unsigned short int Dimension;        /* Number of mitochondrion              */
  struct Mitochondrion **Mitochondria; /* Array of mitochondrion               */
};

struct Blob{
  unsigned long int Size;        /* Length of Primary key                      */
  char *Nucleus;                 /* Primary key                                */
};

struct Cytoskelenton{
  unsigned long int Size;        /* Length of Primary key Nucleus              */
  struct Blob *Core;             /* Primary Key                                */
  unsigned char Lock;            /* For threading                              */
  unsigned char Segment;         /* which part of the database this resides in */
  /* All amoebae will have Header, References, DS1 and DS2 pseudopods          */
  unsigned short int Dimension;  /* Number of pseudopods                       */
  struct Pseudopod **Pseudopods; /* Array of pseudopods                        */
};

struct Blob *CreateBlob(unsigned long int S1,char *D1);
int         BlobSame(struct Blob *B1,struct Blob *B2);

#endif

/*

          /:`:::.     .        :       ...    .,::::::  :::::::.    :::.          
   ,,    /;` ;;`;;    ;;,.    ;;;   .;;;;;;;. ;;;;''''   ;;;'';;'   ;;`;;    ,,   
 ,[['   n[' ,[[ '[[,  [[[[, ,[[[[, ,[[     \[[,[[cccc    [[[__[[\. ,[[ '[[,  '[[, 
c$     c$" c$$$cc$$$c $$$$$$$$"$$$ $$$,     $$$$$""""    $$""""Y$$c$$$cc$$$c    $c
"8bo, o8"   888   888,888 Y88" 888o"888,_ _,88P888oo,__ _88o,,od8P 888   888,,od8"
  "MPmM"    YMM   ""` MMM  M'  "MMM  "YMMMMMP" """"YUMMM""YUMMMP"  YMM   ""` YM"  

*/