#pragma once

#include <openssl/evp.h>
#include <iostream>
#include <string>
#include <vector>


class CtxWrapper {
	EVP_CIPHER_CTX* const ctx_;
public:
	CtxWrapper() :ctx_(EVP_CIPHER_CTX_new()) {}
	~CtxWrapper() { EVP_CIPHER_CTX_free(ctx_); }
	EVP_CIPHER_CTX* ctx() { return ctx_; }
};


class ResourceManager {
    std::vector<FILE*> files;
    std::vector<unsigned char *> buffers;

public:
    void add_file( FILE *f ) {
        files.push_back(f);
    }

    void add_buffer( unsigned char * b ) {
        buffers.push_back(b);
    }

    ~ResourceManager() {
        for ( auto& f : files )
            fclose(f);
        for ( auto& b : buffers )
            delete [] b;
    }
};


void error ( int errnum, std::string errmes) {
    std::cerr << "ERROR " << errnum << ": ";
	std::cerr << errmes << std::endl;
    exit(errnum);
}
