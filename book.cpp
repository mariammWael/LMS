#include "book.h"
#include <string.h>

void book_init(Book *b, int id, const char *title, const char *author,
               const char *isbn, const char *link)
{
    if (!b) return;
    b->bookID = id;

    strncpy(b->title, title, BOOK_TITLE_MAX);
    b->title[BOOK_TITLE_MAX - 1] = '\0';

    strncpy(b->author, author, BOOK_AUTHOR_MAX);
    b->author[BOOK_AUTHOR_MAX - 1] = '\0';

    strncpy(b->ISBN, isbn, BOOK_ISBN_MAX);
    b->ISBN[BOOK_ISBN_MAX - 1] = '\0';

    strncpy(b->digitalCopyLink, link, BOOK_LINK_MAX);
    b->digitalCopyLink[BOOK_LINK_MAX - 1] = '\0';

    b->status = STATUS_AVAILABLE;
}

bool book_checkAvailability(const Book *b)
{
    if (!b) return false;
    return (b->status == STATUS_AVAILABLE);
}

void book_updateStatus(Book *b, Status s)
{
    if (!b) return;
    b->status = s;
}
