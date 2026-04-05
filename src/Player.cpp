#include "Player.h"
#include <iostream>

// PDCurses key codes
#define KEY_UP    259
#define KEY_DOWN  258
#define KEY_LEFT  260
#define KEY_RIGHT 261

Player::Player(int hp, int attackPower, int defensePower)
    : Entity("Commander", hp, attackPower, defensePower),
    //TODO: These classes
      inventory(nullptr),
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
    // Full logic once Weapon is built
    equippedWeapon = weapon;
    std::cout << "Commander equipped a weapon.\n";
}

void Player::equipArmor(Armor* armor) {
    // Full logic once Armor is built
    equippedArmor = armor;
    std::cout << "Commander equipped armor.\n";
}

void Player::useItem(int itemIndex) {
    // Full logic once Inventory is built
    std::cout << "Commander uses item at index " << itemIndex << ".\n";
}