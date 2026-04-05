#include "Weapon.h"
#include "Player.h"
#include <iostream>

Weapon::Weapon(std::string name, std::string description, int damage, std::string type):
    Item(name, description),
    damage(damage),
    type(type) {}

void Weapon::use(Player* player){
    player->equip(this);
    std::cout << player->name << " equips " << name << " [" << type << " | " << damage << " dmg]\n";

}