#include "puzzle/puzzle.h"
#include <openssl/evp.h>

/* initSubPuzzle */
void
initSubPuzzle (SHA256SubPuzzle *subpuzzle)
{
	if (! subpuzzle)
		return; /* Nothing to do with NULL input */
	
	/* initialize everything to NULL */
	subpuzzle->preimage = NULL;
	subpuzzle->image = NULL;
	subpuzzle->next = NULL;
}

/* initSubPuzzle */
void 
initSubPuzzle (SHA256SubPuzzle *subpuzzle,
		unsigned char *_preimage,
		unsigned char *_image)
{
	if (! subpuzzle)
		return; /* Nothing to do with NULL input */

	/* initialize the member of the subpuzzle */
	subpuzzle->preimage = _preimage;
	subpuzzle->image = _image;
	subpuzzle->next = NULL;
} /* initSubPuzzle */

/* initChallenge */
void 
initChallenge (SHA256Challenge *challenge,
		uint32_t _timestamp,
		uint8_t _num_subpuzzles,
		uint16_t _difficulty,
		SHA256SubPuzzle *head)
{
	if (! challenge)
		return; /* Nothing to do with NULL input */


	/* Set the appropriate arguments */
	challenge->timestamp = _timestamp;
	challenge->num_subpuzzles = _num_subpuzzles;
	challenge->difficulty = _difficulty;

	challenge->puzzle = head;
}

/* initSubSolution */
void 
initSubSolution (SHA256SubSolution *subsolution)
{
	if (! subsolution)
		return; /* Nothing to do with NULL input */

	/* initialize everything to NULL */
	subsolution->solution = NULL;
	subsolution->next = NULL;
}

/* initSolution */
void 
initSolution (SHA256Solution *solution,
		uint32_t _timestamp,
		SHA256SubSolution *head)
{
	if (! solution)
		return; /* Nothing to do with NULL input */

	/* Set up the arguments correctly */
	solution->timestamp = _timestamp;
	solution->solution = head;
}

/* insert_subsolution */
void insert_subsolution (SHA256SubSolution *head,
        SHA256SubSolution *item)
{
    if (head == NULL) /* empty list, do nothing */
        return; 

    /* iterate until the tail */
    SHA256SubSolution * tail = head;
    while (tail->next) {
        tail = tail->next;
    }

    /* append the item */
    tail->next = item;
    item->next = NULL;
}

/* insert_subpuzzle */
SHA256SubPuzzle * insert_subpuzzle (SHA256SubPuzzle *head,
		SHA256SubPuzzle *item)
{
	if (head == NULL) /* empty list, return the iteam as head */
		return item;

	SHA256SubPuzzle *tail = head;
	while (tail->next) {
		tail = tail->next;
	}

	/* append the item */
	tail->next = item;
	item->next = NULL;

	/* always return the head */
	return head;
}

/* free_challenge_mem */
void
free_challenge_mem (SHA256Challenge *challenge)
{
	if (challenge == NULL) return;

	free_list (challenge->puzzle);
}

/* free_list */
void
free_list (SHA256SubPuzzle *head)
{
	if (head == NULL) return;

	while (head)
	{/* iterate until all elements are free */
		SHA256SubPuzzle *next = head;
		head = head->next;

		if (next->preimage) { OPENSSL_free (next->preimage); }

		if (next->image) { OPENSSL_free (next->image); }

		free (next);
	}
}

/* free_solution_mem  */
void
free_solution_mem (SHA256Solution *sol)
{
	if (sol == NULL) return;

	free_solution_list (sol->solution);
}

/* free_solution_list */
void
free_solution_list (SHA256SubSolution *head)
{
	if (head == NULL) return;

	while (head) 
	{/* iterate over all the list */
		SHA256SubSolution *next = head;
		head = head->next;

		if (next->solution) { OPENSSL_free (next->solution); }

		free (next);
	}
}

