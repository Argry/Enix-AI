/*! \file */

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

#ifndef WITCHCRAFT_COLOUR
#define WITCHCRAFT_COLOUR

/* Colours */

#define WC_BLACK   (char *)"\033[0;30m"
#define WC_RED     (char *)"\033[0;31m"
#define WC_GREEN   (char *)"\033[0;32m"
#define WC_BROWN   (char *)"\033[0;33m"
#define WC_BLUE    (char *)"\033[0;34m"
#define WC_PURPLE  (char *)"\033[0;35m"
#define WC_CYAN    (char *)"\033[0;36m"
#define WC_LGRAY   (char *)"\033[0;37m"

#define WC_DGRAY   (char *)"\033[1;30m"
#define WC_LRED    (char *)"\033[1;31m"
#define WC_LGREEN  (char *)"\033[1;32m"
#define WC_YELLOW  (char *)"\033[1;33m"
#define WC_LBLUE   (char *)"\033[1;34m"
#define WC_LPURPLE (char *)"\033[1;35m"
#define WC_LCYAN   (char *)"\033[1;36m"
#define WC_WHITE   (char *)"\033[1;37m"

#define WC_BBLACK  (char *)"\033[40m"
#define WC_BRED    (char *)"\033[41m"
#define WC_BGREEN  (char *)"\033[42m"
#define WC_BBROWN  (char *)"\033[43m"
#define WC_BBLUE   (char *)"\033[44m"
#define WC_BPURPLE (char *)"\033[45m"
#define WC_BCYAN   (char *)"\033[46m"
#define WC_BLGRAY  (char *)"\033[47m"

#define WC_RESET   (char *)"\033[0m"
#define WC_ITALIC  (char *)"\033[1m"
#define WC_BOLD    (char *)"\033[3m"
#define WC_UNDERL  (char *)"\033[4m"
#define WC_FLASH   (char *)"\033[5m"
#define WC_REVID   (char *)"\033[7m"
#define WC_INVIS   (char *)"\033[8m"

/* Functions */
#define WC_CLS   (char *)"\033[2J"
#define WC_TITLE (char *)"\033]0;WitchCraft\007"

class ColourTest{
 public:
  static void ASCII_ART(const char **A1,const char **A2,const char **A3,const int X,const int Y);
};

#endif

/*

            88 oooooooooooo ooooo      ooo  o8o  ooooooo  ooooo   .oooo.          
   .dP     .8' `888'     `8 `888b.     `8'  `"'   `8888    d8'  .dP""Y88b  Yb     
 .dP      .8'   888          8 `88b.    8  oooo     Y888..8P          ]8P'  `Yb   
dP       .8'    888oooo8     8   `88b.  8  `888      `8888'         <88b.     `Yb 
Yb      .8'     888    "     8     `88b.8   888     .8PY888.         `88b.    .dP 
 `Yb   .8'      888       o  8       `888   888    d8'  `888b   o.   .88P   .dP   
   `Yb 88      o888ooooood8 o8o        `8  o888o o888o  o88888o `8bd88P'   dP     


 */
