#include "inventory.h"
#include <string.h>

void inventory_store_init(InventoryStore *store)
{
    if (!store) return;
    store->count = 0;
}

int inventory_store_add(InventoryStore *store,
                        const char *inventoryID,
                        ItemType type,
                        int quantity)
{
    int index;
    if (!store) return -1;
    if (store->count >= INVENTORY_MAX_ITEMS) return -1;

    index = store->count;
    strncpy(store->items[index].inventoryID, inventoryID, INVENTORY_ID_MAX);
    store->items[index].inventoryID[INVENTORY_ID_MAX - 1] = '\0';
    store->items[index].itemType = type;
    store->items[index].quantity = quantity;

    store->count = store->count + 1;
    return index;
}

Inventory *inventory_store_find(InventoryStore *store,
                                const char *inventoryID)
{
    int i;
    if (!store || !inventoryID) return NULL;
    for (i = 0; i < store->count; ++i) {
        if (strcmp(store->items[i].inventoryID, inventoryID) == 0) {
            return &store->items[i];
        }
    }
    return NULL;
}

void inventory_updateInventory(Inventory *inv, int newQuantity)
{
    if (!inv) return;
    inv->quantity = newQuantity;
}

bool inventory_checkItemAvailability(const Inventory *inv)
{
    if (!inv) return false;
    return inv->quantity > 0;
}
