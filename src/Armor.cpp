#include "Armor.h"
#include "Player.h"
#include "curses.h"

Armor::Armor(std::string name, std::string description, int defensePower):
    Item(name, description),
    defensePower(defensePower) {}

void Armor::use(Player* player){
    player->equipArmor(this);
    printw("%s equips %s [+%d defense]\n", player->name.c_str(), name.c_str(), defensePower);
    refresh();
}
