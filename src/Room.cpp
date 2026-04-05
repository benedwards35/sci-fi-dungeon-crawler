#include "Room.h"
#include "Enemy.h"
#include "Item.h"
#include <iostream>
#include <algorithm>  // for std::find
#include "curses.h"

Room::Room(std::string name, std::string description)
    : name(name),
      description(description),
      exits{nullptr, nullptr, nullptr, nullptr}  // all four exits bricked up by default
{}

void Room::describe() const {
    clear();
    printw("\n=== %s ===\n", name.c_str());
    printw("%s\n", description.c_str());

    printw("Exits: ");
    bool anyExit = false;
    if (exits[NORTH]) { printw("[N] "); anyExit = true; }
    if (exits[SOUTH]) { printw("[S] "); anyExit = true; }
    if (exits[EAST])  { printw("[E] "); anyExit = true; }
    if (exits[WEST])  { printw("[W] "); anyExit = true; }
    if (!anyExit) printw("None. You are trapped.");
    printw("\n");

    if (!enemies.empty()) {
        printw("Hostiles: ");
        for (Enemy* e : enemies) printw("%s  ", e->name.c_str());
        printw("\n");
    }

    if (!items.empty()) {
        printw("Items on the floor: ");
        for (Item* i : items) printw("%s  ", i->name.c_str());
        printw("\n");
    }

    refresh();
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