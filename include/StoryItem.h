#pragma once
#include "Item.h"
#include <vector>
#include <string>

class Player;

// A readable story/lore item.  Logs its text lines to the main window
// and removes itself from the player's inventory (one-time read).
class StoryItem : public Item {
public:
    std::vector<std::string> lines;   // lore text, one element per log line

    StoryItem(std::string name, std::string description,
              std::vector<std::string> loreLines);

    void use(Player* player) override;
};
