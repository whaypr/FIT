/*
 *
 * Program than finds a random string which sha512 hash starts
 * with a given number of 0 bits.
 *
 * The number of bits is passed as a command line argument.
 *
 */

#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define MAX_STR_LEN 100


//-------------------------------------------------
void EVP_error ( EVP_MD_CTX * ctx, int errnum, char * errmes ) {
    EVP_MD_CTX_free(ctx);
    fprintf( stderr, "ERROR: %s\n", errmes );
    exit(errnum);
}


//-------------------------------------------------
int min ( int a, int b ) {
    return a < b ? a : b;
}


//-------------------------------------------------
int main ( int argc, char * argv[] ) {
    if ( argc != 2 ) {
        fprintf( stderr, "You must provide exactly one integer argument!\n" );
        return 10;
    }

    /* Read and check input */
    int zeros = atoi( argv[1] );

    if ( zeros == 0 ) {
        fprintf( stderr, "Entered value is not an integer!\n" );
        return 11;
    }

    /* Prepare structures */
    EVP_MD_CTX * ctx;                    // context structure
    const EVP_MD * type;                 // hash function type
    unsigned char hash[EVP_MAX_MD_SIZE]; // hash char array
    unsigned int hash_len;               // resulting hash lenght

    srand( time(NULL) );
    char str[MAX_STR_LEN];
    int str_len;

    /* Init OpenSSL and its structures */
    OpenSSL_add_all_digests();

    type = EVP_get_digestbyname("sha512"); // get type
    if ( !type ) {
        fprintf( stderr, "This function type does not exist!\n" );
        return 1;
    }

    ctx = EVP_MD_CTX_new(); // create context for hashing
    if ( !ctx )
        EVP_error(ctx, 2, "Context failed to create!");


    /* Find the correct string */
    while ( 1 ) { // repeat until the desirable string is found
        /* Generate string */
        str_len = rand() % MAX_STR_LEN; // length in bytes

        for (int i = 0; i < str_len; ++i)
            str[i] = (char)( rand() % (127-33) + 33 ); // use only printable characters

        /* Init EVP for new hashing */
        if ( !EVP_DigestInit_ex(ctx, type, NULL) ) // context setup for the hash type
            EVP_error(ctx, 3, "Digest failed to init!");
    
        /* Hash the string */
        if ( !EVP_DigestUpdate(ctx, str, str_len) ) // feed the message in
            EVP_error(ctx, 4, "Update failed!");

        if ( !EVP_DigestFinal_ex(ctx, hash, &hash_len) ) // get the hash
            EVP_error(ctx, 5, "Final failed!");
        
        /* Check the condition */
        int id = 0, zeros_rem = zeros, found = 1;

        while ( zeros_rem > 0 ) {
            if ( hash[id] >= pow( 2, 8 - min(zeros_rem,8) ) ) {
                found = 0;
                break;
            }

            zeros_rem -= 8;
            id++;
        }

        if (found) break;
    }

    /* Destroy the context */
    EVP_MD_CTX_free(ctx); 

    /* Print results */
    for( unsigned int i = 0; i < str_len; ++i ) // string
        printf( "%02x", str[i] );
    printf("\n");

    for( unsigned int i = 0; i < hash_len; ++i ) // its hash
        printf( "%02x", hash[i] );
    printf("\n");

    return 0;
}
