#include "Player.h"
#include "Weapon.h"
#include "Armor.h"
#include "Inventory.h"
#include "curses.h"

Player::Player(int hp, int attackPower, int defensePower)
    : Entity("Commander", hp, attackPower, defensePower),
    //TODO: These classes
      inventory(new Inventory()),
      equippedWeapon(nullptr),
      equippedArmor(nullptr),
      currentRoom(nullptr) {}

void Player::move(int key) {
    int direction = -1;

    switch (key) {
        case KEY_UP:    case 'w': case 'W': direction = NORTH; break;
        case KEY_DOWN:  case 's': case 'S': direction = SOUTH; break;
        case KEY_LEFT:  case 'a': case 'A': direction = WEST;  break;
        case KEY_RIGHT: case 'd': case 'D': direction = EAST;  break;
        default: return;
    }

    if (currentRoom == nullptr) return;

    Room* next = currentRoom->exits[direction];
    if (next != nullptr) {
        currentRoom = next;
    } else {
        printw("No exit that way.\n");
        refresh();
    }
}

void Player::pickUpItem(int index) {
    if (currentRoom == nullptr) {
        printw("You're not in a room.\n");
        refresh();
        return;
    }

    if (index < 0 || index >= (int)currentRoom->items.size()) {
        printw("No item at that index.\n");
        refresh();
        return;
    }

    Item* item = currentRoom->items[index];

    if (inventory->addItem(item)) {       // returns false if inventory full
        currentRoom->removeItem(item);
    }
}

void Player::equip(Weapon* weapon) {
    if (equippedWeapon != nullptr) {
        printw("%s unequips %s.\n", name.c_str(), equippedWeapon->name.c_str());
        refresh();
    }
    equippedWeapon = weapon;
    attackPower = weapon->damage;
}

void Player::equipArmor(Armor* armor) {
    if (equippedArmor != nullptr) {
        printw("%s unequips %s.\n", name.c_str(), equippedArmor->name.c_str());
        refresh();
    }
    equippedArmor = armor;
    defensePower = armor->defensePower;
}

void Player::useItem(int itemIndex) {
    if (inventory == nullptr) {
        printw("No inventory!\n");
        refresh();
        return;
    }

    if (itemIndex < 0 || itemIndex >= (int)inventory->items.size()) {
        printw("Invalid inventory slot.\n");
        refresh();
        return;
    }
    inventory->items[itemIndex]->use(this);
}
