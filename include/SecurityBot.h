#pragma once
#include "Enemy.h"

class SecurityBot : public Enemy {
public:
    SecurityBot();
    void behavior(Entity* target) override;

private:
    bool charging;
};
