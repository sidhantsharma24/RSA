#include <stdio.h>
#include <mpir.h>
#include "include/prime_gen.h"
#include "include/rsa.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define EXIT_ERROR 1

int main()
{  
    char input[16];
    unsigned int bits;

    printf_s("Enter key size (bits): ");

    if (fgets(input, sizeof(input), stdin) == NULL)
    {
        printf_s("Error getting key size");
        exit(EXIT_FAILURE);
    }
    input[strcspn(input, "\n")] = '\0';

    for (size_t i = 0; i < strlen(input); i++)
    {
        if (!isdigit(input[i]) && !(i == 0 && input[i] == '-'))
        {
            printf_s("Invalid key size.\n");
            exit(EXIT_FAILURE); 
        }
    }

    bits = atoi(input);

    switch (bits) {
    case 512:
    case 1024:
    case 2048:
    case 3072:
    case 4096:
        break;
    default:
        fprintf(stderr, "Invalid key size (must be 512, 1024, 2048, 3072, or 4096)\n");
        return EXIT_FAILURE;
    }
   
    struct rsa_mpz_key key;
    rsa_key_init(&key);

    if (rsa_keygen(&key, bits) != 0)
    {
        fprintf(stderr, "Keygen failed.\n");
        rsa_clear_key(&key);
        return EXIT_ERROR;
    }

    
    gmp_printf("Public Modulus n (%d bits): %Zd\n", bits, key.n);
    gmp_printf("Public Exponent e: %Zd\n", key.e);

    rsa_clear_key(&key);
    return 0;
}
