// drbalpat

#include "helpers.h"
#include <cstdio>
#include <iostream>
#include <openssl/evp.h>
#include <openssl/objects.h>
#include <openssl/pem.h>
#include <cstring>

using namespace std;


int main ( int argc, char * argv[] ) {
    // check number of args
    if (argc != 4)
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

    // get symetric cipher type
    const EVP_CIPHER * sym_type = EVP_get_cipherbyname( argv[3] );
    if ( !sym_type )
        error(4, "This function type does not exist!");

    // -------------------------------------------------------------
    // load public key
    EVP_PKEY * pubkey = PEM_read_PUBKEY(pem_file, NULL, NULL, NULL); // no password protection of the key itself

    // init structures for symmetric cryptography
    unsigned char *key = new unsigned char[ EVP_PKEY_size(pubkey) ]; // public key encrypted key for symmetric encryption
    int keyLen;
    unsigned char *iv = new unsigned char[ EVP_CIPHER_iv_length(sym_type) ];

    // init structures for EVP_Seal
    CtxWrapper ctx;
    const int block_size = EVP_CIPHER_block_size(sym_type);
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
    // init EVP_Seal
    if ( !EVP_SealInit( ctx.ctx(), sym_type, &key, &keyLen, iv, &pubkey, 1) )
        error(6, "EVP_Seal failed to init!");

    // -------------------------------------------------------------
    // open output file
    char out_file_name[100];

    strcpy(out_file_name, argv[2]);
    strcat(out_file_name, "_sealed");

    FILE *out_file = fopen( out_file_name, "w" );
    if ( !out_file )
        error(5, "Invalid output file path!");
    rm.add_file(out_file);

    // add header information to the output file for later decryption
    int nid = OBJ_txt2nid(argv[3]);
    fwrite( &nid, 1, sizeof(nid), out_file);

    fwrite( &keyLen, 1, sizeof(keyLen), out_file);

    memcpy( out_chunk, key, keyLen ); 
    fwrite( out_chunk, 1, keyLen, out_file );

    memcpy( out_chunk, iv, block_size * 8 ); 
    fwrite( out_chunk, 1, block_size * 8, out_file );

    // -------------------------------------------------------------
    // update EVP_Seal
    size_t data_read;
    while ( (data_read = fread(in_chunk, 1, in_buf_len, in_file)) > 0 ) {
        if ( !EVP_SealUpdate(ctx.ctx(), out_chunk, &out_chunkLength, in_chunk, data_read) )
            error(7, "EVP_Seal failed to update!");

        fwrite(out_chunk, 1, out_chunkLength, out_file);
    }

    // end EVP_Seal
    if ( !EVP_SealFinal(ctx.ctx(), out_chunk, &out_chunkLength) )
        error(8, "EVP Seal_failed to end!");

    fwrite(out_chunk, 1, out_chunkLength, out_file);

    return 0;
}
