#pragma once
#include "Consumable.h"

class CaptainLog : public Consumable {
public:
    CaptainLog();
    void use(Player* player) override;
};
