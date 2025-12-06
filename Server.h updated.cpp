//server.h updated : 

#ifndef SERVER_H
#define SERVER_H

#include <pistache/http.h>
#include <pistache/endpoint.h>
#include <pistache/router.h>
#include <pistache/net.h>
#include <memory>
#include <string>
#include <vector>

// Forward declarations of your own types if needed
#include "book.h"
#include "laptop.h"
#include "StudyRoom.h"
#include "auth.h"
#include "borrow.h"
#include "inventory.h"

class LibraryServer {
public:
    explicit LibraryServer(Pistache::Address addr);

    void init(size_t threads = 1);
    void start();
    void shutdown();

private:
    void setupRoutes();

    // Handlers
    void handleAuthEmail(const Pistache::Rest::Request& request,
                         Pistache::Http::ResponseWriter response);
    
    void handleGetBooks(const Pistache::Rest::Request& request,
                        Pistache::Http::ResponseWriter response);

    void handleBorrowBook(const Pistache::Rest::Request& request,
                          Pistache::Http::ResponseWriter response);

    void handleGetLaptops(const Pistache::Rest::Request& request,
                          Pistache::Http::ResponseWriter response);

    void handleGetRooms(const Pistache::Rest::Request& request,
                        Pistache::Http::ResponseWriter response);

private:
    std::shared_ptr<Pistache::Http::Endpoint> httpEndpoint;
    Pistache::Rest::Router router;

    // Add references to your data / managers as needed
    Inventory inventory;
    BorrowManager borrowManager;
    AuthManager authManager;
};

#endif // SERVER_H