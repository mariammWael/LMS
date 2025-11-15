#ifndef INVENTORY_H
#define INVENTORY_H

#include "common.h"
#include <stdbool.h>

#define INVENTORY_ID_MAX 30
#define INVENTORY_MAX_ITEMS 200

typedef struct {
    char     inventoryID[INVENTORY_ID_MAX];
    ItemType itemType;
    int      quantity;
} Inventory;

typedef struct {
    Inventory items[INVENTORY_MAX_ITEMS];
    int       count;
} InventoryStore;

void inventory_store_init(InventoryStore *store);
int  inventory_store_add(InventoryStore *store,
                         const char *inventoryID,
                         ItemType type,
                         int quantity);

Inventory *inventory_store_find(InventoryStore *store,
                                const char *inventoryID);

void inventory_updateInventory(Inventory *inv, int newQuantity);
bool inventory_checkItemAvailability(const Inventory *inv);

#endif /* INVENTORY_H */
