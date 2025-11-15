#include <iostream>
#include <string>
#include <vector>

#include "common.h"
#include "inventory.h"
#include "borrow.h"
#include "member.h"
#include "librarian.h"
#include "preferences.h"
#include "bookloader.h"
#include "auth.h"
#include "laptop.h"
#include "studyRoom.h"

int main()
{
    // 1) auth
    AuthService auth("members.db");
    auth.load();

    std::cout << "1) Register\n2) Login\nChoice: ";
    int choice;
    std::cin >> choice;
    std::cin.ignore();

    int loggedMemberID = -1;
    std::string name, email, username, password;

    if (choice == 1) {
        std::cout << "Enter full name: ";
        std::getline(std::cin, name);
        std::cout << "Enter email: ";
        std::getline(std::cin, email);
        std::cout << "Choose username: ";
        std::getline(std::cin, username);
        std::cout << "Choose password: ";
        std::getline(std::cin, password);

        int id = auth.register_user(name, email, MEMBERSHIP_STUDENT, username, password);
        if (id == -1) {
            std::cout << "Username taken or file error.\n";
            return 0;
        }
        std::cout << "Registered. Your memberID = " << id << "\n";
        loggedMemberID = id;
    } else {
        std::cout << "Username: ";
        std::getline(std::cin, username);
        std::cout << "Password: ";
        std::getline(std::cin, password);
        int id = auth.login(username, password);
        if (id == -1) {
            std::cout << "Login failed.\n";
            return 0;
        }
        std::cout << "Welcome back, member " << id << "\n";
        loggedMemberID = id;
    }

    // 2) current member object
    Member currentMember;
    member_init(&currentMember,
                loggedMemberID,
                "Loaded Member",
                "loaded@example.com",
                MEMBERSHIP_STUDENT);

    // 3) stores
    InventoryStore invStore;
    inventory_store_init(&invStore);

    BorrowStore borrowStore;
    borrow_store_init(&borrowStore);

    // 4) load books from file -> inventory
    LoadedData data;
    if (load_books_from_file("books.txt", data)) {
        add_loaded_to_inventory(data, invStore, 3);
        std::cout << "Loaded " << data.books.size() << " books from books.txt\n";
    } else {
        std::cout << "books.txt not found, continuing with laptops/rooms only.\n";
    }

    // 5) create some laptops
    std::vector<Laptop> laptops;
    {
        Laptop l1, l2;
        laptop_init(&l1, "LAP01", "Dell Latitude");
        laptop_init(&l2, "LAP02", "MacBook Air");
        laptops.push_back(l1);
        laptops.push_back(l2);

        // add them to inventory so they can be referenced by itemID
        inventory_store_add(&invStore, "LAP01", ITEMTYPE_LAPTOP, 1);
        inventory_store_add(&invStore, "LAP02", ITEMTYPE_LAPTOP, 1);
    }

    // 6) create some study rooms
    std::vector<StudyRoom> rooms;
    {
        StudyRoom r1, r2;
        studyRoom_init(&r1, "ROOM01", 4);
        studyRoom_init(&r2, "ROOM02", 6);
        rooms.push_back(r1);
        rooms.push_back(r2);

        // also reflect in inventory (quantity 1 each)
        inventory_store_add(&invStore, "ROOM01", ITEMTYPE_STUDYROOM, 1);
        inventory_store_add(&invStore, "ROOM02", ITEMTYPE_STUDYROOM, 1);
    }

    // 7) librarian (to approve normal borrows)
    Librarian lib;
    librarian_init(&lib, 1, "Main Librarian", "lib@uni.edu");

    // 8) menu
    while (true) {
        std::cout << "\n===== LMS MENU =====\n";
        std::cout << "1) Check item availability (books/laptops/rooms)\n";
        std::cout << "2) Borrow BOOK (needs itemID like B0001)\n";
        std::cout << "3) Return BOOK (by borrowID)\n";
        std::cout << "4) Borrow / rent LAPTOP\n";
        std::cout << "5) Return LAPTOP\n";
        std::cout << "6) Book STUDY ROOM\n";
        std::cout << "7) Cancel STUDY ROOM booking\n";
        std::cout << "8) Exit\n";
        std::cout << "Choice: ";
        int c;
        std::cin >> c;
        std::cin.ignore();

        if (c == 8) break;

        if (c == 1) {
            std::string itemID;
            std::cout << "Enter itemID (B0001, LAP01, ROOM01): ";
            std::getline(std::cin, itemID);
            bool ok = member_checkAvailability(&currentMember, &invStore, itemID.c_str());
            if (ok) std::cout << "Item " << itemID << " is available.\n";
            else    std::cout << "Item " << itemID << " is NOT available.\n";
        }

        else if (c == 2) {
            // Borrow BOOK
            std::string itemID;
            std::cout << "Enter BOOK itemID (B0001 ...): ";
            std::getline(std::cin, itemID);

            Date today = date_make(11, 11, 2025);
            int borrowID = librarian_approveBorrowRequest(&lib,
                                                          &borrowStore,
                                                          currentMember.memberID,
                                                          itemID.c_str(),
                                                          today);
            if (borrowID > 0) {
                std::cout << "Book borrowed, borrowID = " << borrowID << "\n";
                Inventory *inv = inventory_store_find(&invStore, itemID.c_str());
                if (inv && inv->quantity > 0) {
                    inv->quantity = inv->quantity - 1;
                }
            } else {
                std::cout << "Borrow failed.\n";
            }
        }

        else if (c == 3) {
            // Return BOOK
            int borrowID;
            std::cout << "Enter borrowID to return: ";
            std::cin >> borrowID;
            std::cin.ignore();
            Date ret = date_make(13, 11, 2025);
            member_returnBook(&currentMember, &borrowStore, borrowID, ret);
            std::cout << "Book returned.\n";
        }

        else if (c == 4) {
            // Borrow / rent LAPTOP
            std::string lapID;
            std::cout << "Enter laptop ID (LAP01, LAP02): ";
            std::getline(std::cin, lapID);

            bool found = false;
            for (std::size_t i = 0; i < laptops.size(); ++i) {
                if (lapID == laptops[i].laptopID) {
                    if (laptop_borrowLaptop(&laptops[i], "member")) {
                        std::cout << "Laptop " << lapID << " borrowed.\n";
                        // reflect in inventory
                        Inventory *inv = inventory_store_find(&invStore, lapID.c_str());
                        if (inv && inv->quantity > 0) {
                            inv->quantity = inv->quantity - 1;
                        }
                    } else {
                        std::cout << "Laptop not available.\n";
                    }
                    found = true;
                    break;
                }
            }
            if (!found) {
                std::cout << "Laptop ID not found.\n";
            }
        }

        else if (c == 5) {
            // Return LAPTOP
            std::string lapID;
            std::cout << "Enter laptop ID to return: ";
            std::getline(std::cin, lapID);

            bool found = false;
            for (std::size_t i = 0; i < laptops.size(); ++i) {
                if (lapID == laptops[i].laptopID) {
                    laptop_returnLaptop(&laptops[i], "member");
                    std::cout << "Laptop returned.\n";
                    // reflect in inventory
                    Inventory *inv = inventory_store_find(&invStore, lapID.c_str());
                    if (inv) {
                        inv->quantity = inv->quantity + 1;
                    }
                    found = true;
                    break;
                }
            }
            if (!found) {
                std::cout << "Laptop ID not found.\n";
            }
        }

        else if (c == 6) {
            // Book STUDY ROOM
            std::string roomID;
            std::cout << "Enter room ID (ROOM01, ROOM02): ";
            std::getline(std::cin, roomID);

            bool found = false;
            for (std::size_t i = 0; i < rooms.size(); ++i) {
                if (roomID == rooms[i].roomID) {
                    if (studyRoom_bookRoom(&rooms[i], "memberID", "2025-11-11 10:00")) {
                        std::cout << "Room booked.\n";
                        // reflect in inventory
                        Inventory *inv = inventory_store_find(&invStore, roomID.c_str());
                        if (inv && inv->quantity > 0) {
                            inv->quantity = inv->quantity - 1;
                        }
                    } else {
                        std::cout << "Room is not available.\n";
                    }
                    found = true;
                    break;
                }
            }
            if (!found) {
                std::cout << "Room ID not found.\n";
            }
        }

        else if (c == 7) {
            // Cancel STUDY ROOM
            std::string roomID;
            std::cout << "Enter room ID to cancel: ";
            std::getline(std::cin, roomID);

            bool found = false;
            for (std::size_t i = 0; i < rooms.size(); ++i) {
                if (roomID == rooms[i].roomID) {
                    studyRoom_cancelBooking(&rooms[i], "bookingID");
                    std::cout << "Room booking cancelled.\n";
                    // reflect in inventory
                    Inventory *inv = inventory_store_find(&invStore, roomID.c_str());
                    if (inv) {
                        inv->quantity = inv->quantity + 1;
                    }
                    found = true;
                    break;
                }
            }
            if (!found) {
                std::cout << "Room ID not found.\n";
            }
        }
    }

    std::cout << "Exiting...\n";
    return 0;
}
