//
// Created by waqi on 9/13/16.
//

#ifndef LICMAKE_CLICENSE_H
#define LICMAKE_CLICENSE_H

#include <string>
#include "CLicTool.h"

class CLicense : public CLicTool {
public:
    CLicense();

    virtual ~CLicense();

    static void Init();

    void setSerialNo(const std::string serialNo) {
        CLicense::serial_no = serialNo;
    }

    const std::string &getSerialNo() const {
        return serial_no;
    }

    void setDays(int days) {
        CLicense::days = days;
    }

    int getDays() const {
        return days;
    }

    void setRootCert(const std::string rootCert) {
        CLicense::root_cert = rootCert;
    }

    int Generate(std::string &license, std::string &keyFile, std::string &password);
    int Verify(std::string &license, std::string &derFile);

    int VerifySignature(std::string &source, std::string &signature, std::string &derFile);

private:
    std::string serial_no;
    int days;

    std::string root_cert;
};


#endif //LICMAKE_CLICENSE_H
