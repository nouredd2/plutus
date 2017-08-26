/*
 * =====================================================================================
 *
 *       Filename:  optpuzzle.h
 *
 *    Description:  Definitions for the optimized puzzle types
 *
 *        Version:  1.0
 *        Created:  08/14/2017 04:18:09 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Mohammad Noureddine (nouredd2), nouredd2@illinois.edu
 *   Organization:  University of Illinois at Urbana-Champaign
 *
 * =====================================================================================
 */


#ifndef __OPTPUZZLE_H
#define __OPTPUZZLE_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

/* Struct definitions */
typedef struct SHA256OptChallenge {
	unsigned char *preimage;	/* The preimage x to be sent to the client */

	unsigned int timestamp;		/* The timestamp that the client must return */
	uint16_t len; 				/* The length of x + z */
	uint16_t num_subpuzzles;	/* The number of sub puzzles to solve for */
	uint16_t difficulty;		/* The number of difficult bits  */
} SHA256OptChallenge;


typedef struct SHA256OptSubSolution {
	unsigned char *zi;						/* The actual solution returned */

	struct SHA256OptSubSolution *next;		/* A pointer to the next solution in the list */
} SHA256OptSubSolution;

typedef struct SHA256OptSolution {
	unsigned int timestamp; 		/* The timestamp provided by the server */
	SHA256OptSubSolution *head;	/* The head of the solution lists */
} SHA256OptSolution;



/*-----------------------------------------------------------------------------
 *  Utility functions for initializing and inserting puzzle and subpuzzles
 *-----------------------------------------------------------------------------*/


/* initialize an optimized puzzle
 *
 * arguments are:
 *
 *  challenge		-- The puzzle to initialize
 *  _preimage		-- The preimage of the puzzle
 *  _timestamp		-- The timestamp of the server
 *  _len			-- The value of l for the length of the x and z
 *  _np				-- The number of subpuzzles to return
 *  _diff			-- The difficulty of each subpuzzle
 */
void
initOptChallenge		(SHA256OptChallenge *challenge,
		unsigned char *_preimage,
		unsigned int _timestamp,
		uint16_t _len,
		uint16_t _np,
		uint16_t _diff);


/* initialize the optimized solution
 *
 * arguments are:
 *
 *  sol				-- The solution to initialize
 *  _timestamp		-- The timestamp provided by the server
 *  _head			-- The head of the list of sub solutions
 */
void
initOptSolution 	(SHA256OptSolution *sol,
		unsigned int _timestamp,
		SHA256OptSubSolution *_head);


/* initialize the optimized sub solution
 *
 * arguments are:
 *
 *  ssol			-- The subsolution to initalize
 *  _zi				-- The actual subsolution to be used
 *  _next			-- The pointer to the next subsolution in the list
 */
void
initOptSubSolution 		(SHA256OptSubSolution *ssol,
		unsigned char *_zi,
		SHA256OptSubSolution *_next);

/* insert a subsolution in the list provided by head
 *
 * arguments are:
 *
 *  head			-- The head of the list
 *  item			-- The item to insert
 *
 * returns the head of the list
 */
SHA256OptSubSolution *
insert_subsolution 		(SHA256OptSubSolution *head,
		SHA256OptSubSolution *item);

/* free the memory occupied by a solution
 *
 * arguments are:
 *
 *  sol				-- The solution to free
 */
void
free_solution_mem 		(SHA256OptSolution *sol);

#endif /* optpuzzle.h */
