#pragma once
#include <string>
#include "Entity.h"

// TODO: These classes
class Inventory;
class Weapon;
class Armor;
class Room;

class Player : public Entity {
public:
    Inventory* inventory;
    Weapon* equippedWeapon;
    Armor* equippedArmor;
    Room* currentRoom;

    // Constructor — name hardcoded to "Commander" for now
    Player(int hp, int attackPower, int defensePower);

    // Takes a PDCurses key code: KEY_UP/DOWN/LEFT/RIGHT or 'w'/'a'/'s'/'d'
    void move(int key);

    void equip(Weapon* weapon);
    void equipArmor(Armor* armor);
    void useItem(int itemIndex);
};