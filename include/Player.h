#pragma once
#include <string>
#include "Entity.h"
#include "Armor.h"
#include "Weapon.h"
#include "Inventory.h"
#include "Room.h"

class Player : public Entity {
public:
    Inventory* inventory;
    Weapon* equippedWeapon;
    Armor* equippedArmor;
    Room* currentRoom;

    // Direction the player TRAVELED to enter the current room
    // (NORTH/SOUTH/EAST/WEST as defined in Room.h).  -1 = starting room.
    // Retreat direction = entryDirection ^ 1.
    int entryDirection;

    // Base stats before equipment bonuses — used when unequipping.
    int baseAttack;
    int baseDefense;

    Player(int hp, int attackPower, int defensePower);
    ~Player();

    // Takes a PDCurses key code: KEY_UP/DOWN/LEFT/RIGHT or 'w'/'a'/'s'/'d'.
    // Blocks non-retreat movement while enemies are present in the room.
    void move(int key);

    void pickUpItem(int index);

    // Equip a weapon/armor already in inventory.
    // Returns false (and shows a message) if inventory is full for a swap.
    bool equip(Weapon* weapon);
    bool equipArmor(Armor* armor);

    // Return equipped item to inventory.
    void unequipWeapon();
    void unequipArmor();

    void useItem(int itemIndex);
};
