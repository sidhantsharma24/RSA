#pragma once
#include <openssl/types.h>
#ifndef RSA_H
#define RSA_H
#include <mpir.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bn.h>

struct rsa_mpz_key {
	mpz_t n;
	mpz_t e;
	mpz_t d;
	mpz_t p;
	mpz_t q;
	mpz_t dP;
	mpz_t dQ;
	mpz_t qInv;
};

void rsa_key_init(struct rsa_mpz_key *key);
void rsa_clear_key(struct rsa_mpz_key *key);
int rsa_keygen(struct rsa_mpz_key *key, unsigned int bits);
BIGNUM *mpz_to_bignum(const mpz_t z);
int export_key_pem(const struct rsa_mpz_key *key, const char *pub_filename, const char *priv_filename);
#endif // !RSA_H
