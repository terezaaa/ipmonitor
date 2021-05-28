#include <string>

struct IpStatus {
    std::string ip;
    int port;
    volatile bool available;
};