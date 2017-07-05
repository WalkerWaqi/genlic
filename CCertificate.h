//
// Created by waqi on 9/18/16.
//

#ifndef GENLIC_CCERTIFICATE_H
#define GENLIC_CCERTIFICATE_H

#include <string>
#include "CLicTool.h"

class CCertificate : public CLicTool {
public:
    CCertificate() {};

    virtual ~CCertificate() {};

    void setRootCert(const std::string rootCert) {
        CCertificate::root_cert = rootCert;
    }

    int Verify(std::string certFile);

private:
    std::string root_cert;
};


#endif //GENLIC_CCERTIFICATE_H
