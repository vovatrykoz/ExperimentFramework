#include <filesystem>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "queue/ThreadSafeQueue.h"

int main(void) {
    std::cout << "Starting setup..." << std::endl;

    ThreadSafeQueue messageQueue;

    const std::string tempDirPath = "./.temp";
    const std::string mailboxAddrFile = "mailboxAddress.bin";
    const std::string pathToMailboxAddrFile =
        tempDirPath + "/" + mailboxAddrFile;

    if (!std::filesystem::exists(tempDirPath)) {
        if (!std::filesystem::create_directory(tempDirPath)) {
            std::cerr << "Could not create temp directory" << std::endl;
            return -1;
        }
    }

    std::ofstream addressFile(pathToMailboxAddrFile, std::ios::binary);
    if(!addressFile) {
        std::cerr << "Could not create binary file for mailbox address writing" << std::endl;
        return -1;
    }

    ThreadSafeQueue* mailboxAddress = &messageQueue;
    addressFile.write(reinterpret_cast<char*>(mailboxAddress), sizeof(mailboxAddress));
    addressFile.close();

    std::cout << "Setup done!" << std::endl;

    std::cout << "Starting cleanup..." << std::endl;

    if (!std::filesystem::exists(tempDirPath)) {
        std::cerr << "Temp directory does not exist!" << std::endl;
        return -1;
    }

    std::error_code ec;
    std::filesystem::remove_all(tempDirPath, ec);

    if(ec) {
        std::cerr << "Error: " << ec.message() << std::endl;
        return -1;
    }

    std::cout << "Cleanup done!" << std::endl;

    return 0;
}