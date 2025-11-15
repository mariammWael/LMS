#include "bookloader.h"
#include <fstream>
#include <iomanip>
#include <sstream>

// load "books.txt" => each non-empty line becomes a Book
bool load_books_from_file(const std::string &filename,
                          LoadedData &outData)
{
    outData.books.clear();
    outData.itemIDs.clear();

    std::ifstream in(filename);
    if (!in.is_open()) {
        return false;
    }

    std::string line;
    int id = 1;

    while (std::getline(in, line)) {
        if (line.empty())
            continue;

        Book b;
        // placeholders for author/isbn/link
        std::string author = "Unknown";
        std::string isbn   = "N/A";
        std::string link   = "";

        book_init(&b,
                  id,
                  line.c_str(),
                  author.c_str(),
                  isbn.c_str(),
                  link.c_str());

        outData.books.push_back(b);

        // build itemID like B0001
        std::ostringstream oss;
        oss << "B" << std::setw(4) << std::setfill('0') << id;
        outData.itemIDs.push_back(oss.str());

        id = id + 1;
    }

    return true;
}

void add_loaded_to_inventory(const LoadedData &data,
                             InventoryStore &store,
                             int defaultQuantity)
{
    for (std::size_t i = 0; i < data.itemIDs.size(); ++i) {
        inventory_store_add(&store,
                            data.itemIDs[i].c_str(),
                            ITEMTYPE_BOOK,
                            defaultQuantity);
    }
}
