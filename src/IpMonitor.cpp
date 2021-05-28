#include "IpMonitor.h"

void IpMonitor::monitor(int poolSize) {
    if (!list.size()) return;

    distribute(poolSize, [](RoutineScope *scope) {
        pthread_t pid;
        pthread_create(&pid, nullptr, monitorIpListRoutine, scope);
    });

    statusCheckTask();
}

void IpMonitor::distribute(const int poolSize, const std::function<void(RoutineScope*)> &routine) {
    int listSize = list.size();

    int offset = poolSize > listSize ? 1 : listSize / poolSize;

    int c = 0;
    while (c < listSize) {
        routine(new RoutineScope{c, ((c + offset) < listSize ? (c + offset) : listSize), &list});
        c += offset;
    }
}

[[noreturn]] void *IpMonitor::monitorIpListRoutine(void *arg) {
    auto *scope = (RoutineScope *) arg;

    int c = scope->start;

    while (true) {
        IpStatus *ipStatus = &(*scope->list)[c];

        int fd = socket(AF_INET, SOCK_STREAM, 0);

        struct sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(ipStatus->ip.c_str());
        addr.sin_port = htons(ipStatus->port);

        if (connect(fd, (sockaddr *) &addr, sizeof(addr)) == 0) {
            if (!ipStatus->available) {
                std::stringstream message;
                message << ipStatus->ip << ":" << ipStatus->port << " up";
                log(message.str());
            }
            ipStatus->available = true;
        } else {
            if (ipStatus->available) {
                std::stringstream message;
                message << ipStatus->ip << ":" << ipStatus->port << " down";
                log(message.str());
            }
            ipStatus->available = false;
        }

        close(fd);

        c++;
        if (c >= scope->end) {
            c = scope->start;
        }
    }
}

void IpMonitor::statusCheckTask() {

    auto routine = [](void *arg) -> void * {
        while (true) {
            // check status body
            sleep(10);
        }
    };
    pthread_t tid;
    pthread_create(&tid, nullptr, routine, nullptr);
    pthread_join(tid, nullptr);
}

void IpMonitor::log(const std::string &message) {
    auto now = std::chrono::system_clock::now();
    auto timer = std::chrono::system_clock::to_time_t(now);
    auto tm = *std::localtime(&timer);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    std::stringstream ss;

    ss << std::put_time(&tm, "%d-%m-%Y %H:%M:%S") << "." << std::setfill('0') << std::setw(3) << ms.count() << ", "
       << message << std::endl;

    std::cout << ss.str();
}