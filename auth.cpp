#include "auth.h"
#include <fstream>
#include <sstream>
#include <iostream>

AuthService::AuthService(const std::string &filename)
    : dbFile(filename), nextID(1)
{
}

bool AuthService::load()
{
    records.clear();
    std::ifstream in(dbFile);
    if (!in.is_open()) {
        // no file yet, that's ok
        return true;
    }
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string token;
        AuthRecord rec;

        std::getline(ss, token, '|');
        rec.memberID = std::stoi(token);

        std::getline(ss, rec.name, '|');
        std::getline(ss, rec.email, '|');

        std::getline(ss, token, '|');
        rec.type = static_cast<MembershipType>(std::stoi(token));

        std::getline(ss, rec.username, '|');
        std::getline(ss, rec.password, '|');

        records.push_back(rec);
        if (rec.memberID >= nextID) {
            nextID = rec.memberID + 1;
        }
    }
    return true;
}

bool AuthService::save_append(const AuthRecord &rec)
{
    std::ofstream out(dbFile, std::ios::app);
    if (!out.is_open()) return false;
    out << rec.memberID << "|"
        << rec.name << "|"
        << rec.email << "|"
        << static_cast<int>(rec.type) << "|"
        << rec.username << "|"
        << rec.password << "\n";
    return true;
}

int AuthService::register_user(const std::string &name,
                               const std::string &email,
                               MembershipType type,
                               const std::string &username,
                               const std::string &password)
{
    // check if username exists
    for (const auto &r : records) {
        if (r.username == username) {
            return -1; // taken
        }
    }
    AuthRecord rec;
    rec.memberID = nextID;
    rec.name = name;
    rec.email = email;
    rec.type = type;
    rec.username = username;
    rec.password = password;

    records.push_back(rec);
    nextID = nextID + 1;
    if (!save_append(rec)) {
        return -1;
    }
    return rec.memberID;
}

int AuthService::login(const std::string &username,
                       const std::string &password)
{
    for (const auto &r : records) {
        if (r.username == username && r.password == password) {
            return r.memberID;
        }
    }
    return -1;
}
