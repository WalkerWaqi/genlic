//
// Created by waqi on 9/13/16.
//

#include "CLicense.h"
#include <fstream>
#include <json.hpp>
using json = nlohmann::json;
#include "CSignature.h"
#include "CCertificate.h"
#include "license.h"
#include "apps.h"

CLicense::CLicense()
{
    days = 0;
    root_cert = ROOT_CERT;
}

CLicense::~CLicense()
{
}

void CLicense::Init()
{
    apps_startup();
}

int CLicense::Generate(std::string &license, std::string &keyFile, std::string &password)
{
    int nRet = -1;

    if (keyFile.empty() || serial_no.empty() || days <= 0)
    {
        return nRet;
    }

    json jLicense;

    jLicense["version"] = LICENSE_VERSION;

    json jInfo;
    jInfo["serial"] = serial_no;
    jInfo["days"] = days;

    jLicense["info"] = jInfo;

    {
        CSignature sign;

        std::string strSource(jLicense["info"].dump());
        std::string strHash;
        nRet = sign.Hash(strSource, strHash);
        if (nRet == 0)
        {
            sign.setKeyFile(keyFile);
            sign.setKeyPassword(password);

            std::string strSign;
            nRet = sign.Sign(strHash, strSign);
            //nRet = sign.Verify(strHash, strSign);

            if (nRet == 0) {
                jLicense["signature"] = strSign;
                std::string data = jLicense.dump();

                nRet = write_file(license, data);
            }
        }
    }

    return nRet;
}

int CLicense::Verify(std::string &license, std::string &derFile)
{
    int nRet = -1;

    serial_no.clear();
    days = 0;

    CCertificate cert;
    cert.setRootCert(root_cert);
    nRet = cert.Verify(derFile);
    if (nRet == 0)
    {
        std::string data;
        nRet = read_file(license, data);
        if(nRet == 0 && !data.empty())
        {
            json jLicense = json::parse(data);

            CSignature sign;

            std::string strSource(jLicense["info"].dump());
            std::string strHash;
            nRet = sign.Hash(strSource, strHash);
            if (nRet == 0)
            {
                std::string strSign = jLicense["signature"].get<std::string>();
                sign.setKeyFile(derFile);

                nRet = sign.VerifyWithDer(strHash, strSign);
                if (nRet == 0)
                {
                    serial_no = jLicense["info"]["serial"].get<std::string>();
                    days = jLicense["info"]["days"].get<int>();
                }
            }
        }
    }

    return nRet;
}

int CLicense::VerifySignature(std::string &source, std::string &signature, std::string &derFile)
{
    int nRet = -1;

    CCertificate cert;
    cert.setRootCert(root_cert);
    nRet = cert.Verify(derFile);
    if (nRet == 0)
    {
        CSignature sign;
        std::string strHash;
        nRet = sign.HashFile(source, strHash);
        if (nRet == 0)
        {
            std::string strSign;
            nRet = read_file(signature, strSign);
            if(nRet == 0 && !strSign.empty())
            {
                sign.setKeyFile(derFile);

                nRet = sign.VerifyWithDer(strHash, strSign);
            }
        }
    }

    return nRet;
}