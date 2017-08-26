/*
 * =====================================================================================
 *
 *       Filename:  client_test.cc
 *
 *    Description:  Testing the client's abilities to solve puzzles
 *
 *        Version:  1.0
 *        Created:  08/10/2017 11:53:20 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Mohammad Noureddine (nouredd2), nouredd2@illinois.edu
 *   Organization:  University of Illinois at Urbana-Champaign
 *
 * =====================================================================================
 */

#include "client/client.h"
#include "puzzle/crypto_util.h"
#include "puzzle/factory.h"
#include <time.h>
#include <unistd.h>
#include <ctype.h>

#ifndef IMAGE_LEN
#define IMAGE_LEN 32
#endif

typedef struct {
	unsigned int k;		/* number of subpuzzles */
	unsigned int m;		/* number of bits of difficulty */
	bool verbose;		/* verbose option */
} arguments;

/* read command line arguments */
int
read_comd_args (int argc, char **argv, 
		arguments *args);

/* get a random character */
unsigned char
get_random_char()
{
	return (unsigned char) (rand()%255);
} /* get_random_char */

/* build a random subpuzzle */
int
build_random_subpuzzle (SHA256SubPuzzle *subpuzzle)
{
	/* build a random pre-image */
	unsigned char * preimage = (unsigned char *)
		malloc (IMAGE_LEN * sizeof(unsigned char));

	for (unsigned int i=0; i < IMAGE_LEN; i++)
		preimage[i] = get_random_char();

	// puts ("The preimage of the subpuzzle:");
	// print_digest (preimage, IMAGE_LEN);

	/* compute the image using SHA256 */
	unsigned int image_len;
	unsigned char * image = digest_message (preimage,
			IMAGE_LEN, &image_len);
	if (! image) return -1;


	//puts ("The image of the subpuzzle:");
	//print_digest (image, image_len);

	/*  make the assignment */
	subpuzzle->preimage = preimage;
	subpuzzle->image = image;

	return 0;
} /* build_random_subpuzzle */

int
main (int argc, char **argv)
{
	arguments args;
	args.verbose = false;
	int argerr = read_comd_args (argc, argv, &args);
	if (argerr != 0)
		exit (-1);


	SHA256Challenge *challenge = createChallenge();

	/* seed the random generator */
	srand(time(NULL));

	/* phony data and timestamps */
	uint32_t 	timestamp = 1.0;
	uint8_t 	num_subpuzzles = args.k; 	/* number of subpuzzles */
	uint16_t 	difficulty = args.m;		/* bits of difficulty */

	/* create the list of sub-puzzles */
	SHA256SubPuzzle *head = NULL;

	for (unsigned int i=0; i < num_subpuzzles; i++)
	{
		SHA256SubPuzzle *item = createSubPuzzle();
		initSubPuzzle (item);
		int err = build_random_subpuzzle (item);
		if (err != 0) 
		{/* failed doing sha256(preimage) */
			printf ("[ERROR]: Cannot create a random subpuzzle!\n");

			/* free stuff up */
			if (head != NULL) free_list (head);
			free (challenge);

			exit (-1);
		}

		head = insert_subpuzzle (head, item);
	}

	/* WARNING: I am not clearing out the diff bits of the subpuzzle's preimage */
	initChallenge (challenge, timestamp, num_subpuzzles, difficulty, head);

	/* now solve the subpuzzle */
	SHA256Solution *sol = solvePuzzle (challenge);

	/* print the solution  */
	// print_digest (sol->solution->solution, IMAGE_LEN);

	/* free stuff up */
	free_challenge_mem (challenge);
	free (challenge);

} /* main */

/* read command line arguments */
int
read_comd_args (int argc, char **argv, 
		arguments *args)
{
	int c;
	int k = -1;
	int m = -1;

	while ( (c = getopt (argc, argv, "k:m:hv")) != -1)
	{
		switch (c)
		{
			case 'k':
				k = atoi(optarg);
				break;
			case 'm':
				m = atoi(optarg);
				break;
			case 'v':
				args->verbose = true;
				break;
			case 'h':
				printf ("Usage: %s -k num_subpuzzle -m bits_difficulty [-vh?]\n", argv[0]);
				return -1;
			case '?':
				if (optopt == 'k' || optopt == 'm')
					printf ("[ERROR]: Option -%c requires an argument.\n", optopt);
				else if (isprint (optopt))
					printf ("[ERROR]: Unknow option `-%c'.\n", optopt);
				else
					printf ("[ERROR]: Unknow option character `\\x%x'.\n",
							optopt);
				return -1;
			default:
				return -1;
		}
	}

	for (int index=optind; index < argc; index++)
	{
		printf ("[ERROR]: Non-optim argument %s\n", argv[index]);
		return -1;
	}

	/* check that both k and m are set */
	if (k == -1 || m == -1)
	{
		printf ("[ERROR]: Usage: %s -k num_subpuzzle -m bits_difficulty [-vh?]\n", 
				argv[0]);
		return -1;
	}

	/* pass on the arguments */
	args->k = k;
	args->m = m;

	return 0;
}
