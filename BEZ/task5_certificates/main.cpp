// drbalpat

#include "helpers.h"

#include <cstddef>
#include <cstdio>
#include <cstring>

#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <unistd.h>

using namespace std;


int main () {
    ResourceManager rm;

    // Establish TCP connection with fit.cvut.cz on port 443
    struct sockaddr_in servaddr;
    int socketFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("147.32.232.212"); // ip address
    servaddr.sin_port = htons(443);                         // port

    if ( connect(socketFD, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0 )
        error(1, "TCP connection failed to open!");
    rm.add_socket(socketFD);

    // SSL init and connect
    SSL_library_init(); // registers the available SSL/TLS ciphers and digests
    SSLWrapper ssl;

    SSL_CTX_set_options( // Disable deprecated protocols
        ssl.ctx(),
        SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 | SSL_OP_NO_TLSv1 | SSL_OP_NO_TLSv1_1
    );
    SSL_CTX_set_default_verify_paths( ssl.ctx() );
    SSL_set_fd( ssl.ssl(), socketFD );
    SSL_set_tlsext_host_name( ssl.ssl(), "fit.cvut.cz");
    SSL_set_ciphersuites( ssl.ssl(), "TLS_CHACHA20_POLY1305_SHA256:TLS_AES_128_GCM_SHA256" );  // banning the default cipher

    if ( SSL_connect( ssl.ssl() ) != 1 )
        error(2, "SSL failed to connect!");

    // Verify certificate
    if ( SSL_get_verify_result( ssl.ssl() ) != X509_V_OK )
        error(3, "Certificate verification failed!");
    cout << "Certificate successfully verified." << endl;

    // Open file for writing
    FILE *file_response = fopen( "files/response.txt", "w" );
    if ( !file_response )
        error(4, "Output file failed to open!");
    rm.add_file(file_response);

    // Send HTTP request
    const char buf_request[] = "GET /cs/fakulta/o-fakulte HTTP/1.0\r\nHost:fit.cvut.cz\r\n\r\n";
    if ( SSL_write( ssl.ssl(), buf_request, sizeof(buf_request) ) <= 0 )
        error(5, "SSL failed to write!");

    // Read HTTP response
    char buf_response[4000];
    size_t readBytes;
    while ( SSL_read_ex( ssl.ssl(), buf_response, 4000, &readBytes ) )
        fwrite( buf_response, 1, readBytes, file_response);



    // Print certificate information on stdout
	X509_NAME_print_ex(
        BIO_new_fp(stdout, BIO_NOCLOSE),
        X509_get_subject_name(
            SSL_get_peer_certificate( ssl.ssl() )
        ),
        0,
        XN_FLAG_RFC2253
    );
    cout << endl;

    // Write certificate information in the file
    FILE *file_certificate = fopen( "files/certificate.pem", "w" );
    if ( !file_certificate )
        error(6, "Output file failed to open!");
    rm.add_file(file_certificate);

    PEM_write_X509( file_certificate, SSL_get_peer_certificate(ssl.ssl()) );

    // Print used ciphersuite
    cout << "In the default setting the cipher used is              : TLS_AES_256_GCM_SHA384" << endl;
    cout << "After prohibiting the default cipher the cipher used is: " << SSL_get_cipher_name( ssl.ssl() ) << endl;



    return 0;
}
