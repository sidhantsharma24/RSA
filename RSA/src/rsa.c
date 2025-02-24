#include <mpir.h>
#include "../include/prime_gen.h"
#include "../include/rsa.h"
#include <stdio.h>

void rsa_key_init(struct rsa_mpz_key *key) 
{
	mpz_init(key->n);
	mpz_init(key->e);
	mpz_init(key->d);
	mpz_init(key->p);
	mpz_init(key->q);
}

void rsa_clear_key(struct rsa_mpz_key* key)
{
	mpz_clear(key->n);
	mpz_clear(key->e);
	mpz_clear(key->d);
	mpz_clear(key->p);
	mpz_clear(key->q);
}

int rsa_keygen(struct rsa_mpz_key* key, unsigned int bits)
{
	generate_two_primes(key->p, key->q, bits, 25);
	
	/* n = p * q */
	mpz_mul(key->n, key->p, key->q);

	/* phi_N = (p - 1) * (q - 1) */
	mpz_t phi_n, p_minus_1, q_minus_1; 
	mpz_init(phi_n);
	mpz_init(p_minus_1);
	mpz_init(q_minus_1);

	mpz_sub_ui(p_minus_1, key->p, 1);
	mpz_sub_ui(q_minus_1, key->q, 1);
	mpz_mul(phi_n, p_minus_1, q_minus_1);

	/*Choosing and setting e = 65536*/
	mpz_set_ui(key->e, 65537);

	/*
	* Computing private exponent d by e*d = 1 mod phi_N;
	* d = inv(e) mod phi_N, should be 1;
	*/
	if (mpz_invert(key->d, key->e, phi_n) == 0)
	{
		fprintf(stderr, "Keygen failed.\n");
		mpz_clear(phi_n);
		mpz_clear(p_minus_1);
		mpz_clear(q_minus_1);
		return 1;
	}

	mpz_clear(phi_n);
	mpz_clear(p_minus_1);
	mpz_clear(q_minus_1);

	return 0;
}