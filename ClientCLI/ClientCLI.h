//
// Created by arie1 on 1/16/2023.
//

#ifndef ASS4_CLIENTCLI_H
#define ASS4_CLIENTCLI_H

#include "Command.h"
#include "DisplayClassiffications.h"
#include "DownloadFile.h"
#include "IO/FileCSVIO.h"
#include <vector>

class ClientCLI {
private:
    DefaultIO* dio;
    std::vector<Command *> commands;
    // vector <lineNumber, classification>
    void initCommands();
    std::vector<std::pair<int,std::string>>;
public:
    explicit ClientCLI(DefaultIO *dio);
    virtual ~ClientCLI() = default;
    std::string path;

    void start(int i);
};


#endif //ASS4_CLIENTCLI_H