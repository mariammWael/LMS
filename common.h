#ifndef COMMON_H
#define COMMON_H

typedef enum {
    MEMBERSHIP_STUDENT = 0,
    MEMBERSHIP_FACULTY,
    MEMBERSHIP_RESEARCHER
} MembershipType;

typedef enum {
    STATUS_AVAILABLE = 0,
    STATUS_BORROWED,
    STATUS_ONHOLD
} Status;

typedef enum {
    ITEMTYPE_BOOK = 0,
    ITEMTYPE_LAPTOP,
    ITEMTYPE_STUDYROOM
} ItemType;

typedef struct {
    int day;
    int month;
    int year;
} Date;

Date date_make(int d, int m, int y);

#endif 