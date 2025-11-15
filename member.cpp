#include "member.h"
#include <cstring>
#include <cstdio>

void member_init(Member *m, int id, const char *name,
                 const char *email, MembershipType type)
{
    if (!m) return;
    m->memberID = id;
    std::strncpy(m->name, name, MEMBER_NAME_MAX);
    m->name[MEMBER_NAME_MAX - 1] = '\0';

    std::strncpy(m->email, email, MEMBER_EMAIL_MAX);
    m->email[MEMBER_EMAIL_MAX - 1] = '\0';

    m->membershipType = type;
}

int member_searchCatalog(const Member *m,
                         Book *books, int bookCount,
                         const char *title)
{
    (void)m;
    if (!books || !title) return -1;
    for (int i = 0; i < bookCount; ++i) {
        if (std::strcmp(books[i].title, title) == 0) {
            return i;
        }
    }
    return -1;
}

Book *member_viewItemDetails(const Member *m,
                             Book *books, int bookCount,
                             int bookID)
{
    (void)m;
    for (int i = 0; i < bookCount; ++i) {
        if (books[i].bookID == bookID) {
            return &books[i];
        }
    }
    return nullptr;
}

bool member_checkAvailability(const Member *m,
                              InventoryStore *store,
                              const char *inventoryID)
{
    (void)m;
    Inventory *inv = inventory_store_find(store, inventoryID);
    if (!inv) return false;
    return inventory_checkItemAvailability(inv);
}

int member_borrowBook(Member *m,
                      BorrowStore *borrowStore,
                      const char *inventoryID,
                      Date today)
{
    (void)m;
    Date due = today;
    due.day = due.day + 14;        // naive
    return borrow_createBorrowRecord(borrowStore,
                                     m->memberID,
                                     inventoryID,
                                     today,
                                     due);
}

void member_returnBook(Member *m,
                       BorrowStore *borrowStore,
                       int borrowID,
                       Date returnDate)
{
    (void)m;
    borrow_updateReturnDate(borrowStore, borrowID, returnDate);
}

void member_placeHold(Member *m, Book *b)
{
    (void)m;
    if (!b) return;
    book_updateStatus(b, STATUS_ONHOLD);
}

const char *member_accessDigitalCopy(Member *m, const Book *b)
{
    (void)m;
    if (!b) return nullptr;
    return b->digitalCopyLink;
}

void member_getRecommendations(Member *m, Preferences *p)
{
    char memberStr[20];
    std::snprintf(memberStr, sizeof(memberStr), "%d", m->memberID);
    preferences_generateRecommendations(p, memberStr);
}

void member_provideFeedback(Member *m, Preferences *p, const char *feedback)
{
    char memberStr[20];
    std::snprintf(memberStr, sizeof(memberStr), "%d", m->memberID);
    preferences_recordFeedback(p, memberStr, feedback);
}
