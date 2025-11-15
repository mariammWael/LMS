#ifndef BOOK_H
#define BOOK_H

#include "common.h"
#include <stdbool.h>

#define BOOK_TITLE_MAX 100
#define BOOK_AUTHOR_MAX 60
#define BOOK_ISBN_MAX 25
#define BOOK_LINK_MAX 120

typedef struct {
    int    bookID;
    char   title[BOOK_TITLE_MAX];
    char   author[BOOK_AUTHOR_MAX];
    char   ISBN[BOOK_ISBN_MAX];
    Status status;
    char   digitalCopyLink[BOOK_LINK_MAX];
} Book;

void book_init(Book *b, int id, const char *title, const char *author,
               const char *isbn, const char *link);

bool book_checkAvailability(const Book *b);
void book_updateStatus(Book *b, Status s);

#endif /* BOOK_H */
