#ifndef BORROW_H
#define BORROW_H

#include "common.h"

#define BORROW_ITEM_ID_MAX 30
#define BORROW_STATUS_MAX 20
#define BORROW_MAX_RECORDS 200

typedef struct {
    int  borrowID;
    int  memberID;
    char itemID[BORROW_ITEM_ID_MAX];
    Date borrowDate;
    Date dueDate;
    Date returnDate;
    char status[BORROW_STATUS_MAX]; /* e.g. "Borrowed", "Returned" */
} Borrow;

typedef struct {
    Borrow records[BORROW_MAX_RECORDS];
    int    count;
} BorrowStore;

void borrow_store_init(BorrowStore *store);

int  borrow_createBorrowRecord(BorrowStore *store,
                               int memberID,
                               const char *itemID,
                               Date borrowDate,
                               Date dueDate);

void borrow_updateReturnDate(BorrowStore *store,
                             int borrowID,
                             Date returnDate);

int  borrow_calculateFine(const Borrow *b,
                          int finePerDay);

const char *borrow_checkBorrowStatus(const Borrow *b);

Borrow *borrow_find_by_id(BorrowStore *store, int borrowID);

#endif /* BORROW_H */