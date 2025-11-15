
#include "borrow.h"
#include <string.h>

/* naive date diff helper (same month/year only) */
static int date_diff_days(Date a, Date b)
{
    int diff;
    diff = (a.day - b.day);
    return diff;
}

void borrow_store_init(BorrowStore *store)
{
    if (!store) return;
    store->count = 0;
}

int borrow_createBorrowRecord(BorrowStore *store,
                              int memberID,
                              const char *itemID,
                              Date borrowDate,
                              Date dueDate)
{
    int index;
    if (!store) return -1;
    if (store->count >= BORROW_MAX_RECORDS) return -1;

    index = store->count;

    store->records[index].borrowID = index + 1; /* simple auto id */
    store->records[index].memberID = memberID;

    strncpy(store->records[index].itemID, itemID, BORROW_ITEM_ID_MAX);
    store->records[index].itemID[BORROW_ITEM_ID_MAX - 1] = '\0';

    store->records[index].borrowDate = borrowDate;
    store->records[index].dueDate = dueDate;
    store->records[index].returnDate = date_make(0, 0, 0);

    strncpy(store->records[index].status, "Borrowed", BORROW_STATUS_MAX);
    store->records[index].status[BORROW_STATUS_MAX - 1] = '\0';

    store->count = store->count + 1;
    return store->records[index].borrowID;
}

void borrow_updateReturnDate(BorrowStore *store,
                             int borrowID,
                             Date returnDate)
{
    Borrow *b = borrow_find_by_id(store, borrowID);
    if (!b) return;
    b->returnDate = returnDate;
    strncpy(b->status, "Returned", BORROW_STATUS_MAX);
    b->status[BORROW_STATUS_MAX - 1] = '\0';
}

int borrow_calculateFine(const Borrow *b, int finePerDay)
{
    int late;
    if (!b) return 0;
    /* if no return date, no fine yet */
    if (b->returnDate.day == 0) return 0;

    late = date_diff_days(b->returnDate, b->dueDate);
    if (late <= 0) return 0;
    return late * finePerDay;
}

const char *borrow_checkBorrowStatus(const Borrow *b)
{
    if (!b) return "Unknown";
    return b->status;
}

Borrow *borrow_find_by_id(BorrowStore *store, int borrowID)
{
    int i;
    if (!store) return NULL;
    for (i = 0; i < store->count; ++i) {
        if (store->records[i].borrowID == borrowID) {
            return &store->records[i];
        }
    }
    return NULL;
}
