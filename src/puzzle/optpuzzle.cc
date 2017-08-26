/*
 * =====================================================================================
 *
 *       Filename:  optpuzzle.cc
 *
 *    Description:  Implementation of the optimized puzzle structures
 *
 *        Version:  1.0
 *        Created:  08/14/2017 05:07:39 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Mohammad Noureddine (nouredd2), nouredd2@illinois.edu
 *   Organization:  University of Illinois at Urbana-Champaign
 *
 * =====================================================================================
 */

#include "puzzle/optpuzzle.h"
#include <openssl/evp.h>

/* initOptChallenge */
void
initOptChallenge (SHA256OptChallenge *challenge, unsigned char *_preimage,
		unsigned int _timestamp, uint16_t _len,
		uint16_t _np, uint16_t _diff)
{
	if (! challenge)
		return; /* nothing to do with empty stuff */

	/* simply do the assignments */
	challenge->preimage  		= _preimage;
	challenge->timestamp 		= _timestamp;
	challenge->len       		= _len;
	challenge->num_subpuzzles 	= _np;
	challenge->difficulty 		= _diff;
} /* initOptChallenge */


/* initOptSolution */
void
initOptSolution (SHA256OptSolution *sol,
		unsigned int _timestamp, 
		SHA256OptSubSolution *_head)
{
	if (! sol)
		return; /* nothing to do */

	/* simply do the assignments */
	sol->timestamp  = _timestamp;
	sol->head 		= _head;
} /* initOptSolution */


/* initOptSubSolution */
void
initOptSubSolution (SHA256OptSubSolution *ssol,
		unsigned char *_zi, SHA256OptSubSolution *_next)
{
	if (! ssol)
		return;	/* nothing to do */

	/* simply do the assignment */
	ssol->zi   = _zi;
	ssol->next = _next;
} /* initOptSubSolution */


/* insert_subsolution */
SHA256OptSubSolution * 
insert_subsolution (SHA256OptSubSolution *head,
		SHA256OptSubSolution *item)
{
	if (! head)
		return item; /* if head is null return new head */

	SHA256OptSubSolution *tail = head;
	while (tail->next)
	{
		tail = tail->next;
	}

	/* found the tail, do the insertion */
	tail->next = item;
	item->next = NULL;

	return head;
} /* insert_subsolution */


/* free_solution_mem */
void
free_solution_mem (SHA256OptSolution *sol)
{
	if (! sol)
		return; /* nothing to do */

	SHA256OptSubSolution *head = sol->head;

	if (head) 
	{
		while (head)
		{
			SHA256OptSubSolution *next = head;
			head = head->next;

			/* free the memory */
			OPENSSL_free (next->zi);
			free (next);
		}
	}

	/* done free the actual solution memory */
	free (sol);
} /* free_solution_mem */
