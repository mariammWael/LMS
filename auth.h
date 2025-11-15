#ifndef AUTH_H
#define AUTH_H

#include <string>
#include <vector>
#include "common.h"
#include "member.h"

/*
   File format (members.db):
   memberID|name|email|membershipType|username|password
*/

struct AuthRecord {
    int memberID;
    std::string name;
    std::string email;
    MembershipType type;
    std::string username;
    std::string password; // keep it simple for now
};

class AuthService {
public:
    explicit AuthService(const std::string &filename);

    bool load();
    bool save_append(const AuthRecord &rec);

    // register new user (returns memberID or -1)
    int register_user(const std::string &name,
                      const std::string &email,
                      MembershipType type,
                      const std::string &username,
                      const std::string &password);

    // login (returns memberID or -1)
    int login(const std::string &username,
              const std::string &password);

private:
    std::string dbFile;
    std::vector<AuthRecord> records;
    int nextID;
};

#endif
