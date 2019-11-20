/*! \file */

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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Amoeba-Cytoskelenton.h"

extern struct Colony *Nucleii[2];

/*! \brief Create an Amoeba blob.
 *
 */
struct Blob *CreateBlob(unsigned long int S1,char *D1){
  struct Blob *R=(struct Blob *)malloc(sizeof(struct Blob ));
  if(S1<0||S1>10000){
    printf("CreateBlob Size: %ld. Looks weird, aborting!!\n",S1);
    exit(1);
  }
  R->Size=S1;
  R->Nucleus=D1;
  return R;
}

/*! \brief Determine if two blobs are the same.
 *
 */
int BlobSame(struct Blob *B1,struct Blob *B2){
  unsigned long MinLen=0;
  if(B1->Size!=B2->Size)
    return 0;
  MinLen=(B1->Size>B2->Size)?B2->Size:B1->Size;
  if(memcmp(B1->Nucleus,B2->Nucleus,MinLen))
    return 0;
  return 1;
}

/*

          /:`:::.     .        :       ...    .,::::::  :::::::.    :::.          
   ,,    /;` ;;`;;    ;;,.    ;;;   .;;;;;;;. ;;;;''''   ;;;'';;'   ;;`;;    ,,   
 ,[['   n[' ,[[ '[[,  [[[[, ,[[[[, ,[[     \[[,[[cccc    [[[__[[\. ,[[ '[[,  '[[, 
c$     c$" c$$$cc$$$c $$$$$$$$"$$$ $$$,     $$$$$""""    $$""""Y$$c$$$cc$$$c    $c
"8bo, o8"   888   888,888 Y88" 888o"888,_ _,88P888oo,__ _88o,,od8P 888   888,,od8"
  "MPmM"    YMM   ""` MMM  M'  "MMM  "YMMMMMP" """"YUMMM""YUMMMP"  YMM   ""` YM"  

*/
