#include "Weapon.h"
#include "Player.h"
#include "curses.h"

Weapon::Weapon(std::string name, std::string description, int damage, std::string type):
    Item(name, description),
    damage(damage),
    type(type) {}

void Weapon::use(Player* player){
    player->equip(this);
    printw("%s equips %s [%s | %d dmg]\n", player->name.c_str(), name.c_str(), type.c_str(), damage);
    refresh();
}
