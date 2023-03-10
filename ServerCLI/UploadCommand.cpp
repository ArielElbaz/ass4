#include "ServerCLI/UploadCommand.h"

UploadCommand::UploadCommand(DefaultIO *dio, Database *database) {
    this->dio = dio;
    this->database = database;
}
/*
 * Runs server side of upload command (option 1)
 * Reads file contents from socket and inits respective database (train or test)
 */
void UploadCommand::execute() {
    database->resetFiles();
    std::string fileContent;
    // upload a data file to the server
    dio->write("Please upload your local train CSV file.\n");
    try {
        fileContent = dio->read();
    }
    catch (...) {
        std::cerr << "Error reading from socket." << std::endl;
        return;
    }
    if (fileContent.empty()) {
        return;
    }
    try {
        database->initTrainVectors(fileContent);
    } catch (std::exception const &ex) {
        std::cerr << ex.what() << std::endl;
        dio->write("-1");
        return;
    }
    dio->write("Upload complete.\nPlease upload your local test CSV file.\n");
    try {
        fileContent = dio->read();
    }
    catch (...) {
        std::cerr << "Error reading from socket." << std::endl;
        return;
    }
    if (fileContent.empty()) {
        return;
    }
    try {
        database->initTestVectors(fileContent);
    } catch (std::ios_base::failure const &ex) {
        dio->write("-1");
        return;
    }
    dio->write("Upload complete.\n");
    // get approval from user.
    try {
        dio->read();
    } catch (...) {
        std::cerr << "Error reading from socket." << std::endl;
    }
}
