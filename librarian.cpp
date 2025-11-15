#include "librarian.h"
#include <cstring>

void librarian_init(Librarian *l, int id, const char *name, const char *email)
{
    if (!l) return;
    l->librarianID = id;
    std::strncpy(l->name, name, LIBRARIAN_NAME_MAX);
    l->name[LIBRARIAN_NAME_MAX - 1] = '\0';
    std::strncpy(l->email, email, LIBRARIAN_EMAIL_MAX);
    l->email[LIBRARIAN_EMAIL_MAX - 1] = '\0';
}

void librarian_manageInventory(Librarian *l,
                               Inventory *inv,
                               int newQuantity)
{
    (void)l;
    inventory_updateInventory(inv, newQuantity);
}

int librarian_approveBorrowRequest(Librarian *l,
                                   BorrowStore *borrowStore,
                                   int memberID,
                                   const char *itemID,
                                   Date today)
{
    (void)l;
    Date due = today;
    due.day = due.day + 14;
    return borrow_createBorrowRecord(borrowStore, memberID, itemID, today, due);
}

void librarian_manageStudyRoom(Librarian *l, StudyRoom *room, bool makeAvailable)
{
    (void)l;
    if (!room) return;
    room->availabilityStatus = makeAvailable;
}

void librarian_manageLaptopInventory(Librarian *l, Laptop *lap, bool makeAvailable)
{
    (void)l;
    if (!lap) return;
    lap->availabilityStatus = makeAvailable;
}
