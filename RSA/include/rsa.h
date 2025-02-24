#pragma once
#ifndef RSA_H
#define RSA_H
#include <mpir.h>

struct rsa_mpz_key {
	mpz_t n;
	mpz_t e;
	mpz_t d;
	mpz_t p;
	mpz_t q;
};

void rsa_key_init(struct rsa_mpz_key *key);
void rsa_clear_key(struct rsa_mpz_key *key);
int rsa_keygen(struct rsa_mpz_key *key);

#endif // !RSA_H
