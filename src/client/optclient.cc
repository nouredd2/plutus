/*
 * =====================================================================================
 *
 *       Filename:  optclient.cc
 *
 *    Description:  Implementation of the puzzle solving capabilities in the optimized
 *    				version of the puzzle generation
 *
 *        Version:  1.0
 *        Created:  08/25/2017 10:16:26 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Mohammad Noureddine (nouredd2), nouredd2@illinois.edu
 *   Organization:  University of Illinois at Urbana-Champaign
 *
 * =====================================================================================
 */

#include "client/optclient.h"
#include "puzzle/factory.h"
#include "puzzle/crypto_util.h"

#include <assert.h>
#include <time.h>
#include <string.h>

/* solveChallenge */
SHA256OptSolution *
solveChallenge (SHA256OptChallenge *challenge)
{	
	if (!challenge) 
	{
		printf ("[ERROR]: Cannot find challenge to solve!\n");
		return NULL;
	}

	/* get the puzzle information */
	uint32_t timestamp = challenge->timestamp;
	uint16_t l = challenge->len;
	uint16_t k = challenge->num_subpuzzles;
	uint16_t m = challenge->difficulty;

	uint16_t len = l/2;

	unsigned char *preimage = challenge->preimage;

	/* quick error checking */
	if  (!preimage)
	{
		printf ("[ERROR]: Empty challenge to solve. Nothing to do!\n");
		return NULL;
	}

	SHA256OptSubSolution *head = NULL;

	/* get starting time */
	timespec start, end;
	clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &start);

	for (uint16_t i = 0; i < k; i++)
	{ /* iteratore over all the subpuzzles */
		/* allocated a buffer big enough to hold what we need to concatenate */
		unsigned int buf_len = l + sizeof (uint16_t);
		unsigned char *buf = (unsigned char *)
			malloc (buf_len * sizeof (unsigned char));

		/* create the substring x || i */
		unsigned char *cbuf = append_buffer (buf, preimage, len);
		cbuf = append_buffer (cbuf, (unsigned char *)(&i), sizeof (uint16_t));

		/* start trying the z's */
		bool found = false;
		uint16_t itr = 0;
		while (!found)
		{ /* keep iterating until you find something */
			unsigned char *zi = (unsigned char*) calloc (len, sizeof(unsigned char));
			memcpy (zi + len - sizeof(uint16_t), 
					(unsigned char *)&itr, sizeof(uint16_t));

			/* form x || i || zi */
			append_buffer (cbuf, zi, len);

			/* get the hash of the full buffer */
			unsigned int dlen;
			unsigned char *digest = 
				digest_message (buf, buf_len, &dlen);

			/* compare the first m bits */
			found = compare_bits (buf, digest, m);
			itr++;

			/* check if found and create a sub solution */
			if (found)
			{
				/* create a subsolution */
				SHA256OptSubSolution *sub = create_optsubsolution();

				/* copy the solution to save it */
				unsigned char * zic = (unsigned char *) malloc (len);
				memcpy (zic, zi, len);
				initOptSubSolution (sub, zic, NULL);

				/* insert the sub solution into the list */
				head = insert_subsolution (head, sub);
			}

			/* free the digest */
			free (zi);
			OPENSSL_free (digest);
		}

		/* just print how many iterations it took */
		printf ( "[Log]: Found solution in %d iterations.\n", (itr-1) );

		/* free the allocated buffer */
		free (buf);
	}

	clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &end);
	double difftime = time_diff (start, end);
	printf ( "[Log]: Time need to find solution if %lf seconds.\n", difftime);

	SHA256OptSolution *sol = create_optsolution();
	initOptSolution (sol, timestamp, head);

	return sol;

} /* solveChallenge */
