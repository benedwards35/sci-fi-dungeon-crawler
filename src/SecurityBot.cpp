#include "SecurityBot.h"
#include "Armor.h"
#include "UI.h"
#include <cstdlib>

SecurityBot::SecurityBot()
    : Enemy("Security Bot", "Hulking enforcement unit, dented but fully operational.", 80, 20, 8),
      charging(false)
{
    lootTable.push_back(new Armor("Blast Plating", "Salvaged bot chassis plating.", 6));
}

void SecurityBot::behavior(Entity* target) {
    if (charging) {
        // Bug fix #7: was calling takeDamage() directly, skipping the attack
        // log. Temporarily double attackPower so attackTarget() logs correctly.
        UI::get().log(UI::STYLE_ENEMY,
            "%s releases stored charge — OVERCHARGE STRIKE!", name.c_str());
        int saved = attackPower;
        attackPower *= 2;
        attackTarget(target);
        attackPower = saved;
        charging = false;
    } else if (rand() % 100 < 30) {
        UI::get().log(UI::STYLE_ENEMY,
            "%s powers down momentarily, capacitors charging...", name.c_str());
        charging = true;
    } else {
        attackTarget(target);
    }
}
