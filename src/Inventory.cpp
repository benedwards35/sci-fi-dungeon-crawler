#include "Inventory.h"
#include <algorithm>
#include <iostream>

Inventory::Inventory(int maxCapacity):
    maxCapacity(maxCapacity) {}

bool Inventory::addItem(Item* item){
    if ((int)items.size() >= maxCapacity) {
        std::cout << "Inventory is full!" << std::endl;
        return false;
    }
    items.push_back(item);
    std::cout << item->name << " added to inventory." << std::endl;
    return true;
}


void Inventory::removeItem(Item* item){
    auto it = std::find(items.begin(), items.end(), item);
    if (it != items.end()){
        std::cout << (*it)->name << " removed from inventory.\n";
        items.erase(it);
    } else {
        std::cout << "Item not found in inventory" << std::endl;
    }
}


void Inventory::removeItem(int index){
   if (index < 0 || index >= (int)items.size()) {
        std::cout << "Invalid inventory slot.\n";
        return;
    }
    std::cout << items[index]->name << " removed from inventory.\n";
    items.erase(items.begin() + index);
}


bool Inventory::hasItem(Item* item) const {
    return std::find(items.begin(), items.end(), item) != items.end();
}


void Inventory::displayItems() const {
    if (items.empty()){
        std::cout << "Inventory is empty." << std::endl;
        return;
    }
    std::cout << "Inventory (" << items.size() << "/" << maxCapacity << "):\n";
    for (int i = 0; i < (int)items.size(); i++) {
        std::cout << "  [" << i << "] " << items[i]->name
                  << " - " << items[i]->description << "\n";
    }
}