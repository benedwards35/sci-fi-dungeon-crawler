#pragma once
#include <string>
#include <vector>
#include "Entity.h"

// TODO: Item Class
class Item;

class Enemy : public Entity {
public:
    std::string description;
    std::vector<Item*> lootTable;

    Enemy(std::string name, std::string description, int hp, int attackPower, int defensePower);

    // Each subtype will override this with their own AI
    virtual void behavior();
};