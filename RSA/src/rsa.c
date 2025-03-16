#include <mpir.h>

#include "../include/prime_gen.h"
#include "../include/rsa.h"
#include <openssl/evp.h>
#include <openssl/types.h>
#include <stdio.h>
#include <openssl/bn.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

#include <time.h>


/*Helper function to export key params as BIGNUMS */
BIGNUM *mpz_to_bignum(const mpz_t z)
{
	char *dec_str = mpz_get_str(NULL, 10, z);
	if(!dec_str)
	{
		fprintf(stderr, "Error while exporting keys.\n");
		return NULL;
	}

	BIGNUM *bn = NULL;
	if(!BN_dec2bn(&bn, dec_str))
	{
		fprintf(stderr, "Error while exporting keys.\n" );
		free(dec_str);
		return NULL;
	}

	free(dec_str);
	return bn;
}

int export_key_pem(const struct rsa_mpz_key *key, const char *pub_filename, const char *priv_filename)
{
	int status = 1;
	/* Initiate the openssl's RSA struct*
	 * They are included via openssl/rsa.h
	 * */
	RSA *rsa = RSA_new(); if(!rsa) { fprintf(stderr, "Failed to generate RSA key struct.\n"); return status; }

	/*This converts the key params to bignums*/
	BIGNUM *n = mpz_to_bignum(key->n);
	BIGNUM *e = mpz_to_bignum(key->e);
	BIGNUM *p = mpz_to_bignum(key->p);
	BIGNUM *q = mpz_to_bignum(key->q);
	BIGNUM *d = mpz_to_bignum(key->d);
	BIGNUM *dP = mpz_to_bignum(key->dP);
	BIGNUM *dQ = mpz_to_bignum(key->dQ);
	BIGNUM *qInv = mpz_to_bignum(key->qInv);

	if(!n || !e || !p || !q || !d || !dP || !dQ || !qInv)
	{
		fprintf(stderr, "Failed to process key params.\n");
		goto cleanup;
	}

	//This assigns the BIGNUMS params to RSA struct
	if(RSA_set0_key(rsa, n, e, d) != 1)
	{
		fprintf(stderr, "Failed to set key params while exporting.\n");
		goto cleanup;
	}

	if(RSA_set0_factors(rsa, p, q) != 1)
	{
		fprintf(stderr, "Failed to set public key while exporting.\n");
		goto cleanup;
	}

	if(RSA_set0_crt_params(rsa, dP, dQ, qInv) != 1)
	{
		fprintf(stderr, "Failed to set RSA CRT params.\n");
		BN_free(dP);
		BN_free(dQ);
		BN_free(qInv);

		goto cleanup;
	}

	if(RSA_check_key(rsa) != 1)
	{
		fprintf(stderr, "RSA key validation failed.\n");
 		ERR_print_errors_fp(stderr);
    	goto cleanup;
	} else {
		printf("[+] RSA key validated successfully.\n");
	}


	//Here we export the key to the file; First public key file
	FILE *pub_fp = fopen(pub_filename, "wb");
	if(!pub_fp)
	{
		perror("Failed to open public key file.\n");
		goto cleanup;
	}

	//PEM_write_RSA_PUBKEY provided by openssl/rsa.h exports in SPKI format
	if(PEM_write_RSAPublicKey(pub_fp, rsa) != 1)
	{
		fprintf(stderr, "Failed to write public key.\n");
		fclose(pub_fp);
		goto cleanup;
	}
	
	fclose(pub_fp);
	printf("[+] Public key exported to %s\n", pub_filename);

	//Exporting private key PKCS#1
	FILE *priv_fp = fopen(priv_filename, "wb");
	if(!priv_fp)
	{
		perror("Failed to open private key file.\n");
		goto cleanup;
	}

	// create passphrase encryption
	const char *passphrase = "passphrase123"; //I'll add the user prompt funcitonality in future
	const EVP_CIPHER *cipher = EVP_aes_256_cbc();
	

	if(PEM_write_RSAPrivateKey(priv_fp, rsa, cipher, 
				NULL, 0, NULL, (void *)passphrase) != 1) {
		fprintf(stderr, "Failed to write private key.\n");
		ERR_print_errors_fp(stderr);
		fclose(priv_fp);
		goto cleanup;
	}

	fclose(priv_fp);
	printf("[+] Private key exported to %s\n", priv_filename);
	
	status = 0;
cleanup:
	RSA_free(rsa);
	if (status != 0)
	{
		if (n && !RSA_get0_n(rsa)) BN_free(n);
		if (e && !RSA_get0_e(rsa)) BN_free(e);
		if (d && !RSA_get0_d(rsa)) BN_free(d);
		if (p && !RSA_get0_p(rsa)) BN_free(p);
		if (q && !RSA_get0_q(rsa)) BN_free(q);
		if (dP && !RSA_get0_dmp1(rsa)) BN_free(dP);
		if (dQ && !RSA_get0_dmq1(rsa)) BN_free(dQ);
		if (qInv && !RSA_get0_iqmp(rsa)) BN_free(qInv);
	}

	return status;
}

void rsa_key_init(struct rsa_mpz_key *key) 
{
	mpz_init(key->n);
	mpz_init(key->e);
	mpz_init(key->d);
	mpz_init(key->p);
	mpz_init(key->q);
	mpz_init(key->dP);
	mpz_init(key->dQ);
	mpz_init(key->qInv);
}

void rsa_clear_key(struct rsa_mpz_key* key)
{
	mpz_clear(key->n);
	mpz_clear(key->e);
	mpz_clear(key->d);
	mpz_clear(key->p);
	mpz_clear(key->q);
	mpz_clear(key->dP);
	mpz_clear(key->dQ);
	mpz_clear(key->qInv);
}

int rsa_keygen(struct rsa_mpz_key* key, unsigned int bits)
{
	generate_two_primes(key->p, key->q, bits / 2, 25);
	
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

	/*Choosing and setting e = 65537*/
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

	/*CRT params*/
	mpz_mod(key->dP, key->d, p_minus_1);
	mpz_mod(key->dQ, key->d, q_minus_1);

	if(mpz_invert(key->qInv, key->q, key->p) == 0)
	{
		fprintf(stderr, "Keygen failed: couldn't compute qInv.\n");
		mpz_clear(phi_n);
		mpz_clear(q_minus_1);
		mpz_clear(p_minus_1);
		return 1;
	}

	mpz_clear(phi_n);
	mpz_clear(p_minus_1);
	mpz_clear(q_minus_1);

	return 0;
}
