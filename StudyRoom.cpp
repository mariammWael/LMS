#include "studyroom.h"
#include <string.h>

void studyRoom_init(StudyRoom *r, const char *roomID, int capacity)
{
    if (!r) return;
    strncpy(r->roomID, roomID, ROOM_ID_MAX);
    r->roomID[ROOM_ID_MAX - 1] = '\0';
    r->capacity = capacity;
    r->availabilityStatus = true;
}

bool studyRoom_viewAvailability(const StudyRoom *r)
{
    if (!r) return false;
    return r->availabilityStatus;
}

bool studyRoom_bookRoom(StudyRoom *r, const char *memberID, const char *dateTime)
{
    (void)memberID;
    (void)dateTime;
    if (!r) return false;
    if (!r->availabilityStatus) return false;
    r->availabilityStatus = false;
    return true;
}

void studyRoom_cancelBooking(StudyRoom *r, const char *bookingID)
{
    (void)bookingID;
    if (!r) return;
    r->availabilityStatus = true;
}
