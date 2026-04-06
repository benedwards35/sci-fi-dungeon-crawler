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
      currentRoom(nullptr),
      entryDirection(-1),
      baseAttack(attackPower),
      baseDefense(defensePower) {}

Player::~Player() {
    // Equipped items are owned by the player when NOT in inventory.
    // Check BEFORE deleting inventory (hasItem walks the live list).
    if (equippedWeapon && !inventory->hasItem(equippedWeapon)) delete equippedWeapon;
    if (equippedArmor  && !inventory->hasItem(equippedArmor))  delete equippedArmor;
    delete inventory;  // ~Inventory() deletes all items still inside it
}

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

    // ── Block non-retreat movement while enemies are present ─────────────
    if (!currentRoom->enemies.empty() && entryDirection >= 0) {
        int retreat = entryDirection ^ 1;   // opposite direction
        if (direction != retreat) {
            const char* retreatKey =
                (retreat == NORTH) ? "W" :
                (retreat == SOUTH) ? "S" :
                (retreat == EAST)  ? "D" : "A";
            UI::get().message(
                "Enemies block the path! Retreat [%s] or press [C] to fight.",
                retreatKey);
            return;
        }
    }

    Room* next = currentRoom->exits[direction];
    if (next != nullptr) {
        entryDirection = direction;   // record direction traveled
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

    if (inventory->addItem(item)) {   // addItem shows "X added to inventory."
        currentRoom->removeItem(item);
    }
}

bool Player::equip(Weapon* weapon) {
    // If something is already equipped, return it to inventory first.
    if (equippedWeapon != nullptr) {
        if (!inventory->addItem(equippedWeapon)) {
            UI::get().message("Inventory full — can't swap weapons.");
            return false;
        }
        UI::get().log(UI::STYLE_ITEM, "You stow %s.", equippedWeapon->name.c_str());
    }
    // Remove the new weapon from inventory (it's going into the weapon slot).
    inventory->removeItem(weapon);
    equippedWeapon = weapon;
    attackPower    = weapon->damage;
    return true;
}

bool Player::equipArmor(Armor* armor) {
    // If something is already equipped, return it to inventory first.
    if (equippedArmor != nullptr) {
        if (!inventory->addItem(equippedArmor)) {
            UI::get().message("Inventory full — can't swap armor.");
            return false;
        }
        UI::get().log(UI::STYLE_ITEM, "You stow %s.", equippedArmor->name.c_str());
    }
    inventory->removeItem(armor);
    equippedArmor = armor;
    defensePower  = armor->defensePower;
    return true;
}

void Player::unequipWeapon() {
    if (equippedWeapon == nullptr) {
        UI::get().message("No weapon equipped.");
        return;
    }
    if (!inventory->addItem(equippedWeapon)) {
        UI::get().message("Inventory full — can't unequip weapon.");
        return;
    }
    UI::get().log(UI::STYLE_ITEM, "You unequip %s.", equippedWeapon->name.c_str());
    equippedWeapon = nullptr;
    attackPower    = baseAttack;
}

void Player::unequipArmor() {
    if (equippedArmor == nullptr) {
        UI::get().message("No armor equipped.");
        return;
    }
    if (!inventory->addItem(equippedArmor)) {
        UI::get().message("Inventory full — can't unequip armor.");
        return;
    }
    UI::get().log(UI::STYLE_ITEM, "You unequip %s.", equippedArmor->name.c_str());
    equippedArmor = nullptr;
    defensePower  = baseDefense;
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
    Item* item = inventory->items[itemIndex];
    item->use(this);

    // If the item left the inventory (consumed or equipped), we may need to
    // free it.  Equipped items are now owned by the equipment slots — don't
    // delete them.  Only delete truly consumed items (no longer in inventory
    // AND not held in an equipment slot).
    if (!inventory->hasItem(item)
            && item != equippedWeapon
            && item != equippedArmor) {
        delete item;
    }
}
