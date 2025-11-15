#ifndef LIBRARIAN_H
#define LIBRARIAN_H

#include "inventory.h"
#include "borrow.h"
#include "studyroom.h"
#include "laptop.h"

#define LIBRARIAN_NAME_MAX 60
#define LIBRARIAN_EMAIL_MAX 60

typedef struct {
    int  librarianID;
    char name[LIBRARIAN_NAME_MAX];
    char email[LIBRARIAN_EMAIL_MAX];
} Librarian;

void librarian_init(Librarian *l, int id, const char *name, const char *email);

void librarian_manageInventory(Librarian *l,
                               Inventory *inv,
                               int newQuantity);

int  librarian_approveBorrowRequest(Librarian *l,
                                    BorrowStore *borrowStore,
                                    int memberID,
                                    const char *itemID,
                                    Date today);

void librarian_manageStudyRoom(Librarian *l, StudyRoom *room, bool makeAvailable);
void librarian_manageLaptopInventory(Librarian *l, Laptop *lap, bool makeAvailable);

#endif /* LIBRARIAN_H */
