#include "Consumable.h"
#include "Player.h"
#include "Inventory.h"
#include "curses.h"
#include <algorithm>

Consumable::Consumable(std::string name, std::string description, int healAmount)
    : Item(name, description),
      healAmount(healAmount) {}

void Consumable::use(Player* player) {
    int actual = std::min(healAmount, player->maxHp - player->hp);
    player->hp += actual;

    printw("%s uses %s and recovers %d HP (%d/%d)\n",
           player->name.c_str(), name.c_str(), actual, player->hp, player->maxHp);
    refresh();

    if (player->inventory != nullptr){
        player->inventory->removeItem(this);
    }
}
