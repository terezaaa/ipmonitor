# ipmonitor

### How to build and run
```shell script
# build
mkdir build
cmake -S . -B build
make -C build

# run
./build/ipmonitor ./iplist
./build/ipmonitor ./iplist 50 # with thread pool size 50 (default 100)
```
