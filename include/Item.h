#pragma once
#include <string>

class Player;


class Item {
public:
    std::string name;
    std::string description;
    
    Item(std::string name, std::string description);

    virtual void use(Player* player) = 0;

    virtual ~Item() = default;

};