#pragma once
#include "Enemy.h"

class ScoutDrone : public Enemy {
public:
    ScoutDrone();
    void behavior(Entity* target) override;
};
