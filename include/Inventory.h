#pragma once
#include <vector>
#include "Item.h"

class Inventory {
public:
    std::vector<Item*> items;
    int maxCapacity;

    Inventory(int maxCapacity = 10);

    bool addItem(Item* item);           // returns false if inventory is full
    void removeItem(Item* item);        // remove by pointer — used by Consumable
    void removeItem(int index);         // remove by index — used by Player::useItem
    bool hasItem(Item* item) const;
    void displayItems() const;
};