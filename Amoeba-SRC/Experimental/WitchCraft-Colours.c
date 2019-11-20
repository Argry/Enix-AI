/*

This file is part of ENiX3.

ENiX3 is free software you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

ENiX3 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ENiX3.  If not, see <http://www.gnu.org/licenses/>.

       oooooooooooo ooooo      ooo  o8o  ooooooo  ooooo   .oooo.          
   .dP `888'     `8 `888b.     `8'  `"'   `8888    d8'  .dP""Y88b  Yb     
 .dP    888          8 `88b.    8  oooo     Y888..8P          ]8P'  `Yb   
dP      888oooo8     8   `88b.  8  `888      `8888'         <88b.     `Yb 
Yb      888    "     8     `88b.8   888     .8PY888.         `88b.    .dP 
 `Yb    888       o  8       `888   888    d8'  `888b   o.   .88P   .dP   
   `Yb o888ooooood8 o8o        `8  o888o o888o  o88888o `8bd88P'   dP     


Programmed By M.S.Braithwaite 2007 - 2011.

*/


#include <stdio.h>
#include "WitchCraft-Colours.h"

char *Banner1[]={
  "         #@@@@@@## @@@@@@@@@@## #@@@@@@## @@@@@@@@# @@@@@@@## #@@@@@@## ",
  "         @@@@@@@@# @@@@@@@@@@@# @@@@@@@@# @@@@@@@@# @@@@@@@@# @@@@@@@@# ",
  "         @@!##@@@# @@!#@@!#@@!# @@!##@@@# @@!###### @@!##@@@# @@!##@@@# ",
  "         !@!##@!@# !@!#!@!#!@!# !@!##@!@# !@!###### !@###@!@# !@!##@!@# ",
  "         @!@!@!@!# @!!#!!@#@!@# @!@##!@!# @!!!:!### @!@!@!@## @!@!@!@!# ",
  "         !!!@!!!!# !@!###!#!@!# !@!##!!!# !!!!!:### !!!@!!!!# !!!@!!!!# ",
  "         !!:##!!!# !!:#####!!:# !!:##!!!# !!:###### !!:##!!!# !!:##!!!# ",
  "         :!:##!:!# :!:#####:!:# :!:##!:!# :!:###### :!:##!:!# :!:##!:!# ",
  "         ::###:::# :::#####::## :::::#::# #::#::::# #::#::::# ::###:::# ",
  "         :###:#:## :######:#### :#:##:###:#::#::###::#:#::### :###:#:## "
};

char *Banner2[]={
  "         0FFFFFF00 FFFFFFFFFF00 0FFFFFF00 FFFFFFFF0 FFFFFFF00 0FFFFFF00 ",
  "         FFFFFFFF0 FFFFFFFFFFF0 FFFFFFFF0 FFFFFFFF0 FFFFFFFF0 FFFFFFFF0 ",
  "         FFF00FFF0 FFF0FFF0FFF0 FFF00FFF0 FFF000000 FFF00FFF0 FFF00FFF0 ",
  "         777007770 777077707770 777007770 777000000 770007770 777007770 ",
  "         777777770 777077707770 777007770 777777000 777777700 777777770 ",
  "         777777770 777000707770 777007770 777777000 777777770 777777770 ",
  "         777007770 777000007770 777007770 777000000 777007770 777007770 ",
  "         888008880 888000008880 888008880 888000000 888008880 888008880 ",
  "         880008880 888000008800 888880880 088088880 088088880 880008880 ",
  "         800080800 800000080000 80800800080880880008808088000 800080800 "
};

char *Banner3[]={
  "        1100000011100000000001111000000111000000001100000001111000000111",
  "        1000000001100000000000110000000011000000001100000000110000000011",
  "        1000110001100010001000110001100011000111111100011000110001100011",
  "        1000110001100010001000110001100011000111111100111000110001100011",
  "        1111111111111111111111111111111111111111111111111111111111111111",
  "        1111111111111111111111111111111111111111111111111111111111111111",
  "        1111111111111111111111111111111111111111111111111111111111111111",
  "        1111111111111111111111111111111111111111111111111111111111111111",
  "        1111111111111111111111111111111111111111111111111111111111111111",
  "        1111111111111111111111111111111111111111111111111111111111111111"
};

void ASCII_ART(char **A1,char **A2,char **A3,int X,int Y){
  int A,B;
  char *FG,*BG;
  for(A=0;A<Y;A++){
    for(B=0;B<X;B++){
      switch(A2[A][B]){
      case '0':
	FG=WC_BLACK;
	break;
      case '1':
	FG=WC_RED;
	break;
      case '2':
	FG=WC_GREEN;
	break;
      case '3':
	FG=WC_BROWN;
	break;
      case '4':
	FG=WC_BLUE;
	break;
      case '5':
	FG=WC_PURPLE;
	break;
      case '6':
	FG=WC_CYAN;
	break;
      case '7':
	FG=WC_LGRAY;
	break;
      case '8':
	FG=WC_DGRAY;
	break;
      case '9':
	FG=WC_LRED;
	break;
      case 'A':
	FG=WC_LGREEN;
	break;
      case 'B':
	FG=WC_YELLOW;
	break;
      case 'C':
	FG=WC_LBLUE;
	break;
      case 'D':
	FG=WC_LPURPLE;
	break;
      case 'E':
	FG=WC_LCYAN;
	break;
      case 'F':
	FG=WC_WHITE;
	break;
      default:
	FG=WC_RESET;
	break;
      }
      switch(A3[A][B]){
      case '0':
	BG=WC_BBLACK;
	break;
      case '1':
	BG=WC_BRED;
	break;
      case '2':
	BG=WC_BGREEN;
	break;
      case '3':
	BG=WC_BBROWN;
	break;
      case '4':
	BG=WC_BBLUE;
	break;
      case '5':
	BG=WC_BPURPLE;
	break;
      case '6':
	BG=WC_BCYAN;
	break;
      case '7':
	BG=WC_BLGRAY;
	break;

      default:
	BG="";
	break;
      }

      printf("%s%s%c",FG,BG,A1[A][B]);
    } 
    puts(WC_RESET);
  }
  
  
}

void main(){

  ASCII_ART(Banner1,Banner2,Banner3,72,10);
}

/*

            88 oooooooooooo ooooo      ooo  o8o  ooooooo  ooooo   .oooo.          
   .dP     .8' `888'     `8 `888b.     `8'  `"'   `8888    d8'  .dP""Y88b  Yb     
 .dP      .8'   888          8 `88b.    8  oooo     Y888..8P          ]8P'  `Yb   
dP       .8'    888oooo8     8   `88b.  8  `888      `8888'         <88b.     `Yb 
Yb      .8'     888    "     8     `88b.8   888     .8PY888.         `88b.    .dP 
 `Yb   .8'      888       o  8       `888   888    d8'  `888b   o.   .88P   .dP   
   `Yb 88      o888ooooood8 o8o        `8  o888o o888o  o88888o `8bd88P'   dP     


 */
