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
                                        


Programmed By M.S.Braithwaite 2017.

*/

#include "ENiX_Time.h"

  
  
unsigned long int Time::SecondsNow(){
  unsigned long int Seconds=0;
  time_t Time=0;
  time(&Time);
  Seconds=(unsigned long int)Time;
  return Seconds;
}

string Time::FormatTime2Str(){
  string FormattedTime={};
  char *R=(char *)malloc(20*sizeof(char));
  time_t Time=(time_t)SecondsNow();
  struct tm *TimeInfo=NULL;
  TimeInfo=localtime(&Time); 
  strftime(R,20,"%H:%M:%S-%Y/%m/%d",TimeInfo);
  FormattedTime=string(R);
  free(R);
  return FormattedTime;
}

BinData Time::FormatTime2Bin(){
  string TimeNow=FormatTime2Str();
  BinData FormattedTime(TimeNow);
  return FormattedTime;
}

string Time::FormatTime2Str(unsigned long int Seconds){
  string FormattedTime={};
  char *R=(char *)malloc(20*sizeof(char));
  time_t Time=(time_t)Seconds;
  struct tm *TimeInfo=NULL;
  TimeInfo=localtime(&Time); 
  strftime(R,20,"%H:%M:%S-%Y/%m/%d",TimeInfo);
  FormattedTime=string(R);
  free(R);
  return FormattedTime;
}

BinData Time::FormatTime2Bin(unsigned long int Seconds){
  string TimeNow=FormatTime2Str(Seconds);
  BinData FormattedTime(TimeNow);
  return FormattedTime;  
}

/*
  __       _________   ___ _  __    __ ____  
 / /     _/_/ ____/ | / (_) |/ /   / // /\ \ 
/ /    _/_// __/ /  |/ / /|   /   / // /_ \ \
\ \  _/_/ / /___/ /|  / //   |   /__  __/ / /
 \_\/_/  /_____/_/ |_/_//_/|_|     /_/   /_/ 
                                             
 */
