#include <iostream>
#include <sstream>
#include <string>

#include "queue/ThreadSafeQueue.h"

int main(void) {
    ThreadSafeQueue messageQueue;

    std::ostringstream oss;
    oss << &messageQueue;
    std::string addressStr = oss.str();
    std::cout << "The address of the queue is: " << addressStr << std::endl;

    return 0;
}