//
// Created by waqi on 9/18/16.
//

#ifndef GENLIC_CLICTOOL_H
#define GENLIC_CLICTOOL_H

#include <string>

class CLicTool {
public:
    CLicTool() {}

    virtual ~CLicTool() {}

public:
    int read_file(std::string &file, std::string &data);
    int write_file(std::string &file, std::string &data);

};


#endif //GENLIC_CLICTOOL_H
