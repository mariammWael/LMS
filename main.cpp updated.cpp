//main.cpp updated: 
#include "server.h"
#include <pistache/net.h>
#include <iostream>

int main() {
    Pistache::Port port(9080);     // or any port you like
    Pistache::Address addr(Pistache::Ipv4::any(), port);

    LibraryServer server(addr);

    server.init(1);  // 1 = number of threads
    std::cout << "Starting server on port " << port << std::endl;
    server.start();

    return 0;
}