#pragma once
#include <string>
#include <vector>

class Enemy;
class Item;

const int NORTH = 0;
const int SOUTH = 1;
const int EAST  = 2;
const int WEST  = 3;

class Room {
public:
    std::string name;
    std::string description;

    std::vector<Enemy*> enemies;
    std::vector<Item*>  items;

    Room* exits[4];

    Room(std::string name, std::string description);
    ~Room();  // deletes all Enemy* and Item* it still owns

    void describe() const;

    void addEnemy(Enemy* enemy);
    void addItem(Item* item);
    void removeItem(Item* item);
};