#include "Entity.h"
#include "UI.h"
#include <algorithm>

Entity::Entity(std::string name, int hp, int attackPower, int defensePower)
    : name(name), hp(hp), maxHp(hp), attackPower(attackPower), defensePower(defensePower) {}

void Entity::takeDamage(int amount) {
    int damage = std::max(1, amount - defensePower);
    hp -= damage;
    hp = std::max(0, hp);

    UI::get().log("%s takes %d damage! (%d/%d HP)", name.c_str(), damage, hp, maxHp);
}

bool Entity::isAlive() const {
    return hp > 0;
}

void Entity::attackTarget(Entity* target) {
    UI::get().log("%s attacks %s!", name.c_str(), target->name.c_str());
    target->takeDamage(attackPower);
}
