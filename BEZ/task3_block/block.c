// drbalpat

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <openssl/evp.h>
#include <openssl/rand.h>


//-------------------------------------------------
#define KEY_LEN 256/8

#define BLOCK_SIZE 128/8

#define INP_BUF_LEN 4000 // 4 KB
#define OUT_BUF_LEN INP_BUF_LEN + BLOCK_SIZE// - 1

#define FILE_MAX_CNT 3


//-------------------------------------------------
void str_lower ( char *str ) {
    for( ; str[0] != '\0'; str++ )
        str[0] = (char)tolower(str[0]);
}


//-------------------------------------------------
void error ( int errnum, char *errmes, EVP_CIPHER_CTX *ctx, FILE **fps ) {
    if (ctx) EVP_CIPHER_CTX_free(ctx);

    for ( int i = 0; i < FILE_MAX_CNT; i++ )
        if ( fps[i] ) fclose( fps[i] );

    fprintf( stderr, "ERROR: %s\n", errmes );
    exit(errnum);
}


//-------------------------------------------------
int main ( int argc, char * argv[] ) {
    // check number of arguments
    if ( argc != 4 )
        error(10, "Wrong number of arguments!", NULL, NULL);

    // process encypher/decypher
    str_lower(argv[2]);
    if ( strcmp(argv[2], "e") && strcmp(argv[2], "d") )
        error( 13, "You must provide either e or d as the second argument!\n", NULL, NULL );
    int encrypt = strcmp(argv[2], "d") ? 1 : 0; // encrypt/decrypt switch

    // process mode
    str_lower(argv[3]);
    const EVP_CIPHER * (*typePtr)(void); // cbc/ecb switch
    if ( strcmp(argv[3], "cbc") == 0 ) {
        typePtr = &EVP_aes_256_cbc;
    } else if ( strcmp(argv[3], "ecb") == 0 ) {
        typePtr = &EVP_aes_256_ecb;
    } else
        error( 14, "You must provide either ecb or cbc as the third argument!\n", NULL, NULL );

    // prepare array for opened files
    FILE * file_ptrs[FILE_MAX_CNT];
    int files_count = 0;

    // prepare input file
     if ( strlen(argv[1]) > 80 )
        error( 12, "File name is too long!\n", NULL, NULL );

    char filename_inp[100];

    argv[1][strlen(argv[1])-4] = '\0';
    strcpy(filename_inp, argv[1]);

    if ( strcmp(argv[2], "d") == 0 ) {
        strcat( filename_inp, "_" );
        strcat( filename_inp, argv[3] ); 
    }
    strcat( filename_inp, ".tga\0" );

    FILE *inImage_ptr = fopen( filename_inp, "rb" );
    if ( !inImage_ptr )
        error( 11, "You must provide correct file path as the first argument!\n", NULL, NULL );
    file_ptrs[files_count++] = inImage_ptr;

    // prepare output file
    char filename_out[100];

    strcpy(filename_out, argv[1]);
    strcat( filename_out, "_" );

    if ( strcmp(argv[2], "e") == 0 ) {
        strcat( filename_out, argv[3] );    
    } else { // decrypt
        if ( strcmp(argv[3], "cbc") == 0 ) strcat( filename_out, "cbc_" );
        strcat( filename_out, "dec" );
    }
    strcat( filename_out, ".tga\0" );

    FILE *outImage_ptr = fopen( filename_out, "wb" );
    if ( !outImage_ptr )
        error( 13, "Output file cannot be opened!\n", NULL, file_ptrs );
    file_ptrs[files_count++] = outImage_ptr;

    // set key and initial vector
    unsigned char key[KEY_LEN];
    unsigned char iv[BLOCK_SIZE];

    char secrets_path[100];
    strcpy( secrets_path, "secrets/" );
    strcat( secrets_path, argv[1] );
    strcat( secrets_path, "_" );
    strcat( secrets_path, argv[3] );

    char openmode[4];
    if ( strcmp(argv[2], "e") == 0 )
        strcpy(openmode, "wb");
    else // decrypt
        strcpy(openmode, "rb");

    FILE *secrets_ptr = fopen( secrets_path, openmode );
    if ( !secrets_ptr )
        error( 15, "Secrets file cannot be opened!\n", NULL, file_ptrs );
    file_ptrs[files_count++] = secrets_ptr;

    if ( strcmp(argv[2], "e") == 0 ) {
        // generate secrets
        RAND_bytes(key, KEY_LEN);
        RAND_bytes(iv, BLOCK_SIZE);

        // save secrets
        fwrite(key, 1, KEY_LEN, secrets_ptr);
        fwrite(iv, 1, BLOCK_SIZE, secrets_ptr);
    } else { // decrypt
        // load secrets
        fread(key, 1, KEY_LEN, secrets_ptr);
        fread(iv, 1, BLOCK_SIZE, secrets_ptr);
    }

    // set input and output vars
    unsigned char inp_chunk[INP_BUF_LEN];

    unsigned char out_chunk[OUT_BUF_LEN];
    int out_chunkLength = 0;

    // set context structure
    EVP_CIPHER_CTX * ctx = EVP_CIPHER_CTX_new();
    if ( !ctx )
        error(2, "Context failed to create!", ctx, file_ptrs);

    // add ciphers
    OpenSSL_add_all_ciphers();

    // copy image header
    fread(inp_chunk, 1, 18, inImage_ptr);
    fwrite(inp_chunk, 1, 18, outImage_ptr); 

    // cipher init
    if ( !EVP_CipherInit_ex(ctx, typePtr(), NULL, key, iv, encrypt) )
        error(3, "EVP cipher init failed!", ctx, file_ptrs);

    // encrypt or decrypt
    size_t data_read;
    while ( (data_read = fread(inp_chunk, 1, INP_BUF_LEN, inImage_ptr)) > 0 ) {
        if ( !EVP_CipherUpdate(ctx, out_chunk, &out_chunkLength, inp_chunk, data_read) )
            error(4, "EVP cipher update failed!", ctx, file_ptrs);

        fwrite(out_chunk, 1, out_chunkLength, outImage_ptr);
    }

    if ( !EVP_CipherFinal_ex(ctx, out_chunk, &out_chunkLength) )
        error(5, "EVP cipher final failed!", ctx, file_ptrs);

    fwrite(out_chunk, 1, out_chunkLength, outImage_ptr);

    // clean up
    EVP_CIPHER_CTX_free(ctx);
    for ( int i = 0; i < FILE_MAX_CNT; i++ )
        fclose( file_ptrs[i] );

    return 0;
}
