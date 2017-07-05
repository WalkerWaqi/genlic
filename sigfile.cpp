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

        std::string appName = boost::filesystem::basename(argv[0]);
        std::string strKeyFile;
        std::string strKeyPassword;
        std::string strOut;
        std::string strIn;

        /** Define and parse the program options
         */
        namespace po = boost::program_options;
        po::options_description desc("Options");
        desc.add_options()
                ("help,h", "Print help messages")
                ("key,k", po::value<std::string>(&strKeyFile)->required(), "pem key file")
                ("password,p", po::value<std::string>(&strKeyPassword), "key password")
                ("in,i", po::value<std::string>(&strIn)->required(), "input file")
                ("out,o", po::value<std::string>(&strOut)->required(), "output signature file")
                ;

        po::positional_options_description positionalOptions;
        positionalOptions.add("in", 1);

        po::variables_map vm;

        try
        {
            po::store(po::command_line_parser(argc, argv).options(desc)
                              .positional(positionalOptions).run(),
                      vm);

            if ( vm.count("help")  )
            {
                std::cout << "generate license file" << std::endl << std::endl;
                rad::OptionPrinter::printStandardAppDesc(appName,
                                                         std::cout,
                                                         desc,
                                                         &positionalOptions);
                return SUCCESS;
            }

            po::notify(vm);

            {
                CSignature signature;
                std::string strHash;
                if (signature.HashFile(strIn, strHash) != 0)
                {
                    throw std::runtime_error("hash error");
                }

                signature.setKeyFile(strKeyFile);
                signature.setKeyPassword(strKeyPassword);

                std::string strSign;
                if (signature.Sign(strHash, strSign) != 0)
                {
                    throw std::runtime_error("signature error");
                }

                if (signature.write_file(strOut, strSign) != 0)
                {
                    throw std::runtime_error("write file error");
                }
            }
        }
        catch(boost::program_options::required_option& e)
        {
            rad::OptionPrinter::formatRequiredOptionError(e);
            std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
            rad::OptionPrinter::printStandardAppDesc(appName,
                                                     std::cout,
                                                     desc,
                                                     &positionalOptions);
            return ERROR_IN_COMMAND_LINE;
        }
        catch(boost::program_options::error& e)
        {
            std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
            rad::OptionPrinter::printStandardAppDesc(appName,
                                                     std::cout,
                                                     desc,
                                                     &positionalOptions);
            return ERROR_IN_COMMAND_LINE;
        }
    }
    catch(std::exception& e)
    {
        std::cerr << "Unhandled Exception reached the top of main: "
                  << e.what() << ", application will now exit" << std::endl;
        return ERROR_UNHANDLED_EXCEPTION;
    }

    return SUCCESS;
}