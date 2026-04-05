#pragma once
#include <string>
#include "Item.h"

class Player;

class Weapon : public Item {
public:
    int damage;
    std::string type;
    
    Weapon(std::string name, std::string description, int damage, std::string type);

    void use(Player* player) override;

};