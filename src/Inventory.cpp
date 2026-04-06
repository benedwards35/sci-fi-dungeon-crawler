#include "Inventory.h"
#include "UI.h"
#include <algorithm>

Inventory::Inventory(int maxCapacity)
    : maxCapacity(maxCapacity) {}

Inventory::~Inventory() {
    for (Item* item : items) {
        delete item;
    }
}

bool Inventory::addItem(Item* item) {
    if ((int)items.size() >= maxCapacity) {
        UI::get().message("Inventory is full!");
        return false;
    }
    items.push_back(item);
    UI::get().message("%s added to inventory.", item->name.c_str());
    return true;
}

void Inventory::removeItem(Item* item) {
    // Silent — callers (e.g. Consumable::use) own the feedback message.
    auto it = std::find(items.begin(), items.end(), item);
    if (it != items.end()) {
        items.erase(it);
    }
}

void Inventory::removeItem(int index) {
    if (index < 0 || index >= (int)items.size()) {
        UI::get().message("Invalid inventory slot.");
        return;
    }
    UI::get().message("%s removed from inventory.", items[index]->name.c_str());
    items.erase(items.begin() + index);
}

bool Inventory::hasItem(Item* item) const {
    return std::find(items.begin(), items.end(), item) != items.end();
}

void Inventory::displayItems() const {
    UI::get().drawInventory(this);
}
