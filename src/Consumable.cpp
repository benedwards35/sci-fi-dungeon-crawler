#include "Consumable.h"
#include "Player.h"
#include "Inventory.h"
#include "UI.h"
#include <algorithm>

Consumable::Consumable(std::string name, std::string description, int healAmount)
    : Item(name, description),
      healAmount(healAmount) {}

void Consumable::use(Player* player) {
    int actual = std::min(healAmount, player->maxHp - player->hp);
    player->hp += actual;

    // Remove first (silent), then show the heal message last so the bar is informative.
    if (player->inventory != nullptr) {
        player->inventory->removeItem(this);
    }

    UI::get().message("%s uses %s and recovers %d HP (%d/%d)",
        player->name.c_str(), name.c_str(), actual, player->hp, player->maxHp);
}
