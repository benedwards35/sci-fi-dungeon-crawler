#include "Armor.h"
#include "Player.h"
#include <iostream>

Armor::Armor(std::string name, std::string description, int defensePower):
    Item(name, description),
    defensePower(defensePower) {}

void Armor::use(Player* player){
    player->equipArmor(this);
    std::cout << player->name << " equips " << name << " [+" << defensePower << " defense]\n";
}