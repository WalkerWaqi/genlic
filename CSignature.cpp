//
// Created by waqi on 9/13/16.
//

#include "CSignature.h"
#include "apps.h"
#include <memory.h>
#include <openssl/md5.h>
#include <openssl/x509.h>
#include <fstream>

int CSignature::Hash(std::string &source, std::string &result)
{
    int nRet = -1;

    unsigned char signature[16] = {0};
    MD5_CTX  md5_ctx;

    MD5_Init(&md5_ctx);
    MD5_Update(&md5_ctx, source.c_str(), source.length());
    MD5_Final(signature, &md5_ctx);

    char *signature_hex = (char *)malloc(sizeof(signature) * 2 + 1);
    if (signature_hex) {
        for (int i = 0; i < sizeof(signature); i++) sprintf(signature_hex + i * 2, "%02x", signature[i]);
        signature_hex[sizeof(signature) * 2] = 0;

        result = signature_hex;
        free(signature_hex);

        nRet = 0;
    }

    return nRet;
}

int CSignature::HashFile(std::string &source, std::string &result)
{
    int nRet = -1;

    std::ifstream rfile(source);
    if (!rfile.fail())
    {
        unsigned char signature[16] = {0};
        MD5_CTX  md5_ctx;

        MD5_Init(&md5_ctx);
        char tmp[1024];
        while(!rfile.eof())
        {
            rfile.read(tmp, sizeof(tmp));
            MD5_Update(&md5_ctx, tmp, rfile.gcount());
        }
        MD5_Final(signature, &md5_ctx);

        rfile.close();

        char *signature_hex = (char *)malloc(sizeof(signature) * 2 + 1);
        if (signature_hex) {
            for (int i = 0; i < sizeof(signature); i++) sprintf(signature_hex + i * 2, "%02x", signature[i]);
            signature_hex[sizeof(signature) * 2] = 0;

            result = signature_hex;
            free(signature_hex);

            nRet = 0;
        }
    }

    return nRet;
}

int CSignature::Sign(std::string &source, std::string &result)
{
    int nRet = -1;

    EVP_PKEY *pkey = load_key(bio_err, key_file.c_str(), FORMAT_PEM, 1, key_password.empty() ? NULL : key_password.c_str(), NULL, NULL);
    if (pkey)
    {
        RSA *rsa = EVP_PKEY_get1_RSA(pkey);
        if (rsa)
        {
            if (RSA_check_key(rsa) == 1)
            {
                unsigned char* signature = (unsigned char *)malloc(RSA_size(rsa));
                if (signature)
                {
                    memset(signature, 0, RSA_size(rsa));
                    unsigned int slen;
                    RSA_sign(NID_md5, (unsigned char *)source.c_str(), source.length(), signature, &slen, rsa);

                    char *signature_hex = (char *)malloc(slen * 2 + 1);
                    if (signature_hex) {
                        for (int i = 0; i < slen; i++) sprintf(signature_hex + i * 2, "%02x", signature[i]);

                        result = signature_hex;
                        free(signature_hex);

                        nRet = 0;
                    }

                    free(signature);
                }
            }

            RSA_free(rsa);
        }

        EVP_PKEY_free(pkey);
    }

    return nRet;
}

int CSignature::Verify(std::string &source, std::string &strSignature, void* key)
{
    int nRet = -1;

    EVP_PKEY *pkey = NULL;
    if(key)
    {
        pkey = (EVP_PKEY*)key;
    } else
    {
        pkey = load_key(bio_err, key_file.c_str(), FORMAT_PEM, 1, key_password.empty() ? NULL : key_password.c_str(), NULL, NULL);
    }

    if (pkey)
    {
        RSA *rsa = EVP_PKEY_get1_RSA(pkey);
        if (rsa)
        {
            //if (RSA_check_key(rsa) == 1)
            {
                unsigned int slen = strSignature.length() / 2;
                unsigned char* signature = (unsigned char *)malloc(slen);
                if (signature)
                {
                    for (int i = 0; i < slen; i++) sscanf(strSignature.c_str() + i * 2, "%02hhx", signature + i);

                    unsigned int verified = RSA_verify(NID_md5, (unsigned char *)source.c_str(), source.length(), signature, slen, rsa);
                    if (verified == 1)
                    {
                        nRet = 0;
                    }

                    free(signature);
                }
            }

            RSA_free(rsa);
        }

        if (!key)
        {
            EVP_PKEY_free(pkey);
        }
    }

    return nRet;
}

int CSignature::VerifyWithDer(std::string &source, std::string &strSignature)
{
    int nRet = -1;

    std::string der;
    if (read_file(key_file, der) == 0 && !der.empty()) {
        const unsigned char *der_str = (const unsigned char *) der.data();
        X509 *ca = d2i_X509(NULL, &der_str, der.length());
        if (ca)
        {
            EVP_PKEY *pkey = X509_get_pubkey(ca);
            if (pkey) {
                nRet = Verify(source, strSignature, pkey);

                EVP_PKEY_free(pkey);
            }

            X509_free(ca);
        }
    }

    return nRet;
}