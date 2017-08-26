/*
 * =====================================================================================
 *
 *       Filename:  optclient.h
 *
 *    Description:  definitions for the capabilities to solve optimized puzzles
 *
 *        Version:  1.0
 *        Created:  08/25/2017 10:16:58 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Mohammad Noureddine (nouredd2), nouredd2@illinois.edu
 *   Organization:  University of Illinois at Urbana-Champaign
 *
 * =====================================================================================
 */

#ifndef __OPTCLIENT_H
#define __OPTCLIENT_H

#include "puzzle/optpuzzle.h"

/* solve a challenge built in the optimized version.
 *
 * arguments are
 *
 *  challenge		-- The challenge to solve
 *
 * returns an solution structure
 */
SHA256OptSolution 		*solveChallenge (SHA256OptChallenge *challenge);


#endif
