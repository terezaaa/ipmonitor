cmake_minimum_required(VERSION 3.19)
project(ipmonitor)

set(CMAKE_CXX_STANDARD 17)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(ipmonitor main.cpp src/IpMonitor.h src/IpMonitor.cpp)

target_link_libraries(ipmonitor PRIVATE Threads::Threads)
