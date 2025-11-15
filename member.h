#ifndef MEMBER_H
#define MEMBER_H

#include "common.h"
#include "book.h"
#include "inventory.h"
#include "borrow.h"
#include "preferences.h"

#define MEMBER_NAME_MAX 60
#define MEMBER_EMAIL_MAX 60

struct Member {
    int             memberID;
    char            name[MEMBER_NAME_MAX];
    char            email[MEMBER_EMAIL_MAX];
    MembershipType  membershipType;
};

void member_init(Member *m, int id, const char *name,
                 const char *email, MembershipType type);

int  member_searchCatalog(const Member *m,
                          Book *books, int bookCount,
                          const char *title);

Book *member_viewItemDetails(const Member *m,
                             Book *books, int bookCount,
                             int bookID);

bool member_checkAvailability(const Member *m,
                              InventoryStore *store,
                              const char *inventoryID);

int  member_borrowBook(Member *m,
                       BorrowStore *borrowStore,
                       const char *inventoryID,
                       Date today);

void member_returnBook(Member *m,
                       BorrowStore *borrowStore,
                       int borrowID,
                       Date returnDate);

void member_placeHold(Member *m, Book *b);
const char *member_accessDigitalCopy(Member *m, const Book *b);

void member_getRecommendations(Member *m, Preferences *p);
void member_provideFeedback(Member *m, Preferences *p, const char *feedback);

#endif
