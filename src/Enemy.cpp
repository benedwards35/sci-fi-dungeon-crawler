#include "Enemy.h"
#include <iostream>

Enemy::Enemy(std::string name, std::string description, int hp, int attackPower, int defensePower)
    : Entity(name, hp, attackPower, defensePower),
      description(description) {}

void Enemy::behavior() {
    // TODO: Subtype Behavior
    std::cout << name << " stands still, its sensors scanning the corridor...\n";
}