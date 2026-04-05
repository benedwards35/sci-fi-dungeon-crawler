#include "Entity.h"
#include <iostream>
#include <algorithm>

Entity::Entity(std::string name, int hp, int attackPower, int defensePower)
    : name(name), hp(hp), maxHp(hp), attackPower(attackPower), defensePower(defensePower) {}

void Entity::takeDamage(int amount) {
    int damage = std::max(1, amount - defensePower);
    hp -= damage;
    hp = std::max(0, hp); 

    std::cout << name << " takes " << damage << " damage! ("
              << hp << "/" << maxHp << " HP)\n";
}

bool Entity::isAlive() const {
    return hp > 0;
}

void Entity::attackTarget(Entity* target) {
    std::cout << name << " attacks " << target->name << "!\n";
    target->takeDamage(attackPower);
}