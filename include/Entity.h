#pragma once
#include <string>

class Entity {
public:
    std::string name;
    int hp;
    int maxHp;
    int attackPower;
    int defensePower;

    Entity(std::string name, int hp, int attackPower, int defensePower);

    void takeDamage(int amount);
    bool isAlive() const;
    virtual void attackTarget(Entity* target);

    virtual ~Entity() = default;
};