#include "laptop.h"
#include <string.h>

void laptop_init(Laptop *l, const char *id, const char *model)
{
    if (!l) return;
    strncpy(l->laptopID, id, LAPTOP_ID_MAX);
    l->laptopID[LAPTOP_ID_MAX - 1] = '\0';

    strncpy(l->model, model, LAPTOP_MODEL_MAX);
    l->model[LAPTOP_MODEL_MAX - 1] = '\0';

    l->availabilityStatus = true;
}

bool laptop_viewAvailability(const Laptop *l)
{
    if (!l) return false;
    return l->availabilityStatus;
}

bool laptop_borrowLaptop(Laptop *l, const char *memberID)
{
    (void)memberID;
    if (!l) return false;
    if (!l->availabilityStatus) return false;
    l->availabilityStatus = false;
    return true;
}

void laptop_returnLaptop(Laptop *l, const char *memberID)
{
    (void)memberID;
    if (!l) return;
    l->availabilityStatus = true;
}
