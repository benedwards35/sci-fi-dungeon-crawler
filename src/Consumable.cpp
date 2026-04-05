#include "Consumable.h"
#include "Player.h"
#include <iostream>
#include <algorithm> 

Consumable::Consumable(std::string name, std::string description, int healAmount)
    : Item(name, description),
      healAmount(healAmount) {}

void Consumable::use(Player* player) {
    
    int actual = std::min(healAmount, player->maxHp - player->hp);
    player->hp += actual;

    std::cout << player->name << " uses " << name
              << " and recovers " << actual << " HP ("
              << player->hp << "/" << player->maxHp << ")\n";

    // TODO: remove from inventory once Inventory is built
    // player->inventory->removeItem(this);
}