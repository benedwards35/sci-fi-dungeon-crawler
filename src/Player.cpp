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
    // Room connections not built yet — prints direction for now
    switch (key) {
        case KEY_UP:
        case 'w':
        case 'W':
            std::cout << "Moving north...\n";
            break;
        case KEY_DOWN:
        case 's':
        case 'S':
            std::cout << "Moving south...\n";
            break;
        case KEY_LEFT:
        case 'a':
        case 'A':
            std::cout << "Moving west...\n";
            break;
        case KEY_RIGHT:
        case 'd':
        case 'D':
            std::cout << "Moving east...\n";
            break;
        default:
            std::cout << "Unknown direction.\n";
            break;
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