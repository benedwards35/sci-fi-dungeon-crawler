#include "Inventory.h"
#include "curses.h"
#include <algorithm>

Inventory::Inventory(int maxCapacity):
    maxCapacity(maxCapacity) {}

bool Inventory::addItem(Item* item){
    if ((int)items.size() >= maxCapacity) {
        printw("Inventory is full!\n");
        refresh();
        return false;
    }
    items.push_back(item);
    printw("%s added to inventory.\n", item->name.c_str());
    refresh();
    return true;
}


void Inventory::removeItem(Item* item){
    auto it = std::find(items.begin(), items.end(), item);
    if (it != items.end()){
        printw("%s removed from inventory.\n", (*it)->name.c_str());
        refresh();
        items.erase(it);
    } else {
        printw("Item not found in inventory.\n");
        refresh();
    }
}


void Inventory::removeItem(int index){
   if (index < 0 || index >= (int)items.size()) {
        printw("Invalid inventory slot.\n");
        refresh();
        return;
    }
    printw("%s removed from inventory.\n", items[index]->name.c_str());
    refresh();
    items.erase(items.begin() + index);
}


bool Inventory::hasItem(Item* item) const {
    return std::find(items.begin(), items.end(), item) != items.end();
}


void Inventory::displayItems() const {
    if (items.empty()){
        printw("Inventory is empty.\n");
        refresh();
        return;
    }
    printw("Inventory (%d/%d):\n", (int)items.size(), maxCapacity);
    for (int i = 0; i < (int)items.size(); i++) {
        printw("  [%d] %s - %s\n", i, items[i]->name.c_str(), items[i]->description.c_str());
    }
    refresh();
}
