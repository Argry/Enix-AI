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

#ifndef ENiX_HYBRID
#define ENiX_HYBRID

#include "stdlib.h"
#include "ENiX_Debug.h"
#include "ENiX_Poly.h"
#include "ENiX_Print.h"
#include "ENiX_Recursion.h"
#include "ENiX_Vector.h"
#include "ENiX_Omega.h"
#include "ENiX_Chaos.h"

/*******************************************************************************************************************************************************
 *                                                        START OF CONFIGURABLE PARAMETERS:                                                            *
 *******************************************************************************************************************************************************/

/*[SCANRADIUS:]*******************
 * Value     Meaning             *
 *-------------------------------*
 * [-oo,0] = Meaningless         *
 * [1,3]   = Narrow (Long Seqs)  *
 * [4,6]   = Normal              *
 * [7,9]   = Thorough            *
 * [10,oo] = SLOW                *
 *********************************/
#define SCANRADIUS               6

/*[AUTOSTOP:]*********************
 * Value  Meaning                *
 *-------------------------------*
 * 0    = List All Solutions     *
 * 1    = Stop At First Solution *
 *********************************/
#define AUTOSTOP                 0

/*[USEOMEGA:]*********************
 * Value  Meaning                *
 *-------------------------------*
 * 0    = Use Omega Test         *
 * 1    = Bypass Omega Test      *
 *********************************/
#define USEOMEGA                 0

/*[IDENTITYGEN:]**************************************
 * Value  Meaning                                    *
 *---------------------------------------------------*
 * 0    = Stop At ENiXHybridOr If Solution Is Found  *
 * 1    = Continue To Look For Identities anyway...  *
 *****************************************************/
#define IDENTITYGEN                                  1

/*[AGGRESSIVEFILTER:]*********************************
 * Value  Meaning                                    *
 *---------------------------------------------------*
 * 0    = Let User Sort Out Valid Identities         *
 * 1    = Only Print Identities That Are Fairly Sure *
 *****************************************************/
#define AGGRESSIVEFILTER                             1


/********************************************************************************************************************************************************
 *                                                            START GLOBAL PARAMETERS (READONLY):                                                       *
 ********************************************************************************************************************************************************/

/*[ENiXHybridSel:]********************
 * Value  Meaning                    *
 *-----------------------------------*
 * 1    = Polynomial Solution        *
 * 0    = Recurrence Relation        *
 * -1   = Polynomial Hybrid Solution *
 * -2   = No Solution                *
 *************************************/
int ENiXHybridSel;


/*[ENiXBlankEquation:]****************
 * Internal Only                     *
 *************************************/
float *ENiXBlankEquation;

/*[Dimensions:]***********************/
int PolynomialSize;
int RecurrenceSize;
#define CHAOS_DIAMETER 2



/* Compute 2nd Gen Hybrid */
float *ENiXHybrid(float *Vector);

/* Compute 1st Gen Hybrid */
float *ENiXHybridOr(float *Vector);

/* Compute if ChaosGen has got a sol */
float *ENiXHybridCom(float *Vector,float *Method);

/* Return vector of calculated hybrid values */
float *ENiXHybridCal(int Size,int R0,float *Vector);

/* Return 1 if Equation passes aggressive filter 0 otherwise */
float ENiXHybridFilter(float *Equation);

/* Fabricate Hybrid equation from Poly and Rec */
float *ENiXHybridFabric(float *Poly,float *Rec);

#endif

/*
                                                                                                               
    _/_/_/_/  _/      _/  _/  _/      _/      _/    _/            _/                  _/        _/        _/   
   _/        _/_/    _/        _/  _/        _/    _/  _/    _/  _/_/_/    _/  _/_/        _/_/_/      _/_/    
  _/_/_/    _/  _/  _/  _/      _/          _/_/_/_/  _/    _/  _/    _/  _/_/      _/  _/    _/        _/     
 _/        _/    _/_/  _/    _/  _/        _/    _/  _/    _/  _/    _/  _/        _/  _/    _/        _/      
_/_/_/_/  _/      _/  _/  _/      _/      _/    _/    _/_/_/  _/_/_/    _/        _/    _/_/_/        _/       
                                                         _/                                                    
                                                    _/_/                                                       
*/
