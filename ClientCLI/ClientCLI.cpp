//
// Created by arie1 on 1/16/2023.
//

#include "ClientCLI.h"

#include <utility>
#include "IO/DefaultIO.h"


void ClientCLI::initCommands() {

    commands.push_back(new UploadFile(this->dio));
    commands.push_back(new DisplayClassiffications(this->dio));
    commands.push_back(new DownloadFile(this->dio));

}


void ClientCLI::start() {
    int input = 0;
    std::string menu = "";
    do {
        input = 0;
        //read menu from user
        menu = this->dio->read();
        // print the menu

        std::cout << menu << std::endl;
        // read the choice from the user

        std::cin >> input;


        string s = std::to_string(input);
        std::cout << s << std::endl;
        this->dio->write(s);

        switch (input) {
            case 1:
                commands[0]->execute();
                break;
            case 2:
                commands[1]->execute();
                break;
            case 8:
                break;
            default:
                break;
        }
    } while (input != 8);
}


ClientCLI::ClientCLI(DefaultIO *dio) {
    this->dio = dio;
    initCommands();

}

