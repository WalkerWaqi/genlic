#include <iostream>
#include <string>
#include "boost/filesystem.hpp"
#include <boost/program_options.hpp>
#include <fstream>
#include "OptionPrinter.hpp"
#include "CLicense.h"
#include "CSignature.h"

namespace
{
    const size_t ERROR_IN_COMMAND_LINE = 1;
    const size_t SUCCESS = 0;
    const size_t ERROR_UNHANDLED_EXCEPTION = 2;
}

int main(int argc, char* argv[]) {

    try
    {
        CLicense::Init();

        CLicense license;
        std::string caDer = "/home/waqi/CA/cacert.der";
        license.setRootCert(caDer);

        std::string appDer = "/home/waqi/CA/appcert.der";

        std::string lic = "/home/waqi/tmp/lll";
        license.Verify(lic, appDer);
        if (license.Verify(lic, appDer) != 0)
        {
            throw std::runtime_error("verify signature error");
        }

//        std::string source = "/home/waqi/tmp/aaa";
//        std::string sign = "/home/waqi/tmp/bbb";
//        if (license.VerifySignature(source, sign, appDer) != 0)
//        {
//            throw std::runtime_error("verify signature error");
//        }

        std::cout<< "ok : " << license.getSerialNo() << " : " << license.getDays() << std::endl;
    }
    catch(std::exception& e)
    {
        std::cerr << "Unhandled Exception reached the top of main: "
                  << e.what() << ", application will now exit" << std::endl;
        return ERROR_UNHANDLED_EXCEPTION;
    }

    return SUCCESS;
}