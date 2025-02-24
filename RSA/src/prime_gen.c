#include "../include/prime_gen.h"
#include <openssl/bn.h>
#include <openssl/rand.h>
#define EXIT_ERROR 1

int generate_prime(mpz_t out_prime, unsigned int bits, unsigned int reps)
{
	/*Generate prime*/
	while (1)
	{
		/*Init a BIGNUM*/
		BIGNUM* bn = BN_new();
		if (!BN_rand(bn, bits, 1/*ensure MSB is 1 so that the number generated is actually 2048*/,
			1/*ensure LSB is 1 to prevent even number generation*/)) {
			fprintf(stderr, "Error ! Fatal error.\n");
			BN_free(bn);
			return EXIT_ERROR;
		}

		//this will convert the "bytes" char array into gmp bignumber using mpz_set_str
		char* bytes = BN_bn2dec(bn);
		mpz_set_str(out_prime, bytes, 10);

		//to prevent memory leak on each loop iteration
		BN_free(bn);
		OPENSSL_free(bytes);

		if (mpz_probab_prime_p(out_prime, reps) != 0) {
			return 0;
		}
	}
	
}

int generate_two_primes(mpz_t p, mpz_t q, unsigned int bits, unsigned int reps)
{
	if (generate_prime(p, bits, reps) != 0)
	{
		return EXIT_ERROR;
	}

	if (!generate_prime(q, bits, reps) != 0)
	{
		return EXIT_ERROR;
	}

	return 0;
}