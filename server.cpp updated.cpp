//server.cpp updated: 

#include "server.h"
#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>
#include <pistache/mime.h>
#include <nlohmann/json.hpp>   // if you use JSON; otherwise remove

using json = nlohmann::json;   // remove if not using

LibraryServer::LibraryServer(Pistache::Address addr) {
    httpEndpoint = std::make_shared<Pistache::Http::Endpoint>(addr);
}

void LibraryServer::init(size_t threads) {
    auto opts = Pistache::Http::Endpoint::options()
            .threads(static_cast<int>(threads))
            .flags(Pistache::Tcp::Options::ReuseAddr);

    httpEndpoint->init(opts);
    setupRoutes();
}

void LibraryServer::start() {
    httpEndpoint->setHandler(router.handler());
    httpEndpoint->serve();
}

void LibraryServer::shutdown() {
    httpEndpoint->shutdown();
}

void LibraryServer::setupRoutes() {
    using namespace Pistache::Rest;

    Routes::Post(router, "/auth/email",
        Routes::bind(&LibraryServer::handleAuthEmail, this));

    Routes::Get(router, "/books",
        Routes::bind(&LibraryServer::handleGetBooks, this));

    Routes::Post(router, "/borrow",
        Routes::bind(&LibraryServer::handleBorrowBook, this));

    Routes::Get(router, "/laptops",
        Routes::bind(&LibraryServer::handleGetLaptops, this));

    Routes::Get(router, "/rooms",
        Routes::bind(&LibraryServer::handleGetRooms, this));
}

// -------------------- HANDLERS ------------------------

void LibraryServer::handleAuthEmail(const Pistache::Rest::Request& request,
                                    Pistache::Http::ResponseWriter response) {
    auto body = request.body();

    // TODO: use your real auth logic here
    bool ok = true; // authManager.verifyEmail(body);

    auto mime = Pistache::Http::Mime::MediaType::fromString("application/json");
    if (ok) {
        std::string resp = R"({"status":"ok"})";
        response.send(Pistache::Http::Code::Ok, resp, mime);
    } else {
        std::string resp = R"({"status":"error","message":"invalid email"})";
        response.send(Pistache::Http::Code::Unauthorized, resp, mime);
    }
}

void LibraryServer::handleGetBooks(const Pistache::Rest::Request&,
                                   Pistache::Http::ResponseWriter response) {
    auto mime = Pistache::Http::Mime::MediaType::fromString("application/json");

    // Example static JSON; replace with real data from your inventory
    std::string resp = R"({"books":[{"title":"Example","author":"Someone"}]})";

    response.send(Pistache::Http::Code::Ok, resp, mime);
}

void LibraryServer::handleBorrowBook(const Pistache::Rest::Request& request,
                                     Pistache::Http::ResponseWriter response) {
    auto body = request.body();

    auto mime = Pistache::Http::Mime::MediaType::fromString("application/json");

    // TODO: parse body and call your borrow logic
    // bool success = borrowManager.borrow(...);

    bool success = true;

    if (success) {
        std::string resp = R"({"status":"borrowed"})";
        response.send(Pistache::Http::Code::Ok, resp, mime);
    } else {
        std::string resp = R"({"status":"error","message":"cannot borrow"})";
        response.send(Pistache::Http::Code::Bad_Request, resp, mime);
    }
}

void LibraryServer::handleGetLaptops(const Pistache::Rest::Request&,
                                     Pistache::Http::ResponseWriter response) {
    auto mime = Pistache::Http::Mime::MediaType::fromString("application/json");
    std::string resp = R"({"laptops":[{"id":1,"brand":"Dell"}]})";
    response.send(Pistache::Http::Code::Ok, resp, mime);
}

void LibraryServer::handleGetRooms(const Pistache::Rest::Request&,
                                   Pistache::Http::ResponseWriter response) {
    auto mime = Pistache::Http::Mime::MediaType::fromString("application/json");
    std::string resp = R"({"rooms":[{"id":1,"name":"Room A"}]})";
    response.send(Pistache::Http::Code::Ok, resp, mime);
}