#include "Weapon.h"
#include "Player.h"
#include "UI.h"

Weapon::Weapon(std::string name, std::string description, int damage, std::string type)
    : Item(name, description),
      damage(damage),
      type(type) {}

void Weapon::use(Player* player) {
    if (player->equip(this)) {
        UI::get().message("%s equips %s  [%s | %d dmg]",
            player->name.c_str(), name.c_str(), type.c_str(), damage);
    }
    // If equip() returns false it already showed a "can't swap" message.
}
