#include <stdio.h>
#include <openssl/rand.h>
#include <openssl/bn.h>
#include <mpir.h>
#define MEM_ALLOC_ERROR 1

int main()
{
    
    const mpz_t p, q;  mpz_init(p); mpz_init(q);
    
    /*Generate prime factor p*/
    do 
    {
        /*Init a BIGNUM*/
        BIGNUM* bn = BN_new();

        if (!BN_rand(bn, 2048, 1/*ensure MSB is 1 so that the number generated is actually 2048*/, 
            0/*ensure LSB is 0 to prevent even number generation*/))
        {
            fprintf(stderr, "ERROR! failed\n");
            BN_free(bn);
            return MEM_ALLOC_ERROR;
        }

        char *bytes = BN_bn2dec(bn);

        //printf("%s", bytes);

        //this will convert the "bytes" char array into gmp bignumber using mpz_set_str
        mpz_set_str(p/*dest*/, bytes/*src*/, 10/*base 10*/);

        //to prevent memory leak on each loop iteration
        BN_free(bn); OPENSSL_free(bytes);
        
    } while (mpz_probab_prime_p(p, 25) == 0);

    /*Generate prime factor q*/
    do
    {
        BIGNUM* bn = BN_new();

        if (!BN_rand(bn, 2048, 1, 1))
        {
            fprintf("ERROR! failed\n", stderr);
            BN_free(bn);
            return MEM_ALLOC_ERROR;
        }

        char* bytes = BN_bn2dec(bn);

        mpz_set_str(q, bytes, 10);

        BN_free(bn);
        OPENSSL_free(bytes);
    } while (mpz_probab_prime_p(q, 25) == 0);

    gmp_printf("Prime factor p = %Zd\n\n", p);
    gmp_printf("Prime factor q = %Zd\n", q);
       
    return 0;
}
