//
// Created by waqi on 9/18/16.
//

#include "CCertificate.h"
#include <openssl/x509.h>

int CCertificate::Verify(std::string certFile) {
    int nRet = -1;

    X509_STORE *ca_store = NULL;
    X509_STORE_CTX *ctx = NULL;
    STACK_OF(X509) *ca_stack = NULL;
    X509 *ca = NULL;
    X509 *user = NULL;

    do {
        X509_STORE *ca_store = X509_STORE_new();
        X509_STORE_CTX *ctx = X509_STORE_CTX_new();
        if (!ca_store || !ctx)
        {
            break;
        }

        std::string der;
        if (read_file(root_cert, der) < 0 || der.empty()) {
            break;
        }
        const unsigned char *der_str = (const unsigned char *) der.data();
        X509 *ca = d2i_X509(NULL, &der_str, der.length());
        if (!ca)
        {
            break;
        }

        if (X509_STORE_add_cert(ca_store, ca) != 1)
        {
            break;
        }

        der.clear();
        if (read_file(certFile, der) < 0 || der.empty()) {
            break;
        }
        der_str = (const unsigned char *) der.data();
        X509 *user = d2i_X509(NULL, &der_str, der.length());
        if (!user)
        {
            break;
        }

        if (X509_STORE_CTX_init(ctx, ca_store, user, ca_stack) != 1)
        {
            break;
        }

        if (X509_verify_cert(ctx) == 1)
        {
            nRet = 0;
        }

    } while (0);

    if (user)
    {
        X509_free(user);
    }
    if (ca)
    {
        X509_free(ca);
    }

    if (ctx)
    {
        X509_STORE_CTX_cleanup(ctx);
        X509_STORE_CTX_free(ctx);
    }

    if (ca_store)
    {
        X509_STORE_free(ca_store);
    }

    return nRet;
}
