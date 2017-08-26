#ifndef __PUZZLE_H
#define __PUZZLE_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

/* Struct defintions */

typedef struct SHA256SubPuzzle {
	unsigned char *preimage;		/* The preimage to solve for (x) */
	unsigned char *image;   		/* The image y = h(x) of x       */

	struct SHA256SubPuzzle * next;	/* Pointer to the next in the list */
} SHA256SubPuzzle;

typedef struct {
	uint32_t timestamp;			/* The timestamp that the client must return */
	uint8_t num_subpuzzles;   	/* The number of subp-puzzles per puzzle     */
	uint16_t difficulty;		/* The bits of difficulty per sub-puzzle     */

	SHA256SubPuzzle *puzzle;	/* Pointer to the head of the sub-puzzles */
} SHA256Challenge;

typedef struct SHA256SubSolution {
	unsigned char *solution;    	/* The solution for the current subpuzzle */

	struct SHA256SubSolution *next;	/* Point to the next in the list */
} SHA256SubSolution;

typedef struct {
	uint32_t timestamp;		        /* The timestamp to return to the server */

	SHA256SubSolution *solution;	/* Pointer to the head of the solution */
} SHA256Solution;


/* initialize a subpuzzle
 *
 * arguments are:
 *
 *	subpuzzzle		-- The subpuzzle to initialize
 */

void initSubPuzzle 		(SHA256SubPuzzle *subpuzzle);

/* initialize a subpuzzle
 *
 * arguments are:
 *
 *  subpuzzle		-- The subpuzzle to initialize
 *  _preimage		-- The preimage in the subpuzzle
 *  _image			-- The image in the subpuzzle
 *
 */
void initSubPuzzle 		(SHA256SubPuzzle *subpuzzle,
		unsigned char *_preimage,
		unsigned char *_image);

/* initialize a challenge puzzle
 *
 * arguments are:
 *
 *  challenge		-- The challenge to initialize
 *  _timestamp		-- The timestamp to include
 *  _num_subpuzzles -- The number of subpuzzles per challenge
 *  _difficulty		-- The bit difficulty for each subpuzzle
 *  head			-- The head of the list of subpuzzles
 */

void initChallenge 		(SHA256Challenge *challenge,
		uint32_t _timestamp,
		uint8_t _num_subpuzzles,
		uint16_t _difficulty,
		SHA256SubPuzzle *head);

/* initialize a subpuzzle solution
 *
 * arguments are:
 *
 *  subsolution		-- The subsolution to intialize
 */

void initSubSolution 	(SHA256SubSolution *subsolution);

/* initialize a challenge solution
 *
 * arguments are:
 *
 *  solution 		-- The solution to initialize
 *  _timestampe		-- The timestamp to initialize
 *  head			-- The head of the list of subpuzzle solutions
 */

void initSolution 		(SHA256Solution *solution,
		uint32_t _timestamp,
		SHA256SubSolution *head);

/* insert a subsolution into the list
 *
 * arguments are:
 *
 *  head        -- The head of the list
 *  item        -- The item to insert
 */
void insert_subsolution      (SHA256SubSolution *head,
        SHA256SubSolution *item);

/* insert a subpuzzle into the list
 *
 * arguments are:
 *
 *  head		-- The head of the list
 *  item		-- The item to insert
 *
 * returns the head of the list
 */
SHA256SubPuzzle * insert_subpuzzle	(SHA256SubPuzzle *head,
		SHA256SubPuzzle *item);

/* free the memory occupied by the challenge
 *
 * arguments are:
 *
 *  challenge	-- The challenge to free
 */
void free_challenge_mem 	(SHA256Challenge *challenge);

/* free a linked list of sub puzzles
 *
 * arguments are:
 *
 *  head		-- The head of the list
 */
void free_list		(SHA256SubPuzzle *head);

/* free a solution structure
 *
 * arguments are:
 *
 *  sol 		-- The solution to free
 */
void free_solution_mem 		(SHA256Solution *sol);

/* free subsolution list
 *
 * arguments are:
 *
 *  head		-- The subsolution head to free
 *
 */
void free_solution_list 	(SHA256SubSolution *head);


#endif /* puzzle.h */
