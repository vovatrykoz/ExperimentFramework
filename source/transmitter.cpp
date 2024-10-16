#include <atomic>
#include <csignal>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "queue/ThreadSafeQueue.h"

std::atomic<bool> running(true);

void signalHandler(int signum) {
    std::cout << std::endl << "Shutting down the transmitter" << std::endl;
    running = false;
}

int main(void) {
    std::signal(SIGINT, signalHandler);

    std::cout << "Starting setup..." << std::endl;

    const std::string tempDirPath = "./.temp";
    const std::string mailboxAddrFile = "mailboxAddress.bin";
    const std::string pathToMailboxAddrFile =
        tempDirPath + "/" + mailboxAddrFile;

    if (!std::filesystem::exists(tempDirPath)) {
        std::cerr << "Could not locate temp directory" << std::endl;
        return -1;
    }

    std::ifstream addressFile(pathToMailboxAddrFile, std::ios::binary);
    if (!addressFile) {
        std::cerr << "Could not retrieve the mailbox address" << std::endl;
        return -1;
    }

    ThreadSafeQueue* mailboxAddress = nullptr;
    addressFile.read(reinterpret_cast<char*>(&mailboxAddress),
                      sizeof(mailboxAddress));
    addressFile.close();

    std::cout << "Setup done!" << std::endl;

    while (running) {
    }

    std::cout << "Starting cleanup..." << std::endl;
    std::cout << "Cleanup done!" << std::endl;

    return 0;
}