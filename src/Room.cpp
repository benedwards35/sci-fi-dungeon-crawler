#include "Room.h"
#include "Enemy.h"
#include "Item.h"
#include "UI.h"
#include <algorithm>

Room::Room(std::string name, std::string description)
    : name(name),
      description(description),
      exits{nullptr, nullptr, nullptr, nullptr}
{}

void Room::describe() const {
    UI::get().drawRoom(this);
}

void Room::addEnemy(Enemy* enemy) {
    enemies.push_back(enemy);
}

void Room::addItem(Item* item) {
    items.push_back(item);
}

void Room::removeItem(Item* item) {
    auto it = std::find(items.begin(), items.end(), item);
    if (it != items.end()) {
        items.erase(it);
    }
}
