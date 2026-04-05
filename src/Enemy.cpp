#include "Enemy.h"
#include "UI.h"

Enemy::Enemy(std::string name, std::string description, int hp, int attackPower, int defensePower)
    : Entity(name, hp, attackPower, defensePower),
      description(description) {}

void Enemy::behavior() {
    // TODO: Subtype Behavior
    UI::get().log(UI::STYLE_ENEMY,
        "%s stands still, its sensors scanning the corridor...", name.c_str());
}
