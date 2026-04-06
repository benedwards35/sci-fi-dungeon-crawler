#include "Armor.h"
#include "Player.h"
#include "UI.h"

Armor::Armor(std::string name, std::string description, int defensePower)
    : Item(name, description),
      defensePower(defensePower) {}

void Armor::use(Player* player) {
    if (player->equipArmor(this)) {
        UI::get().message("%s equips %s  [+%d defense]",
            player->name.c_str(), name.c_str(), defensePower);
    }
    // If equipArmor() returns false it already showed a "can't swap" message.
}
