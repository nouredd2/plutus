/*
 * =====================================================================================
 *
 *       Filename:  server.h
 *
 *    Description:  The server responsible for generating the puzzles when receiving requests
 *
 *        Version:  1.0
 *        Created:  08/11/2017 10:44:24 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Mohammad Noureddine (nouredd2), nouredd2@illinois.edu
 *   Organization:  University of Illinois at Urbana-Champaign
 *
 * =====================================================================================
 */

#include <puzzle/puzzle.h>

/* generate a puzzle according to the naiive method
 *
 * arguments are:
 *
 *  data		-- The data to hash (must be prepared before)
 *  data_len	-- The length of the data in bytes
 *  key			-- The server's private key
 *  key_len		-- The length of the key in bytes
 *  timestamp	-- The puzzle's timestamp
 *  k			-- The number of subpuzzles in each puzzle
 *  m			-- The number of difficulty bits
 *
 * returns a puzzle challenge
 */
SHA256Challenge *generate_puzzle		(unsigned char *data,	/* the data to hash */
		unsigned int data_len,		/* The length of the data */
		unsigned char *key,			/* The server's secret key */
		unsigned int key_len,		/* The length of the key */
		uint32_t timestamp,			/* The server's current timestamp */
		uint8_t k,					/* The number of subpuzzles per puzzle */
		uint16_t m					/* The number of difficulty bits per puzzle */
		);

/* verify a solution to a challenge
 *
 * arguments are:
 *
 *  sol				-- The solution to verify
 *  data			-- The data used to generate the puzzles
 *  data_len		-- The length of the data in bytes
 *  key				-- The server's private key
 *  key_len			-- The length of the key in bytes
 *  k				-- The number of subpuzzles (by the server)
 *
 * returns true if verified, false otherwise.
 */
bool verify_solution 		(SHA256Solution *sol, 		/* the solution to verify */
		unsigned char *data,	/* The data used to generate the puzzles */
		unsigned int data_len,  /* The length of the data in bytes */
		unsigned char *key,		/* The server's key */
		unsigned int key_len,	/* The length of the key in bytes */
		uint8_t k		/* The number of subpuzzles */
		);

/*-----------------------------------------------------------------------------
 *  Utility functions needed to generate puzzles
 *-----------------------------------------------------------------------------*/

/* scramble the bits of a buffer
 *
 * arguments are:
 *
 *  buf		-- The buffer to scramble
 *  bits	-- The number of bits to scramble
 *
 * returns the buffer with scrambled bits
 */
unsigned char *
scramble_bits		(unsigned char *buf, unsigned int bits);
