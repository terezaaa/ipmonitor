#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "src/IpMonitor.h"

#define DEFAULT_THREAD_POOL_SIZE 100

std::vector<IpStatus> ipList(char *fileName) {
    std::vector<IpStatus> list;

    std::ifstream file(fileName);
    if (!file.good()) {
        std::cout << "Invalid config file" << std::endl;
    }

    std::string s;
    while (std::getline(file, s) && s.find(':') != std::string::npos && s.find('#') == std::string::npos) {
        int delimiter = s.find(':');

        list.push_back({s.substr(0, delimiter), std::stoi(s.substr(delimiter + 1, s.size())), false});
    }

    return list;
}

int main(int argc, char **argv) {


    if (argc < 2) {
        std::cout << "Please enter config file" << std::endl;
        exit(EXIT_FAILURE);
    }

    int threadPoolSize = DEFAULT_THREAD_POOL_SIZE;
    if (argc > 2) {
        if (std::stoi(argv[2]) < 1) {
            std::cout << "Invalid pool size value" << std::endl;
            exit(EXIT_FAILURE);
        }

        threadPoolSize = std::stoi(argv[2]);
    }

    auto list = ipList(argv[1]);
    (new IpMonitor(list))->monitor(threadPoolSize);
}
