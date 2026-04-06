#include "StoryItem.h"
#include "Player.h"
#include "Inventory.h"
#include "UI.h"

StoryItem::StoryItem(std::string name, std::string description,
                     std::vector<std::string> loreLines)
    : Item(name, description), lines(loreLines) {}

void StoryItem::use(Player* player) {
    UI::get().log(UI::STYLE_HEADER, "");
    UI::get().log(UI::STYLE_HEADER, "=== %s ===", name.c_str());
    for (const std::string& line : lines) {
        UI::get().log(UI::STYLE_NORMAL, "%s", line.c_str());
    }
    UI::get().log(UI::STYLE_HEADER, "=== END ===");

    if (player->inventory != nullptr) {
        player->inventory->removeItem(this);
    }
    UI::get().message("You read %s.", name.c_str());
}
