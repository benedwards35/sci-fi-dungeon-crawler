#include "Player.h"
#include "Weapon.h"
#include "Armor.h"
#include "Inventory.h"
#include <iostream>

// PDCurses key codes
#define KEY_UP    259
#define KEY_DOWN  258
#define KEY_LEFT  260
#define KEY_RIGHT 261

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
        std::cout << "No exit that way.\n";
    }
}

void Player::pickUpItem(int index) {
    if (currentRoom == nullptr) {
        std::cout << "You're not in a room.\n";
        return;
    }

    if (index < 0 || index >= (int)currentRoom->items.size()) {
        std::cout << "No item at that index.\n";
        return;
    }

    Item* item = currentRoom->items[index];

    if (inventory->addItem(item)) {       // returns false if inventory full
        currentRoom->removeItem(item);
    }
}

void Player::equip(Weapon* weapon) {
    if (equippedWeapon != nullptr)
        std::cout << name << " unequips " << equippedWeapon->name << ".\n";
    equippedWeapon = weapon;
    attackPower = weapon->damage;  
}
 
void Player::equipArmor(Armor* armor) {
    if (equippedArmor != nullptr)
        std::cout << name << " unequips " << equippedArmor->name << ".\n";
    equippedArmor = armor;
    defensePower = armor->defensePower; 
}
 
void Player::useItem(int itemIndex) {
    if (inventory == nullptr) {
        std::cout << "No inventory!\n";
        return;
    }

    if (itemIndex < 0 || itemIndex >= (int)inventory->items.size()) {
        std::cout << "Invalid inventory slot.\n";
        return;
    }
    inventory->items[itemIndex]->use(this);
}