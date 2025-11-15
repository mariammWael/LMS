#ifndef LAPTOP_H
#define LAPTOP_H

#include <stdbool.h>

#define LAPTOP_ID_MAX 30
#define LAPTOP_MODEL_MAX 40

typedef struct {
    char laptopID[LAPTOP_ID_MAX];
    char model[LAPTOP_MODEL_MAX];
    bool availabilityStatus;
} Laptop;

void laptop_init(Laptop *l, const char *id, const char *model);
bool laptop_viewAvailability(const Laptop *l);
bool laptop_borrowLaptop(Laptop *l, const char *memberID);
void laptop_returnLaptop(Laptop *l, const char *memberID);

#endif /* LAPTOP_H */
