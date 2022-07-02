// drbalpat

#include "helpers.h"
#include <cstdio>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <cstring>
#include <iostream>

using namespace std;

int main ( int argc, char * argv[] ) {
    // check number of args
    if (argc != 3)
        error(1, "Invalid number of arguments!");

    ResourceManager rm;

    // open PEM file
    FILE *pem_file = fopen( argv[1], "r" );
    if (!pem_file)
        error(2, "Invalid PEM file path!");
    rm.add_file(pem_file);

    // open input file
    FILE *in_file = fopen( argv[2], "r" );
    if (!in_file)
        error(3, "Invalid input file path!");
    rm.add_file(in_file);

    // -------------------------------------------------------------
    // load data for symmetric cryptography from input file header
    int nid;
    fread( &nid, sizeof(int), 1, in_file );
    const EVP_CIPHER * sym_type = EVP_get_cipherbyname( OBJ_nid2ln(nid) );

    int keyLen;
    fread( &keyLen, 1, sizeof(keyLen), in_file );
    const int block_size = EVP_CIPHER_block_size(sym_type);

    unsigned char *key = new unsigned char[keyLen];
    fread( key, 1, keyLen, in_file );

    unsigned char *iv = new unsigned char[block_size*8];
    fread( iv, 1, block_size * 8, in_file );

    // init structures for EVP_Open
    const int in_buf_len = 4000; // 4 KB
    const int out_buf_len =  in_buf_len + block_size;
    unsigned char *in_chunk = new unsigned char[in_buf_len];
    unsigned char *out_chunk = new unsigned char[out_buf_len];
    int out_chunkLength = 0;

    rm.add_buffer(key);
    rm.add_buffer(iv);
    rm.add_buffer(in_chunk);
    rm.add_buffer(out_chunk);

    // -------------------------------------------------------------
    // open output file
    char out_file_name[100];

    strcpy(out_file_name, argv[2]);
    strcat(out_file_name, "_opened");

    FILE *out_file = fopen( out_file_name, "w" );
    if ( !out_file )
        error(6, "Invalid output file path!");
    rm.add_file(out_file);

    // -------------------------------------------------------------
    // load private key
    EVP_PKEY * privkey = PEM_read_PrivateKey(pem_file, NULL, NULL, NULL);

    // init EVP_Open
    CtxWrapper ctx;

    if ( !EVP_OpenInit( ctx.ctx(), sym_type, key, keyLen, iv, privkey) )
        error(5, "EVP_Open failed to init!");

    // update EVP_Open
    size_t data_read;
    while ( (data_read = fread(in_chunk, 1, in_buf_len, in_file)) > 0 ) {
        if ( !EVP_OpenUpdate(ctx.ctx(), out_chunk, &out_chunkLength, in_chunk, data_read) )
            error(7, "EVP_Open failed to update!");

        fwrite(out_chunk, 1, out_chunkLength, out_file);
    }

    // end EVP_Open
    if ( !EVP_OpenFinal(ctx.ctx(), out_chunk, &out_chunkLength) )
        error(8, "EVP_Open failed to end!");

    fwrite(out_chunk, 1, out_chunkLength, out_file);

    return 0;
}
