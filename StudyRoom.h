#ifndef STUDYROOM_H
#define STUDYROOM_H

#include <stdbool.h>

#define ROOM_ID_MAX 30

typedef struct {
    char roomID[ROOM_ID_MAX];
    int  capacity;
    bool availabilityStatus;
} StudyRoom;

void studyRoom_init(StudyRoom *r, const char *roomID, int capacity);
bool studyRoom_viewAvailability(const StudyRoom *r);
bool studyRoom_bookRoom(StudyRoom *r, const char *memberID, const char *dateTime);
void studyRoom_cancelBooking(StudyRoom *r, const char *bookingID);

#endif /* STUDYROOM_H */
