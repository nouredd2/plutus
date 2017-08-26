/*
 * =====================================================================================
 *
 *       Filename:  optserver.h
 *
 *    Description:  Definitions of the puzzle generation and verification routines
 *
 *        Version:  1.0
 *        Created:  08/15/2017 12:07:13 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Mohammad Noureddine (nouredd2), nouredd2@illinois.edu
 *   Organization:  University of Illinois at Urbana-Champaign
 *
 * =====================================================================================
 */

#ifndef __OPTSERVER_H
#define __OPTSERVER_H

#include "puzzle/optpuzzle.h"

/* generate a challenge using the optimized implementation
 *
 * returns a new challenge using the optimized implementation
 */
SHA256OptChallenge *
generate_challenge 		(unsigned char *data,	/* The data to use for the hash */
		unsigned int data_len,					/* The length of the data in bytes */
		unsigned char *key, 					/* The server's secret key */
		unsigned int key_len,					/* The length of the key in bytes */
		uint32_t timestamp, 					/* The server's current timestamp */
		uint16_t k, 							/* The number of subpuzzles in the challenge */
		uint16_t m,								/* The number of bits of difficulty */
		unsigned int l							/* The number of bits to send to the client */
		);


/* verify the solution of a given client's solution
 *
 * returns true if verified, false otherwise
 */
bool
verify_solution 	(SHA256OptSolution *sol,	/* The solution provided by the client */
		unsigned char *data,					/* The data used for generating the hash */
		unsigned int data_len, 					/* The length of the data in bytes */
		unsigned char *key,						/* The server's private key */
		unsigned int key_len,					/* The length of the key in bytes */
		uint16_t len,							/* The length of x + z_i in bytes */
		uint16_t k,								/* The number of subpuzzles in the challenge */
		uint16_t m								/* The number of bits of difficulty */
		);



#endif /* optserver.h */
