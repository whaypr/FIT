#pragma once

#include <unistd.h>
#include <openssl/ssl.h>
#include <iostream>
#include <string>
#include <vector>


class SSLWrapper {
	SSL_CTX* const ctx_;
    SSL* const ssl_;
public:
	SSLWrapper()
        : ctx_( SSL_CTX_new(TLS_client_method()) ),
          ssl_( SSL_new(ctx_) )
    {}

	~SSLWrapper() {
        SSL_shutdown(ssl_);
        SSL_free(ssl_);
        SSL_CTX_free(ctx_);
    }

	SSL_CTX* ctx() { return ctx_; }
    SSL* ssl() { return ssl_; }
};


class ResourceManager {
    std::vector<FILE*> files;
    std::vector<int> sockets;
public:
    ~ResourceManager() {
        for ( auto& f : files )
            fclose(f);
        for ( auto& s : sockets )
            close(s);
    }

    void add_file( FILE *f ) { files.push_back(f); }
    void add_socket( int s ) { sockets.push_back(s); }
};


void error ( int errnum, std::string errmes) {
    std::cerr << "ERROR " << errnum << ": ";
	std::cerr << errmes << std::endl;
    exit(errnum);
}
