#include <stdio.h>
#include <mpir.h>
#include "prime_gen.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define EXIT_ERROR 1

int main()
{
    mpz_t p, q;
    mpz_init(p);
    mpz_init(q);
    
    char input[32];
    int key;

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

    key = atoi(input);

    switch (key) {
    case 512:
    case 1024:
    case 2048:
    case 3072:
    case 4096:
        if (generate_two_primes(p, q, key, 25) == 0)
        {
            fprintf(stderr, "Keygen failed.\n");
        }
        else {
            gmp_printf("Prime p: %Zd\n", p);
            gmp_printf("Prime q: %Zd\n", q);
        }
        break;
    default:
        fprintf(stderr, "Invalid key size (must be 512, 1024, 2048, 3072, or 4096)\n");
        return EXIT_FAILURE;
    }
    

    mpz_clear(p);
    mpz_clear(q);

    return 0;
}
