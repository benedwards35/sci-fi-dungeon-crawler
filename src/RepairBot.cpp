#include "RepairBot.h"
#include "Consumable.h"
#include "UI.h"
#include <algorithm>

RepairBot::RepairBot()
    : Enemy("Repair Bot", "A frantic maintenance unit, limbs whirring.", 30, 4, 2),
      roomEnemies(nullptr)
{
    lootTable.push_back(new Consumable("Repair Kit", "Emergency patch kit.", 25));
}

void RepairBot::setRoomEnemies(std::vector<Enemy*>* enemies) {
    roomEnemies = enemies;
}

void RepairBot::behavior(Entity* target) {
    if (roomEnemies != nullptr) {
        for (Enemy* ally : *roomEnemies) {
            if (ally == this) continue;
            if (ally->isAlive() && ally->hp < ally->maxHp) {
                ally->hp = std::min(ally->maxHp, ally->hp + 10);
                UI::get().log(UI::STYLE_ENEMY,
                    "%s chirps frantically, patching %s for 10 HP! (%d/%d)",
                    name.c_str(), ally->name.c_str(), ally->hp, ally->maxHp);
                return;
            }
        }
    }
    // All allies at full HP (or no room reference) — reluctantly attack
    UI::get().log(UI::STYLE_ENEMY,
        "%s reluctantly turns its repair tools on you!", name.c_str());
    attackTarget(target);
}
