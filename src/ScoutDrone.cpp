#include "ScoutDrone.h"
#include "Consumable.h"
#include "UI.h"

ScoutDrone::ScoutDrone()
    : Enemy("Scout Drone", "A battered recon unit, one eye flickering.", 25, 6, 1)
{
    lootTable.push_back(new Consumable("Battery Pack", "Salvaged power cell.", 10));
}

void ScoutDrone::behavior(Entity* target) {
    UI::get().log(UI::STYLE_ENEMY, "%s sparks and twitches — erratic double strike!", name.c_str());
    attackTarget(target);
    if (target->isAlive()) {
        UI::get().log(UI::STYLE_ENEMY, "%s flickers, lunging again on failing servos!", name.c_str());
        attackTarget(target);
    }
}
