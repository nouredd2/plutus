/*
 * =====================================================================================
 *
 *       Filename:  crypto_util.h
 *
 *    Description:  cryptographic utility functions
 *
 *        Version:  1.0
 *        Created:  08/09/2017 03:29:56 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Mohammad Noureddine (nouredd2), nouredd2@illinois.edu
 *   Organization:  University of Illinois at Urbana-Champaign
 *
 * =====================================================================================
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <openssl/evp.h>

/* digest a message by creating a SHA256 hash
 *
 * arguments are:
 *
 *  message         -- The message to digest
 *  message_len     -- The length of the message
 *  digest_len      -- The length of the digest to create (return variable)
 *
 * returns sha256(message)
 */
unsigned char *
digest_message (const unsigned char *message, size_t message_len,
        unsigned int *digest_len);

/* print a digest to the std output
 *
 * arguments are:
 *
 *  digest		-- The digest to print
 *  len			-- The length of the digest in bytes
 */
void
print_digest (unsigned char *digest, unsigned int len);

/* difference between two timespec structs (for high resolution timing)
 *
 * arguments are:
 *
 *  t1		-- The first timespec struct
 *  t2		-- The second timespec struct
 *
 * returns the difference in seconds
 */
double
time_diff		(timespec t1, timespec t2);

/* append a buffer at the end of the current one
 *
 * arguments are:
 *
 *  dest		-- The destination buffer
 *  src			-- The source buffer to append
 *  n			-- The number of bytes to append
 *
 * returns a pointer to the end of the buffer
 */
unsigned char *
append_buffer (unsigned char *dest, unsigned char *src, size_t n);

/* compare the first k bits of x and y
 *
 * arguments are:
 *
 *  x		-- The first byte array to compare
 *  y		-- The second byte array to compare
 *  len		-- The number of bits to compare
 *
 * returns true if the first len bits of x and y are equal
 */
bool
compare_bits (unsigned char *x, unsigned char *y, unsigned int len);
