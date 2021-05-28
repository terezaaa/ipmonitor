#include <iostream>
#include "pthread.h"
#include "unistd.h"
#include "sys/socket.h"
#include "sys/types.h"
#include <arpa/inet.h>
#include <vector>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <string>
#include <functional>
#include <netdb.h>
#include "IpStatus.h"

struct RoutineScope {
    int start;
    int end;
    std::vector<IpStatus> *list;
};

class IpMonitor {
private:
    std::vector<IpStatus> list;

public:
    explicit IpMonitor(std::vector<IpStatus> &addr) : list{addr} {};

    void monitor(int poolSize);

private:
    void distribute(int poolSize, const std::function<void(RoutineScope* scope)> &routine);

    [[noreturn]] static void *monitorIpListRoutine(void *arg);

    void statusCheckTask();

    static void log(const std::string& message);
};