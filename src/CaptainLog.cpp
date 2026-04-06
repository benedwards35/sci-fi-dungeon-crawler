#include "CaptainLog.h"
#include "Player.h"
#include "Inventory.h"
#include "UI.h"

CaptainLog::CaptainLog()
    : Consumable("Captain's Log", "A blood-stained data chip.", 0) {}

void CaptainLog::use(Player* player) {
    UI::get().log(UI::STYLE_HEADER, "");
    UI::get().log(UI::STYLE_HEADER, "=== CAPTAIN'S LOG — DAY 847 ===");
    UI::get().log(UI::STYLE_NORMAL, "The crew is gone. All of them.");
    UI::get().log(UI::STYLE_NORMAL, "The thing in the reactor is not what we thought.");
    UI::get().log(UI::STYLE_NORMAL, "Containment failed on day 811. I watched the bots");
    UI::get().log(UI::STYLE_NORMAL, "change. They still follow their routines. They just");
    UI::get().log(UI::STYLE_NORMAL, "don't stop when there's nothing left to repair.");
    UI::get().log(UI::STYLE_NORMAL, "If anyone reads this — don't go to the reactor.");
    UI::get().log(UI::STYLE_NORMAL, "Run.");
    UI::get().log(UI::STYLE_HEADER, "=== END LOG ===");

    if (player->inventory != nullptr) {
        player->inventory->removeItem(this);
    }
}
