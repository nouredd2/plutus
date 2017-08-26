/*
 * =====================================================================================
 *
 *       Filename:  optserver.cc
 *
 *    Description:  Implementation of the puzzle generation and verification routines
 *
 *        Version:  1.0
 *        Created:  08/15/2017 12:06:37 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Mohammad Noureddine (nouredd2), nouredd2@illinois.edu
 *   Organization:  University of Illinois at Urbana-Champaign
 *
 * =====================================================================================
 */

#include "server/optserver.h"
#include "puzzle/factory.h"
#include "puzzle/crypto_util.h"

#include <string.h>

/* generate_challenge */
SHA256OptChallenge *
generate_challenge (unsigned char *data, unsigned int data_len,
		unsigned char *key, unsigned int key_len,
		uint32_t timestamp, uint16_t k, uint16_t m, unsigned int l)
{
	/**
	 * Here we generate the challenge using the optimized version presented 
	 * in the Juels 1999 paper on client puzzles. Basically it goes as follows.
	 * We only to do one hash operation 
	 * 		x = h (key || data || timestamp)
	 * and then set x to be a certain (l/2) bits of x. We'll assume they are the 
	 * first (l/2) bits of x. And that is all we need to generate the puzzle.
	 */

	/* quick sanity check */
	if (!data || !key)
	{
		printf ("[ERROR]: Empty data or key passed to gerenate_puzzle!\n");
		return NULL;
	}

	/* record timing information */
	timespec start, end;
	clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &start);

	/* build the buffer = key || data || timestamp */
	unsigned int buf_len = data_len + key_len + sizeof (uint32_t);
	unsigned char *buf = (unsigned char *)
		malloc (buf_len * sizeof (unsigned char));

	/* start appending the concatenation elements */
	unsigned char *tbuf = buf;
	tbuf = append_buffer (tbuf, key, key_len);
	tbuf = append_buffer (tbuf, data, data_len);
	tbuf = append_buffer (tbuf, (unsigned char*) &timestamp, 
			sizeof (uint32_t));

	/* perform the hasing operation */
	unsigned int h_len;
	unsigned char *h = digest_message (buf, buf_len, &h_len);

	/* another sanity check */
	if (!h)
	{
		printf ("[ERROR]: Cannot build SHA256\n");
		exit (-1); /* something really went wrong, just exit */
	}

	/* save the first l bits of x */
	if (l % 8 != 0) 
	{
		printf ("[ERROR]: (l/2) needs to be a multiple of 8.\n");
		exit(-1);
	}

	unsigned int x_len = (l/2)/8;
	unsigned char *x = (unsigned char *)
		malloc (x_len * sizeof (unsigned char));
	/* copy the data from the hash output */
	memcpy (x, h, x_len);

	/* allocate and initialize the challenge */
	SHA256OptChallenge *challenge = create_optchallenge ();
	initOptChallenge (challenge, x, timestamp, 2*x_len, k, m);

	/* record the end timing */
	clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &end);
	double difftime = time_diff (start, end);
	printf ("Time needed to generate the challenge is %lf seconds.\n", difftime);

	/* free the created buffer */
	OPENSSL_free (h);
	free (buf);

	/* done here. return */
	return challenge;
} /* generate_challenge */

/* verify_solution */
bool
verify_solution (SHA256OptSolution *sol,
		unsigned char *data, unsigned int data_len,
		unsigned char *key, unsigned int key_len,
		uint16_t len, uint16_t k, uint16_t m)
{
	if (!sol)
		return false; /* empty solution then return false */

	SHA256OptSubSolution *head = sol->head;
	uint32_t timestamp = sol->timestamp;
	uint16_t i = 0; /* the iterator over the k subsolutions */

	/* record timing information */
	timespec start, end;
	clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &start);

	/* prepare the buffer for the initial hash operation */
	unsigned int buf_len = key_len + data_len + sizeof(uint32_t);
	unsigned char *buf = (unsigned char *)
		malloc (buf_len * sizeof(unsigned char));

	/* build the concatenation */
	unsigned char *tbuf = buf;
	tbuf = append_buffer (tbuf, key, key_len);
	tbuf = append_buffer (tbuf, data, data_len);
	tbuf = append_buffer (tbuf, (unsigned char*) &timestamp, 
			sizeof(uint32_t));

	/* get the hash of the concatenation */
	unsigned int hlen;
	unsigned char *h = digest_message (buf, buf_len, &hlen);

	/* get the first (l/2) bits of h */
	unsigned int l = len/2;
	if (l % 8 != 0) 
	{
		printf ("[ERROR]: (l/2) needs to be a multiple of 8.\n");
		exit(-1);
	}

	unsigned int xlen = l/8;
	unsigned char *x = (unsigned char *)
		malloc (xlen * sizeof (unsigned char));
	memcpy (x, h, xlen);

	/* free h, we no longer need it */
	OPENSSL_free (h);
	free (buf);

	/* only need one place holder for doing hashes, it is
	 * x || i || zi
	 */
	unsigned int digestlen = 2*xlen + sizeof(uint16_t);
	unsigned char *digestptr = (unsigned char *)
		malloc (digestlen * sizeof(unsigned char));

	/* put in x from now since it is fixed everywhere */
	unsigned char *digest = append_buffer (digestptr, x, xlen);
	
	while (i < k) 
	{ /* iterate over all subpuzzles */
		unsigned char *zi = head->zi;

		/* sanity check */
		if (!zi) 
		{
			printf ("[ERROR]: Empty sub solution!\n");
			exit(-1);
		}

		/* build the concatenation */
		unsigned char *tmp = append_buffer (digest, (unsigned char *)&i, 
				sizeof(uint16_t));
		tmp = append_buffer (tmp, zi, xlen);

		/* build the hash of the digest */
		unsigned int hashlen;
		unsigned char *hash = digest_message (digestptr, digestlen, &hashlen);


		/* verify that first m bits of (x || i || zi) are the same as h(x||i||zi) */
		bool cmp = compare_bits (hash, digestptr, m);

		if (!cmp) 
		{/* free stuff and return */
			OPENSSL_free (hash);
			free (digestptr);
			free (x);
			return false;
		}

		/* subpuzzle check passed! */
		OPENSSL_free (hash);

		head = head->next;
		i++;
	}

	clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &end);
	double difftime = time_diff (start, end);
	printf ("[Log]: Time needed to verify the solution is %lf seconds.\n", difftime);

	/* free allocated memory */
	free (digestptr);
	free (x);

	/* done here, verification passed */
	return true;
} /* verify_solution */
