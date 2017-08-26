/*
 * =====================================================================================
 *
 *       Filename:  server.cc
 *
 *    Description:  Implementation of the server's functionality to generate puzzles
 *
 *        Version:  1.0
 *        Created:  08/11/2017 10:45:25 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Mohammad Noureddine (nouredd2), nouredd2@illinois.edu
 *   Organization:  University of Illinois at Urbana-Champaign
 *
 * =====================================================================================
 */

#include "server/server.h"
#include "puzzle/factory.h"
#include "puzzle/crypto_util.h"

#include <string.h>

/* generate_puzzle */
SHA256Challenge *generate_puzzle (unsigned char *data, unsigned int data_len,
		unsigned char *key, unsigned int key_len, 
		uint32_t timestamp, uint8_t k, uint16_t m)
{
	/**
	 * This functions implements the naiive puzzle generation algorithm described in 
	 * Jeuls1999. Basically it goes as follows. 
	 * The puzzle is composed of k subpuzzles, each subpuzzle has m bits of difficulty.
	 * To generate a subpuzzle, we first get 
	 * 		x = h (key || data || timestamp || i) 
	 * and then do 
	 * 		y = h(x) 
	 * We then mask off the first m bits of x which are to be found by
	 * the puzzle solver. 
	 */

	/* quick sanity check */
	if (!data || !key)
	{
		printf ("[ERROR]: Empty data or key passed to gerenate_puzzle!\n");
		return NULL;
	}

	uint8_t i = 0;
	SHA256SubPuzzle * head = NULL;

	timespec start, end;
	clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &start);

	// first build buf = key || data || timestamp || i 
	unsigned int buf_len = data_len + key_len + sizeof(unsigned int)
		+ sizeof(uint32_t);
	unsigned char *buf = (unsigned char *)
		malloc (buf_len * sizeof(unsigned char));

	/* build concatenation by a series of mem copies */
	unsigned char *_buf = buf;
	memcpy (_buf, key, key_len);

	_buf += key_len;
	memcpy (_buf, data, data_len);

	_buf += data_len;
	memcpy (_buf, (unsigned char*) &timestamp, sizeof(uint32_t));

	/* keep the place where to insert i each time */
	_buf = _buf + sizeof(uint32_t);

	while (i < k)
	{/* iterate over each subpuzzle */

		/* replace the last part of the buffer */
		memcpy (_buf, (unsigned char*) &i, sizeof(uint8_t));

		/* obtain the hash of the concatenation */
		unsigned int x_len;
		unsigned char *x = digest_message (buf, buf_len, &x_len);

		/* sanity checking */
		if (!x) {
			printf ("[ERROR]: Cannot build SHA256\n");
			exit(-1); /* exiting here, no need for returning */
		}

		/* obtain the hash of y */
		unsigned int y_len;
		unsigned char *y = digest_message (x, x_len, &y_len);

		/* sanity checking */
		if (!y) {
			printf ("[ERROR]: Cannot build SHA256\n");
			exit(-1); /* exiting here, no need for returning */
		}

		/* scramble the first m bits of x */
		unsigned char *preimage = scramble_bits (x, m);

		/* create and insert the subpuzzle */
		SHA256SubPuzzle *item = createSubPuzzle ();
		initSubPuzzle (item, preimage, y);

		head = insert_subpuzzle (head, item);

		/* done */
		i++;
	}

	clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &end);
	double difftime = time_diff (start, end);
	printf ("Time needed to generate the challenge is %lf seconds.\n", difftime);

	/* free the buffer */
	free (buf);

	/* create the challenge */
	SHA256Challenge *challenge = createChallenge();
	initChallenge (challenge, timestamp, k, m, head);

	/* done */
	return challenge;
} /* generate_puzzle */

/* verify_solution */
bool
verify_solution (SHA256Solution *sol, 
		unsigned char *data, unsigned int data_len,
		unsigned char *key, unsigned int key_len,
		uint8_t k)
{
	if (! sol)
		return false; /* empty challenge or solution, not verified */

	SHA256SubSolution * shead = sol->solution;
	uint32_t timestamp = sol->timestamp;
	uint8_t i = 0;

	timespec start, end;
	clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &start);

	/* prepare the buffer */
	// first build buf = key || data || timestamp || i 
	unsigned int buf_len = data_len + key_len + sizeof(unsigned int)
		+ sizeof(uint32_t);
	unsigned char *buf = (unsigned char *)
		malloc (buf_len * sizeof(unsigned char));

	/* build concatenation by a series of mem copies */
	unsigned char *_buf = buf;
	memcpy (_buf, key, key_len);

	_buf += key_len;
	memcpy (_buf, data, data_len);

	_buf += data_len;
	memcpy (_buf, (unsigned char*) &timestamp, sizeof(uint32_t));

	/* keep the place where to insert i each time */
	_buf = _buf + sizeof(uint32_t);

	while (shead)
	{/* iterate over all subpuzzles */
		unsigned char *digest = shead->solution;

		/*  prepare the buffer data */
		memcpy (_buf, (unsigned char *)&i, sizeof (uint8_t));

		/* obtain the hash of the concatenation */
		unsigned int x_len;
		unsigned char *x = digest_message (buf, buf_len, &x_len);

		/* compare the two hashes */
		if (memcmp (x, digest, x_len) != 0) 
		{
			printf ("Failing at %d\n", i);
			OPENSSL_free (x);
			free (buf);
			return false; /* if one does not match that's it! */
		}

		OPENSSL_free (x);
		i++;
		shead = shead->next;
	}

	clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &end);
	double difftime = time_diff (start, end);
	printf ("Time needed to verify the solution is %lf seconds.\n", difftime);

	free (buf);

	/* check if all subpuzzles solved */
	if (i != k) {
		return false;
	}

	/* no violations found, return true */
	return true;
} /* verify_solution */

/* scramble_bits */
unsigned char *
scramble_bits (unsigned char *buf, unsigned int bits)
{
	if (! buf)
		return NULL;

	/* scramble the first `bits' bits of buf */
	unsigned int pos = 0;
	while (bits)
	{
		unsigned char mask = (unsigned char) rand() % 255;
		if (bits >= 8)
		{
			buf[pos] = 0;
		} else 
		{
			/* the last (8-bits) bits of mask should be 0 */
			mask = mask << (8 - bits);
			buf[pos] = buf[pos] | mask;
		}

		bits = bits >> 3;
	}

	return buf;
}
