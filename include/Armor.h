#pragma once
#include <string>
#include "Item.h"

class Player;

class Armor : public Item {
public:
    int defensePower;

    Armor(std::string name, std::string description, int defensePower);

    void use(Player* player) override;

};