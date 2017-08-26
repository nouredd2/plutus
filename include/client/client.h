/*
 * =====================================================================================
 *
 *       Filename:  client.h
 *
 *    Description:  Definitions for the puzzle solution capabilities
 *
 *        Version:  1.0
 *        Created:  08/09/2017 02:33:26 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Mohammad Noureddine (nouredd2), nouredd2@illinois.edu
 *   Organization:  University of Illinois at Urbana-Champaign
 *
 * =====================================================================================
 */

#ifndef __CLIENT_H
#define __CLIENT_H

#include <puzzle/puzzle.h>


/* solve a puzzle. Returns a puzzle solution structure
 *
 * arguments are:
 *
 *  challenge		-- The puzzle challenge to solve
 */

SHA256Solution *solvePuzzle (SHA256Challenge *challenge);


/*-----------------------------------------------------------------------------
 *  Utility functions needed to setup and solve puzzles
 *-----------------------------------------------------------------------------*/

/* set up a puzzle mask from an iteration number
 *
 * arguments are:
 *
 *  itr			-- The iteration number to create a mask for
 *  diff		-- The number of difficulty bits
 *  mask_len	-- The length of the mask (return variable)
 *
 * returns the mask array
 */

unsigned char *get_puzzle_mask (unsigned int itr,
	   unsigned int diff, unsigned int *mask_len);

/* clear len left most bits of a char array
 *
 * arguments are:
 *
 *  msg			-- The message from which to clear the bits
 *  len			-- The number of bits to clear
 *
 * returns the same msg
 */
unsigned char *
clear_bits (unsigned char *msg, uint8_t len);


/* compare two message digests byte by byte
 *
 * arguments are:
 *  a		-- The first digest to compare
 *  b		-- The second digest to compare
 *  len		-- The number of bytes to compare
 *
 * returns 0 if equal, 1 if a > b, -1 if a < b
 */
int
compare_digests (unsigned char *a,
        unsigned char *b, unsigned int digest_len);

#endif
