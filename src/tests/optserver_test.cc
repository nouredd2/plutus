/*
 * =====================================================================================
 *
 *       Filename:  optserver_test.cc
 *
 *    Description:  Testing the optimized implementation of generating and verifying 
 *    				challenges
 *
 *        Version:  1.0
 *        Created:  08/24/2017 02:21:01 PM
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
#include "client/optclient.h"

#include <time.h>
#include <ctype.h>
#include <unistd.h>

#ifndef KEY_LEN
#define KEY_LEN 128 /* in bytes */
#endif

#ifndef DATA_LEN
#define DATA_LEN 256 /* in bytes */
#endif

#ifndef IMAGE_LEN
#define IMAGE_LEN 32 /* in bytes */
#endif


/* struct to hold the arguments for the program */
typedef struct {
	unsigned int k;
	unsigned int m;
	unsigned int l;
	bool verbose;
} arguments_t;

/* read command line arguments */
int
read_cmd_args (int argc, char **argv, arguments_t *args);

/* create random set of bytes */
static void create_random_bytes (unsigned char *buf,
		unsigned int buf_len);

int
main (int argc, char **argv)
{
	/* try to read cmd line arguments */
	arguments_t args;
	if ( read_cmd_args (argc, argv, &args) != 0 )
	{
		exit(-1);
	}

	/* get the arguments */
	uint32_t timestamp = 1;
	uint16_t k = args.k;
	uint16_t m = args.m;
	unsigned int l = args.l;

	/* seed the pseudo-rando number generator */
	srand (time(NULL));

	/* we need to generatekey and a random set of data
	 * they key size is KEY_LEN and the image length 
	 * is IMAGE_LEN in bytes 
	 */
	unsigned char *key = (unsigned char *)
		malloc ( KEY_LEN * sizeof (unsigned char) );
	create_random_bytes (key, KEY_LEN);

	unsigned char *data = (unsigned char *)
		malloc ( DATA_LEN * sizeof (unsigned char) );
	create_random_bytes (data, DATA_LEN);

	/* generate the puzzle */
	SHA256OptChallenge *challenge = 
		generate_challenge (data, DATA_LEN,
				key, KEY_LEN, timestamp, k, m, l);

	/* find the solution */
	SHA256OptSolution *sol = solveChallenge (challenge);

	/* verify the solution */
	bool verified = verify_solution (sol, data, DATA_LEN,
			key, KEY_LEN, l, k, m);

	if (!verified) 
		printf ("[Log]: Solution verification failed!\n");
	else
		printf ("[Log]: Solution verified!\n");


	/* free the memory allocated */
	free_solution_mem (sol);
	free (key);
	free (data);
	OPENSSL_free (challenge->preimage);
	free (challenge);

	return 1;
} /* main */

void
create_random_bytes (unsigned char *buf, unsigned int buf_len)
{
	if (! buf)
		return; /* nothing to do */
	
	for (unsigned int i=0;i<buf_len;i++)
	{
		buf[i] = (unsigned char) rand()%255;
	}
} /* create_random_bytes */


int
read_cmd_args (int argc, char **argv, arguments_t *args)
{
	int c;
	int k = -1;
	int m = -1;
	args->l = 128; /* default value */

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
				printf ("Usage: %s -k num_subpuzzle\
						-m bits_difficulty -l prefix_len (128) [-vh?]\n", 
						argv[0]);
				return -1;
			case 'l':
				args->l = atoi(optarg);
				break;
			case '?':
				if (optopt == 'k' || optopt == 'm' || optopt == 'l')
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
		printf ("Usage: %s -k num_subpuzzle -m bits_difficulty -l prefix_len [-vh?]\n", 
				argv[0]);
		return -1;
	}

	/* pass on the arguments */
	args->k = k;
	args->m = m;

	return 0;
} /* read_cmd_args */
