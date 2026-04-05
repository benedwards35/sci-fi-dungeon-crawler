#include "Player.h"
#include "Weapon.h"
#include "Armor.h"
#include "Inventory.h"
#include "UI.h"

Player::Player(int hp, int attackPower, int defensePower)
    : Entity("Commander", hp, attackPower, defensePower),
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
        UI::get().message("No exit that way.");
    }
}

void Player::pickUpItem(int index) {
    if (currentRoom == nullptr) {
        UI::get().message("You're not in a room.");
        return;
    }

    if (index < 0 || index >= (int)currentRoom->items.size()) {
        UI::get().message("No item at that index.");
        return;
    }

    Item* item = currentRoom->items[index];

    if (inventory->addItem(item)) {   // addItem shows "X added to inventory." message
        currentRoom->removeItem(item);
    }
}

void Player::equip(Weapon* weapon) {
    if (equippedWeapon != nullptr) {
        UI::get().message("%s unequips %s.", name.c_str(), equippedWeapon->name.c_str());
    }
    equippedWeapon = weapon;
    attackPower = weapon->damage;
}

void Player::equipArmor(Armor* armor) {
    if (equippedArmor != nullptr) {
        UI::get().message("%s unequips %s.", name.c_str(), equippedArmor->name.c_str());
    }
    equippedArmor = armor;
    defensePower = armor->defensePower;
}

void Player::useItem(int itemIndex) {
    if (inventory == nullptr) {
        UI::get().message("No inventory!");
        return;
    }

    if (itemIndex < 0 || itemIndex >= (int)inventory->items.size()) {
        UI::get().message("Invalid inventory slot.");
        return;
    }
    inventory->items[itemIndex]->use(this);
}
