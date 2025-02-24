#pragma once
#ifndef PRIME_GEN_H
#define PRIME_GEN_H

#include <mpir.h>

/**
* generate prime:
*	Generates a prime of specified bit length and stores it in mpz_t out_prime
*	param out_prime is an initialized mpzt
*	param bits is the length of desired bits (i may add a switch case for diffrent bit lengths in future)
*	param reps is the no. of repititions for primality testing
*	return 0 on success else throw an error
*/
int generate_prime(mpz_t out_prime, unsigned int bits, unsigned int reps);

/**
 * generate_two_primes:
 *  Generates two distinct primes (p, q) of specified bit length.
 *  param p, q: initialized mpz_t variables
 *  param bits: number of bits for each prime
 *  param reps: number of primality tests
 *
 *  return: 0 on success, or nonzero on error
 */

int generate_two_primes(mpz_t p, mpz_t q, unsigned int bits, unsigned int reps);

#endif
