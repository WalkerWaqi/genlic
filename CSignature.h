//
// Created by waqi on 9/13/16.
//

#ifndef LICMAKE_CSIGNATURE_H
#define LICMAKE_CSIGNATURE_H

#include <string>
#include "CLicTool.h"

class CSignature : public CLicTool {
public:
    CSignature() {}

    virtual ~CSignature() {

    }

    void setKeyFile(const std::string &keyFile) {
        CSignature::key_file = keyFile;
    }

    void setKeyPassword(const std::string &keyPassword) {
        CSignature::key_password = keyPassword;
    }

    int Hash(std::string &source, std::string &result);
    int HashFile(std::string &source, std::string &result);
    int Sign(std::string &source, std::string &result);
    int Verify(std::string &source, std::string &strSignature, void* key = NULL);
    int VerifyWithDer(std::string &source, std::string &strSignature);

private:
    std::string key_file;
    std::string key_password;
};


#endif //LICMAKE_CSIGNATURE_H
