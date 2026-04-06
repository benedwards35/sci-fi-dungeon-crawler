#pragma once
#include "Item.h"

class Player;

// Area-of-effect explosive — deals flat damage to every living enemy
// in the player's current room, then removes itself from inventory.
class FragGrenade : public Item {
public:
    int damage;

    FragGrenade(std::string name, std::string description, int damage);
    void use(Player* player) override;
};
