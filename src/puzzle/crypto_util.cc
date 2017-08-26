/*
 * =====================================================================================
 *
 *       Filename:  crypto_util.cc
 *
 *    Description:  implementation of the crypto util functions 
 *
 *        Version:  1.0
 *        Created:  08/09/2017 03:35:13 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Mohammad Noureddine (nouredd2), nouredd2@illinois.edu
 *   Organization:  University of Illinois at Urbana-Champaign
 *
 * =====================================================================================
 */

#include "puzzle/crypto_util.h"
#include <string.h>
#include <math.h>

/* digest_message */
unsigned char *
digest_message (const unsigned char *message, size_t message_len,
        unsigned int *digest_len)
{
    EVP_MD_CTX *mdctx;

    /* create a message digest context */
    mdctx = EVP_MD_CTX_create();
    if (mdctx == NULL) {
        printf ("[ERROR]: Failed to create digest context!\n");
        return NULL;
    }

    /* initialize the digest context */
    int err = EVP_DigestInit_ex (mdctx, EVP_sha256(), NULL);
    if (err != 1) {
        printf ("[ERROR]: Failed to initialized digest context!\n");
        return NULL;
    }

    /* update the digest context with the message */
    err = EVP_DigestUpdate (mdctx, message, message_len);
    if (err != 1) {
        printf ("[ERROR]: Failed to update digest context!\n");
        return NULL;
    }

    /* allocate the digest  */
    unsigned char *digest = (unsigned char *)
        OPENSSL_malloc (EVP_MD_size(EVP_sha256()));
    if (! digest) {
        printf ("[ERROR]: Failed to allocate digest!\n");
        return NULL;
    }

    /* finalize the operation */
    err = EVP_DigestFinal_ex (mdctx, digest, digest_len);
    if (err != 1) {
        printf ("[ERROR]: Failed to perform sha256 digest!\n");
        return NULL;
    }

    /* done. destory the context */
    EVP_MD_CTX_destroy (mdctx);

    return digest;
}

/* print a digest */
void
print_digest (unsigned char *digest, unsigned int len)
{

	for ( unsigned int i=0; i<len; i++ ) {
		printf ("%x", digest[i]);
	}
	printf("\n");
}


/* time_diff */
double
time_diff (timespec t1, timespec t2)
{
	timespec temp;

	if ( (t2.tv_nsec - t1.tv_nsec) < 0 )
	{
		temp.tv_sec = t2.tv_sec - t1.tv_sec -1;
		temp.tv_nsec = 1000000000 + t2.tv_nsec - t1.tv_nsec;
	} else 
	{
		temp.tv_sec = t2.tv_sec - t1.tv_sec;
		temp.tv_nsec = t2.tv_nsec - t1.tv_nsec;
	}

	double seconds = pow(10.0, -9) * temp.tv_nsec  + (double) temp.tv_sec;

	return seconds;
}


/* append_buffer */
unsigned char *
append_buffer (unsigned char *dest, unsigned char *src, size_t n)
{
	if (!dest || !src)
		return NULL; /* empty inputs. */

	/* copy the source data into the destination */
	memcpy (dest, src, n);

	/* return the pointer to the end of the buffer */
	return dest + n;
} /* append_buffer */

/* compare_bits */
bool
compare_bits (unsigned char *x, unsigned char *y, unsigned int len)
{
	/* sanity checks */
	if (!x || !y)
		return false;
	
	/* check empty stuff */
	if (len == 0)
		return true;

	/* compare the first len bits of x and y */
	bool cmp = ( memcmp (x,y, len/8) == 0 );

	/* do this anyway to account for the case where len/8 is 0 */
	unsigned int rem = len%8;
	if (rem > 0)
	{
		unsigned int index = len/8;
		/* get copies of the bytes in question */
		unsigned char cx = x[index], cy = y[index];
		
		/* shift right to make the byte contain 
		 * (8 - index)0's || (index) bits of c 
		 * */
		cx = cx >> (8 - rem);
		cy = cy >> (8 - rem);

		return (cx == cy) && cmp;
	}

	/* no remaining bits, return outcome of memcmp, which can only be true here */
	return cmp;
} /* compare_bits */
