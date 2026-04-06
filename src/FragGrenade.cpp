#include "FragGrenade.h"
#include "Player.h"
#include "Room.h"
#include "Enemy.h"
#include "Inventory.h"
#include "UI.h"

FragGrenade::FragGrenade(std::string name, std::string description, int damage)
    : Item(name, description), damage(damage) {}

void FragGrenade::use(Player* player) {
    if (player->currentRoom == nullptr || player->currentRoom->enemies.empty()) {
        UI::get().message("No targets in range for the grenade.");
        return;
    }

    UI::get().log(UI::STYLE_HEADER, "");
    UI::get().log(UI::STYLE_HEADER, "=== FRAG GRENADE ===");
    UI::get().log(UI::STYLE_ENEMY, "You hurl the grenade — BOOM!");

    for (Enemy* e : player->currentRoom->enemies) {
        if (!e->isAlive()) continue;
        e->takeDamage(damage);
        UI::get().log(UI::STYLE_ENEMY, "  %s takes %d damage  (%d/%d HP)",
            e->name.c_str(), damage, e->hp, e->maxHp);
    }

    if (player->inventory != nullptr) {
        player->inventory->removeItem(this);
    }
    UI::get().message("Grenade detonated — %d damage to all enemies.", damage);
}
