#ifndef BOOKLOADER_H
#define BOOKLOADER_H

#include <string>
#include <vector>
#include "book.h"       // for Book
#include "inventory.h"  // for InventoryStore, ItemType

// holds what we loaded from the file
struct LoadedData {
    std::vector<Book>       books;
    std::vector<std::string> itemIDs;  // B0001, B0002, ...
};

// read lines from a text file and turn them into Book objects + itemIDs
bool load_books_from_file(const std::string &filename,
                          LoadedData &outData);

// take what we loaded and add it to the inventory store
void add_loaded_to_inventory(const LoadedData &data,
                             InventoryStore &store,
                             int defaultQuantity);

#endif // BOOKLOADER_H
