#include "Entity.h"
#include "curses.h"
#include <algorithm>

Entity::Entity(std::string name, int hp, int attackPower, int defensePower)
    : name(name), hp(hp), maxHp(hp), attackPower(attackPower), defensePower(defensePower) {}

void Entity::takeDamage(int amount) {
    int damage = std::max(1, amount - defensePower);
    hp -= damage;
    hp = std::max(0, hp);

    printw("%s takes %d damage! (%d/%d HP)\n", name.c_str(), damage, hp, maxHp);
    refresh();
}

bool Entity::isAlive() const {
    return hp > 0;
}

void Entity::attackTarget(Entity* target) {
    printw("%s attacks %s!\n", name.c_str(), target->name.c_str());
    refresh();
    target->takeDamage(attackPower);
}
