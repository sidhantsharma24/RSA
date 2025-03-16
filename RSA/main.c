#include <stdio.h>
#include <mpir.h>
#include "include/prime_gen.h"
#include "include/rsa.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h>
#define mkdir(dir, mode) _mkdir(dir)
#endif

#define EXIT_ERROR 1
#define DEFAULT_OUTDIR "output"

int main(int argc, char *argv[])
{  
    char input[16];
    unsigned int bits;
    const char *outdir = DEFAULT_OUTDIR; //place where keys will be stored


    for(int i = 1 ; i < argc ; i++)
    {
        if(strcmp(argv[i], "--outdir") == 0 && (i+1) < argc)
        {
            outdir = argv[i + 1];
            i++;
        } else {
            printf("Usage: %s --outdir <directory>\n", argv[0]);
            return EXIT_FAILURE;
        }
    }
    
    printf("Enter key size (bits): ");

    if (fgets(input, sizeof(input), stdin) == NULL)
    {
        printf("Error getting key size");
        exit(EXIT_FAILURE);
    }
    input[strcspn(input, "\n")] = '\0';

    for (size_t i = 0; i < strlen(input); i++)
    {
        if (!isdigit(input[i]) && !(i == 0 && input[i] == '-'))
        {
            printf("Invalid key size.\n");
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

    printf("[+] Generating %u-bit RSA keypair...\n", bits);

    if (rsa_keygen(&key, bits) != 0)
    {
        fprintf(stderr, "Keygen failed.\n");
        rsa_clear_key(&key);
        return EXIT_ERROR;
    }
    
    char pub_path[256];
    char priv_path[256];

    mkdir(outdir, 0755);

    snprintf(pub_path, sizeof(pub_path), "%s/public.pem", outdir);
    snprintf(priv_path, sizeof(priv_path), "%s/private.pem", outdir);

    printf("[+] Exporting keys...\n");
    if(export_key_pem(&key, pub_path, priv_path) != 0)
    {
        fprintf(stderr, "[-] Failed to export keys.\n");
        rsa_clear_key(&key);
        return EXIT_ERROR;
    }
    
   // gmp_printf("Public Modulus n (%d bits): %Zd\n", bits, key.n);
   // gmp_printf("Public Exponent e: %Zd\n", key.e);
      gmp_printf("[debug] \n\n n has %u bits\n", mpz_sizeinbase(key.n, 2));

    rsa_clear_key(&key);

    printf("[+] RSA keypair generation complete. Keys are stored in '%s'.\n", outdir);
    return 0;
}
