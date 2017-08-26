/*
 * =====================================================================================
 *
 *       Filename:  server_test.cc
 *
 *    Description:  Tests for the server puzzle generation capabilities
 *
 *        Version:  1.0
 *        Created:  08/11/2017 01:45:42 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Mohammad Noureddine (nouredd2), nouredd2@illinois.edu
 *   Organization:  University of Illinois at Urbana-Champaign
 *
 * =====================================================================================
 */

#include <server/server.h>
#include <client/client.h>
#include <puzzle/crypto_util.h>
#include <puzzle/factory.h>

#include <time.h>
#include <ctype.h>
#include <unistd.h>

#ifndef IMAGE_LEN
#define IMAGE_LEN 32
#endif 

#ifndef KEY_LEN
#define KEY_LEN 128
#endif

#ifndef DATA_LEN
#define DATA_LEN 512
#endif

/* struct to hold the arguments for the program */
typedef struct {
	unsigned int k;
	unsigned int m;
	bool verbose;
} arguments_t;

/* read command line arguments */
int
read_cmd_args (int argc, char **argv, arguments_t *args);

/* create random set of bytes */
void create_random_bytes (unsigned char *buf, 
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

	/* get the stuff */
	uint32_t timestamp = 1;
	uint8_t k = args.k;
	uint16_t m = args.m;

	/* seed the pseudo-random number generator */
	srand (time(NULL));

	/* so we need a random set of data and a random secret key.
	 * assume that the random key length is 128 bytes and generate
	 * random data of size 512 bytes.
	 */
	unsigned char *key = (unsigned char*)
		malloc (KEY_LEN * sizeof(unsigned char));
	create_random_bytes (key, KEY_LEN);

	unsigned char *data = (unsigned char*)
		malloc (DATA_LEN * sizeof(unsigned char));
	create_random_bytes (data, DATA_LEN);

	/* generate the puzzle */
	SHA256Challenge *challenge = generate_puzzle (data, DATA_LEN,
			key, KEY_LEN,
			timestamp, k, m);

	/* attempt puzzle solution */
	SHA256Solution *sol = solvePuzzle (challenge);

	/* verify the solution */
	bool v = verify_solution (sol, data, DATA_LEN, key, KEY_LEN, k);
	if (!v)
	{
		printf ("[ERROR]: Solution verification failed!\n");
	}

	/* free things */
	free_challenge_mem (challenge);
	free_solution_mem (sol);
	free (data);
	free (key);
	free (challenge);
	free (sol);
	

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
} /* read_cmd_args */
