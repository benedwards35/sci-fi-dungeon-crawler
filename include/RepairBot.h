#pragma once
#include "Enemy.h"
#include <vector>

class RepairBot : public Enemy {
public:
    RepairBot();
    void behavior(Entity* target) override;

    // Call this after placing the bot in a room so it can heal its allies.
    void setRoomEnemies(std::vector<Enemy*>* enemies);

private:
    std::vector<Enemy*>* roomEnemies;
};
