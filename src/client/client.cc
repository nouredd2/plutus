/*
 * =====================================================================================
 *
 *       Filename:  client.cc
 *
 *    Description:  Implementation of the puzzle solving capabilities
 *
 *        Version:  1.0
 *        Created:  08/09/2017 02:32:07 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Mohammad Noureddine (nouredd2), nouredd2@illinois.edu
 *   Organization:  University of Illinois at Urbana-Champaign
 *
 * =====================================================================================
 */

#include "client/client.h"
#include "puzzle/factory.h"
#include "puzzle/crypto_util.h"

#include <assert.h>
#include <math.h>
#include <time.h>
#include <string.h>

#ifndef IMAGE_LEN
#define IMAGE_LEN 32
#endif

/* solvePuzzle */
SHA256Solution *
solvePuzzle (SHA256Challenge *challenge)
{
    /* Error checking */
    if (! challenge) {
        printf ("[ERROR]: Cannot find challenge to solve!\n");
        return NULL;
    }

    /* Extract arguments  */
    uint32_t ts = challenge->timestamp;
    // uint8_t ns = challenge->num_subpuzzles;
    uint16_t diff = challenge->difficulty;
    
    SHA256SubPuzzle *head = challenge->puzzle;
    if (! head) { /* error checking */
        printf ("[ERROR}: Empty challenge!\n");
        return NULL;
    }

    /* create needed structures */
    SHA256SubSolution *sol_head = NULL;

	timespec start, end;
	clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &start);
    while (head) 
    { /* Iterate until reaching the tail */

        /* Using the naming convention of Juels' paper */
        unsigned char *x = head->preimage;
        unsigned char *y = head->image;

        unsigned int max_possible = 0x01 << diff;
        unsigned int itr = 0;

        while (itr < max_possible) { /* currenlty, iterate in order */
            unsigned int mask_len;
            unsigned char *mask = get_puzzle_mask (itr, diff, &mask_len);    

            /* WARNING: THIS WILL OVERWRITE THE ORIGINAL PUZZLE */
            x = clear_bits (x, diff);
            if (mask_len == 1) {/* replace the left most byte */
                x[0] = x[0] | mask[0];
            } else {/* replace two left most bytes */
                x[0] = mask[0];
                x[1] = x[1] | mask[1];
            }

			/* somve verbose debugging */
			// puts ("Now trying:");
			// print_digest (x, IMAGE_LEN);

            /* compute the message digest */
            unsigned int digest_len;
            unsigned char * digest = digest_message (x, IMAGE_LEN, &digest_len);

            /* sanity checking */
            assert (digest_len == IMAGE_LEN);

            int n = compare_digests (digest, y, digest_len);
            if (n == 0) {/* found the solution for this subpuzzle */
                /* copy correct preimage */
                unsigned char *sol = (unsigned char *)
                    malloc (IMAGE_LEN * sizeof(unsigned char));
                memcpy (sol, x, IMAGE_LEN);

                /* add it to the list of subsolutions */
                if (sol_head == NULL) { /* empty list */
                    sol_head = createSubSolution();
                    sol_head->solution = sol;
                    sol_head->next = NULL;
                } else { /* non-empty list */
                    SHA256SubSolution *sol_item = createSubSolution();
                    sol_item->solution = sol;
                    sol_item->next = NULL;

                    insert_subsolution (sol_head, sol_item);
                }

                /* free some stuff and break */
                free (mask);
                OPENSSL_free (digest);

                break;
            }


            /* increment counter */
			// print_digest (digest, digest_len);
            free (mask);
            OPENSSL_free (digest);
            itr++;
        }

		if (itr == max_possible) { /* couldn't find a solution */
			printf("[ERROR]: Could not find a solution!\n");
			return NULL;
		}
		printf ("[Log]: Obtained solution in %d trials.\n", (itr+1));


        /* Move through the list */
        head = head->next;
    }

	clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &end);
	double difftime = time_diff (start, end);
	printf ("Time needed to find solution is %lf seconds.\n", difftime);

	/* some sanity checking */
	assert (sol_head != NULL);


    /* done. create a challenge solution and return it */
    SHA256Solution *chall_sol = createSolution();
    chall_sol->timestamp = ts;
    chall_sol->solution  = sol_head;

    return chall_sol;
}


/*-----------------------------------------------------------------------------
 *  Utility functions
 *-----------------------------------------------------------------------------*/

/* get_puzzle_mask */
unsigned char *
get_puzzle_mask (unsigned int itr, 
        unsigned int diff, unsigned int *mask_len)
{
    /* currently not supporting more than 16 bits of difficulty */
    assert (diff <= 16);

    /* get how many bytes do we need */
    // if less than 8 bits of difficulty then need 1 byte,
    // else we need 2 bytes
    size_t byte_len = (diff <= 8)? 1:2;

    /* allocate the return byte array */
    unsigned char * mask = (unsigned char *)
        calloc (byte_len,  sizeof (unsigned char));


    if (byte_len == 1) { /* only one byte mask */
        /* align msb of itr with the start of the mask (to the byte) */
        itr = itr << (8 - diff);

        /* extract least byte of itr */
        mask[0] = 0xFF & itr;
    } else {/* two byte mask */
        /* align msb of itr with the start of the mask (to the byte) */
        itr = itr << (16-diff);

        mask[1] = 0xFF & itr;
        mask[0] = 0xFF & (itr >> 8);
    }

    /* return the required things */
    *mask_len = byte_len;
    return mask;
}

/* clear_bits */
unsigned char *
clear_bits (unsigned char *msg, uint8_t len)
{
    /* currently not supporting more than 16 bits of difficulty */
    assert (len <= 16);

    if (len <= 8) {/* clear left most len bits */
        // kill off len leading bits
        msg[0] = msg[0] & (0xFF >> len);
    } else {/* clean left most 8 + (len - 8) bits */
        msg[0] = 0;
        // kill of (len-8) leading bits of msg[1]
        msg[1] = msg[1] & (0xFF >> (len-8));
    }

	return msg;
}

/* compare_digests */
int 
compare_digests (unsigned char *a, 
        unsigned char *b, unsigned int digest_len)
{
    /* compare digests byte by byte */
    return memcmp (a, b, digest_len);
}
