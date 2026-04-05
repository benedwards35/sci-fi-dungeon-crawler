#include "Enemy.h"
#include "curses.h"

Enemy::Enemy(std::string name, std::string description, int hp, int attackPower, int defensePower)
    : Entity(name, hp, attackPower, defensePower),
      description(description) {}

void Enemy::behavior() {
    // TODO: Subtype Behavior
    printw("%s stands still, its sensors scanning the corridor...\n", name.c_str());
    refresh();
}
