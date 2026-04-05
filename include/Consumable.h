#pragma once
#include "Item.h"

class Player;

class Consumable : public Item {
public:
    int healAmount;

    Consumable(std::string name, std::string description, int healAmount);

    void use(Player* player) override;
};